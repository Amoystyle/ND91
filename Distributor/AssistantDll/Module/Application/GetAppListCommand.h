#pragma once

#include "Module/BaseCommand.h"
#include <vector>
#include <map>
using namespace std;

namespace ND91Assistant
{
    class AppData;

    //获取程序信息列表
    class GetAppListCommand : public BaseCommand
    {
    public:
		// appType : 要获取的App类型
		// appOwner : 要获取的App所有者类型， user / system
        GetAppListCommand(const DeviceData* pDevice, APP_TYPE appType, APP_OWNER appOwner);
        virtual ~GetAppListCommand();
		
        // 执行命令
        virtual void Execute();

		int GetIosAppCount();

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
		// 获取取到的App列表
		const vector<AppData*> GetDataList();
        // 排序
        void SortData();

	private:
		vector<AppData*> _DataList;
		wstring m_TempOnComputerFolder;

		APP_TYPE _appType;
		APP_OWNER _appOwner;

	private:
		void GetIosDataList(APP_TYPE appType, vector<AppData*>& vtData);

        // 获取安卓用户程序大小
        void GetAndroidUserAppSize();

//         // 获取安卓程序大小
//         void GetAndroidAppSize();
//         // 获取安卓系统程序MAP
//         void GetAndroidSystemAppMap(map<wstring, long>& appMap);
//         // 获取安卓用户程序MAP
//         void GetAndroidUserAppMap(map<wstring, long>& appMap);
// 
//         // 获取安卓程序MAP
//         void GetAndroidAppMap(map<wstring, long>& appMap, wstring path);
    };

}