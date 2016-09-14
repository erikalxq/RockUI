#include "stdafx.h"
#include "RockImagePool.h"

namespace RockUI
{
    __declspec(selectany) RImagePool* RImagePool::s_imagePool = NULL;

    RImagePool::RImagePool()
    {

    }

    RImagePool::~RImagePool()
    {
        _clear();
    }

    RImagePool* RImagePool::GetInstance()
    {
        if (!s_imagePool)
        {
            s_imagePool = new RImagePool;
        }
        return s_imagePool;
    }

    void RImagePool::ReleaseInstance()
    {
        if (s_imagePool)
        {
            delete s_imagePool;
            s_imagePool = NULL;
        }
    }

    Gdiplus::Image* RImagePool::GetImage(std::wstring& sFile)
    {
        Gdiplus::Image* pImage = NULL;
        for (RResourcePathSet::iterator it = m_resourcePathSet.begin(); it != m_resourcePathSet.end(); ++it)
        {
            pImage = _GetImage(*it, sFile);
            if (pImage)
                break;
        }
        return pImage;
    }

    Gdiplus::Image* RImagePool::_GetImage(std::wstring& sPath, std::wstring& sFile)
    {
        std::wstring sFilePath = sPath + _T("\\") + sFile;
        RImageMap::iterator itRes = m_imageMap.find(sFile);
        if (itRes == m_imageMap.end())
        {
            Gdiplus::Image* newImage = Gdiplus::Image::FromFile(sFilePath.c_str());
            if (newImage && newImage->GetLastStatus() == Gdiplus::Ok)
            {
                m_imageMap[sFile] = newImage;
                return newImage;
            }
            else if (newImage != NULL)
            {
                delete newImage;
            }
            return NULL;
        }
        return itRes->second;
    }

    void RImagePool::AddResourcePath(std::wstring sPath)
    {
        m_resourcePathSet.insert(sPath);
    }

    void RImagePool::_clear()
    {
        for (RImageMap::iterator itRes = m_imageMap.begin(); itRes != m_imageMap.end(); ++itRes)
        {
            delete itRes->second;
        }
        m_imageMap.clear();
    }
}

