#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
	//IOS上修改PLIST文件为XML格式的命令，因为某些文件下载到本地后修改成XML文件有问题
	class IosPlistChangeCommand : public BaseCommand
	{
	public:
		// Parameter: const DeviceData * pDevice
		// Parameter: std::wstring strPlistFileFullName ：欲修改的PLIST文件名（含路径，在设备上）
		IosPlistChangeCommand(const DeviceData* pDevice, std::wstring strPlistFileFullName);
		~IosPlistChangeCommand(void);

		bool GetIsComplete(){return _bComplete;}

		// 执行命令
		virtual void Execute();

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

	private:
		bool _bComplete;		//是否修改成功
		std::wstring _strPlistFileFullName; //欲修改的PLIST文件名（含路径，在设备上）
	};
}