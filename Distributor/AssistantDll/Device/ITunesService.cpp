#include "stdafx.h"
#include "ITunesService.h"

#include "Core/DeviceManager.h"
#include "Common/ServiceHelper.h"
#include "Device/IosUsbConnection.h"
#include "Device/IosRecoveryConnection.h"
#include "Common/version.h"

bool CITunesService::_bIosEvnInited = false;
#ifdef WIN32
iTunesVersionType CITunesService::_itunesType = iTunesOld;
#else
iTunesVersionType CITunesService::_itunesType = iTunes92;
#endif
bool CITunesService::_bCallbackInited = false;
void* CITunesService::_subscribe_info_ptr = NULL;
bool CITunesService::_bDevConnAlready = false;
#define	IS_SOFT_ITUNES					L"SOFTWARE\\Apple Computer, Inc.\\iPod\\RegisteredApps\\4"

bool CITunesService::InitIosEnviroment()
{
	LOG_FUNTCION_BEGIN;

	if (!_bIosEvnInited)
	{
		_bIosEvnInited = true; //��ֹ�ⲿ���̵߳���
		
#ifdef WIN32	
		wstring szPath = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, DLLiTunesMobileDeviceReg, _T("InstallDir"));
		wstring szPathTwo = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, DLLCoreFoundationReg, _T("InstallDir"));
		if (szPath.length() <= 0 || szPathTwo.length() <= 0)
		{
			_bIosEvnInited = false;
			return false;
		}

		wstring iTunesName = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, IS_SOFT_ITUNES, _T("PATH"));
		CheckiTunesMobileDeviceVersion(szPath.c_str());

		iTunesName += L";" ;
		iTunesName += szPath;
		iTunesName += L";" ;
		iTunesName +=szPathTwo;

    SetEnvironmentVariable(_T("PATH"), iTunesName.c_str()); 
    //SetEnvironmentVariable(_T("PATH"), szPath.c_str());
    //SetEnvironmentVariable(_T("PATH"), szPathTwo.c_str());
    ServiceHelper sh(L"Apple Mobile Device");
    _bIosEvnInited = sh.Start();
#endif  
	}

	LOG_FUNTCION_END;

	return _bIosEvnInited;
}

void CITunesService::InitStateInfo()
{
	// ��ȫ�ֱ�����Ϊ��ֵ
	_bIosEvnInited = false;
#ifdef WIN32
	_itunesType = iTunesOld;
#else
	_itunesType = iTunes92;
#endif

	_bCallbackInited = false;
	_subscribe_info_ptr = NULL;
	_bDevConnAlready = false;
}

void CITunesService::UnRegisterIosDeviceCallback()
{
	LOG_FUNTCION_BEGIN;

	CITunesMobileDevice::iTunes_AMRestoreRegisterForDeviceNotifications(NULL, NULL, NULL, NULL, 0, NULL);
	if (_subscribe_info_ptr && _bDevConnAlready)
	{
		CITunesMobileDevice::iTunes_AMDeviceNotificationUnsubscribe(_subscribe_info_ptr);
		_subscribe_info_ptr = NULL;
	}	

	InitStateInfo();

	//�����м��ʱ���ԭ������������
//	LOG_FUNTCION_END;
}

//�����豸����/�Ͽ��ص�
void ND91Assistant::OnAMDConnectionNotify(struct am_device_notification_callback_info *info)
{
    LOG_FUNTCION_BEGIN;

    if (!DEVICE_MANAGER)
        return ;   

    static DeviceInfo di;
    di._deviceType = Ios;
    di._connectType = USB;
    di._ios_devptr = info->dev;
    IosUsbConnection conn(&di);
    CITunesService::_bDevConnAlready = true;

    wstringstream wss;
    wss << CCodeOperation::GB2312ToUnicode("OnAMDConnectionNotify �յ���Ϣ : ") << info->msg;
    LOG->WriteDebug(wss.str());

    switch (info->msg)
    {
    case ADNCI_MSG_CONNECTED:
        LOG->WriteDebug(CCodeOperation::GB2312ToUnicode("����Connect!"));
        CITunesService::Connect(&conn, info->dev);
        break;

    case ADNCI_MSG_DISCONNECTED:
        LOG->WriteDebug(CCodeOperation::GB2312ToUnicode("����Disconnect!"));
        CITunesService::Disconnect(&conn, info->dev);
        break;
    case ADNCI_MSG_UninstallItunes:// ������ж��ʱ���ߵ����� �����ź�ֵδ��ȫȷ���Ǵ���Itunes������ж�أ�
        {
            LOG->WriteDebug(L"ADNCI_MSG_UninstallItunes!");
            CITunesService::Disconnect(&conn, info->dev);         	
            DEVICE_MANAGER->RemoveAllIosUSBDevice();          
            CITunesService::InitStateInfo();//CITunesService::UnRegisterIosDeviceCallback();
            break;
        }		
    default: 
        LOG->WriteDebug(CCodeOperation::GB2312ToUnicode("OnAMDConnectionNotify �յ�δ֪����Ϣ��"));
        break;
    }

    LOG_FUNTCION_END;
}

