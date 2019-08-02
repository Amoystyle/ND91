#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
	class AppData;

	// 获取android设备是否已root
	class GetNdShStateCommand : public BaseCommand
	{
	public:
		GetNdShStateCommand(const DeviceData* pDevice);
		virtual ~GetNdShStateCommand();

		// 执行命令
		virtual void Execute();

		// 生成Android命令数据包
		virtual std::string GetAndroidBodyData();

		// 解析Android返回结果数据包
		virtual void SetAndroidReturnData(std::string data);

		virtual void SetIosReturnFile(std::wstring filename) {;}


		int GetHasNdsh(){return _bHasNdsh;}
		int GetSuAuth(){return _bSuAuth;}

	private:

		bool _bHasNdsh;
		bool _bSuAuth;

	};
}