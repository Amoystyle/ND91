#pragma once
//�豸����
enum EnumPhoneType
{
	//android
	EnumPhoneType_Android,
	//iphone
	EnumPhoneType_IPhone,
};

//�豸�б�䶯����
enum NotifyCollectionChangedAction
{
	//���
	NotifyCollectionChangedAction_Add,
	//�Ƴ�
	NotifyCollectionChangedAction_Remove,
};

//��ǰ���ӹ���
enum EnumConnectState
{
	/// <summary>
	/// ���ӳ�ʼ��
	/// UIԤ�ڣ���ǰ���ڼ������״̬��
	/// </summary>
	EnumConnectState_ConnectInit,
	/// <summary>
	/// ��װ��ʼ��
	/// UIԤ�ڣ���ʼ��װ������
	/// </summary>
	EnumConnectState_InstallInit,
	/// <summary>
	/// ��װ����
	/// UIԤ�ڣ������û��β��豸��
	/// </summary>
	EnumConnectState_InstallEnd,
	/// <summary>
	/// �����а�װ
	/// UIԤ�ڣ���Ҫ��װ�������������޷���װ�ɹ���
	/// </summary>
	EnumConnectState_NotInstall,
	/// <summary>
	/// ��ʼ����
	/// UIԤ�ڣ������û��������ӣ������ʼ����ΪEnumConnectSuccessSign_ADB�����޴˹��̡�
	/// </summary>
	EnumConnectState_Connecting,
	/// <summary>
	/// ����ʧ��
	/// UIԤ�ڣ������û�����ʧ�ܣ�ʧ��ԭ����ConnectError���أ������������ط�����
	/// </summary>
	EnumConnectState_ConnectError,
	/// <summary>
	/// ���ӳɹ�
	/// </summary>
	EnumConnectState_ConnectSuccess,
	/// <summary>
	/// �Ͽ�����
	/// </summary>
	EnumConnectState_UnConnect,
	/// <summary>
	/// ��ʼ��״̬��һ��Ҫ�����
	/// </summary>
	EnumConnectState_NULL,
};
/// <summary>
/// ����豸������
/// </summary>
enum EnumCheckErrorCode
{
	/// <summary>
	/// ���ӳɹ�
	/// </summary>
	EnumCheckErrorCode_Success,
	/// <summary>
	/// �쳣����
	/// </summary>
	EnumCheckErrorCode_Exception,
	/// <summary>
	/// �ֻ��Ͽ�
	/// </summary>
	EnumCheckErrorCode_offline,
	/// <summary>
	/// ������װʧ��
	/// </summary>
	EnumCheckErrorCode_DriverError,
	/// <summary>
	/// ����ģʽû��
	/// </summary>
	EnumCheckErrorCode_OffDebug,
	/// <summary>
	/// ���������ģʽ��
	/// </summary>
	EnumCheckErrorCode_ConnectTypeError,
	/// <summary>
	/// �豸�Ѿ��϶�������������ADBbug����������װ���ˡ�
	/// </summary>
	EnumCheckErrorCode_ADBError,
	/// <summary>
	/// �豸�Ͽ�
	/// </summary>
	EnumCheckErrorCode_UnConnect,
	/// <summary>
	/// itunesδ�ҵ��豸
	/// </summary>
	EnumCheckErrorCode_ITunesNoFind,
	/// <summary>
	/// itunes��װ����Ҫ���������ֻ�
	/// </summary>
	EnumCheckErrorCode_ItunesNeedReconnect,
	/// <summary>
	/// itunes��װ����Ҫ���������ֻ�������������
	/// </summary>
	EnumCheckErrorCode_ItunesNeedReconnect2,
};

