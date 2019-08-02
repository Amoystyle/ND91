#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    class DeviceData;
    class AppData;

	class ArchiveAppCommand : public BaseCommand
	{
	public:
		// AppType			: App的类型，用来驱分不同的调用类型
		// strIdentifier	: App的唯一标识
		// strFileOnPC		：备份后存放的文件名（含路径）
		ArchiveAppCommand(const DeviceData* pDevice, const AppData* appData, wstring strFileOnPC);
		~ArchiveAppCommand(void);

		// 执行命令
		virtual void Execute();

		// 生成Android数据体，
		virtual std::string GetAndroidBodyData(){return "";}

		// 解析返回数据包
		virtual void SetAndroidReturnData(std::string data){}

		// 解析特殊的返回包头
		bool parseAndroidPackageHead( BinaryBufferReader* pReader ){return false;}

		// 命令传送到IOS设备上的文件名（全路径）
		virtual std::wstring GetIosCommandFilePath() { return L""; }

		// 从IOS上获取的命令结果文件名（全路径）
		virtual std::wstring GetIosResultFilePath() { return L""; }

		// 生成ios文件。返回文件名，调用者负责删除文件
		virtual std::wstring GetIosCommandFileContent() { return L""; }

		// 解析返回文件，解析后删除文件
		virtual  void SetIosReturnFile(std::wstring filename) {}

	private:
		const AppData* _appData;
		wstring _strFileOnPC;	//备份后存放的文件名（含路径）
	};
}
