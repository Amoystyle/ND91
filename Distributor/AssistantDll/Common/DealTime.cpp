#include "stdafx.h"

#include "Common/DealTime.h"

CDealTime::CDealTime()
{
    _seconds = time(NULL);
    _times = (*gmtime(&_seconds));
}

CDealTime::CDealTime(time_t tm)
{
    _seconds = tm;
    _times = (*gmtime(&_seconds));
}

const int DAYS[2][12] = {
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, },
    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, },
};

bool CDealTime::IsLeap(int year)
{
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

CDealTime::CDealTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec)
{
    _times.tm_sec = nSec;           // Seconds after minute (0 每 59).
    _times.tm_min = nMin;           // Minutes after hour (0 每 59).
    _times.tm_hour = nHour;         // Hours after midnight (0 每 23).

    assert(nDay >= 1 && nDay <= 31);
    _times.tm_mday = nDay;          // Day of month (1 每 31).

    assert(nMonth >= 1 && nMonth <= 12);
    _times.tm_mon = nMonth - 1;     // Month (0 每 11; January = 0).
    
    assert(nYear >= 1900);
    _times.tm_year = nYear - 1900;  // Year (current year minus 1900).
    
    _times.tm_isdst = 0;            //  0 if daylight saving time is not in effect

    _times.tm_wday = 0;
    _times.tm_yday = 0;
    _seconds = mktime(&_times);

    if (_seconds == -1)
    {
        _seconds = 0;
        _times = (*gmtime(&_seconds));
    }
}

time_t CDealTime::GetTime() const
{
    return _seconds;
}

int CDealTime::GetYear() const
{
    return _times.tm_year + 1900;   // Year (current year minus 1900).
}

int CDealTime::GetMonth() const
{
    return _times.tm_mon + 1;       // Month (0 每 11; January = 0).
}

int CDealTime::GetDay() const
{
    return _times.tm_mday;
}

int CDealTime::GetWeekDay() const
{
	return _times.tm_wday;
}

int CDealTime::GetHour() const
{
    return _times.tm_hour;
}

int CDealTime::GetMinute() const
{
    return _times.tm_min;
}

int CDealTime::GetSecond() const
{
    return _times.tm_sec;
}

CDealTime  CDealTime::operator+(const CDealTime& tm)
{
    return CDealTime(_seconds + tm._seconds);
}

CDealTime  CDealTime::operator-(const CDealTime& tm)
{
    return CDealTime(_seconds - tm._seconds);
}

double  CDealTime::ToDATE(time_t tm)
{
	return (tm+timezone)/(24.0*60.0*60.0)+25569;
}

time_t  CDealTime::FromDATE(double dt)
{
	return (time_t)((dt-25569)*(24*60*60)-timezone+0.5);
}


string CDealTime::TimeFormat()
{
	string  timestr = "";
	ostringstream oss;
	oss<<GetYear()/1000<<(GetYear()%1000)/100<<((GetYear()%1000)%100)/10<<GetYear()%1000%100%10
		<<"-"<<GetMonth()/10<<GetMonth()%10<<"-"<<GetDay()/10<<GetDay()%10<<" "
		<<GetHour()/10<<GetHour()%10<<":"<<GetMinute()/10<<GetMinute()%10<<":"
		<<GetSecond()/10<<GetSecond()%10;
	timestr = oss.str();
	return timestr;
}

wstring CDealTime::GetGmtTime()
{
	struct tm *newtime; 
	__int64 ltime; 

	_time64( &ltime ); 

	/*   Obtain   coordinated   universal   time:   */ 
	newtime = _gmtime64(&ltime); 
  
	string dayTime = asctime( newtime );
	dayTime += "GMT";
	return CCodeOperation::GB2312ToUnicode(dayTime);
}

time_t CDealTime::addMonth( int count )
{
	int month = _times.tm_mon +count+1;
	_times.tm_year += (month-1)/12;
	_times.tm_mon = month%12;
	_seconds = mktime(&_times);
	return _seconds;
}

time_t CDealTime::AddDay(int count)
{
    _seconds += count*24*3600;
	_times = (*gmtime(&_seconds));
    return _seconds;
}

time_t CDealTime::AddYear( int count )
{
	_times.tm_year += count;
	_seconds = mktime(&_times);
	return _seconds;
}

int CDealTime::DaysToTime( const CDealTime& tm )
{
	CDealTime time(GetYear(), GetMonth(), GetDay());
	CDealTime timeTo(tm.GetYear(), tm.GetMonth(), tm.GetDay());
	int days = (int)((timeTo.GetTime()-time.GetTime())/(24*3600));
	return days;
}

CDealTime CDealTime::WeekRange()
{
	return CDealTime(_seconds-_times.tm_wday*24*3600);
}

time_t CDealTime::StringToTime( wstring timeformat )
{
	vector<wstring> splits = CStrOperation::parseStringsW(timeformat, L" ");
	wstring datestr = splits.size()>0?splits[0]:L"";
	wstring timestr = splits.size()>1?splits[1]:L"";
	if (datestr.empty())
		return 0;
	int year=0, month=0, day=0, hour=0, minute=0, secend=0;
	splits.clear();
	splits = CStrOperation::parseStringsW(datestr, L"-");
// 	year = splits.size() > 0?_wtoi(splits[0].c_str()):0;
// 	month = splits.size() > 1?_wtoi(splits[1].c_str()):0;
// 	day = splits.size() > 2?_wtoi(splits[2].c_str()):0;
    year = splits.size() > 0?atoi(CCodeOperation::UnicodeToUTF_8(splits[0]).c_str()):0;
    month = splits.size() > 1?atoi(CCodeOperation::UnicodeToUTF_8(splits[1]).c_str()):0;
    day = splits.size() > 2?atoi(CCodeOperation::UnicodeToUTF_8(splits[2]).c_str()):0;

	if(!timestr.empty())
	{
		splits.clear();
		splits = CStrOperation::parseStringsW(timestr, L":");
// 		hour = splits.size() > 0?_wtoi(splits[0].c_str()):0;
// 		minute = splits.size() > 1?_wtoi(splits[1].c_str()):0;
// 		secend = splits.size() > 2?_wtoi(splits[2].c_str()):0;
        hour = splits.size() > 0?atoi(CCodeOperation::UnicodeToUTF_8(splits[0]).c_str()):0;
        minute = splits.size() > 1?atoi(CCodeOperation::UnicodeToUTF_8(splits[1]).c_str()):0;
        secend = splits.size() > 2?atoi(CCodeOperation::UnicodeToUTF_8(splits[2]).c_str()):0;
	}
	return CDealTime(year, month, day, hour, minute, secend).GetTime();
}