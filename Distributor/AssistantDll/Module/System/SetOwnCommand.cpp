#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "SetOwnCommand.h"
#include "Core/DeviceData.h"
#include "Common/Path.h"

SetOwnCommand::SetOwnCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
	_pathOnPhone = L"";
	_strIosCmdFileName = L"Chown." + CFileOperation::GetGuidW();
	_own = "";
}

SetOwnCommand::~SetOwnCommand()
{

}

void SetOwnCommand::SetParams(const wstring& pathOnPhone, const string& mode)
{
	_pathOnPhone = pathOnPhone;
	_own = mode;
}

// ִ������
void SetOwnCommand::Execute()
{
	DefaultExecute();
}

// ����ios�ļ��������ļ����������߸���ɾ���ļ�
std::wstring SetOwnCommand::GetIosCommandFileContent()
{
	return _pathOnPhone;
}

// ���������ļ���������ɾ���ļ�
void SetOwnCommand::SetIosReturnFile(std::wstring filename)
{
	Sleep(100);
	
	wstring strfolder = _pDevice->GetPath()->GetLocalDeamonFolder()
        + _strIosCmdFileName + L".done";

	string strLine = CFileOperation::ReadFile(strfolder, false);
 
    CFileOperation::DeleteFile(strfolder);
}



#endif