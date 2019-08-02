#include "pch_module.h"

#ifdef MODULE_SYSTEM

#include "GetIosDeviceTypeCommand.h"

#include "Device/IosFileIO.h"
#include "Device/NoDeamonInterface/NoDeamonInterface.h"

GetIosDeviceTypeCommand::GetIosDeviceTypeCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
	_strIosCmdFileName = L"type." + CFileOperation::GetGuidW();
	_deviceType = L"";
}

// 执行命令
void GetIosDeviceTypeCommand::Execute()
{
	switch( _pDevice->GetDevInfo()->_deviceType )
	{
	case Android:
		SetAndroidReturnData(GetIO()->SendAndroidDatagram(GetAndroidDatagram()));
		break;

	case Ios:
		{
            if (_pDevice->GetDevInfo()->_connectType == USB 
                && _pDevice->GetDevInfo()->_ios_bJailBreaked)
            {
				SetCSLock(((DeviceInfo*)_pDevice->GetDevInfo())->_csIOSSessionLock);
                PTCHAR type = NULL;
                _success = (NoDeamonInterface::GetType(_pDevice->GetDevInfo()->_ios_devptr, &type) == 0);
                _result = _success ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
                if (type != NULL)
                {
                    _deviceType = type;
                    NoDeamonInterface::Delete(type);
                }
            }
            else
            {
			    IosFileIO* pFileIO = dynamic_cast<IosFileIO*>(GetIO());
			    if ( ! pFileIO )
				    THROW_ASSISTANT_EXCEPTION(INVALID_IO_OBJECT);

			    SetIosReturnFile(pFileIO->SendIosFile(GetIosCommandFilePath(), GetIosResultFilePath(), GetIosCommandFileContent(), false));
            }
		}
		break;

	default:
		THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);
		break;
	}
}

// 解析返回文件，解析后删除文件
void GetIosDeviceTypeCommand::SetIosReturnFile(std::wstring filename)
{
	_deviceType = GetIosDoneInfoFromFileName();
}

std::wstring ND91Assistant::GetIosDeviceTypeCommand::GetDeviceType()
{
    return _deviceType;
}



#endif