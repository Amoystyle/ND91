#pragma once
#include "Module/TableQueryCommand.h"

namespace ND91Assistant
{
	// 日历账户、事件分开加载命令
	class CalendarLibrary;
	class BinaryBufferReader;
	class LoadOnlyCalendarCommand :public TableQueryCommand
	{
	public:
		LoadOnlyCalendarCommand(const DeviceData* pDevice, CalendarLibrary* pLibrary);
		virtual ~LoadOnlyCalendarCommand(void);

		// Summary  : 仅加载账户
		// Returns	: void
		void LoadCalendars();

		// Summary  : 根据账户Id加载事件
		// Returns	: void
		// Parameter: int calendarId日历账户Id
		void LoadCalendarEventsByCalendarId(int calendarId);

	private:

		// 解析返回数据包
		virtual void SetAndroidReturnData(std::string data);

		// Summary  : 解析账户
		// Returns	: void
		// Parameter: BinaryBufferReader & reader
		void ParseCalendars(BinaryBufferReader& reader);

		// Summary  : 解析事件
		// Returns	: void
		// Parameter: BinaryBufferReader & reader
		void ParseCalendarEvents(BinaryBufferReader& reader);

	private:

		enum ActionType			// 操作类型
		{
			ELoadCalendars,		// 仅加载日历账户
			ELoadCalendarEvents,// 根据账户Id加载事件
		};

		ActionType		 _type;		// 操作类型

		CalendarLibrary* _pLibrary; // 日历数据操作类
	};

}
