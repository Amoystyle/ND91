#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "SetAttributeCommand.h"

SetAttributeCommand::SetAttributeCommand(const DeviceData* pDevice, std::wstring strPath, wstring strRights, std::wstring strOwner)
: BaseCommand(pDevice)
{
	_strIosCmdFileName = L"SetAttribute." + CFileOperation::GetGuidW();
	_bSuccess = false;
	_strRights = strRights;
	_strPath = strPath;
	_strOwner = strOwner;
}

SetAttributeCommand::~SetAttributeCommand(void)
{
}

// 执行命令
void SetAttributeCommand::Execute()
{
	DefaultExecute();
}

// 生成ios文件。返回文件名，调用者负责删除文件
std::wstring SetAttributeCommand::GetIosCommandFileContent()
{
	int nVer = IosPublicFunc::GetVerNum(GetDeviceInfo()->_deviceProductVersion);
	wstring strCmdMemo = L"";
	if (_strOwner.length() <= 0)
	{
		if (nVer >= 103)
			strCmdMemo += IOS_NORMAL_OWNER;
		else
			strCmdMemo += IOS_LOWER_OWNER;

		strCmdMemo += L"\t" + _strPath + L"\t" + _strRights + L"\n";
	}
	else
		strCmdMemo += _strOwner + L"\t" + _strPath + L"\t" + _strRights + L"\n";

	return strCmdMemo;
}

// 解析返回文件，解析后删除文件
void SetAttributeCommand::SetIosReturnFile(std::wstring filename)
{
	if (CFileOperation::IsFileExist(filename.c_str()))
		_bSuccess = true;
    CFileOperation::DeleteFile(filename);
}

void ND91Assistant::SetAttributeCommand::SetPath( wstring path )
{
    _strPath = path;
}

bool ND91Assistant::SetAttributeCommand::Success()
{
    return _bSuccess;
}



#endif