#include "stdafx.h"

string MyCodeOperation::UnicodeToUTF_8SQL(wstring ws)
{
	std::wstring wsreplace = CStrOperation::ReplaceW(ws,L"'",L"''");
    return CCodeOperation::UnicodeToUTF_8(wsreplace.c_str(), wsreplace.length());
}

StringUTF8 MyCodeOperation::Unicode2UTF8(wstring ws)
{
    return CCodeOperation::UnicodeToUTF_8(ws);
}
