#pragma once
#include "AssistantDll.h"
#include "Common/BinaryBufferReader.h"
#include "Common/BinaryBufferWriter.h"

namespace ND91Assistant
{
class CalendarData;
class CalendarEventData;
class ReminderData;

// 命令数据解析辅助类
class CalendarStreamHelper
{
public:
	CalendarStreamHelper(void);
	virtual ~CalendarStreamHelper(void);

	// Summary  : 解析日历
	// Returns	: void
	// Parameter: BinaryBufferReader & reader
	// Parameter: CalendarData * pcal
	// Parameter: bool IsNew 是否新建
	static void ParseCalendar(BinaryBufferReader& reader, CalendarData* pcal, bool IsNew = false);

	// Summary  : 解析日历事件
	// Returns	: void
	// Parameter: BinaryBufferReader & reader
	// Parameter: CalendarEventData * pEvent
	// Parameter: bool IsNew 是否新建
	static void ParseCalendarEvent(BinaryBufferReader& reader, CalendarEventData* pEvent, bool IsNew = false);

	// Summary  : 生成日历事件数据流
	// Returns	: void
	// Parameter: BinaryBufferWriter & writer
	// Parameter: CalendarEventData * pEvent
	static void CalendarEventToStream(BinaryBufferWriter& writer, CalendarEventData* pEvent);

	// Summary  : 解析日历提醒
	// Returns	: void
	// Parameter: BinaryBufferReader & reader
	// Parameter: ReminderData * pReminder
	static void ParseReminder(BinaryBufferReader& reader, ReminderData* pReminder);

	// Summary  : 生成日历提醒数据流
	// Returns	: void
	// Parameter: BinaryBufferWriter & writer
	// Parameter: ReminderData * pReminder
	static void ReminderToStream(BinaryBufferWriter& writer, ReminderData* pReminder);

private:

	// Summary  : 读取一个byte，值=0返回true，表示没有内容
	// Returns	: bool
	// Parameter: BinaryBufferReader & reader
	static bool IsNull(BinaryBufferReader& reader);
};

}
