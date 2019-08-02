// System.h: interface for the CSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEM_H__1BB390A6_DE0C_407D_AF69_0CC171B47386__INCLUDED_)
#define AFX_SYSTEM_H__1BB390A6_DE0C_407D_AF69_0CC171B47386__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define VCEXPORT 
#define HIGHEXPORT

struct __POSITION { };
typedef __POSITION* POSITION;

enum EquipmentType
{
	//其它类型
	Other=0x00,
	//完全无驱动时出现的设备
	Main=0x01,
	//ADB设备,android使用
	ADB=0x02,
		
};
//#pragma comment(lib,"lib\\DriverInstall.lib")
class CBackInfo  
{
public:
	//************************************
	// Method:    操作开始
	// FullName:  CBackInfo::OnStart
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	//************************************
	virtual bool OnStart()=0;
	
	//************************************
	// Method:    操作结束
	// FullName:  CBackInfo::OnEnd
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Parameter: bool bSuccess
	// Parameter: int nErrorCode
	//************************************
	virtual bool OnEnd(bool bSuccess,int nErrorCode)=0;
	//************************************
	// Method:    子进度条
	// FullName:  CBackInfo::OnProgress
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const char * szFile
	// Parameter: ULONG nCur
	// Parameter: ULONG nMax
	//************************************
	virtual bool OnProgress(const char*szFile,ULONG nCur,ULONG nMax)=0;
	//************************************
	// Method:    提示消息
	// FullName:  CBackInfo::OnMessage
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const char * szData
	//************************************
	virtual bool OnMessage(const char *szData)=0;
	//************************************
	// Method:    选择提示
	// FullName:  CBackInfo::OnSelect
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const char * szData
	//************************************
	virtual bool OnSelect(const char *szData)=0;
	
	//************************************
	// Method:    总进度条
	// FullName:  CBackInfo::OnMainProgress
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const char * szFile
	// Parameter: ULONG nCur
	// Parameter: ULONG nMax
	//************************************
	virtual bool OnMainProgress(const char *szFile,ULONG nCur,ULONG nMax)=0;
};
////////////////////////////
//设备信息获取
////////////////////////////
class CEquipment  
{
public:
	////////////////////////////
	//信息获取
	////////////////////////////
	//************************************
	// Method:    获取设备错误信息
	// FullName:  CAssembleEnvironment::GetError
	// Access:    public 
	// Returns:   VCEXPORT ULONG
	// Qualifier:
	//************************************
	VCEXPORT ULONG GetError();
	//************************************
	// Method:    获取com口的信息 ： COM %i
	// FullName:  CEquipment::GetCom
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetCom();
		//************************************
	// Method:    获取相关inf文件名
	// FullName:  CEquipment::GetInf
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetInf();
	//************************************
	// Method:    获取设备范例ID（唯一确定一个设备）
	// FullName:  CEquipment::GetInterface
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetInterface();
	//************************************
	// Method:    获取制造厂商
	// FullName:  CEquipment::GetProviderName
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetProviderName();
	//************************************
	// Method:    获取HardID（集合）
	// FullName:  CEquipment::GetHardID
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetHardID();
	//************************************
	// Method:    获取相关名词，具体意义由 厂家决定 
	// FullName:  CEquipment::GetFriendlyName
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetFriendlyName();
	//************************************
	// Method:    获取设备类型
	// FullName:  CEquipment::GetClass
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetClass();
	//************************************
	// Method:    获取设备驱动
	// FullName:  CEquipment::GetDriver
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetDriver();
	//************************************
	// Method:    获取设备名
	// FullName:  CEquipment::GetDeviceDesc
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetDeviceDesc();	
	//************************************
	// Method:    获取PID
	// FullName:  CEquipment::GetPID
	// Access:    public 
	// Returns:   VCEXPORT int
	// Qualifier:
	//************************************
	VCEXPORT int GetPID();
	//************************************
	// Method:    获取VID
	// FullName:  CEquipment::GetVID
	// Access:    public 
	// Returns:   VCEXPORT int
	// Qualifier:
	//************************************
	VCEXPORT int GetVID();
	//************************************
	// Method:    是否连接
	// FullName:  CEquipment::IsConnect
	// Access:    public 
	// Returns:   VCEXPORT BOOL
	// Qualifier:
	//************************************
	VCEXPORT BOOL IsConnect();
	//************************************
	// Method:    获取设备类型
	// FullName:  CEquipment::GetType
	// Access:    public 
	// Returns:   VCEXPORT EquipmentType
	// Qualifier:
	//************************************
	VCEXPORT EquipmentType GetType();
	//************************************
	// Method:    获取配合使用的ID
	// FullName:  CEquipment::GetCompatibleID
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetCompatibleID();
	//************************************
	// Method:    获取GUID
	// FullName:  CEquipment::GetGUID
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetGUID();
	//************************************
	// Method:    获取关联ID
	// FullName:  CEquipment::GetParentIDPrefix
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetParentIDPrefix();
	//************************************
	// Method:    获取本地信息
	// FullName:  CEquipment::GetLocalInfo
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetLocalInfo();

