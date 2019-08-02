#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
	
	//进入恢复模式
	class EnterRecoveryModeCommand : public BaseCommand
	{
	public:
		EnterRecoveryModeCommand(const DeviceData* pDevice);
		~EnterRecoveryModeCommand() {}

		// 执行命令
		virtual void Execute();


	private:		

		// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
		virtual std::string GetAndroidBodyData() { return "";}

		// 解析返回数据包
		virtual void SetAndroidReturnData(std::string data) {}

		// 从IOS上获取的命令结果文件名（全路径）
		virtual std::wstring GetIosResultFilePath() { return L""; }

		// 解析ios设备的返回文件，解析后删除文件
		virtual void SetIosReturnFile(std::wstring filename){};

	private:

	};
}
