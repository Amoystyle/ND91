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

// ִ������
void SetAttributeCommand::Execute()
{
	DefaultExecute();
}

// ����ios�ļ��������ļ����������߸���ɾ���ļ�
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

// ���������ļ���������ɾ���ļ�
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