	//************************************
	// Method:    获取Key
	// FullName:  CEquipment::GetKey
	// Access:    public 
	// Returns:   VCEXPORT int
	// Qualifier:
	//************************************
	VCEXPORT int GetKey();

	//************************************
	// Method:    获取HardID（集合）数量
	// FullName:  CEquipment::GetHardIDNum
	// Access:    public 
	// Returns:   VCEXPORT int
	// Qualifier:
	//************************************
	VCEXPORT int GetHardIDNum() const;
	//************************************
	// Method:    获取指定的HardID
	// FullName:  CEquipment::GetHardID
	// Access:    public 
	// Returns:   VCEXPORT CString
	// Qualifier:
        // Parameter: int i
	//************************************
	VCEXPORT char* GetHardID(int i) const;
};
enum PhoneTypeEnum
{
	PHONE_SYBIAN=0x1000,
	PHONE_S40	=	PHONE_SYBIAN|0x100,
	PHONE_S40_2=		PHONE_S40|0x10,
	PHONE_S40_3	=	PHONE_S40|0x20,
	PHONE_S40_3i	=PHONE_S40_3|0x01,
	PHONE_S40_3_F1	=PHONE_S40_3|0x02,
	PHONE_S40_5	=	PHONE_S40|0x30,
	PHONE_S40_6	=	PHONE_S40|0x40,
	PHONE_S60		=PHONE_SYBIAN|0x200,
	PHONE_S60_1	=	PHONE_S60|0x10,
	PHONE_S60_2	=	PHONE_S60|0x20,
	PHONE_S60_3	=	PHONE_S60|0x30,
	PHONE_S60_3_SUITE	=	PHONE_S60_3|0x0a,
	PHONE_S60_3_SUITE_MR=	PHONE_S60_3|0x0c,
	PHONE_S60_3_FP1=	PHONE_S60_3|0x01,
	PHONE_S60_3_FP2=	PHONE_S60_3|0x02,
	PHONE_S60_5	=	PHONE_S60|0x40,

	PHONE_IPHONE=	0x2000,
	PHONE_WM	=	0x3000,
	PHONE_ANDROID=	0x4000,
	PHONE_ANDROID_NOADB=	0x4000|0x0001,
	PHONE_ANDROID_NEEDDRIVER=0x4000|0x0002,	//无驱动设备
	PHONE_UNKOWN=0x5000,//未知设备
	PHONE_NOPHONE=0x6000,//确定非手机设备
};
enum EnumInfoDes
{
		//设备类别
		EnumInfoDes_DeviceList,
		//通用规则
		EnumInfoDes_CommonRule,
		//自定义规则
		EnumInfoDes_User,
};
class CAssembleEnvironment  
{
public:

