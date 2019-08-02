#include "pch_module.h"

#ifdef MODULE_CALENDAR

#include "Module/Calendar/CalendarEventData.h"
#include "Module/Calendar/ReminderData.h"

CalendarEventData::CalendarEventData(void)
{
	_rowid = 1;				// 事件Id
	_calendarId = 1;		// 日历ID,calendar表中的_id
	_instanceId = -1;		// instance表中的_id

	_endSecond = 0;
	_start = CDealTime().GetTime();				// 开始时间instance begin
	_startSecond = 0;
	_hasChange = false;		// 是否改变
	_alarmTime = 0;			// 闹钟提醒时间
	_weekStart = 1;			// (week_start)
	_alert = false;			// 是否提醒
	_alertBefore = -1;		// 提醒时提前的秒
    _reAlertBefore = -1;    // 再次提醒时提前的秒
	_eventdtend = 0;		// event 上对应的结束时间
	_eventdtstart = 0;		// event 上的开始时间
	_originalInstanceTime = 0;// 原始事件实例时间
	_lastDate = 0;			// 截止日期
	_allDay = false;		// 是否为全天事件
	_repeatCount = 0;		// 重复次数
	_endRepeatDate = _start+5*24*3600;		// 停止重复日期
	_origEventId = 0;		// 关联事件ID
	_origStartDate = 0;		// 关联事件开始时间
	_by_Month_Months = 0;
	_timeZone = L"GMT";
	_repeatForever = true;	// 是否无限重复
	_eventStatus = 0;		// 事件状态
	_instaceBegin = 0;		// 实例起始事件
	_instaceEnd = 0;		// 实例结束时间

	_end = _start+3600;				// 结束时间 instance end

	_lephoneCategory = 0;	// 是否乐phone

    _occurrenceDay = 0;
    _exceptionDate = L"";
    _interval = 0;

}

CalendarEventData::~CalendarEventData(void)
{
	for(size_t i = 0; i < _reminders.size(); i++)
	{
		SAFE_DELETE(_reminders[i]);
	}
	_reminders.clear();

	map<int, CalendarEventData*>::iterator it = _cancelEventList.begin();
	while( it != _cancelEventList.end())
	{
		SAFE_DELETE(it->second);
		it++;
	}
	_cancelEventList.clear();
}

