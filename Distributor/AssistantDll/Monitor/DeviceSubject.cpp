#include "stdafx.h"
#include "Monitor/DeviceSubject.h"
#include "Monitor/DeviceObserver.h"

void DeviceSubject::WifiDeviceConnect(wstring sn)
{
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);

        if ( pObserver )
            pObserver->WifiDeviceConnect(sn);
    }
}

void DeviceSubject::WifiDeviceDisconnect(wstring sn)
{
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);

        if ( pObserver )
            pObserver->WifiDeviceDisconnect(sn);
    }
}

void DeviceSubject::UsbDeviceConnect(wstring sn)
{
	LOG->WriteDebug(CCodeOperation::StringToWstring("����USB������Ϣ") + sn);
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);

        if ( pObserver )
            pObserver->UsbDeviceConnect(sn);
    }
}

void DeviceSubject::UsbDeviceDisconnect(wstring sn)
{
	LOG->WriteDebug(CCodeOperation::StringToWstring("����USB�Ͽ���Ϣ") + sn);
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);

        if ( pObserver )
            pObserver->UsbDeviceDisconnect(sn);
    }
}

void DeviceSubject::OnIosDevJailBreak(wstring sn)
{
	LOG->WriteDebug(CCodeOperation::StringToWstring("����ios�豸Խ����Ϣ") + sn);
	for(size_t i = 0; i < _observers.size(); ++i)
	{
		DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);

		if ( pObserver )
			pObserver->OnIosDevJailBreak(sn);
	}
}
void DeviceSubject::IosPasswordLockDeviceConnect(wstring devicename)
{
	LOG->WriteDebug(CCodeOperation::StringToWstring("���������豸������Ϣ"));
	for(size_t i = 0; i < _observers.size(); ++i)
	{
		DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);

		if ( pObserver )
			pObserver->IosDevicePasswordLockConnect(devicename);
	}
}

void DeviceSubject::DaemonDisable(wstring sn)
{
	for(size_t i = 0; i < _observers.size(); ++i)
	{
		DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);

		if ( pObserver )
			pObserver->DaemonDisable(sn);
	}
}

void DeviceSubject::DFUConnect(wstring sn)
{
    LOG->WriteDebug(CCodeOperation::StringToWstring("����DFU������Ϣ") + sn);
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);
        if ( pObserver )
            pObserver->DFUConnect(sn);
    }
}

void DeviceSubject::DFUDisConnect(wstring sn)
{
    LOG->WriteDebug(CCodeOperation::StringToWstring("����DFU�Ͽ���Ϣ") + sn);
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);
        if ( pObserver )
            pObserver->DFUDisConnect(sn);
    }
}

void DeviceSubject::RecoveryConnect(wstring sn)
{
    LOG->WriteDebug(CCodeOperation::StringToWstring("����Recovery������Ϣ") + sn);
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);
        if ( pObserver )
            pObserver->RecoveryConnect(sn);
    }
}

void DeviceSubject::RecoveryDisConnect(wstring sn)
{
    LOG->WriteDebug(CCodeOperation::StringToWstring("����Recovery�Ͽ���Ϣ") + sn);
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);
        if ( pObserver )
            pObserver->RecoveryDisConnect(sn);
    }
}

void ND91Assistant::DeviceSubject::WifiTickNotify( int nNotify, wstring sn )
{
    LOG->WriteDebug(CCodeOperation::StringToWstring("WifiTickNotify") + sn);
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);
        if ( pObserver )
            pObserver->WifiTickNotify(nNotify, sn);
    }
}

// ��⵽�豸����OffLine״̬
void ND91Assistant::DeviceSubject::UsbDeviceOffLine( wstring sn )
{
    LOG->WriteDebug(CCodeOperation::StringToWstring("����usb DeviceOffLine��Ϣ") + sn);
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);
        if ( pObserver )
            pObserver->UsbDeviceOffLine(sn);
    }  
}
