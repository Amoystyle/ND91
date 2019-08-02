#pragma once

#include "Module/BaseCommand.h"
#include "Module/Application/AppData.h"

namespace ND91Assistant
{
	// 根据标识strPackageName 获取某个程序的信息 
	class GetAppInfoCommand : public BaseCommand
	{
	public:
		GetAppInfoCommand(const DeviceData* pDevice, wstring strPackageName);
		~GetAppInfoCommand();
		// 执行命令
		virtual void Execute();

		const AppData* GetAppInfo(wstring strPackageName);

#pragma region

		// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
		virtual std::string GetAndroidBodyData();

		// 解析返回数据包
		virtual void SetAndroidReturnData(std::string data);

		// 命令传送到IOS设备上的文件名（全路径）
		virtual std::wstring GetIosCommandFilePath(){ return L""; }

		// 从IOS上获取的命令结果文件名（全路径）
		virtual std::wstring GetIosResultFilePath(){ return L""; }

		// 生成ios文件
		virtual std::wstring GetIosCommandFileContent(){ return L""; }

		// 解析ios设备的返回文件，解析后删除文件
		virtual void SetIosReturnFile(std::wstring filename){}

#pragma endregion

		// Summary  : 获取取到的App信息
		// Returns  : const AppData*
		const AppData* GetAppInfo() { return &_appData; };
	private:
		wstring _strPackageName;
		AppData _appData;
	};
}