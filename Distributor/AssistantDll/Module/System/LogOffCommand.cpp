#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "LogOffCommand.h"

#include "Core/DeviceData.h"
#include "Device/IosFileIO.h"
#include "Device/IosWiFiFileSystem.h"
#include "Device/NoDeamonInterface/NoDeamonInterface.h"
#include "Common/Path.h"

LogOffCommand::LogOffCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
	_strIosCmdFileName = L"ReloadSpringboard.0";
}

LogOffCommand::~LogOffCommand(void)
{
}

// Ö´ÐÐÃüÁî
void LogOffCommand::Execute()
{
	switch( _pDevice->GetDevInfo()->_deviceType )
	{
	case Android:
		break;
	case Ios:
		{
            IosExecute();
            /*
			IosFileIO* pFileIO = dynamic_cast<IosFileIO*>(GetIO());
			if ( ! pFileIO )
			{
				_success = false;
				return ;
			}

			if (IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion) >= 200)
			{
				IIosFileSystem* pFileStream = NULL;
				if ( _pDevice->GetDevInfo()->_connectType == WIFI)
				{
					pFileStream =  new IosWiFiFileSystem(_pDevice->GetDevInfo()->_ios_wifi_handle);
				}
				else if (_pDevice->GetDevInfo()->_connectType == USB)
				{
					pFileStream =  new AFCFileSysemBase(
						_pDevice->GetDevInfo()->_ios_afcHandle, 
						_pDevice->GetDevInfo()->_ios_afcHandleApp,
						_pDevice->GetDevInfo()->_ios_bJailBreaked);
				}

				if (pFileStream)
                    pFileStream->Ios_DeleteFile(Path::IosSbShutdownCookie());
				else
				{
					_success = false;
					return;
				}
			}				

			pFileIO->SendIosFile(GetIosCommandFilePath(), GetIosResultFilePath(), GetIosCommandFileContent(), false);

			_success = true;*/
		}
		break;
	}
}

void LogOffCommand::IosExecute()
{
    if (_pDevice->GetDevInfo()->_connectType == USB
        && _pDevice->GetDevInfo()->_ios_bJailBreaked)
	{
		SetCSLock(((DeviceInfo*)_pDevice->GetDevInfo())->_csIOSSessionLock);
        _success = (NoDeamonInterface::Reload(_pDevice->GetDevInfo()->_ios_devptr) == 0);
	}
    else
    {
        IosFileIO* pFileIO = dynamic_cast<IosFileIO*>(GetIO());
        RETURN_IF(!pFileIO);

        if (IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion) >= 200)
        {
            IIosFileSystem* pFileStream =  new IosWiFiFileSystem(_pDevice->GetDevInfo()->_ios_wifi_handle);
            if (pFileStream)
                pFileStream->Ios_DeleteFile(Path::IosSbShutdownCookie());
            else
                return;
        }

        pFileIO->SendIosFile(GetIosCommandFilePath(), GetIosResultFilePath(), GetIosCommandFileContent(), false);
        _success = true;
    }
}


#endif