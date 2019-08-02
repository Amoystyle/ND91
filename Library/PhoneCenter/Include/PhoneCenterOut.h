#pragma once
//设备类型
enum EnumPhoneType
{
	//android
	EnumPhoneType_Android,
	//iphone
	EnumPhoneType_IPhone,
};

//设备列表变动操作
enum NotifyCollectionChangedAction
{
	//添加
	NotifyCollectionChangedAction_Add,
	//移除
	NotifyCollectionChangedAction_Remove,
};

//当前连接过程
enum EnumConnectState
{
	/// <summary>
	/// 连接初始化
	/// UI预期：当前正在检查驱动状态。
	/// </summary>
	EnumConnectState_ConnectInit,
	/// <summary>
	/// 安装初始化
	/// UI预期：开始安装驱动。
	/// </summary>
	EnumConnectState_InstallInit,
	/// <summary>
	/// 安装结束
	/// UI预期：提醒用户拔插设备。
	/// </summary>
	EnumConnectState_InstallEnd,
	/// <summary>
	/// 不进行安装
	/// UI预期：需要安装驱动，但是已无法安装成功。
	/// </summary>
	EnumConnectState_NotInstall,
	/// <summary>
	/// 开始连接
	/// UI预期：提醒用户正在连接，如果初始参数为EnumConnectSuccessSign_ADB，则无此过程。
	/// </summary>
	EnumConnectState_Connecting,
	/// <summary>
	/// 连接失败
	/// UI预期：提醒用户连接失败，失败原因由ConnectError返回，规则由其它地方处理。
	/// </summary>
	EnumConnectState_ConnectError,
	/// <summary>
	/// 连接成功
	/// </summary>
	EnumConnectState_ConnectSuccess,
	/// <summary>
	/// 断开连接
	/// </summary>
	EnumConnectState_UnConnect,
	/// <summary>
	/// 初始化状态，一定要在最后。
	/// </summary>
	EnumConnectState_NULL,
};
/// <summary>
/// 检查设备错误码
/// </summary>
enum EnumCheckErrorCode
{
	/// <summary>
	/// 连接成功
	/// </summary>
	EnumCheckErrorCode_Success,
	/// <summary>
	/// 异常导致
	/// </summary>
	EnumCheckErrorCode_Exception,
	/// <summary>
	/// 手机断开
	/// </summary>
	EnumCheckErrorCode_offline,
	/// <summary>
	/// 驱动安装失败
	/// </summary>
	EnumCheckErrorCode_DriverError,
	/// <summary>
	/// 调试模式没开
	/// </summary>
	EnumCheckErrorCode_OffDebug,
	/// <summary>
	/// 错误的连接模式。
	/// </summary>
	EnumCheckErrorCode_ConnectTypeError,
	/// <summary>
	/// 设备已经认定正常，可能是ADBbug。或者驱动装错了。
	/// </summary>
	EnumCheckErrorCode_ADBError,
	/// <summary>
	/// 设备断开
	/// </summary>
	EnumCheckErrorCode_UnConnect,
	/// <summary>
	/// itunes未找到设备
	/// </summary>
	EnumCheckErrorCode_ITunesNoFind,
	/// <summary>
	/// itunes安装后需要重新连接手机
	/// </summary>
	EnumCheckErrorCode_ItunesNeedReconnect,
	/// <summary>
	/// itunes安装后需要重新连接手机，但不弹窗。
	/// </summary>
	EnumCheckErrorCode_ItunesNeedReconnect2,
};

/// <summary>
/// 设备状态
/// 用户提示错误信息。
/// </summary>
enum HardwareStatus
{
	/// <summary>
	/// 设备连接正常
	/// </summary>
	HardwareStatus_Online,
	/// <summary>
	/// 设备连接睡死
	/// </summary>
	HardwareStatus_Offline,
	/// <summary>
	/// 设备没有打开调试模式（此状态只有 android 设备才会有）
	/// </summary>
	HardwareStatus_OffDebug,
	/// <summary>
	/// 未知错误。
	/// </summary>
	HardwareStatus_Unknown,
	/// <summary>
	/// 内存空间不足
	/// </summary>
	HardwareStatus_NoMemory,
	/// <summary>
	/// 驱动安装失败
	/// </summary>
	HardwareStatus_DriverError,
	/// <summary>
	/// 等待确认，如WIFI升级守护时，需要手动安装
	/// </summary>
	HardwareStatus_WaitForConfirm,
	/// <summary>
	/// iphone设备有密码，但是没有连接过电脑，需提醒用户输入密码。
	/// 
	/// 对不起，您的iPhone"设备名称"已使用口令锁定，系统暂时未能连接到它；
	/// 请在iPhone上输入口令后解锁后，点击”再试一次“以继续！
	/// </summary>
	HardwareStatus_IsPasswordProtected,
	/// <summary>
	/// SDK空间不足
	/// </summary>
	HardwareStatus_NoSDK,
	/// <summary>
	/// 不支持
	/// </summary>
	HardwareStatus_UnSuppert,
	/// <summary>
	/// 设备无法启动
	/// </summary>
	HardwareStatus_Unable,
	/// <summary>
	/// ADB异常。
	/// </summary>
	HardwareStatus_ADBError,
	/// <summary>
	/// 错误的连接模式
	/// </summary>
	HardwareStatus_ConnectTypeError,
};




