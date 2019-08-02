#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "RebootCommand.h"
#include "Core/DeviceData.h"
#include "Device/IosFileIO.h"
#include "Device/NoDeamonInterface/NoDeamonInterface.h"
#include "Device/AndroidAdbHelper.h"
#include "Device/IosUsbConnection.h"

RebootCommand::RebootCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _strIosCmdFileName = L"reboot.0";
	_EnterRecivery = false;
}

RebootCommand::~RebootCommand(void)
{
}

// 执行命令
void RebootCommand::Execute()
{
	AndroidAdbHelper aah;
	string cmd = " -s ";
	switch( _pDevice->GetDevInfo()->_deviceType )
	{		
	case Android:			
		cmd+= CCodeOperation::UnicodeToUTF_8(_pDevice->GetId());
		cmd += ADB_CMD_REBOOT;
		if(_EnterRecivery)
			cmd+= " recovery";
		aah.ExecuteCmd(cmd);
		break;
	case Ios:
        IosExecute();
		break;
	}
}

void RebootCommand::IosExecute()
{
    if (_pDevice->GetDevInfo()->_connectType == USB
        && !_pDevice->GetDevInfo()->_ios_bJailBreaked)
	{
		//SetCSLock(((DeviceInfo*)_pDevice->GetDevInfo())->_csIOSSessionLock);
       // _success = (NoDeamonInterface::Reboot(_pDevice->GetDevInfo()->_ios_devptr) == 0);
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