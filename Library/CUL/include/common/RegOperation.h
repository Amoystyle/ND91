#pragma once

#include "../cul.h"
#include <tchar.h>
#include <string>
using namespace std;

// ×¢²á±í²Ù×÷
class CUL_CLASS_DECL CRegOperation  
{
public:
    static wstring GetRegValue(struct HKEY__* hTopkey, wchar_t* szSubKey, wchar_t* szValueName);

};
