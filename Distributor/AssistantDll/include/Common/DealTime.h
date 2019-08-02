#pragma once

#ifdef WIN32
    #include "AssistantDll.h"
#else
    #include "Common/winPort.h"
#endif
#include <time.h>
#include <string>
#pragma warning( disable : 4996 )

using namespace std;
//this is a class based on second,that can handle time operater
class CLASSINDLL_CLASS_DECL CDealTime
{
public:
    // Summary  : ����Ϊ��ǰʱ��
    CDealTime();

    // Summary  : ����Ϊָ��ʱ��
    CDealTime(time_t tm);

    // Summary  : ����Ϊָ��ʱ��
    CDealTime(int nYear, int nMonth, int nDay, int nHour=0, int nMin=0, int nSec=0);

	// ʱ���ʽyyyy-MM-dd HH:mm:ssת��Ϊʱ��
	static time_t StringToTime(wstring timeformat);

    time_t    GetTime()   const;        //gain time_t value
    int        GetYear()   const;
    int        GetMonth()  const;
    int        GetDay()    const;
    int        GetHour()   const;
    int        GetMinute() const;
    int        GetSecond() const;
	int        GetWeekDay()const;

	time_t	   addMonth(int count);
    time_t     AddDay(int count);

	time_t     AddYear(int count);

	int        DaysToTime(const CDealTime& tm);

	CDealTime  WeekRange();       

	string	   TimeFormat();// yyyy-MM-dd HH:mm:ss
	wstring    GetGmtTime();// Sat,01-Jan-2000 00:00:00 GMT 

    CDealTime  operator-(const CDealTime& tm);
    CDealTime  operator+(const CDealTime& tm);

    // IOS��׼ʱ��Ϊ��2001-01-01 00:00 
    // �밴ʱ������, ���綫��������08:00��ʼ
    static CDealTime IosStartTime()
    {
        return CDealTime(2001,1,1,0,0,0) - GetTimeZone();
    }

	static double  ToDATE(time_t tm);

	static time_t  FromDATE(double dt);

	static int GetTimeZone()
	{
#ifdef WIN32
		long t;
		_get_timezone(&t);
		return t;
#else
		struct timeval tv;
		struct timezone tz;
		gettimeofday(&tv,&tz);
		return tz.tz_minuteswest * 60;
#endif
	}

private:
    // Summary  : �Ƿ�������
    bool IsLeap(int year);

    time_t _seconds;
    struct tm _times;
};
