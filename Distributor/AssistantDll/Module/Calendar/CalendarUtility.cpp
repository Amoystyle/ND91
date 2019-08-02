#include "pch_module.h"

#ifdef MODULE_CALENDAR

#include "CalendarUtility.h"

CalendarUtility::CalendarUtility(void)
{
}

CalendarUtility::~CalendarUtility(void)
{
}

void ND91Assistant::CalendarUtility::SplitRepeatrule( RepeatRule& rrlueObj, string rrlue )
{
	vector<string> splits = CStrOperation::parseStrings(rrlue,';');
	for(size_t i = 0; i < splits.size(); i++)
	{
		vector<string> keys = CStrOperation::parseStrings(splits[i],'=');

		if(keys[0]=="FREQ")
		{
			rrlueObj._frem = CCodeOperation::UTF_8ToUnicode(keys[1]);
		}
		else if(keys[0]=="UNTIL")
		{
			rrlueObj._until = CCodeOperation::UTF_8ToUnicode(ConvertStrToLongDateTime(keys[1]));
		}
		else if(keys[0]== "INTERVAL")
		{
			rrlueObj._interval = atoi(keys[1].c_str());
		}
		else if(keys[0]=="BYDAY")
		{
			ByDays(rrlueObj._byDays, keys[1]);
		}
		else if(keys[0]=="BYMONTHDAY")
		{
			rrlueObj._byMonthday = atoi(keys[1].c_str());
		}
		else if(keys[0]=="COUNT")
		{
			rrlueObj._count = atoi(keys[1].c_str());
		}

	}
}

std::string ND91Assistant::CalendarUtility::ConvertStrToLongDateTime( string s )
{
	if (s.length() >= 12)
	{
		//
		CDealTime newtime(ConvertStringToTime(s)+8*3600);
		return newtime.TimeFormat();
	}
	else if (s.length() >= 8)
	{
		string f = s.substr(0, 4) + "-" + s.substr(4, 2) + "-" + s.substr(6, 2) + "08:00:00";
		return f;
	}
	return "";

}

const char* WEEKDAYS[]=
{
"SU",
"MO",
"TU",
"WE",
"TH",
"FR",
"SA"
};

void ND91Assistant::CalendarUtility::ByDays(Days& d, string s )
{
	d.clear();
	vector<string> splits = CStrOperation::parseStrings(s,',');
	for(size_t i = 0; i< splits.size(); i++)
	{
		int found = -1;
		for(int j = 0; j < 7; j++)
		{
			if(splits[i] == WEEKDAYS[j])
			{
				found = j;
				d.SetDays((RepeatDayType)(1<<j));
				break;
			}
		}
		if(found == -1 && splits[i].length() == 3 || splits[i].length() == 4)
		{
			d.SetDays(ERepeatMonthlyByDay);//这是因为-1 表示最后一个星期
		}
	 }
}

string ND91Assistant::CalendarUtility::RepeatruleToString( RepeatRule repeatObj )
{
	string rrlue = "";

	if (!repeatObj._frem.empty())
	{
		rrlue = "FREQ=" + CCodeOperation::UnicodeToUTF_8(repeatObj._frem) + ";WKST=MO;";
	}

	if (repeatObj._interval != 0 && repeatObj._interval != 1)
	{
		rrlue += "INTERVAL=" + CStrOperation::Int2string(repeatObj._interval) + ";";
	}

	if (!repeatObj._until.empty())
	{
		// TODO:
		string until =CCodeOperation::UnicodeToUTF_8(repeatObj._until);
		__int64 time = CDealTime::StringToTime(repeatObj._until);
		until = ConvertTimeRule(time-8*3600);
		rrlue += "UNTIL=" + until + ";"; //到android会先减去8个小时
	}

	if (repeatObj._count > 0)
	{
		rrlue += "COUNT=" + CStrOperation::Int2string(repeatObj._count) + ";";
	}

	if (!repeatObj._byDays.IsNullOfDays() || !repeatObj._strByDay.empty())
	{
		rrlue += "BYDAY=" + CCodeOperation::UnicodeToUTF_8(repeatObj._strByDay) + ";";
	}

	if (repeatObj._byMonthday != 0)
	{
		rrlue += "BYMONTHDAY=" + CStrOperation::Int2string(repeatObj._byMonthday) + ";";
	}

	//d.ToString("yyyyMM01")
	// FREQ=DAILY;INTERVAL=3;UNTIL=20090223T060000Z;WKST=SU
	// FREQ=WEEKLY;UNTIL=20090212T050035;WKST=SU;BYDAY=MO,TU,WE,TH,FR
	// FREQ=WEEKLY;UNTIL=20090212T050035;WKST=SU;BYDAY=MO,TU,WE,TH,FR
	// FREQ=WEEKLY;WKST=SU;BYDAY=MO,TU,WE,TH,FR

	if (!rrlue.empty())
	{
		rrlue = rrlue.substr(0, rrlue.length() - 1);
	}
	return rrlue;
}

