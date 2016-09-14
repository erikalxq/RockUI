#pragma once
#include <time.h>

template <class TimeType>
class KTimeHelperT
{
public:
	static TimeType MintoSec( TimeType nMinute )
	{
		return nMinute * 60;
	}

	static TimeType HourtoSec( TimeType nHour)
	{
		return nHour * 60 * 60;
	}

	static TimeType Past( TimeType nDuration )
	{
		if (0 <= nDuration)
		{
			nDuration = -nDuration;
		}

		return nDuration;
	}

	static TimeType Next( TimeType nDuration )
	{
		if (0 > nDuration)
		{
			nDuration = -nDuration;
		}

		return nDuration;
	}
};

template <class TimeTicker>
class KTimeTickerT
{
public:
	typedef typename TimeTicker::TimeType	TimeType;
	typedef KTimeHelperT<TimeType>			TimeHelper;

public:
	KTimeTickerT( TimeType nDurationToCurrent = 0)
	{
		Restart();
		m_tmTickStart += nDurationToCurrent;
	}

	void Restart()
	{
		m_tmTickStart = Tick();
	}

	void RestartFrom( TimeType tmStartPoint)
	{
		m_tmTickStart = tmStartPoint;
	}

	TimeType Duration() const
	{
		return Tick() - m_tmTickStart;
	}

public:
	static TimeType Tick()
	{
		return TimeTicker::Tick();
	}

protected:
	TimeType	m_tmTickStart;
};

class KTimeSecTicker
{
public:
	typedef __time64_t TimeType;

public:
	static TimeType Tick()
	{
		return _time64(NULL);
	}
};

class KClockCountTicker
{
public:
	typedef DWORD TimeType;

public:
	static TimeType Tick()
	{
		return GetTickCount();
	}
};

typedef KTimeTickerT<KTimeSecTicker>		KTimeTicker;
typedef KTimeTickerT<KClockCountTicker>		KClockTicker;