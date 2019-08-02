#include "pch_module.h"

#ifdef MODULE_ALARM

#include "Module/Alarm/AlarmData.h"

AlarmData::AlarmData(void)
{
	_id = 0;
	_daysofweek = 0;
	_hour = 8;
	_minutes = 0;
	_alarmtime = 0;
	_enabled = true;
	_vibrate = true;
	_isEdit  = false;
	_ringuri = L"<default>";
	_ios_guid = CFileOperation::GetGuidW();
	_ios_allowSnooze = true;
}

AlarmData::~AlarmData(void)
{
}

bool ND91Assistant::AlarmData::ParseAndroidData( BinaryBufferReader& reader )
{
	reader.ReadInt32(_id);
	reader.ReadInt32(_hour);
	reader.ReadInt32(_minutes);
	reader.ReadInt32(_daysofweek);
	reader.ReadInt64(_alarmtime);
	int enable = 0;
	reader.ReadInt32(enable);
	_enabled = enable==1?true:false;
	int vibrate = 0;
	reader.ReadInt32(vibrate);
	_vibrate = vibrate==1?true:false;
	reader.ReadStringFromUTF8(_message);
	reader.ReadStringFromUTF8(_ringuri);
	if(_ringuri == L"<default>")
		_ringuri = L"content://settings/system/ringtone";
	return true;
}

bool ND91Assistant::AlarmData::IsDayWeekup( AlarmWeekDay day )
{
	return ( (_daysofweek&day) == day);
}

void ND91Assistant::AlarmData::SetWeekdays( AlarmWeekDay day, bool weekup )
{
	if(weekup)
		_daysofweek |= day;
	else
	{
		if((_daysofweek&day) == day)
			_daysofweek -= day;
	}

}

void ND91Assistant::AlarmData::AndroidDataToStream( BinaryBufferWriter& writer )
{
	writer.WriteInt(_id);
	writer.WriteInt(_hour);
	writer.WriteInt(_minutes);
	writer.WriteInt(_daysofweek);
	writer.WriteInt64(_alarmtime);
	writer.WriteInt(_enabled?1:0);
	writer.WriteInt(_vibrate?1:0);
	writer.WriteStringAsUTF8(_message);
	if(_ringuri == L"<default>")
		_ringuri = L"content://settings/system/ringtone";
	writer.WriteStringAsUTF8(_ringuri);
}

AlarmData* ND91Assistant::AlarmData::Clone()
{
	AlarmData* alarm = new AlarmData();
	alarm->_hour = this->_hour;		
	alarm->_minutes = this->_minutes;	
	alarm->_alarmtime = this->_alarmtime;
	alarm->_enabled = this->_enabled;	
	alarm->_vibrate = this->_vibrate;	
	alarm->_message = this->_message;	
	alarm->_ringTittle = this->_ringTittle;
	alarm->_id = this->_id;        
	alarm->_daysofweek = this->_daysofweek;
	alarm->_isEdit = this->_isEdit;	
	alarm->_ringuri = this->_ringuri;	
	return alarm;
}


#endif