/// <summary>
/// 设备属性
/// </summary>
enum EnumPhoneProperty
{
	/// <summary>
	/// 当前连接过程
	/// 以10进制返回，获取后转换成对应枚举
	/// 例：1代表 EnumConnectState_InstallInit
	/// </summary>
	EnumPhoneProperty_ConnectState,
	/// <summary>
	/// 安装驱动进度,最大值为100;
	/// 例：10
	/// </summary>
	EnumPhoneProperty_Progress,
	/// <summary>
	/// 安装驱动信息
	/// 例：安装驱动中
	/// </summary>
	EnumPhoneProperty_ProgressTitle,
	/// <summary>
	/// 与Iphone或者ADB对应的Key
	/// 例：iphone     usb\vid_05ac&pid_
	/// 例：android    0123456789012345  
	/// </summary>
	EnumPhoneProperty_Key,
	/// <summary>
	/// 错误码  EnumCheckErrorCode&0x1000
	/// 注：返回值为10进制。 
	/// 例：0x1000->EnumCheckErrorCode_Success 0x1001->EnumCheckErrorCode_Exception
	/// </summary>
	EnumPhoneProperty_ErrorCode,
};

/// <summary>
/// 事件类型
/// </summary>
enum EnumEvent
{
	/// <summary>
	/// 列表改变
	/// 参数返回：STCollectionChangeParam
	/// </summary>
	EnumEventCollectionChange,
	/// <summary>
	/// 属性改变
	/// 参数返回：STPhonePropertyChanged
	/// </summary>
	EnumEventPropertyChanged,
	/// <summary>
	/// dll异常
	/// </summary>
	EnumExpetion,
};

enum EnumConnectMethod
{
	//自动连接一个,无效
	EnumConnectMethod_AutoConnectOne,
	//自动连接所有
	EnumConnectMethod_AutoConnectAll,
	//不自动连接
	EnumConnectMethod_NoAuto,
};

enum EnumConnectSuccessSign
{
	//以ADB连接为成功标志
	EnumConnectSuccessSign_ADB,
	//以守护连接为成功标志
	EnumConnectSuccessSign_Guard

};

/// <summary>
/// 设备对象
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
// Method:    启动连接
// FullName:  PhoneCenter_Start
// Access:    public 
// Returns:   _DLLEXPORTC int  1代表true
// Qualifier:
// Parameter: EnumConnectMethod connectMethod     自动连接模式
// Parameter: EnumConnectSuccessSign suceessSign  连接成功标志
//************************************
_DLLEXPORTC int PhoneCenter_Start(EnumConnectMethod connectMethod,EnumConnectSuccessSign suceessSign,EventChanged _event,void *pOwn);

//************************************
// Method:    PhoneCenter_Connect
// FullName:  PhoneCenter_Connect
// Access:    public 
// Returns:   _DLLEXPORTC int  1代表true
// Qualifier:
// Parameter: CPhone * pPhone
//************************************
_DLLEXPORTC int PhoneCenter_Connect(CPhone *pPhone);

//************************************
// Method:    退出连接
// FullName:  PhoneCenter_Exit
// Access:    public 
// Returns:   _DLLEXPORTC int  1代表true
// Qualifier:
//************************************
_DLLEXPORTC int PhoneCenter_Exit();


