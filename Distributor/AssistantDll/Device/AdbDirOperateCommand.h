#pragma once

#include "AdbShellCommand.h"
namespace ND91Assistant
{

class AdbDirOperateCommand : public AdbShellCommand
{
public:
	AdbDirOperateCommand(const wstring sn)
	{
		_sn = CCodeOperation::UnicodeToUTF_8(sn);
	}
	virtual ~AdbDirOperateCommand(void){}

	void setParam(const string commandstr)
	{
		_strCommand = commandstr;
	}

	bool IsOk(){return _bReturnOK;}

	string GetReturnData(){return _returnData;}

protected:
	bool ParseReturnData()
	{
		return _bReturnOK;
	}

};

}
