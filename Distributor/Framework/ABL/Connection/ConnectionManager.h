#pragma once

#include "PhoneCenterOut.h"

class DDMManager;
class AblDeviceObserver;
class ScanUsbDeviceThread;
class InitIOSThread;

struct DDMDataItem
{
	DDMDataItem()
	{
		key = L"";
		pPhone = NULL;
		phoneKey = L"";
		bIsRecovery = false;
		bWaitWindows = false;
		nState = DriverInstallState_Init;
		nStep = 0;
		nAdbErrorRetry = 0;
		type = EnumPhoneType_Android;
	}

	wstring key;
	void* pPhone;
	wstring phoneKey;
	bool bIsRecovery;
	bool bWaitWindows;
	DriverInstallState nState;
	int nStep;
	int nAdbErrorRetry;
	EnumPhoneType type;

	bool IsSamSungPhone()
	{
		bool ret = false;
		if(!phoneKey.empty())
		{
			wstring keyTmp = CStrOperation::toLowerW(phoneKey);
			int index = keyTmp.find(L"vid_");
			if(index > 0)
			{
				index += 4;
				wstring instanceId = phoneKey.substr(index, 4);
				ret = instanceId == L"04e8";
			}
		}
		LOG->WriteDebug(L"IsSamSungPhone: " + CStrOperation::IntToWString(ret?1:0));
		return ret;
	}
};

class ConnectionManager
	: public BaseCSLock
	, public BaseGetData<RequestIsRecoveryMode>
	, public BaseHandler<RequestConnectMsg>
	, public BaseHandler<RequestActiveDeviceMsg>
{
public:
	ConnectionManager();
	~ConnectionManager();

	//************************************
	// Method:    Init
	// FullName:  ConnectionManager::Init
	// Access:    public 
	// Returns:   void
	// Qualifier: ��ʼ��
	//************************************
	void Init();

	//************************************
	// Method:    NotifyScan
	// FullName:  ConnectionManager::NotifyScan
	// Access:    public 
	// Returns:   void
	// Qualifier: �����豸ɨ���߳�
	//************************************
	void NotifyScan();

	//************************************
	// Method:    GetPhoneType
	// FullName:  ConnectionManager::GetPhoneType
	// Access:    public 
	// Returns:   EnumPhoneType
	// Qualifier: ��ȡPhoneCenter�豸����
	// Parameter: void * pPhone
	//************************************
	EnumPhoneType GetPhoneType( void* pPhone );

	//************************************
	// Method:    OnDeviceLinkIn
	// FullName:  ConnectionManager::OnDeviceLinkIn
	// Access:    public 
	// Returns:   void
	// Qualifier: �豸����
	// Parameter: void * pPhone
	// Parameter: wstring sn
	// Parameter: EnumPhoneType phoneType
	//************************************
	void OnDeviceLinkIn(void* pPhone, wstring sn, EnumPhoneType phoneType, wstring phoneKey);

	//************************************
	// Method:    OnDeviceLinkOut
	// FullName:  ConnectionManager::OnDeviceLinkOut
	// Access:    public 
	// Returns:   void
	// Qualifier: �豸�Ͽ�
	// Parameter: void * pPhone
	// Parameter: wstring sn
	// Parameter: EnumPhoneType phoneType
	//************************************
	void OnDeviceLinkOut(void* pPhone, wstring sn, EnumPhoneType phoneType);

	//************************************
	// Method:    OnUsbDeviceConnect
	// FullName:  ConnectionManager::OnUsbDeviceConnect
	// Access:    public 
	// Returns:   void
	// Qualifier: �豸����
	// Parameter: wstring sn
	// Parameter: bool bRecovery
	//************************************
	void OnUsbDeviceConnect( wstring sn, bool bRecovery = false );

	//************************************
	// Method:    OnRecoveryConnect
	// FullName:  ConnectionManager::OnRecoveryConnect
	// Access:    public 
	// Returns:   void
	// Qualifier: �ָ�ģʽ
	// Parameter: wstring sn
	//************************************
	void OnRecoveryConnect( wstring sn );

	//************************************
	// Method:    OnIosDevJailBreak
	// FullName:  ConnectionManager::OnIosDevJailBreak
	// Access:    public 
	// Returns:   void
	// Qualifier: �豸Խ�����
	// Parameter: wstring sn
	//************************************
	void OnIosDevJailBreak( wstring sn );

	//************************************
	// Method:    OnIosDevicePasswordLockConnect
	// FullName:  ConnectionManager::OnIosDevicePasswordLockConnect
	// Access:    public 
	// Returns:   void
	// Qualifier: �豸��Ҫ��������
	// Parameter: wstring devicename
	//************************************
	void OnIosDevicePasswordLockConnect( wstring devicename );

	//************************************
	// Method:    OnDevicePropertyChange
	// FullName:  ConnectionManager::OnDevicePropertyChange
	// Access:    public 
	// Returns:   void
	// Qualifier: �豸���Ա仯
	// Parameter: void * pPhone
	// Parameter: EnumPhoneProperty nType
	// Parameter: const char * szData
	//************************************
	void OnDevicePropertyChange(void* pPhone, EnumPhoneProperty nType, const char* szData);

private:
	//************************************
	// Method:    OnConnectStateChange
	// FullName:  ConnectionManager::OnConnectStateChange
	// Access:    private 
	// Returns:   void
	// Qualifier: �豸״̬�仯
	// Parameter: void * pPhone
	// Parameter: EnumPhoneProperty nType
	// Parameter: const char * szData
	//************************************
	void OnConnectStateChange( void* pPhone, EnumPhoneProperty nType, const char* szData );

	//************************************
	// Method:    ParseError
	// FullName:  ConnectionManager::ParseError
	// Access:    private 
	// Returns:   DeviceConnectError
	// Qualifier: ��������DDM��ADCM��IDCM��
	// Parameter: int nValue
	//************************************
	DeviceConnectError ParseError( int nValue );

	//************************************
	// Method:    ParseCheckErrorCode
	// FullName:  ConnectionManager::ParseCheckErrorCode
	// Access:    private 
	// Returns:   DeviceConnectError
	// Qualifier: ����������󣨶Դ������ͽ��з�װ�󣬷�����Module��
	// Parameter: int nValue
	//************************************
	DeviceConnectError ParseCheckErrorCode( int nValue );

	//************************************
	// Method:    FindDDMItem
	// FullName:  ConnectionManager::FindDDMItem
	// Access:    private 
	// Returns:   DDMDataItem*
	// Qualifier: �����豸
	// Parameter: void * pPhone PhoneCenter�豸Ψһ��ʶ
	//************************************
	DDMDataItem* FindDDMItem( void* pPhone );

	//************************************
	// Method:    FindDDMItem
	// FullName:  ConnectionManager::FindDDMItem
	// Access:    private 
	// Returns:   DDMDataItem*
	// Qualifier: �����豸
	// Parameter: wstring sn �豸���к�
	//************************************
	DDMDataItem* FindDDMItem( wstring sn );

	bool Handle( RequestIsRecoveryMode* pMsg );

	bool Handle( const RequestConnectMsg* pMsg );

	bool Handle( const RequestActiveDeviceMsg* pMsg );
private:
	DDMManager* _pDDMManager;
	AblDeviceObserver* _pDeviceObserver;
	ScanUsbDeviceThread* _pScanThread;
	InitIOSThread * _pInitIOSThread;

	vector<DDMDataItem*> _vecDDMItems;
};