ND91Assistant::RepeatTypes ND91Assistant::CalendarUtility::SelectRepeat( RepeatRule repeatObj )
{
	RepeatTypes type = None;
	wstring Frequency = CStrOperation::toLowerW(repeatObj._frem);

	if (Frequency ==  L"daily")
		type = EveryDay;
	else if( Frequency ==  L"weekly")
		type = EveryWeek;
	else if( Frequency ==  L"monthly")
		type = EveryMonth;
	else if( Frequency ==  L"yearly")
		type = EveryYear;

	return type;
}

int ND91Assistant::CalendarUtility::SelectRepeatWeek( RepeatRule repeatObj )
{
	int week = 0;
	if(!repeatObj._frem.empty())
	{
		if(CStrOperation::toLowerW(repeatObj._frem) == L"weekly")
		{
			week |= repeatObj._byDays.IsRepeat(ERepeatSunday)	? ERepeatSunday : week;
			week |= repeatObj._byDays.IsRepeat(ERepeatMonday)	? ERepeatMonday : week;
			week |= repeatObj._byDays.IsRepeat(ERepeatTuesday)	? ERepeatTuesday : week;
			week |= repeatObj._byDays.IsRepeat(ERepeatWednesday)? ERepeatWednesday : week;
			week |= repeatObj._byDays.IsRepeat(ERepeatThursday)	? ERepeatThursday: week;
			week |= repeatObj._byDays.IsRepeat(ERepeatFriday)	? ERepeatFriday: week;
			week |= repeatObj._byDays.IsRepeat(ERepeatSaturday)	? ERepeatSaturday: week;
		}
	}
	if (week==0)
		week = 1;
	return week;
}

__int64 ND91Assistant::CalendarUtility::SelectWorkDatesEndDateTime( __int64 starttime, int count )
{
	int index = 0;
	 __int64 selectEnd = starttime-24*3600;
	 int week = CDealTime(starttime).GetWeekDay();
	 while(count > 0 && index != count)
	 {
		 selectEnd = selectEnd+24*3600;
		 int weekend = CDealTime(selectEnd).GetWeekDay();
		 if(weekend > 0 && weekend < 6)
			 index++;
	 }
	 return selectEnd;
}

deque<__int64> ND91Assistant::CalendarUtility::GetMonthRepeatTimeList( __int64 dtstart, int count )
{
	deque<__int64> repeatList;
	int index = 0;
	int repeatCount = 0;
	while(count > 0 && repeatCount != count)
	{
		CDealTime time(dtstart);
		int day = time.GetDay();
		time.addMonth(index);
		__int64 repeatTime = time.GetTime();
		if(day == time.GetDay())
		{
			repeatCount++;
			repeatList.push_back(repeatTime);
		}
		index++;
	}
	return repeatList;

}

__int64 ND91Assistant::CalendarUtility::GetUntilTime( CalendarEventData* pEvent )
{
	CDealTime nowtime;
	__int64 until = 0;
	if (!pEvent)
		return until;
	if (!pEvent->_repeatRlue._until.empty() && (int)pEvent->_repeatRlue._until.find(L"T") < 0)
	{
		wstring s = pEvent->_repeatRlue._until;
		until = CDealTime::StringToTime(s);
	}
	else if(pEvent->_phoneCategory == L"lenovo" && pEvent->_repeatRlue._count>0)
	{
		switch(pEvent->_repeatType)
		{
		case None:
			break;
		case EveryDay:
			until = pEvent->_start+(pEvent->_repeatRlue._count-1)*24*3600;
			break;
		case EveryWeek:
		case Every2Weeks:
			{
				int weekcount = SelectRepeatWeek(pEvent->_repeatRlue);
				if(weekcount == 62)
				{
					until = SelectWorkDatesEndDateTime(pEvent->_start, pEvent->_repeatRlue._count);
				}
				else
					until = pEvent->_start+(pEvent->_repeatRlue._count-1)*7*24*3600;
			}
			break;
		case EveryMonth:
			{
				deque<__int64> repeatDates = GetMonthRepeatTimeList(pEvent->_start, pEvent->_repeatRlue._count);
				if (repeatDates.size() > 0)
					until = (*(repeatDates.rbegin()));
				else
				{
					CDealTime montime(pEvent->_start);
					montime.addMonth(pEvent->_repeatRlue._count-1);
					until = montime.GetTime();
				}
			}
			break;
		case EveryYear:
			{
				CDealTime ytime(pEvent->_start);
				ytime.AddYear(pEvent->_repeatRlue._count-1);
				until = ytime.GetTime();
			}
			break;
		default:
			break;
		}
	}
	return until;
}