	//************************************
	// Method:    获取设备信息的数量
	// FullName:  CAssembleEnvironment::GetCount
	// Access:    public 
	// Returns:   VCEXPORT int
	// Qualifier:
	//************************************
	VCEXPORT int GetCount();
	//************************************
	// Method:    获取下一个设备信息
	// FullName:  CAssembleEnvironment::GetNext
	// Access:    public 
	// Returns:   VCEXPORT CEquipment*
	// Qualifier:
    // Parameter: POSITION& pos
	//************************************
	VCEXPORT CEquipment* GetNext(POSITION& pos);
	//************************************
	// Method:    获取设备信息集合的头节点
	// FullName:  CAssembleEnvironment::GetHeadPosition
	// Access:    public 
	// Returns:   VCEXPORT POSITION
	// Qualifier:
	//************************************
	VCEXPORT POSITION GetHeadPosition();
	//************************************
	// Method:    是否连接
	// FullName:  CAssembleEnvironment::IsConnect
	// Access:    public 
	// Returns:   VCEXPORT BOOL
	// Qualifier:
	//************************************
	VCEXPORT BOOL IsConnect();
	//************************************
	// Method:    保存规则
	// FullName:  CAssembleEnvironment::SaveRule
	// Access:    public 
	// Returns:   VCEXPORT void
	// Qualifier:
	//************************************
	HIGHEXPORT void SaveRule();
	//************************************
	// Method:    设置手机类型
	// FullName:  CAssembleEnvironment::SetPhoneType
	// Access:    public 
	// Returns:   VCEXPORT void
	// Qualifier:
    // Parameter: PhoneTypeEnum type
	//************************************
	HIGHEXPORT void SetPhoneType(PhoneTypeEnum type);
	//************************************
	// Method:    获取手机类型
	// FullName:  CAssembleEnvironment::GetPhoneType
	// Access:    public 
	// Returns:   VCEXPORT PhoneTypeEnum
	// Qualifier:
	//************************************
	VCEXPORT PhoneTypeEnum GetPhoneType();
	//************************************
	// Method:    驱动是否安装正常
	// FullName:  CAssembleEnvironment::IsOk
	// Access:    public 
	// Returns:   VCEXPORT BOOL
	// Qualifier:
	//************************************
	VCEXPORT BOOL IsOk();
	//************************************
	// Method:    安装驱动
	// FullName:  CAssembleEnvironment::Install
	// Access:    public 
	// Returns:   VCEXPORT BOOL
	// Qualifier:
    // Parameter: const char* szPath
	//************************************
	VCEXPORT BOOL Install(const char*szPath);
	//************************************
	// Method:    保存驱动
	// FullName:  CAssembleEnvironment::SaveDriver
	// Access:    public 
	// Returns:   VCEXPORT void
	// Qualifier:
	//************************************
	VCEXPORT void SaveDriver();
	//************************************
	// Method:    重装驱动
	// FullName:  CAssembleEnvironment::InstallDriver
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	VCEXPORT bool InstallDriver();
	//************************************
	// Method:    卸载驱动
	// FullName:  CAssembleEnvironment::UnInstall
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	HIGHEXPORT	bool UnInstall();
	//************************************
	// Method:    信息来源
	// FullName:  CAssembleEnvironment::GetInfoDes
	// Access:    public 
	// Returns:   VCEXPORT EnumInfoDes
	// Qualifier:
	//************************************
	HIGHEXPORT	EnumInfoDes GetInfoDes();
	//************************************
	// Method:    获取必需安装ID个数，此方法需少调用。
	// FullName:  CAssembleEnvironment::GetInstallNum
	// Access:    public 
	// Returns:   VCEXPORT int
	// Qualifier:
	//************************************
	HIGHEXPORT int GetInstallNum();
	//************************************
	// Method:    获取必需安装ID信息
	// FullName:  CAssembleEnvironment::GetInstallID
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
    // Parameter: int i
	//************************************
	HIGHEXPORT char* GetInstallID(int i);

	//************************************
	// Method:    上传驱动
	// FullName:  CAssembleEnvironment::UploadDevice
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
    // Parameter: CString strPath
	//************************************
	VCEXPORT bool UploadDevice(CString strPath);
	////////////////////////////////////

