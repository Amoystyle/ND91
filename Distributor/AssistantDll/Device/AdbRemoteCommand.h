#pragma once

#include "AdbRemoteCommand.h"

#include "common/BaseSocket.h"
#include "Core/DeviceInfo.h"
#include "Common/SystemException.h"
#include "Common/AssistantException.h"
#include "Common/StrOperation.h"

#define ADB_COMMAND_HOST "127.0.0.1"
#define ADB_COMMAND_PORT 5037
#define ADB_OK_FLAG "OKAY"
#define ADB_CONNECT_TO_DEVICE "host:transport:"

namespace ND91Assistant
{
    // ADB命令基类
    // 通过Socket执行非Shell的Adb命令
    // 每个命令实现一个子类，需实现Execute和ParseReturnData方法
    class AdbRemoteCommand
    {
    public:
        // 构造函数：需要提供设备序列号
        AdbRemoteCommand(wstring id);
        virtual ~AdbRemoteCommand();

        // 执行命令
		void Execute();

	protected:

        // 子类实现时，若无特殊处理只需调用CommonExecute即可
        virtual void InternalExecute() = 0;

        // 解析返回数据
        // 返回值：true 表示成功，false 表示失败
        virtual bool ParseReturnData() = 0;

        // 执行命令的共同流程
        void CommonExecute(string command);

        // 连接到指定设备
        bool ConnectToDevice(string sn);

        // 发送命令
        void SendCommand(string command);

        // 接收返回的OK标志
        // 返回值：true 表示成功，false 表示失败
        bool ReceiveOkFlag();

        // 接收返回的数据内容
        // 返回值：true 表示成功，false 表示失败
        bool ReceiveData(bool bSizeInBody = true);

        // 生成ADB命令字符串
        string FormAdbRequest(string command);

    protected:

        CBaseSocket* _pBaseSocket;  // socket连接实例

		wstring _id;		//识别号
        string _sn;           // 设备号

		string _strCommand;			//发送的命令

        bool _isShellCommand;       // 是否Shell命令

        string  _returnData;        // 执行命令后返回的数据（不含OKAY标志）

        bool _bSizeInBody;    //bSizeInBody  标识返回的数据体里面是否带有4个字节的长度标识，如"001F"
        bool _bReturnData;    //returnData   标识是否返回数据体
        bool _bReturnOK;      //returnOK	 标识是否返回"OKAY"
    };

}