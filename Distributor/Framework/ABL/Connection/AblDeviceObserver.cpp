#include "stdafx.h"
#include "AblDeviceObserver.h"
#include "ConnectionManager.h"

AblDeviceObserver::AblDeviceObserver(ConnectionManager* mgr)
{
	_pMrg = mgr;
}

AblDeviceObserver::~AblDeviceObserver()
{
	DEVICE_MANAGER->Detach(this);
}

void AblDeviceObserver::Init()
{
	DEVICE_MANAGER->Attach(this);
}

void AblDeviceObserver::UsbDeviceConnect( wstring sn )
{
	_pMrg->OnUsbDeviceConnect(sn);
}

void AblDeviceObserver::RecoveryConnect( wstring sn )
{
	_pMrg->OnRecoveryConnect(sn);
}

void AblDeviceObserver::IosDevicePasswordLockConnect( wstring sn )
{
	_pMrg->OnIosDevicePasswordLockConnect(sn);
}

void AblDeviceObserver::OnIosDevJailBreak( wstring sn )
{
	_pMrg->OnIosDevJailBreak(sn);
}

//-------------------------------以下方法暂时不用-----------------------------------

void AblDeviceObserver::WifiDeviceConnect( wstring sn )
{
}

void AblDeviceObserver::WifiDeviceDisconnect( wstring sn )
{
}

void AblDeviceObserver::UsbDeviceDisconnect( wstring sn )
{
}

void AblDeviceObserver::DaemonDisable( wstring sn )
{
}

void AblDeviceObserver::DFUConnect( wstring sn )
{
}

void AblDeviceObserver::DFUDisConnect( wstring sn )
{
}

void AblDeviceObserver::RecoveryDisConnect( wstring sn )
{
}

void AblDeviceObserver::WifiTickNotify( int nNotify, wstring sn )
{
}

void AblDeviceObserver::UsbDeviceOffLine( wstring sn )
{
}