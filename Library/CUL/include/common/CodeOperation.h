#pragma once
#include "../cul.h"

/*********************************************************************
Litz 2010-01
����ת��������������GB2312��UTF_8��Unicode֮��ת������
ʹ��Windows�ײ㺯������������Ƕ��ʽ��
input:
	*pIn:	������ַ���ָ��
	nSize:	������ַ����Ĵ�С
	bHighFirst: Unicode��char*����ʱ�Ƿ��λ��ǰ��TRUE��ʾ��λ��ǰ��false��ʾ��λ�ں�
output:
	**ppOut:�������ַ���ָ���ַ
	strOut: ������std::wstringֵ
return:
	size_t:	�������ַ�����С
Note��
	�����ڲ��Ὺ���ڴ棬�ǵ��ͷ�!
*********************************************************************/

#include <string>
using namespace std;

//////////////////////////////////////////////////////////////////////////
// �ַ��������ʽת������
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

    // ��ƽ̨���ַ���ת��
    static wstring StringToWstring(string s);
    static wstring StringToWstring(const char *pIn, size_t nSize);
    static string WstringToString(wstring s);
    static string WstringToString(const wchar_t *pIn, size_t nSize);

	static string UTF16toUTF8(const char *pIn, size_t nSize);
#ifndef WIN32
	// UTF16 -> UTF8
	static string UTF_16ToUTF_8(wchar_t* pIn, size_t size, bool bigEndian);
#endif

	// ����
	static wstring StrFilter(wstring wstr);	//	����xml��Ч�ַ�

    static int getCharCode(int ch1, int ch2=0);

	static string ws2s(const std::wstring& ws);

	static wstring s2ws(const std::string& s);
};