// �ص�������recovery״̬
void ND91Assistant::OnrecoveryConnected(void * device)
{
    LOG_FUNTCION_BEGIN;

    if (!DEVICE_MANAGER)
        return ;  

    DeviceInfo di;
    di._deviceType = Ios;
    di._connectType = RECOVERY;
    di._ios_devptr = device;

    IosRecoveryConnection conn(&di);

    CITunesService::RecoveryConnect(&conn, device);

    LOG_FUNTCION_END;
}

// �ص�������recovery״̬
void ND91Assistant::OnrecoveryDisconnected(void * device)
{
    LOG_FUNTCION_BEGIN;

    if (!DEVICE_MANAGER)
        return ; 

    DeviceInfo di;
    di._deviceType = Ios;
    di._connectType = RECOVERY;
    di._ios_devptr = device;

    IosRecoveryConnection conn(&di);

    CITunesService::RecoveryDisConnect(&conn, device);

    LOG_FUNTCION_END;
}

//Dfu�豸���ӻص�
void ND91Assistant::OnDfuDeviceConnected(void * device)
{
    //dfu ��ʱ��ʵ��
    LOG_FUNTCION_BEGIN;

    if (!DEVICE_MANAGER)
        return ; 

    DeviceInfo di;
    di._deviceType = Ios;
    di._connectType = DFU;
    di._ios_devptr = device;

    IosRecoveryConnection conn(&di);

    CITunesService::DFUConnect(&conn, device);

    LOG_FUNTCION_END;
}

//Dfu�豸���ӻص�
void ND91Assistant::OnDfuDeviceDisconnected(void * device)
{
    //dfu ��ʱ��ʵ��
    LOG_FUNTCION_BEGIN;

    if (!DEVICE_MANAGER)
        return ; 

    DeviceInfo di;
    di._deviceType = Ios;
    di._connectType = DFU;
    di._ios_devptr = device;

    IosRecoveryConnection conn(&di);

    CITunesService::DFUDisConnect(&conn, device);

    LOG_FUNTCION_END;
}

bool CITunesService::RegisterIosDeviceCallback()
{
	LOG_FUNTCION_BEGIN;

	if (!_bCallbackInited || !_subscribe_info_ptr)
	{
		mach_error_t ret = -1;
		try
		{
            ret = CITunesMobileDevice::iTunes_AMDeviceNotificationSubscribe(OnAMDConnectionNotify, 0, 0, 0, &_subscribe_info_ptr);
			if (ret == 0)
			{                
				int nRec = CITunesMobileDevice::iTunes_AMRestoreRegisterForDeviceNotifications(OnDfuDeviceConnected, OnrecoveryConnected, 
					OnDfuDeviceDisconnected, OnrecoveryDisconnected, 0, NULL);
                if (nRec != 0)
                    LOG->WriteDebug(L"ע��ص�ʧ�� iTunes_AMRestoreRegisterForDeviceNotifications");
				_bCallbackInited = true;
			}
            else
                LOG->WriteDebug(L"ע��ص�ʧ�� iTunes_AMDeviceNotificationSubscribe");
		}
		catch (...)
		{
			LOG->WriteDebug(L"ע��ص������쳣");
		}
	}	

	LOG_FUNTCION_END;

	return (_bCallbackInited && _subscribe_info_ptr);
}

void CITunesService::CheckiTunesMobileDeviceVersion(wstring path)
{
#ifdef WIN32
	_itunesType = iTunesOld;
	if (path.length() > 0)
	{
		int nLen = path.length() + _tcslen(DLLiTunesMobileDevice) + 1;
		TCHAR* szDLLiTunes = new TCHAR[nLen];
		ZeroMemory(szDLLiTunes, nLen*sizeof(TCHAR));
		if (szDLLiTunes)
		{
			_tcscat_s(szDLLiTunes, nLen, path.c_str());
			_tcscat_s(szDLLiTunes, nLen, DLLiTunesMobileDevice);
            CVersion Ver = CVersion::GetFileVersion(szDLLiTunes);
			CVersion V82(283, 0, 0, 10);// = FileVersion(283, 0, 0, 10);
			CVersion V91(319, 14, 0, 2);// = FileVersion(319, 14, 0, 2);
			CVersion V92(392, 8, 0, 1);// = FileVersion(392, 8, 0, 1);

			if (Ver.CompareTo(&V92) >= 0)//(CFileOperation::CompareFileVersion(&Ver, &V92) >= 0)
				_itunesType = iTunes92;
			else if (Ver.CompareTo(&V91) >= 0)//(CFileOperation::CompareFileVersion(&Ver, &V91) >= 0)
				_itunesType = iTunes91;
			else if (Ver.CompareTo(&V82) >= 0)//(CFileOperation::CompareFileVersion(&Ver, &V82) >= 0)
				_itunesType = iTunes82;
		}
		delete []szDLLiTunes;
		szDLLiTunes = NULL;
	}
#endif	
}

