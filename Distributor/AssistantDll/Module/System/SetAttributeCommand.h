#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
#define IOS_NORMAL_OWNER L"501" //正常文件的属主值
#define IOS_LOWER_OWNER  L"0"   //低版本IOS设备的属主值

	//设置文件权限命令,ios上设置和修改是分开的两个命令
	class SetAttributeCommand : public BaseCommand
	{
	public:
		// Parameter: const DeviceData * pDevice 
		// Parameter: std::wstring strPath			: 欲设置权限的文件名（含路径），设备端文件
		// Parameter: std::wstring strRights		: 权限，如L"777"
		// Parameter: std::wstring strOwner			: 属主，如L"501"
		SetAttributeCommand(const DeviceData* pDevice, std::wstring strPath, std::wstring strRights, std::wstring strOwner = L"");
		~SetAttributeCommand(void);

        // 设置目标文件名（全路径）
        void SetPath(wstring path);

		// 执行命令
		virtual void Execute();

        // 是否成功
		bool Success();

    private:

#pragma region 这四个方法是Command的主要内容，处理令格式与结果格式

		// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
		virtual std::string GetAndroidBodyData(){return "";};

		// 解析返回数据包
		virtual void SetAndroidReturnData(std::string data) {}

		// 生成ios文件。返回文件名，调用者负责删除文件
		virtual std::wstring GetIosCommandFileContent();

		// 解析ios设备的返回文件，解析后删除文件
		virtual void SetIosReturnFile(std::wstring filename);

#pragma endregion

		bool _bSuccess;			//是否设置成功
		std::wstring _strRights; //权限值
		std::wstring _strPath; //设备上欲设置权限的文件名（含路径）
		std::wstring _strOwner; //属主值
	};
}
