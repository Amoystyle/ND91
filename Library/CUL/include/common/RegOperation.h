#pragma once

#include "../cul.h"
#include <tchar.h>
#include <string>
using namespace std;

// ע������
class CUL_CLASS_DECL CRegOperation  
{
public:
    static wstring GetRegValue(struct HKEY__* hTopkey, wchar_t* szSubKey, wchar_t* szValueName);

};
