#pragma once
#include "Device/AdbRemoteCommand.h"

using namespace std;

namespace ND91Assistant
{

class USBShellCommand :
	public AdbRemoteCommand
{
public:
	USBShellCommand();
	virtual ~USBShellCommand(void);

	void setShellCommand(const string& aCommand, const string& sn){_command = aCommand;_serialNumber=sn;}

	string GetResult(){return  _returnData;}

	// 执行命令
	virtual void Execute();


	// 解析返回数据
	// 返回值：true 表示成功，false 表示失败
	virtual bool ParseReturnData();

private:
	string  _command;

	string  _serialNumber;

};

}
