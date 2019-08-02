#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
	// IOS修改程序配置文件后，对系统的更新命令
	class IosNotifySBCommand : public BaseCommand
	{
	public:
		
		// Parameter: const DeviceData * pDevice
		// Parameter: unsigned long nTimeOut ：超时等待时间，以毫秒为单位
		IosNotifySBCommand(const DeviceData* pDevice, unsigned long nTimeOut);
		~IosNotifySBCommand(void);

		bool GetIsComplete(){return _bComplete;}

		// 执行命令
		virtual void Execute();

#pragma region 这四个方法是Command的主要内容，处理令格式与结果格式

		// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
		virtual std::string GetAndroidBodyData(){return "";};

		// 解析返回数据包
		virtual void SetAndroidReturnData(std::string data) {}

		// 解析ios设备的返回文件，解析后删除文件
		virtual void SetIosReturnFile(std::wstring filename);

#pragma endregion

	private:
		bool _bComplete; //命令执行是否成功
		unsigned long _nTimeOut; //超时等待时间
	};
}