#include "stdafx.h"
#include "Common/MyStrOperation.h"
#ifdef WIN32

//#include "Common/Des/CommCrypt.hpp"
#include "Common/md5.h"
#include "Common/NDBase64.h"

#pragma warning(disable: 4996)
#endif

int MyStrOperation::Random( int start, int end )
{
    unsigned int i = rand();
    double t = i;
    return int(start + t / RAND_MAX * (end - start + 1));
}

bool In(wchar_t   start,   wchar_t   end,   wchar_t   code)
{
    if   (code   >=   start   &&   code   <=   end)
    {
        return   true;
    }
    return   false;
}

char convert(wchar_t   n)
{
    if   (In(0xB0A1,0xB0C4,n))   return   'a';
    if   (In(0XB0C5,0XB2C0,n))   return   'b';
    if   (In(0xB2C1,0xB4ED,n))   return   'c';
    if   (In(0xB4EE,0xB6E9,n))   return   'd';
    if   (In(0xB6EA,0xB7A1,n))   return   'e';
    if   (In(0xB7A2,0xB8c0,n))   return   'f';
    if   (In(0xB8C1,0xB9FD,n))   return   'g';
    if   (In(0xB9FE,0xBBF6,n))   return   'h';
    if   (In(0xBBF7,0xBFA5,n))   return   'j';
    if   (In(0xBFA6,0xC0AB,n))   return   'k';
    if   (In(0xC0AC,0xC2E7,n))   return   'l';
    if   (In(0xC2E8,0xC4C2,n))   return   'm';
    if   (In(0xC4C3,0xC5B5,n))   return   'n';
    if   (In(0xC5B6,0xC5BD,n))   return   'o';
    if   (In(0xC5BE,0xC6D9,n))   return   'p';
    if   (In(0xC6DA,0xC8BA,n))   return   'q';
    if   (In(0xC8BB,0xC8F5,n))   return   'r';
    if   (In(0xC8F6,0xCBF0,n))   return   's';
    if   (In(0xCBFA,0xCDD9,n))   return   't';
    if   (In(0xCDDA,0xCEF3,n))   return   'w';
    if   (In(0xCEF4,0xD188,n))   return   'x';
    if   (In(0xD1B9,0xD4D0,n))   return   'y';
    if   (In(0xD4D1,0xD7F9,n))   return   'z';
    return   '\0';
}

string MyStrOperation::ChineseToPinyin(wstring chinese)
{
    string sChinese = CCodeOperation::WstringToString(chinese);
    char c = 0;
    char chr[3] = {0};
    wchar_t wchr = 0;
    int len = sChinese.length()/2;
    char* buff = new char[len];
    memset(buff, 0x00, sizeof(wchar_t)*sChinese.length()/2+1);
    size_t i = 0;
    size_t j = 0;
    for (i = 0, j = 0; i < (sChinese.length()/2); ++i)
    {
        memset(chr, 0x00, sizeof(chr));
        chr[0] = sChinese[j++];
        chr[1] = sChinese[j++];
        chr[2] = '\0';
        //  单个字符的编码   如：'我'   =   0xced2
        wchr = 0;
        wchr = (chr[0] & 0xff) << 8;
        wchr |= (chr[1] & 0xff);
        buff[i] = convert(wchr);
    }
    string buffStr = buff;
    return buffStr;
}


std::wstring MyStrOperation::FormatTime( int nSecond, int nDouble )
{
    unsigned int nmS = nSecond * nDouble;
    int nS = nmS / 1000;
    int nM = nS / 60;
    nS = nS % 60;
    int nH = nM / 60;
    nM = nM % 60;
    wchar_t pBuffer[20];    
    //swprintf_s(pBuffer, 20, L"%d:%.2d:%.2d", nH, nM, nS);
    swprintf(pBuffer, 20, L"%d:%.2d:%.2d", nH, nM, nS);
    return wstring(pBuffer);
}

std::wstring MyStrOperation::FormatTime( time_t nTime )
{				
	nTime -= (time_t)CDealTime::GetTimeZone();
    CDealTime dtime(nTime);
    return CCodeOperation::GB2312ToUnicode(dtime.TimeFormat());
}
bool MyStrOperation::IsChineseLetter( wchar_t ch )
{
    if (ch >= 0x4e00 && ch <= 0x9fff)
        return true;
    return false;
}

bool MyStrOperation::IsChineseString( wstring str )
{
    bool b = false;
    for (size_t i=0; i<str.length(); i++)
    {
        if (IsChineseLetter(str[i]))
        {
            b = true;
            break;
        }
    }
    return b;
}

