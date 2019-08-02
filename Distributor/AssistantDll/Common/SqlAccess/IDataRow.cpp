#include "stdafx.h"

#include "IDataRow.h"


ND91Assistant::IDataRow::IDataRow()
{
	_pVector = NULL;
}

ND91Assistant::IDataRow::~IDataRow()
{
	if(_pVector)
	{
		_pVector->clear();
		SAFE_DELETE(_pVector);
	}
}

wstring ND91Assistant::IDataRow::timeToStr( time_t t )
{
		//struct tm when;
		//gmtime_s(&when, &t);
    struct tm when = (*gmtime(&t));

	wchar_t buffer[32];
	//swprintf_s(buffer, 31, L"%4d.%02d.%02d-%02d:%02d:%02d",
	swprintf(buffer, 31, L"%4d.%02d.%02d-%02d:%02d:%02d",
		1900+when.tm_year, when.tm_mon, when.tm_mday,
		when.tm_hour, when.tm_min, when.tm_sec);

	return buffer;
}

time_t ND91Assistant::IDataRow::MacToPc( time_t t )
{
    return t + (SECOND_OF_31_YEAR - CDealTime::GetTimeZone());
}

time_t ND91Assistant::IDataRow::PcToMac( time_t t )
{
	return t - (SECOND_OF_31_YEAR - CDealTime::GetTimeZone());
}

wstring ND91Assistant::IDataRow::Int64ToStr( /*__int64*/int64_t i )
{
	wchar_t buffer[32];
#ifdef WIN32
    _i64tow_s(i, buffer, 31, 10);
#else
    swprintf(buffer, 31, L"%ld", i);
#endif
    return buffer;
}

wstring ND91Assistant::IDataRow::Int32ToStr( /*__int32*/int32_t i )
{
    wchar_t buffer[16];
    CStrOperation::MyItows(i, buffer, 16);
    return buffer;
}

wstring ND91Assistant::IDataRow::ConvertStr( wstring s )
{
	CStrOperation::ReplaceW(s, L"'", L"''");	
	s.insert(s.begin(), '\'');	
	if(s[s.length()-1] == '\0')
		s[s.length()-1] = '\'';
	else
	  s.push_back('\'');	
	return s;
}
wstring ND91Assistant::IDataRow::BinToStr( string s)
{
	//‘›≤ª¥¶¿Ì 
	
return L"0";
}

vector<wstring>* ND91Assistant::IDataRow::GetVector()
{
	if ( ! _pVector )
	{
		_pVector = new vector<wstring>;
	}
	else
	{
		_pVector->clear();
	}

	return _pVector;
}