/// <summary>
/// �豸״̬
/// �û���ʾ������Ϣ��
/// </summary>
enum HardwareStatus
{
	/// <summary>
	/// �豸��������
	/// </summary>
	HardwareStatus_Online,
	/// <summary>
	/// �豸����˯��
	/// </summary>
	HardwareStatus_Offline,
	/// <summary>
	/// �豸û�д򿪵���ģʽ����״ֻ̬�� android �豸�Ż��У�
	/// </summary>
	HardwareStatus_OffDebug,
	/// <summary>
	/// δ֪����
	/// </summary>
	HardwareStatus_Unknown,
	/// <summary>
	/// �ڴ�ռ䲻��
	/// </summary>
	HardwareStatus_NoMemory,
	/// <summary>
	/// ������װʧ��
	/// </summary>
	HardwareStatus_DriverError,
	/// <summary>
	/// �ȴ�ȷ�ϣ���WIFI�����ػ�ʱ����Ҫ�ֶ���װ
	/// </summary>
	HardwareStatus_WaitForConfirm,
	/// <summary>
	/// iphone�豸�����룬����û�����ӹ����ԣ��������û��������롣
	/// 
	/// �Բ�������iPhone"�豸����"��ʹ�ÿ���������ϵͳ��ʱδ�����ӵ�����
	/// ����iPhone��������������󣬵��������һ�Ρ��Լ�����
	/// </summary>
	HardwareStatus_IsPasswordProtected,
	/// <summary>
	/// SDK�ռ䲻��
	/// </summary>
	HardwareStatus_NoSDK,
	/// <summary>
	/// ��֧��
	/// </summary>
	HardwareStatus_UnSuppert,
	/// <summary>
	/// �豸�޷�����
	/// </summary>
	HardwareStatus_Unable,
	/// <summary>
	/// ADB�쳣��
	/// </summary>
	HardwareStatus_ADBError,
	/// <summary>
	/// ���������ģʽ
	/// </summary>
	HardwareStatus_ConnectTypeError,
};




/// <summary>
/// �豸����
/// </summary>
enum EnumPhoneProperty
{
	/// <summary>
	/// ��ǰ���ӹ���
	/// ��10���Ʒ��أ���ȡ��ת���ɶ�Ӧö��
	/// ����1���� EnumConnectState_InstallInit
	/// </summary>
	EnumPhoneProperty_ConnectState,
	/// <summary>
	/// ��װ��������,���ֵΪ100;
	/// ����10
	/// </summary>
	EnumPhoneProperty_Progress,
	/// <summary>
	/// ��װ������Ϣ
	/// ������װ������
	/// </summary>
	EnumPhoneProperty_ProgressTitle,
	/// <summary>
	/// ��Iphone����ADB��Ӧ��Key
	/// ����iphone     usb\vid_05ac&pid_
	/// ����android    0123456789012345  
	/// </summary>
	EnumPhoneProperty_Key,
	/// <summary>
	/// ������  EnumCheckErrorCode&0x1000
	/// ע������ֵΪ10���ơ� 
	/// ����0x1000->EnumCheckErrorCode_Success 0x1001->EnumCheckErrorCode_Exception
	/// </summary>
	EnumPhoneProperty_ErrorCode,
};

/// <summary>
/// �¼�����
/// </summary>
enum EnumEvent
{
	/// <summary>
	/// �б�ı�
	/// �������أ�STCollectionChangeParam
	/// </summary>
	EnumEventCollectionChange,
	/// <summary>
	/// ���Ըı�
	/// �������أ�STPhonePropertyChanged
	/// </summary>
	EnumEventPropertyChanged,
	/// <summary>
	/// dll�쳣
	/// </summary>
	EnumExpetion,
};

enum EnumConnectMethod
{
	//�Զ�����һ��,��Ч
	EnumConnectMethod_AutoConnectOne,
	//�Զ���������
	EnumConnectMethod_AutoConnectAll,
	//���Զ�����
	EnumConnectMethod_NoAuto,
};

enum EnumConnectSuccessSign
{
	//��ADB����Ϊ�ɹ���־
	EnumConnectSuccessSign_ADB,
	//���ػ�����Ϊ�ɹ���־
	EnumConnectSuccessSign_Guard

};

/// <summary>
/// �豸����
/// </summary>
class CPhone;

struct STCollectionChangeParam
{
	NotifyCollectionChangedAction type;
	CPhone*pPhone;
	char*szKey;
	EnumPhoneType PhoneType;
	char* szPhoneKey;
};

struct STPhonePropertyChanged
{
	EnumPhoneProperty type;
	CPhone *pPhone;
	const char* szData;
};

typedef void (*CollectionChanged)(NotifyCollectionChangedAction,CPhone*pPhone,void*pParam);

typedef void (*PropertyChanged)(EnumPhoneProperty szName,CPhone*pPhone,void*pParam);

#ifdef  _CVER_

#ifdef _PHONE_C_DELLEXPORT
	#define  _DLLEXPORTC extern"C" _declspec(dllexport)
#else
	#define  _DLLEXPORTC extern"C"
#endif

