#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
	class AppData;

	// 获取android设备root权限
	class GetNdShAuthCommand : public BaseCommand
	{
	public:
		GetNdShAuthCommand(const DeviceData* pDevice);
		virtual ~GetNdShAuthCommand();

		// 执行命令
		virtual void Execute();

		// 生成Android命令数据包
		virtual std::string GetAndroidBodyData();

		// 解析Android返回结果数据包
		virtual void SetAndroidReturnData(std::string data);

		virtual void SetIosReturnFile(std::wstring filename) {;}

		bool GetResult(){return _bSucess;}
	

	private:

		
		bool _bSucess;

	};
}