#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "GetIMSICommand.h"

#include "Core/DeviceData.h"
#include "Device/NoDeamonInterface/NoDeamonInterface.h"
#include "Device/IosFileIO.h"

GetIMSICommand::GetIMSICommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
	_strIosCmdFileName = L"imsi." + CFileOperation::GetGuidW();
	_strIMSI = L"";
}

GetIMSICommand::~GetIMSICommand(void)
{
}

void ND91Assistant::GetIMSICommand::Execute()
{
	if (!_pDevice || !_pDevice->GetDevInfo())
		return ;

	switch( _pDevice->GetDevInfo()->_deviceType )
	{
	case Android:		
		break;

	case Ios:
		{
            if (_pDevice->GetDevInfo()->_connectType == USB
                && _pDevice->GetDevInfo()->_ios_bJailBreaked)
            {
                PTCHAR imsi = NULL;
				SetCSLock(((DeviceInfo*)_pDevice->GetDevInfo())->_csIOSSessionLock);
                _success = (NoDeamonInterface::GetImsi(_pDevice->GetDevInfo()->_ios_devptr, &imsi) == 0);
                _result = _success ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
                if (imsi != NULL)
                {
                    _strIMSI = imsi;
                    NoDeamonInterface::Delete(imsi);
                }
            }
            else
            {
			    if (_pDevice->GetIosDeamonMode() == 0) // 无守护模式
				    break;

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
void ND91Assistant::GetIMSICommand::SetIosReturnFile(std::wstring filename)
{
	_strIMSI = GetIosDoneInfoFromFileName();
}


#endif