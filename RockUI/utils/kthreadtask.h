#pragma once
#include "KThreadCrack.h"
#include "ktimeticker.h"

template <class ThreadTaskExecutor>
class KThreadTask
{
public:
	typedef ThreadTaskExecutor *ThreadExecutorPtr;
	typedef int (__thiscall ThreadTaskExecutor::*TaskFunction)(LPVOID lpParam);

public:
	KThreadTask( ThreadExecutorPtr pThreadTaskExecutor = NULL, TaskFunction pTaskFun = NULL )
		: m_pThreadExecutor(pThreadTaskExecutor)
		, m_pTaskFun(pTaskFun)
		, m_hThreadCurrent(NULL)
	{
	}

	~KThreadTask(void)
	{
		Stop();
	}

public:
	void SetExecutor( ThreadExecutorPtr pExecutor )
	{
		m_pThreadExecutor = pExecutor;
	}

	void SetTask( TaskFunction pTaskFun )
	{
		m_pTaskFun = pTaskFun;
	}

	BOOL Execute( TaskFunction pfnThreadExecuteFunction, LPVOID lpParam = NULL )
	{
		if ( NULL ==  m_pThreadExecutor || NULL == pfnThreadExecuteFunction )
		{
			return FALSE;
		}

		_ClearFinishThreadData();
		if ( m_hThreadCurrent )
		{
			return FALSE;
		}

		m_hThreadCurrent = KCreateThread<ThreadTaskExecutor>(m_pThreadExecutor, pfnThreadExecuteFunction, NULL, lpParam);
		return NULL != m_hThreadCurrent;
	}

	BOOL Execute( LPVOID lpParam = NULL )
	{
		return Execute(m_pTaskFun, lpParam);
	}

	BOOL Stop( DWORD dwTimeout = 1000 )
	{
		if ( NULL == m_hThreadCurrent )
		{
			return TRUE;
		}

		if ( WAIT_TIMEOUT == WaitForSingleObject(m_hThreadCurrent, dwTimeout) )
		{
			TerminateThread(m_hThreadCurrent, 0);
		}

		CloseHandle(m_hThreadCurrent);
		m_hThreadCurrent = NULL;

		return TRUE;
	}

	void WaitEx( DWORD& dwWaitResult, DWORD dwTimeout = 1000 )
	{
		if (m_hThreadCurrent)
		{
			dwWaitResult = WaitForSingleObject(m_hThreadCurrent, dwTimeout);
		}
		else
		{
			dwWaitResult = WAIT_OBJECT_0;
		}
	}

	BOOL Wait( DWORD dwExpectResult, DWORD dwTimeout = 1000 )
	{
		DWORD dwWaitResult = 0;
		WaitEx(dwWaitResult, dwTimeout);

		return ( dwWaitResult == dwExpectResult );
	}

	BOOL IsFinished( DWORD dwTimeout = 1000 )
	{
		if (NULL == m_hThreadCurrent || Wait(WAIT_OBJECT_0, dwTimeout))
		{
			return TRUE;
		}

		return FALSE;
	}

	BOOL IsTaskExecuting()
	{
		return !IsFinished(0);
	}

	HANDLE Object()
	{
		return m_hThreadCurrent;
	}

	ThreadExecutorPtr Executor()
	{
		return m_pThreadExecutor;
	}

protected:
	BOOL _ClearFinishThreadData()
	{
		if ( m_hThreadCurrent && WAIT_OBJECT_0 == WaitForSingleObject(m_hThreadCurrent, 0) )
		{
			CloseHandle(m_hThreadCurrent);
			m_hThreadCurrent = NULL;
		}

		return ( NULL == m_hThreadCurrent );
	}

protected:
	ThreadExecutorPtr	m_pThreadExecutor;
	TaskFunction		m_pTaskFun;

	HANDLE				m_hThreadCurrent;
};

template <class ThreadTaskExecutor>
class KIntervalThreadTask : public KThreadTask<ThreadTaskExecutor>
{
	class KIntervalTimeTicker : public KTimeTicker
	{
	public:
		KIntervalTimeTicker( DWORD dwInterval )
			: m_dwInterval(dwInterval)
		{
			RestartFrom(0);
		}

		void SetInterval( DWORD dwInterval )
		{
			m_dwInterval = dwInterval;
		}

		BOOL IsIntervalReached()
		{
			return m_dwInterval <= Duration();
		}

	protected:
		DWORD m_dwInterval;
	};

public:
	KIntervalThreadTask( ThreadExecutorPtr pThreadTaskExecutor, TaskFunction pTaskFun = NULL, DWORD dwInterval = 0 )
		: KThreadTask<ThreadTaskExecutor>(pThreadTaskExecutor, pTaskFun)
		, m_interval(dwInterval)
		, m_bForceExecute(FALSE)
	{
	}

public:
	void SetInterval( DWORD dwInterval )
	{
		m_interval.SetInterval(dwInterval);
	}

	void SetForceExecute( BOOL bForceExecute )
	{
		m_bForceExecute = bForceExecute;
	}

public:
	BOOL Execute( TaskFunction pfnThreadExecuteFunction, LPVOID lpParam = NULL )
	{
		if ( FALSE == _IsExecuteValid(pfnThreadExecuteFunction) )
		{
			return FALSE;
		}

		_ResetForceExecuteFlag();
		if ( FALSE == _ClearFinishThreadData() )
		{
			return FALSE;
		}

		m_hThreadCurrent = KCreateThread<ThreadTaskExecutor>(m_pThreadExecutor, pfnThreadExecuteFunction, NULL, lpParam);
		if ( m_hThreadCurrent )
		{
			m_interval.Restart();
			return TRUE;
		}

		return FALSE;
	}

	BOOL Execute( LPVOID lpParam = NULL )
	{
		return Execute(m_pTaskFun, lpParam);
	}

	BOOL _IsExecuteValid( TaskFunction pfnThreadExecuteFunction )
	{
		if ( FALSE == m_bForceExecute && FALSE == m_interval.IsIntervalReached() )
		{
			return FALSE;
		}

		if ( NULL ==  m_pThreadExecutor || NULL == pfnThreadExecuteFunction )
		{
			return FALSE;
		}

		return TRUE;
	}

	void _ResetForceExecuteFlag()
	{
		m_bForceExecute = FALSE;
	}


protected:
	KIntervalTimeTicker m_interval;
	BOOL				m_bForceExecute;
};
