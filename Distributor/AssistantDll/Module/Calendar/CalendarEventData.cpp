#include "pch_module.h"

#ifdef MODULE_CALENDAR

#include "Module/Calendar/CalendarEventData.h"
#include "Module/Calendar/ReminderData.h"

CalendarEventData::CalendarEventData(void)
{
	_rowid = 1;				// �¼�Id
	_calendarId = 1;		// ����ID,calendar���е�_id
	_instanceId = -1;		// instance���е�_id

	_endSecond = 0;
	_start = CDealTime().GetTime();				// ��ʼʱ��instance begin
	_startSecond = 0;
	_hasChange = false;		// �Ƿ�ı�
	_alarmTime = 0;			// ��������ʱ��
	_weekStart = 1;			// (week_start)
	_alert = false;			// �Ƿ�����
	_alertBefore = -1;		// ����ʱ��ǰ����
    _reAlertBefore = -1;    // �ٴ�����ʱ��ǰ����
	_eventdtend = 0;		// event �϶�Ӧ�Ľ���ʱ��
	_eventdtstart = 0;		// event �ϵĿ�ʼʱ��
	_originalInstanceTime = 0;// ԭʼ�¼�ʵ��ʱ��
	_lastDate = 0;			// ��ֹ����
	_allDay = false;		// �Ƿ�Ϊȫ���¼�
	_repeatCount = 0;		// �ظ�����
	_endRepeatDate = _start+5*24*3600;		// ֹͣ�ظ�����
	_origEventId = 0;		// �����¼�ID
	_origStartDate = 0;		// �����¼���ʼʱ��
	_by_Month_Months = 0;
	_timeZone = L"GMT";
	_repeatForever = true;	// �Ƿ������ظ�
	_eventStatus = 0;		// �¼�״̬
	_instaceBegin = 0;		// ʵ����ʼ�¼�
	_instaceEnd = 0;		// ʵ������ʱ��

	_end = _start+3600;				// ����ʱ�� instance end

	_lephoneCategory = 0;	// �Ƿ���phone

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
		 //TODO:��reminder
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
	
	newCal->_rowid			= _rowid;			// �¼�Id
	newCal->_calendarId		= _calendarId;		// ����ID,calendar���е�_id
	newCal->_instanceId		= _instanceId;		// instance���е�_id
	newCal->_summary		= _summary;			// �¼�����
	newCal->_location		= _location;		// �ص�
	newCal->_endSecond		= _endSecond;
	newCal->_start			= _start;			// ��ʼʱ��instance begin
	newCal->_startSecond	= _startSecond;
	newCal->_hasChange		= _hasChange;		// �Ƿ�ı�
	newCal->_specifier		= _specifier;
	newCal->_alarmTime		= _alarmTime;		// ��������ʱ��
	newCal->_weekStart		= _weekStart;		// (week_start)
	newCal->_alert			= _alert ;			// �Ƿ�����
	newCal->_alertBefore	= _alertBefore;		// ����ʱ��ǰ����
	newCal->_description	= _description;		// ����
	newCal->_eventdtend		= _eventdtend;		// event �϶�Ӧ�Ľ���ʱ��
	newCal->_eventdtstart	= _eventdtstart;	// event �ϵĿ�ʼʱ��
	newCal->_originalInstanceTime = _originalInstanceTime;
	newCal->_lastDate		= _lastDate;
	newCal->_repeatType		= _repeatType;		// �ظ���ʽ �͵ķ�ʽ
	newCal->_allDay			= _allDay;			// �Ƿ�Ϊȫ���¼�
	newCal->_repeatCount	= _repeatCount;		// �ظ�����
	newCal->_endRepeatDate	= _endRepeatDate;	// ֹͣ�ظ�����
	newCal->_origEventId	= _origEventId;		// �����¼�ID
	newCal->_originalEvent	= _originalEvent;
	newCal->_repeatRlue		= _repeatRlue;		// �ظ���
	newCal->_origStartDate	= _origStartDate;	// �����¼���ʼʱ��
	newCal->_by_Month_Months = _by_Month_Months;
	newCal->_timeZone		= _timeZone;		// ʱ��
	newCal->_repeatForever	= _repeatForever;	// �Ƿ������ظ�
	newCal->_syncId			= _syncId;			// ��Google������ͬ����id
	newCal->_eventStatus	= _eventStatus;
	newCal->_duration		= _duration;		//���ʱ�䣨��ʼ��������p3600s p2d
	newCal->_instaceBegin	= _instaceBegin;
	newCal->_instaceEnd		= _instaceEnd;

	newCal->_end			= _end;				// ����ʱ�� instance end

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