//************************************
// Method:    获取类型值，说明等同与EnumPhoneProperty
//            不释放空间
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
			// Method:    连接中心实例
			// FullName:  CPhoneCenter::Instance
			// Access:    public 
			// Returns:   _DLLEXPORT  CPhoneCenter*
			// Qualifier:
			//************************************
			_DLLEXPORT static CPhoneCenter* Instance();
			//************************************
			// Method:    释放连接中心实例
			// FullName:  CPhoneCenter::Free
			// Access:    public 
			// Returns:   _DLLEXPORT  void
			// Qualifier:
			//************************************
			_DLLEXPORT static void Free();
			//************************************
			// Method:    启动扫描
			// FullName:  CPhoneCenter::Start
			// Access:    public 
			// Returns:   _DLLEXPORT void
			// Qualifier:
			//************************************
			_DLLEXPORT void Start();
			//************************************
			// Method:    停止扫描
			// FullName:  CPhoneCenter::Stop
			// Access:    public 
			// Returns:   _DLLEXPORT void
			// Qualifier:
			//************************************
			_DLLEXPORT void Stop();

			//************************************
			// Method:    添加设备变动事件
			// FullName:  CPhoneCenter::AddEvent
			// Access:    public 
			// Returns:   _DLLEXPORT void
			// Qualifier:
			// Parameter: CollectionChanged changedFunc
			// Parameter: void * pParam
			//************************************
			_DLLEXPORT void AddEvent(CollectionChanged changedFunc,void*pParam);
			//************************************
			// Method:    移除设备变动事件
			// FullName:  CPhoneCenter::RemoveEvent
			// Access:    public 
			// Returns:   _DLLEXPORT void
			// Qualifier:
			// Parameter: CollectionChanged changedFunc
			//************************************
			_DLLEXPORT void RemoveEvent(CollectionChanged changedFunc);

			//************************************
			// Method:    获得设备列表
			// FullName:  CPhoneCenter::ToArray
			// Access:    public 
			// Returns:   _DLLEXPORT CPhone**
			// Qualifier:
			// Parameter: int * nNum
			//************************************
			_DLLEXPORT CPhone**ToArray(int *nNum);
			//************************************
			// Method:    释放设备列表
			// FullName:  CPhoneCenter::Free
			// Access:    public 
			// Returns:   _DLLEXPORT void
			// Qualifier:
			// Parameter: CPhone * * pArray
			//************************************
			_DLLEXPORT void Free(CPhone** pArray);
			//************************************
			// Method:    wifi连接
			// FullName:  CPhoneCenter::Connect
			// Access:    public 
			// Returns:   _DLLEXPORT CPhone *
			// Qualifier:
			// Parameter: const char * szIP
			// Parameter: EnumPhoneType type
			//************************************
			_DLLEXPORT CPhone *Connect(const char*szIP,EnumPhoneType type);
			//************************************
			// Method:    移除一台设备
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
	// Method:    连接
	// FullName:  CPhone::Connect
	// Access:    public 
	// Returns:   _DLLEXPORT void
	// Qualifier:
	//************************************
	_DLLEXPORT void Connect();
	//************************************
	// Method:    断开
	// FullName:  CPhone::DisConnect
	// Access:    public 
	// Returns:   _DLLEXPORT void
	// Qualifier:
	//************************************
	_DLLEXPORT void DisConnect();
	//************************************
	// Method:    连接状态
	// FullName:  CPhone::ConnectState
	// Access:    public 
	// Returns:   _DLLEXPORT EnumConnectState
	// Qualifier:
	//************************************
	_DLLEXPORT EnumConnectState ConnectState();
	
    //************************************
    // Method:    连接失败原因
    // FullName:  CPhone::ConnectError
    // Access:    public 
    // Returns:   _DLLEXPORT int
    // Qualifier:
    //************************************
    _DLLEXPORT int ConnectError();
	//************************************
	// Method:    连接Key
	// FullName:  CPhone::Key
	// Access:    public 
	// Returns:   _DLLEXPORT char*
	// Qualifier:
	//************************************
	_DLLEXPORT char* Key();
	//************************************
	// Method:    手机是否连接电脑
	// FullName:  CPhone::IsConnect
	// Access:    public 
	// Returns:   _DLLEXPORT bool
	// Qualifier:
	//************************************
	_DLLEXPORT bool IsConnect();

	//************************************
	// Method:    属性事件注册
	// FullName:  CPhone::AddPropertyChanged
	// Access:    public 
	// Returns:   _DLLEXPORT void
	// Qualifier:
	// Parameter: PropertyChanged _func
	//************************************
	_DLLEXPORT void AddPropertyChanged(PropertyChanged _func,void *pParam);
	//************************************
	// Method:    属性事件注销
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
