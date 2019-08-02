#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "ShoutdownCommand.h"
#include "Core/DeviceData.h"
#include "Device/NoDeamonInterface/NoDeamonInterface.h"
#include "Device/IosFileIO.h"
#include "Device/IosUsbConnection.h"

ShoutdownCommand::ShoutdownCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _strIosCmdFileName = L"shutdown.0";
}

ShoutdownCommand::~ShoutdownCommand(void)
{
}

// 执行命令
void ShoutdownCommand::Execute()
{
	switch( _pDevice->GetDevInfo()->_deviceType )
	{
	case Android:
		break;
	case Ios:
        IosExecute();
		break;
	}
}

void ShoutdownCommand::IosExecute()
{
    if (_pDevice->GetDevInfo()->_connectType == USB
        && _pDevice->GetDevInfo()->_ios_bJailBreaked)
	{		 
		//SetCSLock(((DeviceInfo*)_pDevice->GetDevInfo())->_csIOSSessionLock);
        //_success = (NoDeamonInterface::Shutdown(_pDevice->GetDevInfo()->_ios_devptr) == 0);
		//使用新诊断接口
		IosUsbConnection usbconn((DeviceInfo*)_pDevice->GetDevInfo());
		_success = usbconn.RebootWithDiagnosticsRelay();
	}
    else
    {
        IosFileIO* pFileIO = dynamic_cast<IosFileIO*>(GetIO());
        RETURN_IF(!pFileIO);

        pFileIO->SendIosFile(GetIosCommandFilePath(), GetIosResultFilePath(), GetIosCommandFileContent(), false);
        _success = true;
    }
}


#endif