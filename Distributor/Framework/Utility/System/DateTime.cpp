#include "stdafx.h"
#include "DateTime.h"

DateTime::DateTime()
{
	_pSystemTime = new SYSTEMTIME();

	GetLocalTime(_pSystemTime);
}

DateTime::DateTime( time_t timer )
{
	struct tm gm = *gmtime(&timer);

	_pSystemTime = new SYSTEMTIME();
	_pSystemTime->wYear = gm.tm_year + 1900;
	_pSystemTime->wMonth = gm.tm_mon + 1;
	_pSystemTime->wDay = gm.tm_mday;
	_pSystemTime->wDayOfWeek = gm.tm_wday;
	_pSystemTime->wHour = gm.tm_hour + 8;
	_pSystemTime->wMinute = gm.tm_min;
	_pSystemTime->wSecond = gm.tm_sec;
	_pSystemTime->wMilliseconds = (timer / 0x2710L) % 0x3e8L;
}

DateTime::DateTime( int year, int month, int day )
{
	_pSystemTime = new SYSTEMTIME();
	_pSystemTime->wYear = year;
	_pSystemTime->wMonth = month;
	_pSystemTime->wDay = day;
	_pSystemTime->wDayOfWeek = this->CalcDayOfWeek(year, month, day);
	_pSystemTime->wHour = 0;
	_pSystemTime->wMinute = 0;
	_pSystemTime->wSecond = 0;
	_pSystemTime->wMilliseconds = 0;
}

DateTime::DateTime( int year, int month, int day, int hour, int minute, int second )
{
	_pSystemTime = new SYSTEMTIME();
	_pSystemTime->wYear = year;
	_pSystemTime->wMonth = month;
	_pSystemTime->wDay = day;
	_pSystemTime->wDayOfWeek = this->CalcDayOfWeek(year, month, day);
	_pSystemTime->wHour = hour;
	_pSystemTime->wMinute = minute;
	_pSystemTime->wSecond = second;
	_pSystemTime->wMilliseconds = 0;

}

DateTime::DateTime( int year, int month, int day, int hour, int minute, int second, int millisecond )
{
	_pSystemTime = new SYSTEMTIME();
	_pSystemTime->wYear = year;
	_pSystemTime->wMonth = month;
	_pSystemTime->wDay = day;
	_pSystemTime->wDayOfWeek = this->CalcDayOfWeek(year, month, day);
	_pSystemTime->wHour = hour;
	_pSystemTime->wMinute = minute;
	_pSystemTime->wSecond = second;
	_pSystemTime->wMilliseconds = millisecond;
}

DateTime::~DateTime()
{
	SAFE_DELETE(_pSystemTime);
}

int DateTime::GetYear()
{
	return _pSystemTime->wYear;
}

int DateTime::GetMonth()
{
	return _pSystemTime->wMonth;
}

int DateTime::GetDay()
{
	return _pSystemTime->wDay;
}

int DateTime::GetDayOfWeek()
{
	return _pSystemTime->wDayOfWeek;
}

int DateTime::GetDayOfYear()
{
	return this->CalcDayOfYear();
}

int DateTime::GetHour()
{
	return _pSystemTime->wHour;
}

int DateTime::GetMinute()
{
	return _pSystemTime->wMinute;
}

int DateTime::GetSecond()
{
	return _pSystemTime->wSecond;
}

int DateTime::GetMilliseconds()
{
	return _pSystemTime->wMilliseconds;
}

int DateTime::CalcDayOfWeek( int year, int month, int day )
{
	int y = year%100;
	int c = year/100;
	int m = 0;
	if(month <= 0)
		month = 1;
	else if(month > 12)
		month -= 12;

	if(month <= 2)
		m = month + 12;
	else
		m = month;

	int d = day;
	int w = y + (y/4) + (c/4) - 2*c + (26*(m+1)/10) + d - 1;
	int wd = w%7;

	return wd;
}

int DateTime::CalcDayOfYear()
{
	int days[12] = {31, 31, 31, 61, 92, 123, 153, 184, 215, 245, 276, 306};

	int yd = days[11];
	if(this->_pSystemTime->wMonth > 2)
	{
		if(this->IsLeap())
			yd += 29;
		else
			yd += 28;
	}
	yd += this->_pSystemTime->wDay;
	return yd;
}

bool DateTime::IsLeap()
{
	if ((this->_pSystemTime->wYear % 4) != 0)
	{
		return false;
	}
	if ((this->_pSystemTime->wYear % 100) == 0)
	{
		return ((this->_pSystemTime->wYear % 400) == 0);
	}
	return true;

}

time_t DateTime::ToTime_T()
{
	struct tm gm = {
		this->GetSecond(), 
		this->GetMinute(), 
		this->GetHour(), 
		this->GetDay(), 
		this->GetMonth() - 1, 
		this->GetYear() - 1900, 
		this->GetDayOfWeek(), 0, 0};
	return mktime(&gm);
}

std::wstring DateTime::ToString( wstring format /*= L"yyyy-mm-dd HH:MM:ss:ff"*/ )
{
	wstring result = format;

	if(format.find(L"yyyy") >= 0)
	{
		result = CStrOperation::ReplaceW(result, L"yyyy", CStrOperation::IntToWString(this->GetYear()));
	}
	else if(format.find(L"yy") >= 0)
	{
		result = CStrOperation::ReplaceW(result, L"yy", CStrOperation::IntToWString(this->GetYear()));
	}
	else
		throw;

	wstring month = L"";
	if(this->GetMonth() < 10)
		month = L"0" + CStrOperation::IntToWString(this->GetMonth());
	else
		month = CStrOperation::IntToWString(this->GetMonth());

	wstring day = L"";
	if(this->GetDay() < 10)
		day = L"0" + CStrOperation::IntToWString(this->GetDay());
	else
		day = CStrOperation::IntToWString(this->GetDay());

	wstring hour = L"";
	if(this->GetHour() < 10)
		hour = L"0" + CStrOperation::IntToWString(this->GetHour());
	else
		hour = CStrOperation::IntToWString(this->GetHour());

	wstring minute = L"";
	if(this->GetMinute() < 10)
		minute = L"0" + CStrOperation::IntToWString(this->GetMinute());
	else
		minute = CStrOperation::IntToWString(this->GetMinute());

	wstring second = L"";
	if(this->GetSecond() < 10)
		second = L"0" + CStrOperation::IntToWString(this->GetSecond());
	else
		second = CStrOperation::IntToWString(this->GetSecond());

	wstring msecond = L"";
	if(this->GetMilliseconds() < 10)
		msecond = L"0" + CStrOperation::IntToWString(this->GetMilliseconds());
	else
		msecond = CStrOperation::IntToWString(this->GetMilliseconds());

	result = CStrOperation::ReplaceW(result, L"mm", month);
	result = CStrOperation::ReplaceW(result, L"dd", day);
	result = CStrOperation::ReplaceW(result, L"HH", hour);
	result = CStrOperation::ReplaceW(result, L"MM", minute);
	result = CStrOperation::ReplaceW(result, L"ss", second);
	result = CStrOperation::ReplaceW(result, L"f", msecond);
	return result;
}

int DateTime::Equal( DateTime& dt )
{
	time_t t1 = this->ToTime_T();
	time_t t2 = dt.ToTime_T();
	if(t1 == t2)
		return 0;
	else if(t1 > t2)
		return 1;
	else
		return -1;
}

