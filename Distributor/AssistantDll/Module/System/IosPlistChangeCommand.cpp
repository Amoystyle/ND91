#include "pch_module.h"

#ifdef MODULE_SYSTEM

#include "IosPlistChangeCommand.h"

IosPlistChangeCommand::IosPlistChangeCommand(const DeviceData* pDevice, std::wstring strPlistFileFullName)
: BaseCommand(pDevice)
{
	_strIosCmdFileName = L"PlistChange." + CFileOperation::GetGuidW();
	_bComplete = false;
	_strPlistFileFullName = strPlistFileFullName;
}

IosPlistChangeCommand::~IosPlistChangeCommand(void)
{
}

// ִ������
void IosPlistChangeCommand::Execute()
{
	DefaultExecute();
}

// ����ios�ļ��������ļ����������߸���ɾ���ļ�
std::wstring IosPlistChangeCommand::GetIosCommandFileContent()
{
	return _strPlistFileFullName;
}

// ���������ļ���������ɾ���ļ�
void IosPlistChangeCommand::SetIosReturnFile(std::wstring filename)
{
	if (CFileOperation::IsFileExist(filename.c_str()))
	{
		string strReturn = CFileOperation::GetFileAllContent(CCodeOperation::UnicodeToGB2312(filename).c_str());
		strReturn = CStrOperation::toLower(strReturn);
		if (strReturn.find("succeed") > 0)
			_bComplete = true;
        CFileOperation::DeleteFile(filename);
	}
}


#endif