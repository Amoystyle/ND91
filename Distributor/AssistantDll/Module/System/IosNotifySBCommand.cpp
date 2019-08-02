#include "pch_module.h"

#ifdef MODULE_SYSTEM

#include "IosNotifySBCommand.h"
#include "Device/IosFileIO.h"

IosNotifySBCommand::IosNotifySBCommand(const DeviceData* pDevice, unsigned long nTimeOut)
: BaseCommand(pDevice)
{
	_strIosCmdFileName = L"notifySB." + CFileOperation::GetGuidW();
	_bComplete = false;
	_nTimeOut = nTimeOut;
}

IosNotifySBCommand::~IosNotifySBCommand(void)
{
}

// 执行命令
void IosNotifySBCommand::Execute()
{
	switch( _pDevice->GetDevInfo()->_deviceType )
	{
	case Android:
		SetAndroidReturnData(GetIO()->SendAndroidDatagram(GetAndroidDatagram()));
		break;

	case Ios:
		{
			IosFileIO* pFileIO = dynamic_cast<IosFileIO*>(GetIO());
			if ( ! pFileIO )
			{
				THROW_ASSISTANT_EXCEPTION(INVALID_IO_OBJECT);
			}

			if (_nTimeOut > 0)
				pFileIO->SetWaitTime(_nTimeOut);
			SetIosReturnFile(pFileIO->SendIosFile(GetIosCommandFilePath(), GetIosResultFilePath(), GetIosCommandFileContent()));
		}
		break;

	default:
		THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);
		break;
	}
}

// 解析返回文件，解析后删除文件
void IosNotifySBCommand::SetIosReturnFile(std::wstring filename)
{
	if (CFileOperation::IsFileExist(filename.c_str()))
		_bComplete = true;
    CFileOperation::DeleteFile(filename);
}


#endif