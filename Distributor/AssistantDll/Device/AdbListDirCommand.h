#pragma once
#include "Device/AdbShellCommand.h"
#include "Common/CodeOperation.h"

namespace ND91Assistant
{
class AdbListDirCommand :
	public AdbShellCommand
{
public:
	AdbListDirCommand(wstring sn, wstring strPCPath, bool IsSupportColor = false)
	{
		_sn = CCodeOperation::UnicodeToUTF_8(sn);
		_strCommand = (IsSupportColor?"ls -l -a --color=never \"":"ls -l -a \"")+ CCodeOperation::UnicodeToUTF_8(strPCPath)+"\"";
		// 统一在这里增加字符转义处理
		//if (_pDevice->GetDevInfo()->_connectType == USB)
		{
			_strCommand = CStrOperation::Replace(_strCommand, "$", "\\$");
			_strCommand = CStrOperation::Replace(_strCommand, "`", "\\`");
		}
	}
	virtual ~AdbListDirCommand(void){}

	string GetResultData(){return _returnData;}
protected:
	bool ParseReturnData()
	{
		return true;
	}
};
}
