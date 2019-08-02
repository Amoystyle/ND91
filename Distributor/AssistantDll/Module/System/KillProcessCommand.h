#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{

	class KillProcessCommand: public BaseCommand
	{
	public:
		KillProcessCommand(const DeviceData* pDevice, std::wstring strProcessName);
		~KillProcessCommand(void);

		// 执行命令
		virtual void Execute();

#pragma region 这四个方法是Command的主要内容，处理令格式与结果格式

		// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
		virtual std::string GetAndroidBodyData(){return "";};

		// 解析返回数据包
		virtual void SetAndroidReturnData(std::string data) {}

		// 从IOS上获取的命令结果文件名（全路径）
		virtual std::wstring GetIosResultFilePath();

		// 生成ios文件。返回文件名，调用者负责删除文件
		virtual std::wstring GetIosCommandFileContent();

		// 解析ios设备的返回文件，解析后删除文件
		virtual void SetIosReturnFile(std::wstring filename);

#pragma endregion

	private:
		std::wstring _strProcessName;
	};
}