ND91Assistant::EventDateWhere ND91Assistant::CalendarUtility::ExistInSelectDates( __int64 dt, __int64 selectBeginDate, __int64 selectEndDate )
{
	if(dt < selectBeginDate)
		return ELeft;
	if(dt > selectEndDate)
		return ERight;
	if(dt >= selectBeginDate && dt < selectEndDate)
		return EInner;
	return ENothing;
}

std::wstring ND91Assistant::CalendarUtility::GetWeekDay( __int64 time)
{
	CDealTime day(time);
	int week = CDealTime(time).GetWeekDay();
	switch(week)
	{
		case 0:
			return L"SU";
		case 1:
			return	L"MO";
		case 2:
			return	L"TU";
		case 3:
			return	L"WE";
		case 4:
			return	L"TH";
		case 5:
			return	L"FR";
		case 6:
			return	L"SA";
	}
	return L"";
}

__int64 ND91Assistant::CalendarUtility::GetByWeekDate( __int64 begin, int monthdays )
{
	int number = WeekOfMonth(begin, true);

	int week = CDealTime(begin).GetWeekDay();

	CDealTime next(begin);
	next.addMonth(monthdays);
	__int64 nextmonth = CDealTime(next.GetYear(), next.GetMonth(),1).GetTime();

	switch(number)
	{
		case 1:
		case 2:
		case 3:
		case 4:
			{
				for (int i = (number-1)*7+1; i < number*7; i++)
				{
					__int64 tempmonth = nextmonth+(i-1)*24*3600;
					if (week == CDealTime(tempmonth).GetWeekDay())
						nextmonth = tempmonth;
				}
			}
			break;
		case -1:
			{
				bool hasfive = false;
				CDealTime dealNextMonth(nextmonth);
				dealNextMonth.addMonth(1);
				__int64  towNextMonth = (dealNextMonth.GetTime()-24*3600);
				int mthday = CDealTime(towNextMonth).GetDay();
				if (mthday > 28)
				{
					for (int i = 29; i < mthday; i++)
					{
						__int64 tempmonth = nextmonth+(i-1)*24*3600;
						if (week == CDealTime(tempmonth).GetWeekDay())
						{
							nextmonth = tempmonth;
							hasfive = true;
							break;
						}
					}
				}
				if(!hasfive)
				{
					for (int i = 22; i < 28; i++)
					{
						__int64 tempmonth = nextmonth+(i-1)*24*3600;
						if (week == CDealTime(tempmonth).GetWeekDay())
							nextmonth = tempmonth;
					}
				}
			}
			break;
	}

	return nextmonth;
}

int ND91Assistant::CalendarUtility::WeekOfMonth( __int64 dtSel, bool sundayStart )
{
	int daycount = CDealTime(dtSel).GetDay();
	int index = 1;
	if(0 < daycount && daycount <= 7)
	{
		index = 1;
	}
	else if(8 <= daycount && daycount <= 14)
	{
		index = 2;
	}
	else if(15 <= daycount && daycount <= 21)
	{
		index = 3;
	}
	else if(22 <= daycount && daycount <= 28)
	{
		index = 4;
	}
	else if(29 <= daycount)
	{
		index = -1;
	}
	return index;
}

string ND91Assistant::CalendarUtility::ConvertTimeRule( __int64 time )
{
	CDealTime dt(time);
	string  timestr = "";
	ostringstream oss;
	oss<<dt.GetYear()/1000<<(dt.GetYear()%1000)/100<<((dt.GetYear()%1000)%100)/10<<dt.GetYear()%1000%100%10
		<<dt.GetMonth()/10<<dt.GetMonth()%10<<dt.GetDay()/10<<dt.GetDay()%10<<"T"
		<<dt.GetHour()/10<<dt.GetHour()%10<<dt.GetMinute()/10<<dt.GetMinute()%10
		<<dt.GetSecond()/10<<dt.GetSecond()%10<<"Z";
	timestr = oss.str();
	return timestr;
}

