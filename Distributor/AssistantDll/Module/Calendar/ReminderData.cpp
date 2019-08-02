#include "pch_module.h"

#ifdef MODULE_CALENDAR

#include "Module/Calendar/ReminderData.h"

ReminderData::ReminderData(void)
{
	_method = 1;
}

ReminderData::~ReminderData(void)
{
}

/*void ND91Assistant::ReminderData::AndroidDataToStream( BinaryBufferWriter& writer )
{
	writer.WriteInt(_id);
	writer.WriteInt(_eventId);
	writer.WriteInt(_minutes);
	writer.WriteInt(_method);
}

void ND91Assistant::ReminderData::ParseAndroidData( BinaryBufferReader& reader )
{
	reader.ReadInt(_id);
	reader.ReadInt(_eventId);
	reader.ReadInt(_minutes);
	reader.ReadInt(_method);
	}*/

ReminderData* ND91Assistant::ReminderData::Clone()
{
	ReminderData* data = new ReminderData();
	data->_id		=		_id;

	data->_eventId	=		_eventId;	// 事件

	data->_minutes	=		_minutes;	// 提前几分钟

	data->_method	=		_method;	// 方式

	return data;
}


#endif