#pragma once
#include "Module/BaseCommand.h"

namespace ND91Assistant
{
	//日历事件操作命令
	class CalendarEventData;
	class CalendarEventActionCommand : public BaseCommand
	{
	public:
		CalendarEventActionCommand(const DeviceData* pDevice);
		virtual ~CalendarEventActionCommand(void);

		// Summary  : 添加事件
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent
		bool AddEvent(CalendarEventData* pEvent);

		// Summary  : 删除事件
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent
		// Parameter: int deleteType 删除类型
		bool DeleteEvent(CalendarEventData* pEvent, int deleteType);

		// Summary  : 修改事件
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent
		// Parameter: int modifyType 修改类型
		bool EditEvent(CalendarEventData* pEvent, int modifyType);

		// Summary  : 取得事件个数
		// Returns	: int
		int  GetEventCount();

		// Summary  : 命令操作返回的Id
		// Returns	: int
		int  GetRetId(){return _retValue;}

	protected:

		// 执行命令
		virtual void Execute();

#pragma region 这6个方法是Command的主要内容，处理令格式与结果格式

		// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
		virtual std::string GetAndroidBodyData();

		// 解析返回数据包
		virtual void SetAndroidReturnData(std::string data);

		// 解析ios设备的返回文件（全路径, unicode）并删除文件
		virtual void SetIosReturnFile(std::wstring filename){}

	private:
		enum ActionType				// 操作类型
		{
			EAddEvent		= 1,	// 添加
			EDeleteEvent	= 2,	// 删除
			EEditEvent		= 3,	// 编辑
			EGetEventCount	= 5		// 获取个数
		};

		ActionType			_type;			// 操作类型

		CalendarEventData*	_pEvent;		// 参数，被操作的事件

		int					_modifyType;	// 参数，修改类型

		int					_retValue;		// 返回值，EGetEventCount时返回个数，其他返回事件Id

	};

}