	//************************************
	// Method:    获取驱动URL
	// FullName:  CAssembleEnvironment::GetDeviceURL
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	HIGHEXPORT char* GetDeviceURL();
	//************************************
	// Method:    是否可以上传
	// FullName:  CAssembleEnvironment::CanUpload
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	VCEXPORT bool CanUpload();
	//************************************
	// Method:    是否可以下载
	// FullName:  CAssembleEnvironment::CanDown
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	VCEXPORT bool CanDown();

	
	//************************************
	// Method:    获取各平台驱动信息
	// FullName:  CAssembleEnvironment::GetAllServerDevice
	// Access:    public 
	// Returns:   VCEXPORT void
	// Qualifier:
	// Parameter: BOOL * bRet
	//************************************
	HIGHEXPORT void GetAllServerDevice(BOOL *bRet);

	//************************************
	// Method:    批量保存驱动信息
	// FullName:  CAssembleEnvironment::BatchSaveDriver
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier: 
	//************************************
	HIGHEXPORT bool BatchSaveDriver();
	//************************************
	// Method:    上传设备信息
	// FullName:  CAssembleEnvironment::UploadEquipmentInfo
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	VCEXPORT bool UploadEquipmentInfo();
	//************************************
	// Method:    查找网络设备信息
	// FullName:  CAssembleEnvironment::SearchEquipmentInfo
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	//************************************
	VCEXPORT bool SearchEquipmentInfo();
	//************************************
	// Method:    下载设备是否可用
	// FullName:  CAssembleEnvironment::SetEquipmentInfo
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	// Parameter: BOOL bSucc
	//************************************
	VCEXPORT bool SetEquipmentInfo(BOOL bSucc);

	//************************************
	// Method:    获取主设备
	// FullName:  CAssembleEnvironment::GetMainEquipment
	// Access:    public 
	// Returns:   VCEXPORT	CEquipment*
	// Qualifier:
	//************************************
	VCEXPORT	CEquipment* GetMainEquipment();

	//************************************
	// Method:    上传受信设备信息
	// FullName:  CAssembleEnvironment::UploadEquipmentInfo_Trustworthy
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	// Parameter: char * szKey 值
	//************************************
	HIGHEXPORT bool UploadEquipmentInfo_Trustworthy(char*szKey);

	//************************************
	// Method:    是否拥有设备信息
	// FullName:  CAssembleEnvironment::HasEquipmentInfo
	// Access:    public 
	// Returns:   VCEXPORT	bool
	// Qualifier:
	//************************************
	VCEXPORT	bool HasEquipmentInfo();

	
	//************************************
	// Method:    是否可用通用驱动
	// FullName:  CAssembleEnvironment::IsUseCommonDriver
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	VCEXPORT bool CanUseCommonDriver();

	//************************************
	// Method:    通用驱动路径
	// FullName:  CAssembleEnvironment::GetCommonDriverPath
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetCommonDriverPath();
	//************************************
	// Method:    是否是非调试模式
	// FullName:  CAssembleEnvironment::IsNoADB
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	VCEXPORT bool IsNoADB();
	
	
	//************************************
	// Method:    厂家的url
	// FullName:  CAssembleEnvironment::FactoryDriverUrl
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* FactoryDriverUrl();
	
	//************************************
	// Method:    安装驱动，带进度条
	// FullName:  CAssembleEnvironment::InstallDriver
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: CBackInfo * pCallBack 进度条
	//************************************
	VCEXPORT bool InstallDriver(CBackInfo *pCallBack);
	
	//************************************
	// Method:    安装驱动并提供进度条
	// FullName:  CAssembleEnvironment::InstallDriver_Dlg
	// Access:    public 
	// Returns:   VCEXPORT BOOL
	// Qualifier:
	// Parameter: const char * szTitle
	//************************************
	VCEXPORT bool InstallDriver_Dlg(const char* szTitle);
};
class CSystem  
{
public:

     //************************************
	// Method:    获取下一个设备集合
	// FullName:  CSystem::GetNext
	// Access:    public 
	// Returns:   VCEXPORT CAssembleEnvironment*
	// Qualifier:
	// Parameter: POSITION& pos
	//************************************
	VCEXPORT CAssembleEnvironment* GetNext(POSITION& pos);
	
