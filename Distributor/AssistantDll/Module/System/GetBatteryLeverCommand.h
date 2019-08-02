#pragma once
#include "Module/BaseCommand.h"
namespace ND91Assistant
{
	// 获取设备电量信息 
	class GetBatteryLeverCommand : public BaseCommand
	{
	public:
		GetBatteryLeverCommand(const DeviceData* pDevice);
		~GetBatteryLeverCommand();

		// 执行命令
		virtual void Execute();

		// 生成Android命令数据包
		virtual std::string GetAndroidBodyData();

		// 解析Android返回结果数据包
		virtual void SetAndroidReturnData(std::string data);

		// 命令传送到IOS设备上的文件名（全路径）
		virtual std::wstring GetIosCommandFilePath() { return L""; }

		// 从IOS上获取的命令结果文件名（全路径）
		virtual std::wstring GetIosResultFilePath() { return L""; }

		// 生成ios文件。返回文件名，调用者负责删除文件
		virtual std::wstring GetIosCommandFileContent() { return L""; }

		// 解析返回文件，解析后删除文件
		virtual  void SetIosReturnFile(std::wstring filename) { }

		int GetBatteryLever(){return _nBatteryLever;};

	private:
		int _nBatteryLever;
	};
}