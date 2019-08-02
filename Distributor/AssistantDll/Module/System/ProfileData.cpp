#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "ProfileData.h"

ProfileData::ProfileData(void)
{
	_ringModeIndex = -1;      // 响铃模式枚举值
	_preload = false;
	_modify  = false;
}

ProfileData::~ProfileData(void)
{
}

void ND91Assistant::ProfileData::ParseAndroidData( BinaryBufferReader& reader )
{
    reader.ReadInt32(_id);
	reader.ReadStringFromUTF8(_name);
	reader.ReadStringFromUTF8(_ringmode);
	reader.ReadStringFromUTF8(_volume);
	reader.ReadStringFromUTF8(_line1_ringtone);
	reader.ReadStringFromUTF8(_line2_ringtone);
	reader.ReadStringFromUTF8(_message_alert);
	reader.ReadStringFromUTF8(_email_alert);
	reader.ReadStringFromUTF8(_calendar_alert);
	reader.ReadStringFromUTF8(_alarm_alert);
	reader.ReadStringFromUTF8(_reminder_alert);
	reader.ReadStringFromUTF8(_fetion_alert);
}


#endif