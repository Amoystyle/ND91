#pragma once
#include "Module/BaseCommand.h"

namespace ND91Assistant
{
	// 加载全部日历账户、事件、提醒命令
	class CalendarLibrary;
	class LoadCalendarCommand :public BaseCommand
	{
	public:
		LoadCalendarCommand(const DeviceData* pDevice, CalendarLibrary* pLibrary);
		virtual ~LoadCalendarCommand(void);

		// 执行命令
		virtual void Execute() ;

	protected:

#pragma region 这6个方法是Command的主要内容，处理令格式与结果格式

		// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
		virtual std::string GetAndroidBodyData();

		// 解析返回数据包
		virtual void SetAndroidReturnData(std::string data);

		// 解析ios设备的返回文件（全路径, unicode）并删除文件
		virtual void SetIosReturnFile(std::wstring filename);

	private:

		CalendarLibrary*  _pLibrary;  // 日历数据操作类
	};

}