/*void ND91Assistant::CalendarEventData::ParseAndroidData( BinaryBufferReader& reader, bool IsNew )
{
	reader.ReadInt(_calendarId);
	reader.ReadInt(_rowid);
	reader.ReadStringFromUTF8(_summary);
	reader.ReadStringFromUTF8(_location);
	reader.ReadStringFromUTF8(_description);
	_description = CStrOperation::ReplaceW(CStrOperation::ReplaceW(_description, L"\r\n", L"\n"),L"\n", L"\r\n");
	reader.ReadInt64(_start);
	reader.ReadInt64(_end);
	int allday = 0;
	reader.ReadInt(allday);
	_allDay = (allday == 1)?true:false;
	int hasAlert =0;
	reader.ReadInt(hasAlert);
	_alert = (hasAlert==1)?true:false;
	string rruleStr;
	reader.ReadString(rruleStr);

	if(!rruleStr.empty())
	{
		CalendarUtility::SplitRrlue(_repeatRlue, rruleStr);
		if(_repeatRlue._until.empty())
		{
			_repeatForever = true;
		}
		else
		{
			_repeatForever = false;
			wstring until = _repeatRlue._until;
			CDealTime endRepeattime(_wtoi(until.substr(0,4).c_str()), 
									_wtoi(until.substr(5,2).c_str()),
									_wtoi(until.substr(8,2).c_str()), 
									_wtoi(until.substr(11,2).c_str()), 
									_wtoi(until.substr(14,2).c_str()), 
									_wtoi(until.substr(17,2).c_str()));
			_endRepeatDate = endRepeattime.GetTime()*1000;
		}
		_repeatType = CalendarUtility::SelectRepeat(_repeatRlue);
	}

	reader.ReadInt(_eventStatus);
	reader.ReadStringFromUTF8(_timeZone);
	reader.ReadStringFromUTF8(_duration);
	int distance = 0;
	if (!_duration.empty())
		distance = _wtoi(_duration.substr(1, _duration.length()-2).c_str());
	if(_allDay)
	{
		CDealTime time(_start/1000);
		CDealTime newtime(time.GetYear(),time.GetMonth(),time.GetDay());
		_start = (__int64)(newtime.GetTime()*1000);

		if(distance > 0)
			_end = _start +distance*24*3600*1000;
		else
		{
			CDealTime endTime(_end/1000);
			CDealTime newEndTime(endTime.GetYear(),endTime.GetMonth(),endTime.GetDay());
			_end = (__int64)(newEndTime.GetTime()*1000);
		}
	}
	else if(distance > 0)
	{
		_end = _start +distance*1000;
	}
	
	reader.ReadStringFromUTF8(_originalEvent);
	reader.ReadInt64(_originalInstanceTime);
	reader.ReadInt64(_lastDate);
	if(!IsNew)
	{
		reader.ReadInt64(_instaceBegin);
		reader.ReadInt64(_instaceEnd);
	}
	reader.ReadStringFromUTF8(_syncId);
	if (IsNew)
		return;

	reader.ReadInt(_lephoneCategory);
	reader.ReadStringFromUTF8(_phoneCategory);
	reader.ReadStringFromUTF8(_exdate);

	int count = 0;
	 reader.ReadInt(count);
	 for (int i = 0; i < count; i++)
	 {
		 //TODO:读reminder
		 ReminderData* pReminder = new ReminderData();
		 pReminder->ParseAndroidData(reader);
		 _reminders.push_back(pReminder);
	 }
	 int cancelCount = 0;

	 for(int j = 0; j < cancelCount; j++)
	 {
		 //TODO:
	 }
	
}

void ND91Assistant::CalendarEventData::AndroidDataToStream( BinaryBufferWriter& writer )
{
	writer.WriteInt(_calendarId);
	writer.WriteInt(_rowid);
	writer.WriteStringAsUTF8(_summary);
	writer.WriteStringAsUTF8(_location);
	writer.WriteStringAsUTF8(CStrOperation::ReplaceW(_description,L"\r\n", L"\n"));
	__int64 starttime = _allDay?_start+8*3600*1000:_start;
	__int64 endtime =  _allDay?_end+8*3600*1000:_end;

	writer.WriteInt64(starttime);
	writer.WriteInt64(endtime);
	writer.WriteInt(_allDay?1:0);
	writer.WriteInt(_alert?1:0);
	writer.WriteString(CalendarUtility::Rrlue(_repeatRlue));  //rule;
	if (_eventStatus != 2)
		writer.WriteChar(0);
	else
	{
		writer.WriteChar(1);
		writer.WriteInt(_eventStatus);
	}
	writer.WriteStringAsUTF8(_timeZone);
	writer.WriteStringAsUTF8(_duration);
	writer.WriteInt64(_originalInstanceTime);
	writer.WriteInt64(_lastDate);

	writer.WriteInt64(_instaceBegin);
	writer.WriteInt64(_instaceEnd);
	writer.WriteInt(_lephoneCategory);
	writer.WriteStringAsUTF8(_phoneCategory);
	writer.WriteStringAsUTF8(_exdate);

	if(_reminders.size() > 0)
	{
		writer.WriteChar(1);
		writer.WriteInt(_reminders.size());
		for (size_t i = _reminders.size() -1; i >= 0; i--)
		{
			//TODO:
			_reminders.at(i)->AndroidDataToStream(writer);
		}
	}
	else
	{
		writer.WriteChar(0);
	}

	}*/

