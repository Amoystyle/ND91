#pragma once

#include "Module/BaseCommand.h"
#include <string>

using namespace std;

namespace ND91Assistant
{
    class AppData;

    //从手机中卸载文件
    class UninstallCommand : public BaseCommand
    {
    public:
		// AppType			: App的类型，用来驱分不同的调用类型
		// strIdentifier	: App的唯一标识
        UninstallCommand(const DeviceData* pDevice, const AppData* appData);
        virtual ~UninstallCommand();

		// 执行命令
		virtual void Execute();

		// 生成Android数据体，
		virtual std::string GetAndroidBodyData();

		// 解析返回数据包
		virtual void SetAndroidReturnData(std::string data);

		// 解析特殊的返回包头
		bool parseAndroidPackageHead( BinaryBufferReader* pReader );

		// 命令传送到IOS设备上的文件名（全路径）
		virtual std::wstring GetIosCommandFilePath() { return L""; }

		// 从IOS上获取的命令结果文件名（全路径）
		virtual std::wstring GetIosResultFilePath() { return L""; }

		// 生成ios文件。返回文件名，调用者负责删除文件
		virtual std::wstring GetIosCommandFileContent() { return L""; }

		// 解析返回文件，解析后删除文件
		virtual  void SetIosReturnFile(std::wstring filename) {}
		
		// Summary  : 卸载一个App 的简单调用方法  静态
		// Returns  : bool 是否处理成功
		// Parameter: const DeviceData * pDevice 设备
		// Parameter: wstring strPackageName 包名
		// Parameter: APP_TYPE appType APP类型
		// Parameter: bool bKeepData 是否保留程序数据
		static bool SimpleUninstallApp(const DeviceData* pDevice, wstring strPackageName, APP_TYPE appType, bool bKeepData=true);
	private:
		const AppData* _appData;
		vector<const AppData*> _dataList;
    };
}

