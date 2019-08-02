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

// 执行命令
void IosPlistChangeCommand::Execute()
{
	DefaultExecute();
}

// 生成ios文件。返回文件名，调用者负责删除文件
std::wstring IosPlistChangeCommand::GetIosCommandFileContent()
{
	return _strPlistFileFullName;
}

// 解析返回文件，解析后删除文件
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