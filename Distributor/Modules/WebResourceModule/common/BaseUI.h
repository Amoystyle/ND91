#pragma once


class BaseUI
{
public:
    static const wchar_t* LoadString(const wchar_t* id) { return I18NSTR(id); }
   
};