#ifndef _PHONE_CEX_DELLEXPORT
#define  _DLLEXPORTCEX extern"C" _declspec(dllexport)
#else
#define  _DLLEXPORTCEX extern"C"
#endif

#define  _DLLEXPORT
typedef void (*CollectionChanged)(NotifyCollectionChangedAction,CPhone*pPhone,void*pParam);

typedef void (*PropertyChanged)(EnumPhoneProperty szName,CPhone*pPhone,void*pParam);


extern "C" typedef void (*EventChanged)(EnumEvent eve,void* pParam,void*pOwn);

typedef int (*FunPhoneCenter_Start)(EnumConnectMethod connectMethod,EnumConnectSuccessSign suceessSign,EventChanged _event,void *pOwn);
typedef int (*FunPhoneCenter_Exit)();
//************************************
// Method:    ��������
// FullName:  PhoneCenter_Start
// Access:    public 
// Returns:   _DLLEXPORTC int  1����true
// Qualifier:
// Parameter: EnumConnectMethod connectMethod     �Զ�����ģʽ
// Parameter: EnumConnectSuccessSign suceessSign  ���ӳɹ���־
//************************************
_DLLEXPORTC int PhoneCenter_Start(EnumConnectMethod connectMethod,EnumConnectSuccessSign suceessSign,EventChanged _event,void *pOwn);

//************************************
// Method:    PhoneCenter_Connect
// FullName:  PhoneCenter_Connect
// Access:    public 
// Returns:   _DLLEXPORTC int  1����true
// Qualifier:
// Parameter: CPhone * pPhone
//************************************
_DLLEXPORTC int PhoneCenter_Connect(CPhone *pPhone);

//************************************
// Method:    �˳�����
// FullName:  PhoneCenter_Exit
// Access:    public 
// Returns:   _DLLEXPORTC int  1����true
// Qualifier:
//************************************
_DLLEXPORTC int PhoneCenter_Exit();


//************************************
// Method:    ��ȡ����ֵ��˵����ͬ��EnumPhoneProperty
//            ���ͷſռ�
// FullName:  Phone_Type
// Access:    public 
// Returns:   _DLLEXPORTC char*
// Qualifier:
// Parameter: CPhone * pPhone
// Parameter: EnumPhoneProperty type
//************************************
_DLLEXPORTCEX char* Phone_Type(CPhone *pPhone,EnumPhoneProperty type);

_DLLEXPORTCEX void Free_PhoneType(char* szData);


_DLLEXPORTCEX EnumConnectState CPhone_ConnectState(CPhone*pPhone);

_DLLEXPORTCEX int CPhone_ConnectError(CPhone*pPhone);

_DLLEXPORTCEX char* CPhone_Key(CPhone*pPhone);

_DLLEXPORTCEX bool CPhone_IsConnect(CPhone*pPhone);

_DLLEXPORTCEX void CPhone_AddPropertyChanged( PropertyChanged _func ,void *pParam,CPhone*pPhone);

_DLLEXPORTCEX void CPhone_RemovePropertyChanged(PropertyChanged _func,void*pParam,CPhone*pPhone);

#endif

#ifdef _C_VER_



#ifdef _PHONECENTER_DLLEXPORT
	#define  _DLLEXPORT _declspec(dllexport)
