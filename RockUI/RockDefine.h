#ifndef __ROCK_DEFINE_H__
#define __ROCK_DEFINE_H__

namespace RockUI
{
    //> 元素布局方式
    const static char* c_align_fill     = "fill";
    const static char* c_align_vcenter  = "vcenter";
    const static char* c_align_hcenter  = "hcenter";
    const static char* c_align_center   = "center";
    const static char* c_align_right    = "right";
    const static char* c_align_left     = "left";
    const static char* c_align_top      = "top";
    const static char* c_align_bottom   = "bottom";

    typedef DWORD HRITEM;

#define ROCKWM_INVALIDATE       WM_USER + 1001
#define ROCKWM_REDRAW           WM_USER + 1002
#define ROCKWM_TIMER            WM_USER + 1003
}

//////////////////////////////////////////////////////////////////////////
//xml Attribute Declaration
#define ROCKUI_DECLARE_ATTRIBUTES_BEGIN()                           \
public:                                                             \
    virtual HRESULT SetAttribute(                                   \
        std::string strAttribName,                                  \
        std::string strValue,                                       \
        BOOL bLoading)                                              \
    {                                                               \
        HRESULT hRet = __super::SetAttribute(                       \
            strAttribName,                                          \
            strValue,                                               \
            bLoading                                                \
            );                                                      \
        if (SUCCEEDED(hRet))                                        \
            return hRet;                                            \

#define ROCKUI_DECLARE_ATTRIBUTES_END()                             \
            return E_FAIL;                                          \
                                                                    \
        return hRet;                                                \
    }                                                               \

#define ROCKUI_CHAIN_ATTRIBUTE(varname, allredraw)                  \
        if (SUCCEEDED(hRet = varname.SetAttribute(strAttribName, strValue, bLoading)))   \
        {                                                           \
            return hRet;                                            \
        }                                                           \
        else                                                        \
// Custom Attribute
// HRESULT OnAttributeChange(CStringA& strValue, BOOL bLoading)
#define ROCKUI_CUSTOM_ATTRIBUTE(attribname, func)                   \
        if (attribname == strAttribName)                            \
        {                                                           \
            hRet = func(strValue, bLoading);                        \
        }                                                           \
        else                                                        \

// Int = %d StringA
#define ROCKUI_INT_ATTRIBUTE(attribname, varname, allredraw)        \
        if (attribname == strAttribName)                            \
        {                                                           \
            varname = ::StrToIntA(strValue);                        \
            hRet = allredraw ? S_OK : S_FALSE;                      \
        }                                                           \
        else                                                        \

// UInt = %u StringA
#define ROCKUI_UINT_ATTRIBUTE(attribname, varname, allredraw)       \
        if (attribname == strAttribName)                            \
        {                                                           \
            varname = (UINT)::StrToIntA(strValue);                  \
            hRet = allredraw ? S_OK : S_FALSE;                      \
        }                                                           \
        else                                                        \

// DWORD = %u StringA
#define ROCKUI_DWORD_ATTRIBUTE(attribname, varname, allredraw)      \
        if (attribname == strAttribName)                            \
        {                                                           \
            varname = (DWORD)::StrToIntA(strValue);                 \
            hRet = allredraw ? S_OK : S_FALSE;                      \
        }                                                           \
        else                                                        \

// StringA = StringA
#define ROCKUI_STRING_ATTRIBUTE(attribname, varname, allredraw)     \
        if (attribname == strAttribName)                            \
        {                                                           \
            varname = strValue;                                     \
            hRet = allredraw ? S_OK : S_FALSE;                      \
        }                                                           \
        else                                                        \

// StringT = StringA
#define ROCKUI_TSTRING_ATTRIBUTE(attribname, varname, allredraw)    \
        if (attribname == strAttribName)                            \
        {                                                           \
            varname = CA2T(strValue, CP_UTF8);                      \
            hRet = allredraw ? S_OK : S_FALSE;                      \
        }                                                           \
        else                                                        \

// Value In {String1 : Value1, String2 : Value2 ...}
#define ROCKUI_ENUM_ATTRIBUTE(attribname, vartype, allredraw)       \
        if (attribname == strAttribName)                            \
        {                                                           \
            vartype varTemp;                                        \
                                                                    \
            hRet = allredraw ? S_OK : S_FALSE;                      \

#define ROCKUI_ENUM_VALUE(enumstring, enumvalue)                    \
            if (strValue == enumstring)                             \
                varTemp = enumvalue;                                \
            else                                                    \

#define ROCKUI_ENUM_END(varname)                                    \
                return E_FAIL;                                      \
                                                                    \
            varname = varTemp;                                      \
        }                                                           \
        else                                                        \

#endif