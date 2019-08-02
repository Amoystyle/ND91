#pragma once
#include "AssistantDll.h"
#include "Common/BinaryBufferReader.h"
#include "Common/BinaryBufferWriter.h"

namespace ND91Assistant
{
class CalendarData;
class CalendarEventData;
class ReminderData;

// �������ݽ���������
class CalendarStreamHelper
{
public:
	CalendarStreamHelper(void);
	virtual ~CalendarStreamHelper(void);

	// Summary  : ��������
	// Returns	: void
	// Parameter: BinaryBufferReader & reader
	// Parameter: CalendarData * pcal
	// Parameter: bool IsNew �Ƿ��½�
	static void ParseCalendar(BinaryBufferReader& reader, CalendarData* pcal, bool IsNew = false);

	// Summary  : ���������¼�
	// Returns	: void
	// Parameter: BinaryBufferReader & reader
	// Parameter: CalendarEventData * pEvent
	// Parameter: bool IsNew �Ƿ��½�
	static void ParseCalendarEvent(BinaryBufferReader& reader, CalendarEventData* pEvent, bool IsNew = false);

	// Summary  : ���������¼�������
	// Returns	: void
	// Parameter: BinaryBufferWriter & writer
	// Parameter: CalendarEventData * pEvent
	static void CalendarEventToStream(BinaryBufferWriter& writer, CalendarEventData* pEvent);

	// Summary  : ������������
	// Returns	: void
	// Parameter: BinaryBufferReader & reader
	// Parameter: ReminderData * pReminder
	static void ParseReminder(BinaryBufferReader& reader, ReminderData* pReminder);

	// Summary  : ������������������
	// Returns	: void
	// Parameter: BinaryBufferWriter & writer
	// Parameter: ReminderData * pReminder
	static void ReminderToStream(BinaryBufferWriter& writer, ReminderData* pReminder);

private:

	// Summary  : ��ȡһ��byte��ֵ=0����true����ʾû������
	// Returns	: bool
	// Parameter: BinaryBufferReader & reader
	static bool IsNull(BinaryBufferReader& reader);
};

}
