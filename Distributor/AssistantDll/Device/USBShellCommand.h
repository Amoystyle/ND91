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

	// ִ������
	virtual void Execute();


	// ������������
	// ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
	virtual bool ParseReturnData();

private:
	string  _command;

	string  _serialNumber;

};

}