    //************************************
	// Method:    获取集合的节点数
	// FullName:  CSystem::GetCount
	// Access:    public 
	// Returns:   VCEXPORT int
	// Qualifier:
	//************************************
	VCEXPORT int GetCount();
	
    //************************************
	// Method:    获取集合的头节点位置
	// FullName:  CSystem::GetHeadPosition
	// Access:    public 
	// Returns:   VCEXPORT POSITION
	// Qualifier:
	//************************************
	VCEXPORT POSITION GetHeadPosition();
	//************************************
	// Method:    获取指定的节点位置
	// FullName:  CSystem::FindIndex
	// Access:    public 
	// Returns:   VCEXPORT POSITION
	// Qualifier:
	// Parameter: int i
	//************************************
	VCEXPORT POSITION FindIndex(int i);
	//************************************
	// Method:    缓存上次数据
	// FullName:  CSystem::Refresh
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	VCEXPORT bool Refresh();
	//************************************
	// Method:    释放资源
	// FullName:  CSystem::Free
	// Access:    public 
	// Returns:   VCEXPORT static void
	// Qualifier:
	// Parameter: CSystem* psystem
	//************************************
	VCEXPORT static void Free(CSystem*psystem);
	//************************************
	// Method:    创建实例
	// FullName:  CSystem::Free
	// Access:    public 
	// Returns:   VCEXPORT static void
	// Qualifier:
	// Parameter: CSystem* psystem
	//************************************
	VCEXPORT static CSystem* Create(bool bEnforce=false);
	//************************************
	// Method:    开始记录变动设备
	// FullName:  CSystem::StartRecord
	// Access:    public 
	// Returns:   VCEXPORT  void
	// Qualifier:
	//************************************
	VCEXPORT void StartRecord();
	//************************************
	// Method:    获取下一个记录的设备
	// FullName:  CSystem::GetRecordNext
	// Access:    public 
	// Returns:   VCEXPORT  CAssembleEnvironment*
	// Qualifier:
	// Parameter: POSITION& pos
	//************************************
	VCEXPORT CAssembleEnvironment* GetRecordNext(POSITION&pos);
	
	//************************************
	// Method:    获取记录设备 的数量
	// FullName:  CSystem::GetRecordCount
	// Access:    public 
	// Returns:   VCEXPORT int
	// Qualifier:
	//************************************
	VCEXPORT int GetRecordCount();
	//************************************
	// Method:    获取记录设备 的头节点
	// FullName:  CSystem::GetRecordHeadPosition
	// Access:    public 
	// Returns:   VCEXPORT POSITION
	// Qualifier:
	//************************************
	VCEXPORT POSITION GetRecordHeadPosition();
	//************************************
	// Method:    批量下载
	// FullName:  CSystem::BatchDownDriverOnly
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	// Parameter: const char* szStart  时间：2010-01-01 00:00:00
	// Parameter: const char* szPath   保存路径
	// Parameter: const char* szKey    
	//************************************
	HIGHEXPORT bool BatchDown(const char*szStart,const char* szPath,const char*szKey);
	//************************************
	// Method:    批量上传
	// FullName:  CSystem::BatchDownDriverOnly
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	// Parameter: const char* szStart  时间：2010-01-01 00:00:00
	// Parameter: const char* szPath   上传文件路径
	// Parameter: const char* szKey    
	//************************************
	HIGHEXPORT bool BatchUpload(const char*szPath,const char*szKey);
	//************************************
	// Method:    批量上传驱动
	// FullName:  CSystem::BatchDriver
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	// Parameter: const char* szKey    
	//************************************
	HIGHEXPORT bool BatchDriver(const char* szKey);
	//************************************
	// Method:    批量处理
	// FullName:  CSystem::BatchDo
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	// Parameter: const char* szKey    
	//************************************
	HIGHEXPORT bool BatchDo(const char* szKey);
	//************************************
	// Method:    解锁
	// FullName:  CSystem::Unlock
	// Access:    public 
	// Returns:   VCEXPORT void
	// Qualifier:
	//************************************
	VCEXPORT void Unlock();
	
