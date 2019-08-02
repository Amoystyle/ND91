#pragma once

#include "AssistantDll.h"

#include <string>
using namespace std;

// ר���ڴ洢UF8�ַ������࣬�Զ��ͷ��ڴ�
class StringUTF8 : private string
{
    friend class CIosWiFiMobileDevice;
    friend class CITunesMobileDevice;

private:
    // ת��Ϊ��constָ�룬����������2������
    operator char* () const { return const_cast<char*>(c_str()); }

public:
    StringUTF8(const char* pUTF8) : string(pUTF8) {}
    StringUTF8(string s) : string(s) {}
    operator const char* () const { return c_str(); }

    StringUTF8(const char* pUTF8, size_t size) : string(pUTF8, size) {}
    size_t size() const { return string::size(); }
};

//////////////////////////////////////////////////////////////////////////
// �ַ��������ʽת������
//////////////////////////////////////////////////////////////////////////
class CLASSINDLL_CLASS_DECL MyCodeOperation  
{
public:
    static StringUTF8 Unicode2UTF8(wstring ws);  // ר�ã�����
    static string UnicodeToUTF_8SQL(wstring ws);

};

