#pragma once

class ConnectionManager;

class AblDeviceObserver
	: public DeviceObserver
	, public BaseCSLock
{
public:
	AblDeviceObserver(ConnectionManager* mrg);
	~AblDeviceObserver();

	void Init();

private:
	virtual void WifiDeviceConnect( wstring sn );

	virtual void WifiDeviceDisconnect( wstring sn );

	virtual void UsbDeviceConnect( wstring sn );

	virtual void UsbDeviceDisconnect( wstring sn );

	virtual void DaemonDisable( wstring sn );

	virtual void DFUConnect( wstring sn );

	virtual void DFUDisConnect( wstring sn );

	virtual void RecoveryConnect( wstring sn );

	virtual void RecoveryDisConnect( wstring sn );

	virtual void OnIosDevJailBreak( wstring sn );

	virtual void WifiTickNotify( int nNotify, wstring sn );

	virtual void UsbDeviceOffLine( wstring sn );

	virtual void IosDevicePasswordLockConnect( wstring sn );

private:
	ConnectionManager* _pMrg;

};