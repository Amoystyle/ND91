#pragma once

#include "Module/BaseCommand.h"
#include <string>
using namespace std;

namespace ND91Assistant
{
    // 获取程序图标, 与获取程序列表分开，根据需要独立调用
    class GetAppIconCommand : public BaseCommand
    {
    public:
		// packageName : App 的包名(在手机中的唯一标识)
		// strSavePath : 取得的Icon要保存到的PC中的路径
        GetAppIconCommand(const DeviceData* pDevice, wstring packageName, wstring strSavePath);
        ~GetAppIconCommand();
#pragma region
        // 执行命令
        virtual void Execute();

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

	private:
		bool parseAndroidPackageHead( BinaryBufferReader* pReader );
		bool SaveIconData(wstring strFilePath, const char* pIconData, int nIconSize);
   private:
		wstring _strSavePath;	// 取得的Icon要保存到的PC中的路径	
        wstring _packageName;	// App 的包名(在手机中的唯一标识)
    };
}