void CITunesService::ReConnPasswordLockDevice(void* pConnHandle)
{
	if (!pConnHandle) return ;

	DeviceInfo di;
	di._deviceType = Ios;
	di._connectType = USB;
	di._ios_devptr = pConnHandle;
	IosUsbConnection conn(&di);

	CITunesService::Connect(&conn, pConnHandle);
}



void CITunesService::Connect(IConnection* pConn, void * device)
{
	DeviceData* pFind = DEVICE_MANAGER->FindIosDevice(device);
	if (pFind == NULL)
	{
		LOG->WriteDebug(CCodeOperation::GB2312ToUnicode("�����ֻ� start��"));
        // �����豸
        if ( pConn->Connect() )
        {
			LOG->WriteDebug(CCodeOperation::GB2312ToUnicode("��ȡ�ֻ�������Ϣ start��"));
            const DeviceInfo* pInfo = pConn->GetDeviceInfo();
			LOG->WriteDebug(CCodeOperation::GB2312ToUnicode("��ȡ�ֻ�������Ϣ end��"));
            // ���ӵ��豸�б�: ios�豸��UUID��ΪID
			wstring id = CStrOperation::toLowerW(pInfo->_ios_deviceIdentifier);
            DeviceData* pNewDevice = DEVICE_MANAGER->AddDevice(id, pInfo);			

            // �㲥��Ϣ
			if (pNewDevice)
			{
				//pNewDevice->Activate();			
				DEVICE_MANAGER->UsbDeviceConnect(pNewDevice->GetId());
				DEVICE_MANAGER->RemovePasswordLockIosDevice(device, false);		
			}
        }
        else if (((IosUsbConnection*)pConn)->GetIsDeviceLock())
        {	        	
			DEVICE_MANAGER->AddPasswordLockIosDevice(device);
			 const DeviceInfo* pInfo = pConn->GetDeviceInfo();
			 wstring id = CStrOperation::toLowerW(pInfo->_ios_deviceIdentifier);
// ����һ������֪ͨ����������������������Ϣ   by QiuChang 12.7.27:
 			DEVICE_MANAGER->PassWordLockDeviceConnect(pInfo->GetDeviceName());		
        }
		else
            pConn->Disconnect();
		LOG->WriteDebug(CCodeOperation::GB2312ToUnicode("�����ֻ� end��"));
	}
}

void CITunesService::Disconnect(IConnection* pConn, void * device)
{
	LOG->WriteDebug(CCodeOperation::GB2312ToUnicode("�Ͽ��ֻ����� start��"));
		
	DeviceData* pFind = DEVICE_MANAGER->FindIosDevice(device);
	if (pFind != NULL)
	{
        // �Ͽ�����
        //pConn->Disconnect();

        // ���豸�б����Ƴ�: pFindָ��Ķ����ͷ�
        wstring id = pFind->GetId();      
		DEVICE_MANAGER->RemoveDevice(id);

        // �㲥��Ϣ
		DEVICE_MANAGER->UsbDeviceDisconnect(id);
	}
	else
		DEVICE_MANAGER->RemovePasswordLockIosDevice(device, true);
	
	LOG->WriteDebug(CCodeOperation::GB2312ToUnicode("�Ͽ��ֻ����� end��"));
}


void ND91Assistant::CITunesService::DFUConnect( ND91Assistant::IConnection* pConn, void * device )
{
    const DeviceInfo* pInfo = pConn->GetDeviceInfo();
    DEVICE_MANAGER->DFUConnect(pInfo->_ios_deviceIdentifier);
}

void ND91Assistant::CITunesService::DFUDisConnect( ND91Assistant::IConnection* pConn, void * device )
{
    const DeviceInfo* pInfo = pConn->GetDeviceInfo();
    DEVICE_MANAGER->DFUDisConnect(pInfo->_ios_deviceIdentifier);
}

void ND91Assistant::CITunesService::RecoveryConnect( ND91Assistant::IConnection* pConn, void * device )
{
	DeviceData* pFind = DEVICE_MANAGER->FindIosDevice(device);
	if (pFind == NULL)
	{
		if ( pConn->Connect() )
		{		
			const DeviceInfo* pInfo = pConn->GetDeviceInfo();			
			// ���ӵ��豸�б�: ios�豸��UUID��ΪID
			wstring id = CStrOperation::toLowerW(pInfo->_ios_deviceIdentifier);
			DeviceData* pNewDevice = DEVICE_MANAGER->AddDevice(id, pInfo);	

			if (pNewDevice)
			{		
				DEVICE_MANAGER->RecoveryConnect(pNewDevice->GetId());	
			}
		}
	}
}

void ND91Assistant::CITunesService::RecoveryDisConnect( ND91Assistant::IConnection* pConn, void * device )
{
	//const DeviceInfo* pInfo = pConn->GetDeviceInfo();
	DeviceData* pFind = DEVICE_MANAGER->FindIosDevice(device);
	if (pFind != NULL)
	{
		wstring id = pFind->GetId();   
		DEVICE_MANAGER->RemoveDevice(id);
		DEVICE_MANAGER->RecoveryDisConnect(id);
	}

}
