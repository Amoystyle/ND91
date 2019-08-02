#pragma once

#include "Module/BaseCommand.h"
#include <string>
using namespace std;
namespace ND91Assistant
{
	// 获取程序具备的权限列表，与获取程序列表分开，根据需要独立调用
	class GetAppPermissionsCommand : public BaseCommand
	{
	public:
		// packageName : App 的包名(在手机中的唯一标识)
		GetAppPermissionsCommand(const DeviceData* pDevice, wstring packageName);
		~GetAppPermissionsCommand();

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
		vector<wstring>* GetPerminssionList() {return &_PermissionList;};
	protected:
		bool parseAndroidPackageHead( BinaryBufferReader* pReader );
	private:
		vector<wstring> _PermissionList;	//获得的权限列表，如
		wstring _packageName;	//App 的包名(在手机中的唯一标识)
	};
}