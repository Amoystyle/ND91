#pragma once
#include "../cul.h"

/*********************************************************************
Litz 2010-01
编码转换函数，用于在GB2312、UTF_8、Unicode之间转换编码
使用Windows底层函数，不适用于嵌入式等
input:
	*pIn:	传入的字符串指针
	nSize:	传入的字符串的大小
	bHighFirst: Unicode以char*传入时是否高位在前，TRUE表示高位在前，false表示高位在后
output:
	**ppOut:传出的字符串指针地址
	strOut: 传出的std::wstring值
return:
	size_t:	传出的字符串大小
Note：
	函数内部会开辟内存，记得释放!
*********************************************************************/

#include <string>
using namespace std;

//////////////////////////////////////////////////////////////////////////
// 字符串编码格式转换操作
//////////////////////////////////////////////////////////////////////////
class CUL_CLASS_DECL CCodeOperation  
{
public:

    // unicode -> GB2312
    static string UnicodeToGB2312(const wchar_t *pIn, size_t nSize);
    static string UnicodeToGB2312(std::wstring ws);

    // Unicode -> UTF8
    static string UnicodeToUTF_8(const wchar_t *pIn, size_t nSize);
    static string UnicodeToUTF_8(wstring ws);

    // GB2312 -> Unicode
    static wstring GB2312ToUnicode(const char *pIn, size_t nSize);
    static std::wstring GB2312ToUnicode(std::string s);

    // GB2312 -> UTF8
    static string GB2312ToUTF_8(const char *pIn, size_t nSize);
    static string GB2312ToUTF_8(const string s);

    // UTF8 -> GB2312
    static string UTF_8ToGB2312(const char *pIn, size_t nSize);
    static string UTF_8ToGB2312(const string s);

    // UTF8 -> unicode
    static wstring UTF_8ToUnicode(const char *pIn, size_t nSize);
    static wstring UTF_8ToUnicode(string s);

    // 跨平台的字符串转换
    static wstring StringToWstring(string s);
    static wstring StringToWstring(const char *pIn, size_t nSize);
    static string WstringToString(wstring s);
    static string WstringToString(const wchar_t *pIn, size_t nSize);

	static string UTF16toUTF8(const char *pIn, size_t nSize);
#ifndef WIN32
	// UTF16 -> UTF8
	static string UTF_16ToUTF_8(wchar_t* pIn, size_t size, bool bigEndian);
#endif

	// 其他
	static wstring StrFilter(wstring wstr);	//	过滤xml无效字符

    static int getCharCode(int ch1, int ch2=0);

	static string ws2s(const std::wstring& ws);

	static wstring s2ws(const std::string& s);
};

