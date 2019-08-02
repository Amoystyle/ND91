#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "KillProcessCommand.h"

#include "Core/DeviceData.h"
#include "Device/IosFileIO.h"
#include "Device/NoDeamonInterface/NoDeamonInterface.h"

KillProcessCommand::KillProcessCommand(const DeviceData* pDevice, std::wstring strProcessName)
: BaseCommand(pDevice)
{
	_strProcessName = strProcessName;
	_strIosCmdFileName = L"kill." + CFileOperation::GetGuidW();
}

KillProcessCommand::~KillProcessCommand(void)
{
}

// ִ������
void KillProcessCommand::Execute()
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
                _success = (NoDeamonInterface::Kill(_pDevice->GetDevInfo()->_ios_devptr, (wchar_t*)_strProcessName.c_str()) == 0);
                _result = _success ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
            }
            else
            {
		        IosFileIO* pFileIO = dynamic_cast<IosFileIO*>(GetIO());
		        if ( ! pFileIO )
			        THROW_ASSISTANT_EXCEPTION(INVALID_IO_OBJECT);

		        pFileIO->DeleteAllCmdDoneFile();
		        pFileIO->SendIosFile(GetIosCommandFilePath(), GetIosResultFilePath(), GetIosCommandFileContent(), false);
            }
		}
		break;

	default:
		THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);
		break;
	}
}

std::wstring KillProcessCommand::GetIosResultFilePath()
{
	return L"";
}

// ����ios�ļ��������ļ����������߸���ɾ���ļ�
std::wstring KillProcessCommand::GetIosCommandFileContent()
{
	return _strProcessName;
}

// ���������ļ���������ɾ���ļ�
void KillProcessCommand::SetIosReturnFile(std::wstring filename)
{	
}


#endif