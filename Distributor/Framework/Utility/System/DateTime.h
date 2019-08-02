#pragma once
#include "../Utility.h"

class UTILITY_API DateTime
{
public:
	DateTime();
	DateTime(time_t timer);
	DateTime(int year, int month, int day);
	DateTime(int year, int month, int day, int hour, int minute, int second);
	DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond);
	~DateTime();

public:
	int GetYear();
	int GetMonth();
	int GetDay();
	int GetDayOfWeek();
	int GetDayOfYear();
	int GetHour();
	int GetMinute();
	int GetSecond();
	int GetMilliseconds();

	int Equal(DateTime& dt);
	bool IsLeap();
	time_t ToTime_T();
	wstring ToString(wstring format = L"yyyy-mm-dd HH:MM:ss:f");

private:
	int CalcDayOfWeek( int year, int month, int day );
	int CalcDayOfYear();
private:
	SYSTEMTIME* _pSystemTime;
};