string ND91Assistant::CalendarUtility::GetRepeatRuleWithOut( string oldrrule )
{
	vector<string> parts = CStrOperation::parseStrings(oldrrule, ';');
	string s;
	for (size_t i = 0; i < parts.size(); i++)
	{
		if((int)parts[i].find("UNTIL")>-1)
		{
			s += parts[i]+";";
		}
	}
	return s.substr(0, s.length()-1);
}

__int64 ND91Assistant::CalendarUtility::FollowingInstanceEditUntilDate( CalendarEventData* pEvent)
{
	if(pEvent->_repeatType != None)
	{
		int count = pEvent->_repeatRlue._count;
		__int64 repeatUntil = pEvent->_repeatRlue._repeatEnd;

		__int64 repeatDate = 0;
		switch(pEvent->_repeatType)
		{
			case None:
				repeatDate = pEvent->_end;
				break;
			case EveryDay:
				repeatDate = pEvent->_start+(count-1)*24*3600;
				break;
			case EveryWeek:
			case Every2Weeks:
				{
					int week = SelectRepeatWeek(pEvent->_repeatRlue);
					if(week == 62)
						repeatDate = SelectWorkDatesEndDateTime(pEvent->_start, count);
					else
						repeatDate = pEvent->_start+(count-1)*7*24*3600;
				}
				break;
			case EveryMonth:
				{
					deque<__int64> repeatdates =GetMonthRepeatTimeList(pEvent->_start, count);
					if(repeatdates.size()>0)
						repeatDate = repeatdates[repeatdates.size()-1];
					else
					{
						CDealTime mTime(pEvent->_start);
						mTime.addMonth(count-1);
						repeatDate = mTime.GetTime();
					}
				}
				break;
			case EveryYear:
				{
					deque<__int64> repeatyears = LephoneYearRepeatView(pEvent->_start, count);
					if(repeatyears.size() > 0)
						repeatDate = repeatyears[repeatyears.size()-1];
					else
					{
						CDealTime ytime(pEvent->_start);
						ytime.AddYear(count-1);
						repeatDate = ytime.GetTime();
					}
				}
				break;
			default:
				break;
		}
		if (CDealTime(repeatDate).DaysToTime(CDealTime(repeatUntil)) >= 0)
			return repeatDate;
		else
			return repeatUntil;
	}
	return CDealTime().GetTime();
}

deque<__int64> ND91Assistant::CalendarUtility::LephoneYearRepeatView( __int64 dtStart, int count )
{
	deque<__int64> timelist;
	int index = 0;
	int repeatCount = 0;
	__int64  repeatDatetime = 0;
	while(count > 0 && repeatCount != count)
	{
		CDealTime time(dtStart);
		time.AddYear(index);
		repeatDatetime = time.GetTime();
		if(CDealTime(dtStart).GetWeekDay() == time.GetWeekDay())
		{
			repeatCount++;
			timelist.push_back(repeatDatetime);
		}
		index++;
	}
	return timelist;
}

string ND91Assistant::CalendarUtility::ModifyRepeatRule( __int64 instanceTime, RepeatRule repeatObj )
{
	string timestr =ConvertTimeRule(instanceTime-(8*3600+1));
	string rrule = RepeatruleToString(repeatObj);
	string withoutuntil = GetRepeatRuleWithOut(rrule);
	return withoutuntil+";UNTIL="+timestr;
}

__int64 ND91Assistant::CalendarUtility::ConvertStringToTime( string timestr )
{
	int length	= timestr.length();
	int year	= length > 3?  atoi(timestr.substr(0, 4).c_str()) : 0;
	int month	= length > 5?  atoi(timestr.substr(4, 2).c_str()) : 0;
	int day		= length > 7?  atoi(timestr.substr(6, 2).c_str()) : 0;
	int hour	= length > 10? atoi(timestr.substr(9, 2).c_str()) : 0;
	int minute	= length > 12? atoi(timestr.substr(11, 2).c_str()): 0;
	int secend	= length > 14? atoi(timestr.substr(13, 2).c_str()): 0;

	if (year == 0 || month == 0 || day == 0)
		return 0;
	CDealTime time( year, month, day,hour, minute, secend);
	return time.GetTime();
}


#endif