CalendarEventData* ND91Assistant::CalendarEventData::Clone()
{
	CalendarEventData* newCal = new CalendarEventData();
	
	newCal->_rowid			= _rowid;			// 事件Id
	newCal->_calendarId		= _calendarId;		// 日历ID,calendar表中的_id
	newCal->_instanceId		= _instanceId;		// instance表中的_id
	newCal->_summary		= _summary;			// 事件名称
	newCal->_location		= _location;		// 地点
	newCal->_endSecond		= _endSecond;
	newCal->_start			= _start;			// 开始时间instance begin
	newCal->_startSecond	= _startSecond;
	newCal->_hasChange		= _hasChange;		// 是否改变
	newCal->_specifier		= _specifier;
	newCal->_alarmTime		= _alarmTime;		// 闹钟提醒时间
	newCal->_weekStart		= _weekStart;		// (week_start)
	newCal->_alert			= _alert ;			// 是否提醒
	newCal->_alertBefore	= _alertBefore;		// 提醒时提前的秒
	newCal->_description	= _description;		// 描述
	newCal->_eventdtend		= _eventdtend;		// event 上对应的结束时间
	newCal->_eventdtstart	= _eventdtstart;	// event 上的开始时间
	newCal->_originalInstanceTime = _originalInstanceTime;
	newCal->_lastDate		= _lastDate;
	newCal->_repeatType		= _repeatType;		// 重复方式 就的方式
	newCal->_allDay			= _allDay;			// 是否为全天事件
	newCal->_repeatCount	= _repeatCount;		// 重复次数
	newCal->_endRepeatDate	= _endRepeatDate;	// 停止重复日期
	newCal->_origEventId	= _origEventId;		// 关联事件ID
	newCal->_originalEvent	= _originalEvent;
	newCal->_repeatRlue		= _repeatRlue;		// 重复类
	newCal->_origStartDate	= _origStartDate;	// 关联事件开始时间
	newCal->_by_Month_Months = _by_Month_Months;
	newCal->_timeZone		= _timeZone;		// 时区
	newCal->_repeatForever	= _repeatForever;	// 是否无限重复
	newCal->_syncId			= _syncId;			// 与Google行事历同步的id
	newCal->_eventStatus	= _eventStatus;
	newCal->_duration		= _duration;		//间隔时间（开始到结束）p3600s p2d
	newCal->_instaceBegin	= _instaceBegin;
	newCal->_instaceEnd		= _instaceEnd;

	newCal->_end			= _end;				// 结束时间 instance end

	newCal->_lephoneCategory = _lephoneCategory;
	newCal->_phoneCategory	= _phoneCategory;
	newCal->_exdate			= _exdate;

	for (size_t i=0; i < _reminders.size(); i++)
	{
		newCal->_reminders.push_back(_reminders[i]->Clone());
	}

	map<int, CalendarEventData*>::iterator it = _cancelEventList.begin();
	while(it != _cancelEventList.end())
	{
		newCal->_cancelEventList[it->second->_rowid]=it->second->Clone();
		it++;
	}

	map<int, CalendarEventData*>::iterator itUp =  _updateEventList.begin();

	while(itUp != _updateEventList.end())
	{
		newCal->_updateEventList[itUp->second->_rowid]=itUp->second;
		itUp++;
	}
	return newCal;
}

void ND91Assistant::CalendarEventData::ConvertFrequency()
{
	wstring str = L"";
	switch (_repeatType)
	{
		case None:
			break;
		case EveryDay:
			str = L"DAILY";
			break;
		case EveryWeek:
			str = L"WEEKLY";
			break;
		case Every2Weeks:
			str = L"WEEKLY";
			break;
		case EveryMonth:
			str = L"MONTHLY";
			break;
		case EveryYear:
			str = L"YEARLY";
			break;
		default:
			break;
	}
	_repeatRlue._frem = str;
}

void ND91Assistant::CalendarEventData::UpdateReminders( int alertBeforeMinutes )
{
	for(size_t i = 0; i < _reminders.size(); i++)
	{
		SAFE_DELETE(_reminders[i]);
	}
	_reminders.clear();

	if(alertBeforeMinutes > 0)
	{
		ReminderData* premind = new ReminderData();
		premind->_eventId = _rowid;
		premind->_minutes = alertBeforeMinutes;
		_reminders.push_back(premind);
		_alertBefore = alertBeforeMinutes;
		_alert = true;
		_alarmTime = _start + alertBeforeMinutes*60;
	}
	else
		_alert = false;
}


#endif