#else
	#define  _DLLEXPORT 
	class CPhoneCenter
	{
	public:
			//************************************
			// Method:    ��������ʵ��
			// FullName:  CPhoneCenter::Instance
			// Access:    public 
			// Returns:   _DLLEXPORT  CPhoneCenter*
			// Qualifier:
			//************************************
			_DLLEXPORT static CPhoneCenter* Instance();
			//************************************
			// Method:    �ͷ���������ʵ��
			// FullName:  CPhoneCenter::Free
			// Access:    public 
			// Returns:   _DLLEXPORT  void
			// Qualifier:
			//************************************
			_DLLEXPORT static void Free();
			//************************************
			// Method:    ����ɨ��
			// FullName:  CPhoneCenter::Start
			// Access:    public 
			// Returns:   _DLLEXPORT void
			// Qualifier:
			//************************************
			_DLLEXPORT void Start();
			//************************************
			// Method:    ֹͣɨ��
			// FullName:  CPhoneCenter::Stop
			// Access:    public 
			// Returns:   _DLLEXPORT void
			// Qualifier:
			//************************************
			_DLLEXPORT void Stop();

			//************************************
			// Method:    ����豸�䶯�¼�
			// FullName:  CPhoneCenter::AddEvent
			// Access:    public 
			// Returns:   _DLLEXPORT void
			// Qualifier:
			// Parameter: CollectionChanged changedFunc
			// Parameter: void * pParam
			//************************************
			_DLLEXPORT void AddEvent(CollectionChanged changedFunc,void*pParam);
			//************************************
			// Method:    �Ƴ��豸�䶯�¼�
			// FullName:  CPhoneCenter::RemoveEvent
			// Access:    public 
			// Returns:   _DLLEXPORT void
			// Qualifier:
			// Parameter: CollectionChanged changedFunc
			//************************************
			_DLLEXPORT void RemoveEvent(CollectionChanged changedFunc);

			//************************************
			// Method:    ����豸�б�
			// FullName:  CPhoneCenter::ToArray
			// Access:    public 
			// Returns:   _DLLEXPORT CPhone**
			// Qualifier:
			// Parameter: int * nNum
			//************************************
			_DLLEXPORT CPhone**ToArray(int *nNum);
			//************************************
			// Method:    �ͷ��豸�б�
			// FullName:  CPhoneCenter::Free
			// Access:    public 
			// Returns:   _DLLEXPORT void
			// Qualifier:
			// Parameter: CPhone * * pArray
			//************************************
			_DLLEXPORT void Free(CPhone** pArray);
			//************************************
			// Method:    wifi����
			// FullName:  CPhoneCenter::Connect
			// Access:    public 
			// Returns:   _DLLEXPORT CPhone *
			// Qualifier:
			// Parameter: const char * szIP
			// Parameter: EnumPhoneType type
			//************************************
			_DLLEXPORT CPhone *Connect(const char*szIP,EnumPhoneType type);
			//************************************
			// Method:    �Ƴ�һ̨�豸
			// FullName:  CPhoneCenter::Remove
			// Access:    private 
			// Returns:   _DLLEXPORT void
			// Qualifier:
			// Parameter: CPhone * pPhone
			//************************************
			_DLLEXPORT void RemovePhone(CPhone* pPhone);
	};
	class CPhone
{
public:
	//************************************
	// Method:    ����
	// FullName:  CPhone::Connect
	// Access:    public 
	// Returns:   _DLLEXPORT void
	// Qualifier:
	//************************************
	_DLLEXPORT void Connect();
	//************************************
	// Method:    �Ͽ�
	// FullName:  CPhone::DisConnect
	// Access:    public 
	// Returns:   _DLLEXPORT void
	// Qualifier:
	//************************************
	_DLLEXPORT void DisConnect();
	//************************************
	// Method:    ����״̬
	// FullName:  CPhone::ConnectState
	// Access:    public 
	// Returns:   _DLLEXPORT EnumConnectState
	// Qualifier:
	//************************************
	_DLLEXPORT EnumConnectState ConnectState();
	
    //************************************
    // Method:    ����ʧ��ԭ��
    // FullName:  CPhone::ConnectError
    // Access:    public 
    // Returns:   _DLLEXPORT int
    // Qualifier:
    //************************************
    _DLLEXPORT int ConnectError();
	//************************************
	// Method:    ����Key
	// FullName:  CPhone::Key
	// Access:    public 
	// Returns:   _DLLEXPORT char*
	// Qualifier:
	//************************************
	_DLLEXPORT char* Key();
	//************************************
	// Method:    �ֻ��Ƿ����ӵ���
	// FullName:  CPhone::IsConnect
	// Access:    public 
	// Returns:   _DLLEXPORT bool
	// Qualifier:
	//************************************
	_DLLEXPORT bool IsConnect();

	//************************************
	// Method:    �����¼�ע��
	// FullName:  CPhone::AddPropertyChanged
	// Access:    public 
	// Returns:   _DLLEXPORT void
	// Qualifier:
	// Parameter: PropertyChanged _func
	//************************************
	_DLLEXPORT void AddPropertyChanged(PropertyChanged _func,void *pParam);
	//************************************
	// Method:    �����¼�ע��
	// FullName:  CPhone::RemovePropertyChanged
	// Access:    public 
	// Returns:   _DLLEXPORT void
	// Qualifier:
	// Parameter: PropertyChanged _func
	//************************************
	_DLLEXPORT void RemovePropertyChanged(PropertyChanged _func,void*pParam );
	};
#endif

#endif // _C++VER_
