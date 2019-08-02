#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
	//重启设备命令
	class RebootCommand: public BaseCommand
	{
	public:
		RebootCommand(const DeviceData* pDevice);
		~RebootCommand(void);

		// 执行命令
		virtual void Execute();

#pragma region 这四个方法是Command的主要内容，处理令格式与结果格式

		// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
		virtual std::string GetAndroidBodyData(){return "";};

		// 解析返回数据包
		virtual void SetAndroidReturnData(std::string data) {}

		// 解析ios设备的返回文件，解析后删除文件
		virtual void SetIosReturnFile(std::wstring filename){};

		virtual std::wstring GetIosResultFilePath(){return L"";}

		void SetReootRecovery(bool recovery){_EnterRecivery = recovery;}

    private:
        void IosExecute();
		bool _EnterRecivery;
#pragma endregion
	};
}