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
	LOG->WriteDebug(CCodeOperation::StringToWstring("发送USB连接消息") + sn);
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);

        if ( pObserver )
            pObserver->UsbDeviceConnect(sn);
    }
}

void DeviceSubject::UsbDeviceDisconnect(wstring sn)
{
	LOG->WriteDebug(CCodeOperation::StringToWstring("发送USB断开消息") + sn);
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);

        if ( pObserver )
            pObserver->UsbDeviceDisconnect(sn);
    }
}

void DeviceSubject::OnIosDevJailBreak(wstring sn)
{
	LOG->WriteDebug(CCodeOperation::StringToWstring("发送ios设备越狱消息") + sn);
	for(size_t i = 0; i < _observers.size(); ++i)
	{
		DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);

		if ( pObserver )
			pObserver->OnIosDevJailBreak(sn);
	}
}
void DeviceSubject::IosPasswordLockDeviceConnect(wstring devicename)
{
	LOG->WriteDebug(CCodeOperation::StringToWstring("发送屏锁设备接入消息"));
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
    LOG->WriteDebug(CCodeOperation::StringToWstring("发送DFU连接消息") + sn);
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);
        if ( pObserver )
            pObserver->DFUConnect(sn);
    }
}

void DeviceSubject::DFUDisConnect(wstring sn)
{
    LOG->WriteDebug(CCodeOperation::StringToWstring("发送DFU断开消息") + sn);
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);
        if ( pObserver )
            pObserver->DFUDisConnect(sn);
    }
}

void DeviceSubject::RecoveryConnect(wstring sn)
{
    LOG->WriteDebug(CCodeOperation::StringToWstring("发送Recovery连接消息") + sn);
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);
        if ( pObserver )
            pObserver->RecoveryConnect(sn);
    }
}

void DeviceSubject::RecoveryDisConnect(wstring sn)
{
    LOG->WriteDebug(CCodeOperation::StringToWstring("发送Recovery断开消息") + sn);
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

// 检测到设备处于OffLine状态
void ND91Assistant::DeviceSubject::UsbDeviceOffLine( wstring sn )
{
    LOG->WriteDebug(CCodeOperation::StringToWstring("发送usb DeviceOffLine消息") + sn);
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeviceObserver* pObserver = dynamic_cast<DeviceObserver*>(_observers[i]);
        if ( pObserver )
            pObserver->UsbDeviceOffLine(sn);
    }  
}
