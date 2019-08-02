#pragma once

#include "AssistantDll.h"

#include <string>
using namespace std;

// 专用于存储UF8字符串的类，自动释放内存
class StringUTF8 : private string
{
    friend class CIosWiFiMobileDevice;
    friend class CITunesMobileDevice;

private:
    // 转换为非const指针，仅用于上述2个友类
    operator char* () const { return const_cast<char*>(c_str()); }

public:
    StringUTF8(const char* pUTF8) : string(pUTF8) {}
    StringUTF8(string s) : string(s) {}
    operator const char* () const { return c_str(); }

    StringUTF8(const char* pUTF8, size_t size) : string(pUTF8, size) {}
    size_t size() const { return string::size(); }
};

//////////////////////////////////////////////////////////////////////////
// 字符串编码格式转换操作
//////////////////////////////////////////////////////////////////////////
class CLASSINDLL_CLASS_DECL MyCodeOperation  
{
public:
    static StringUTF8 Unicode2UTF8(wstring ws);  // 专用，特殊
    static string UnicodeToUTF_8SQL(wstring ws);

};

