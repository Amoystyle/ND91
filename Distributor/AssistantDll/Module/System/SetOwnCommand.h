///////////////////////////////////////////////////////////
//  SetOwnCommand.h
//  Implementation of the Class SetOwnCommand
//  Created on:      04-七月-2011 10:36:08
//  Original author: lwj
///////////////////////////////////////////////////////////

#if !defined(EA_EEF5FD9B_5D1C_40d9_9E43_EBB15DF4A14C__INCLUDED_)
#define EA_EEF5FD9B_5D1C_40d9_9E43_EBB15DF4A14C__INCLUDED_

#include "Module/BaseCommand.h"
#include <string>

using namespace std;

namespace ND91Assistant
{
    // 设置权限命令
    class SetOwnCommand : public BaseCommand
    {

    public:
        SetOwnCommand(const DeviceData* pDevice);
        virtual ~SetOwnCommand();

		void  SetParams(const wstring& pathOnPhone, const string& own);
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
		wstring  _pathOnPhone;
		 string  _own;

    };
}

#endif // !defined(EA_EEF5FD9B_5D1C_40d9_9E43_EBB15DF4A14C__INCLUDED_)
