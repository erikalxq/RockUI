/* -----------------------------------------------------------------------
 *  File		:	SkInstLayzyFunc.cpp
 *  Author	    :	hexujun
 *  Datet   	:	2015/1/9 16:09
 *  Brief		:	
 *
 * ----------------------------------------------------------------------
 * ----------------------------------------------------------------------*/

#ifndef __SkInstLayzyFunc__h__
#define __SkInstLayzyFunc__h__
//
///////////////////////////////////////////////////////////////////////////

#include "SkFontMgr.h"
#include "SkLazyPtr.h"
#include "SkMessageBus.h"
#include "SkPathRef.h"
#include "SkImageFilter.h"
#include "SkGlyphCache_Globals.h"
#include "SkPicture.h"
#include "SkGr.h"
#include "SkDiscardableMemoryPool.h"
#include "SkXfermode.h"

template< class T>
static void InitLayzyFuncImpl()
{
	T* pT = NULL;
	if (pT)
	{
		Private::sk_delete<T>(pT);
	}
}

int InitLayzyFunc()
{
	InitLayzyFuncImpl<SkFontMgr>();
	InitLayzyFuncImpl<SkMessageBus<SkPicture::DeletionMessage>>();
	InitLayzyFuncImpl<SkPathRef>();
	InitLayzyFuncImpl<SkImageFilter::Cache>();
	InitLayzyFuncImpl<SkGlyphCache_Globals>();
	InitLayzyFuncImpl<SkMessageBus<GrResourceInvalidatedMessage>>();
	InitLayzyFuncImpl<int>();
	InitLayzyFuncImpl<SkXfermode>();
	InitLayzyFuncImpl<SkDiscardableMemoryPool>();

	return 0;
}
///////////////////////////////////////////////////////////////////////////
//
#endif // __SkInstLayzyFunc__h__