    //************************************
	// Method:    上锁,Refresh无效
	// FullName:  CSystem::Lock
	// Access:    public 
	// Returns:   VCEXPORT void
	// Qualifier:
	//************************************
	VCEXPORT void Lock();
	//************************************
	// Method:    批量下载驱动文件
	// FullName:  CSystem::BatchDownDriverOnly
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	// Parameter: const char* szStart  时间：2010-01-01 00:00:00
	// Parameter: const char* szPath   保存路径
	// Parameter: const char* szKey    保留，不填
	//************************************
	HIGHEXPORT bool BatchDownDriverOnly(const char* szStart,const char* szPath,const char*szKey);
	//************************************
	// Method:    IPhone批量处理
	// FullName:  CSystem::Iphone
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	HIGHEXPORT bool Iphone();
	//************************************
	// Method:    强制刷新，清空所有设备信息重新获取设备信息。
	// FullName:  CSystem::ForceRefresh
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	VCEXPORT bool ForceRefresh();

	//************************************
	// Method:    系统快照
	// FullName:  CSystem::Snapshot
	// Access:    public 
	// Returns:   VCEXPORT void
	// Qualifier:
	//************************************
	VCEXPORT void Snapshot();

	//************************************
	// FullName:  CSystem::SetDriverFolderPath
	// Summary:	  设置驱动包目录路径
	// Access:    public 
	// Returns:   VCEXPORT void
	// Parameter: const char * path
	//************************************
	VCEXPORT void SetDriverPath(const char* path);
};
enum ENUM_DeviceType
{
		ENUM_DeviceType_None = 0,
		ENUM_DeviceType_iPhone = 1,
		ENUM_DeviceType_Android = 2,
		ENUM_DeviceType_OMS = 3,
		ENUM_DeviceType_MeiZu = 4,
		ENUM_DeviceType_WM = 5,
		ENUM_DeviceType_S60 = 6,
		ENUM_DeviceType_S40 = 7,
		ENUM_DeviceType_Unsupport = -1,
};
class CEquipmentService
{
public:
	//************************************
	// Method:    批量下载设备信息
	// FullName:  CDeviceService::BatchDownEquipmentInfo
	// Access:    public 
	// Returns:   VCEXPORT  bool
	// Qualifier:
	// Parameter: const char * szStartTimer
	// Parameter: const char * szPath
	//************************************
	VCEXPORT static bool BatchDownEquipmentInfo(const char* szStartTimer,const char*szPath);
	
	//************************************
	// Method:    上传受信信息
	// FullName:  CEquipmentService::UploadEquipmentInfo_Trustworthy
	// Access:    public 
	// Returns:   VCEXPORT  bool
	// Qualifier:
	// Parameter: const char * szHardID
	// Parameter: const char * szPath
	//************************************
	VCEXPORT static bool UploadEquipmentInfo_Trustworthy(const char *szHardID, const char *szPath);
	//************************************
	// Method:    查询设备信息（受信）查询情况
	// FullName:  CEquipmentService::SearchEquipmentSearchInfo
	// Access:    virtual private static 
	// Returns:   VCEXPORT  bool
	// Qualifier:
	// Parameter: char * szStartTimer
	// Parameter: char * szPath
	//************************************
	VCEXPORT static bool SearchEquipmentSearchInfo(const char *szStartTimer,const char *szPath);
	//************************************
	// Method:    查询驱动信息查询情况
	// FullName:  CDeviceService::SearchDeviceSearchInfo
	// Access:    public 
	// Returns:   VCEXPORT  bool
	// Qualifier:
	// Parameter: char * szStartTimer
	// Parameter: char * szPath
	//************************************
	VCEXPORT static bool SearchDeviceSearchInfo(const char *szStartTimer,const char *szPath);
};


#endif // !defined(AFX_SYSTEM_H__1BB390A6_DE0C_407D_AF69_0CC171B47386__INCLUDED_)
