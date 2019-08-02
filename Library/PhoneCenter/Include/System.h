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
	//��������
	Other=0x00,
	//��ȫ������ʱ���ֵ��豸
	Main=0x01,
	//ADB�豸,androidʹ��
	ADB=0x02,
		
};
//#pragma comment(lib,"lib\\DriverInstall.lib")
class CBackInfo  
{
public:
	//************************************
	// Method:    ������ʼ
	// FullName:  CBackInfo::OnStart
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	//************************************
	virtual bool OnStart()=0;
	
	//************************************
	// Method:    ��������
	// FullName:  CBackInfo::OnEnd
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Parameter: bool bSuccess
	// Parameter: int nErrorCode
	//************************************
	virtual bool OnEnd(bool bSuccess,int nErrorCode)=0;
	//************************************
	// Method:    �ӽ�����
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
	// Method:    ��ʾ��Ϣ
	// FullName:  CBackInfo::OnMessage
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const char * szData
	//************************************
	virtual bool OnMessage(const char *szData)=0;
	//************************************
	// Method:    ѡ����ʾ
	// FullName:  CBackInfo::OnSelect
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const char * szData
	//************************************
	virtual bool OnSelect(const char *szData)=0;
	
	//************************************
	// Method:    �ܽ�����
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
//�豸��Ϣ��ȡ
////////////////////////////
class CEquipment  
{
public:
	////////////////////////////
	//��Ϣ��ȡ
	////////////////////////////
	//************************************
	// Method:    ��ȡ�豸������Ϣ
	// FullName:  CAssembleEnvironment::GetError
	// Access:    public 
	// Returns:   VCEXPORT ULONG
	// Qualifier:
	//************************************
	VCEXPORT ULONG GetError();
	//************************************
	// Method:    ��ȡcom�ڵ���Ϣ �� COM %i
	// FullName:  CEquipment::GetCom
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetCom();
		//************************************
	// Method:    ��ȡ���inf�ļ���
	// FullName:  CEquipment::GetInf
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetInf();
	//************************************
	// Method:    ��ȡ�豸����ID��Ψһȷ��һ���豸��
	// FullName:  CEquipment::GetInterface
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetInterface();
	//************************************
	// Method:    ��ȡ���쳧��
	// FullName:  CEquipment::GetProviderName
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetProviderName();
	//************************************
	// Method:    ��ȡHardID�����ϣ�
	// FullName:  CEquipment::GetHardID
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetHardID();
	//************************************
	// Method:    ��ȡ������ʣ����������� ���Ҿ��� 
	// FullName:  CEquipment::GetFriendlyName
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetFriendlyName();
	//************************************
	// Method:    ��ȡ�豸����
	// FullName:  CEquipment::GetClass
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetClass();
	//************************************
	// Method:    ��ȡ�豸����
	// FullName:  CEquipment::GetDriver
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetDriver();
	//************************************
	// Method:    ��ȡ�豸��
	// FullName:  CEquipment::GetDeviceDesc
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetDeviceDesc();	
	//************************************
	// Method:    ��ȡPID
	// FullName:  CEquipment::GetPID
	// Access:    public 
	// Returns:   VCEXPORT int
	// Qualifier:
	//************************************
	VCEXPORT int GetPID();
	//************************************
	// Method:    ��ȡVID
	// FullName:  CEquipment::GetVID
	// Access:    public 
	// Returns:   VCEXPORT int
	// Qualifier:
	//************************************
	VCEXPORT int GetVID();
	//************************************
	// Method:    �Ƿ�����
	// FullName:  CEquipment::IsConnect
	// Access:    public 
	// Returns:   VCEXPORT BOOL
	// Qualifier:
	//************************************
	VCEXPORT BOOL IsConnect();
	//************************************
	// Method:    ��ȡ�豸����
	// FullName:  CEquipment::GetType
	// Access:    public 
	// Returns:   VCEXPORT EquipmentType
	// Qualifier:
	//************************************
	VCEXPORT EquipmentType GetType();
	//************************************
	// Method:    ��ȡ���ʹ�õ�ID
	// FullName:  CEquipment::GetCompatibleID
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetCompatibleID();
	//************************************
	// Method:    ��ȡGUID
	// FullName:  CEquipment::GetGUID
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetGUID();
	//************************************
	// Method:    ��ȡ����ID
	// FullName:  CEquipment::GetParentIDPrefix
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetParentIDPrefix();
	//************************************
	// Method:    ��ȡ������Ϣ
	// FullName:  CEquipment::GetLocalInfo
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetLocalInfo();

	//************************************
	// Method:    ��ȡKey
	// FullName:  CEquipment::GetKey
	// Access:    public 
	// Returns:   VCEXPORT int
	// Qualifier:
	//************************************
	VCEXPORT int GetKey();

	//************************************
	// Method:    ��ȡHardID�����ϣ�����
	// FullName:  CEquipment::GetHardIDNum
	// Access:    public 
	// Returns:   VCEXPORT int
	// Qualifier:
	//************************************
	VCEXPORT int GetHardIDNum() const;
	//************************************
	// Method:    ��ȡָ����HardID
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
	PHONE_ANDROID_NEEDDRIVER=0x4000|0x0002,	//�������豸
	PHONE_UNKOWN=0x5000,//δ֪�豸
	PHONE_NOPHONE=0x6000,//ȷ�����ֻ��豸
};
enum EnumInfoDes
{
		//�豸���
		EnumInfoDes_DeviceList,
		//ͨ�ù���
		EnumInfoDes_CommonRule,
		//�Զ������
		EnumInfoDes_User,
};
class CAssembleEnvironment  
{
public:

	//************************************
	// Method:    ��ȡ�豸��Ϣ������
	// FullName:  CAssembleEnvironment::GetCount
	// Access:    public 
	// Returns:   VCEXPORT int
	// Qualifier:
	//************************************
	VCEXPORT int GetCount();
	//************************************
	// Method:    ��ȡ��һ���豸��Ϣ
	// FullName:  CAssembleEnvironment::GetNext
	// Access:    public 
	// Returns:   VCEXPORT CEquipment*
	// Qualifier:
    // Parameter: POSITION& pos
	//************************************
	VCEXPORT CEquipment* GetNext(POSITION& pos);
	//************************************
	// Method:    ��ȡ�豸��Ϣ���ϵ�ͷ�ڵ�
	// FullName:  CAssembleEnvironment::GetHeadPosition
	// Access:    public 
	// Returns:   VCEXPORT POSITION
	// Qualifier:
	//************************************
	VCEXPORT POSITION GetHeadPosition();
	//************************************
	// Method:    �Ƿ�����
	// FullName:  CAssembleEnvironment::IsConnect
	// Access:    public 
	// Returns:   VCEXPORT BOOL
	// Qualifier:
	//************************************
	VCEXPORT BOOL IsConnect();
	//************************************
	// Method:    �������
	// FullName:  CAssembleEnvironment::SaveRule
	// Access:    public 
	// Returns:   VCEXPORT void
	// Qualifier:
	//************************************
	HIGHEXPORT void SaveRule();
	//************************************
	// Method:    �����ֻ�����
	// FullName:  CAssembleEnvironment::SetPhoneType
	// Access:    public 
	// Returns:   VCEXPORT void
	// Qualifier:
    // Parameter: PhoneTypeEnum type
	//************************************
	HIGHEXPORT void SetPhoneType(PhoneTypeEnum type);
	//************************************
	// Method:    ��ȡ�ֻ�����
	// FullName:  CAssembleEnvironment::GetPhoneType
	// Access:    public 
	// Returns:   VCEXPORT PhoneTypeEnum
	// Qualifier:
	//************************************
	VCEXPORT PhoneTypeEnum GetPhoneType();
	//************************************
	// Method:    �����Ƿ�װ����
	// FullName:  CAssembleEnvironment::IsOk
	// Access:    public 
	// Returns:   VCEXPORT BOOL
	// Qualifier:
	//************************************
	VCEXPORT BOOL IsOk();
	//************************************
	// Method:    ��װ����
	// FullName:  CAssembleEnvironment::Install
	// Access:    public 
	// Returns:   VCEXPORT BOOL
	// Qualifier:
    // Parameter: const char* szPath
	//************************************
	VCEXPORT BOOL Install(const char*szPath);
	//************************************
	// Method:    ��������
	// FullName:  CAssembleEnvironment::SaveDriver
	// Access:    public 
	// Returns:   VCEXPORT void
	// Qualifier:
	//************************************
	VCEXPORT void SaveDriver();
	//************************************
	// Method:    ��װ����
	// FullName:  CAssembleEnvironment::InstallDriver
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	VCEXPORT bool InstallDriver();
	//************************************
	// Method:    ж������
	// FullName:  CAssembleEnvironment::UnInstall
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	HIGHEXPORT	bool UnInstall();
	//************************************
	// Method:    ��Ϣ��Դ
	// FullName:  CAssembleEnvironment::GetInfoDes
	// Access:    public 
	// Returns:   VCEXPORT EnumInfoDes
	// Qualifier:
	//************************************
	HIGHEXPORT	EnumInfoDes GetInfoDes();
	//************************************
	// Method:    ��ȡ���谲װID�������˷������ٵ��á�
	// FullName:  CAssembleEnvironment::GetInstallNum
	// Access:    public 
	// Returns:   VCEXPORT int
	// Qualifier:
	//************************************
	HIGHEXPORT int GetInstallNum();
	//************************************
	// Method:    ��ȡ���谲װID��Ϣ
	// FullName:  CAssembleEnvironment::GetInstallID
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
    // Parameter: int i
	//************************************
	HIGHEXPORT char* GetInstallID(int i);

	//************************************
	// Method:    �ϴ�����
	// FullName:  CAssembleEnvironment::UploadDevice
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
    // Parameter: CString strPath
	//************************************
	VCEXPORT bool UploadDevice(CString strPath);
	////////////////////////////////////

	//************************************
	// Method:    ��ȡ����URL
	// FullName:  CAssembleEnvironment::GetDeviceURL
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	HIGHEXPORT char* GetDeviceURL();
	//************************************
	// Method:    �Ƿ�����ϴ�
	// FullName:  CAssembleEnvironment::CanUpload
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	VCEXPORT bool CanUpload();
	//************************************
	// Method:    �Ƿ��������
	// FullName:  CAssembleEnvironment::CanDown
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	VCEXPORT bool CanDown();

	
	//************************************
	// Method:    ��ȡ��ƽ̨������Ϣ
	// FullName:  CAssembleEnvironment::GetAllServerDevice
	// Access:    public 
	// Returns:   VCEXPORT void
	// Qualifier:
	// Parameter: BOOL * bRet
	//************************************
	HIGHEXPORT void GetAllServerDevice(BOOL *bRet);

	//************************************
	// Method:    ��������������Ϣ
	// FullName:  CAssembleEnvironment::BatchSaveDriver
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier: 
	//************************************
	HIGHEXPORT bool BatchSaveDriver();
	//************************************
	// Method:    �ϴ��豸��Ϣ
	// FullName:  CAssembleEnvironment::UploadEquipmentInfo
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	VCEXPORT bool UploadEquipmentInfo();
	//************************************
	// Method:    ���������豸��Ϣ
	// FullName:  CAssembleEnvironment::SearchEquipmentInfo
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	//************************************
	VCEXPORT bool SearchEquipmentInfo();
	//************************************
	// Method:    �����豸�Ƿ����
	// FullName:  CAssembleEnvironment::SetEquipmentInfo
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	// Parameter: BOOL bSucc
	//************************************
	VCEXPORT bool SetEquipmentInfo(BOOL bSucc);

	//************************************
	// Method:    ��ȡ���豸
	// FullName:  CAssembleEnvironment::GetMainEquipment
	// Access:    public 
	// Returns:   VCEXPORT	CEquipment*
	// Qualifier:
	//************************************
	VCEXPORT	CEquipment* GetMainEquipment();

	//************************************
	// Method:    �ϴ������豸��Ϣ
	// FullName:  CAssembleEnvironment::UploadEquipmentInfo_Trustworthy
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	// Parameter: char * szKey ֵ
	//************************************
	HIGHEXPORT bool UploadEquipmentInfo_Trustworthy(char*szKey);

	//************************************
	// Method:    �Ƿ�ӵ���豸��Ϣ
	// FullName:  CAssembleEnvironment::HasEquipmentInfo
	// Access:    public 
	// Returns:   VCEXPORT	bool
	// Qualifier:
	//************************************
	VCEXPORT	bool HasEquipmentInfo();

	
	//************************************
	// Method:    �Ƿ����ͨ������
	// FullName:  CAssembleEnvironment::IsUseCommonDriver
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	VCEXPORT bool CanUseCommonDriver();

	//************************************
	// Method:    ͨ������·��
	// FullName:  CAssembleEnvironment::GetCommonDriverPath
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* GetCommonDriverPath();
	//************************************
	// Method:    �Ƿ��Ƿǵ���ģʽ
	// FullName:  CAssembleEnvironment::IsNoADB
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	VCEXPORT bool IsNoADB();
	
	
	//************************************
	// Method:    ���ҵ�url
	// FullName:  CAssembleEnvironment::FactoryDriverUrl
	// Access:    public 
	// Returns:   VCEXPORT char*
	// Qualifier:
	//************************************
	VCEXPORT char* FactoryDriverUrl();
	
	//************************************
	// Method:    ��װ��������������
	// FullName:  CAssembleEnvironment::InstallDriver
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: CBackInfo * pCallBack ������
	//************************************
	VCEXPORT bool InstallDriver(CBackInfo *pCallBack);
	
	//************************************
	// Method:    ��װ�������ṩ������
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
	// Method:    ��ȡ��һ���豸����
	// FullName:  CSystem::GetNext
	// Access:    public 
	// Returns:   VCEXPORT CAssembleEnvironment*
	// Qualifier:
	// Parameter: POSITION& pos
	//************************************
	VCEXPORT CAssembleEnvironment* GetNext(POSITION& pos);
	
    //************************************
	// Method:    ��ȡ���ϵĽڵ���
	// FullName:  CSystem::GetCount
	// Access:    public 
	// Returns:   VCEXPORT int
	// Qualifier:
	//************************************
	VCEXPORT int GetCount();
	
    //************************************
	// Method:    ��ȡ���ϵ�ͷ�ڵ�λ��
	// FullName:  CSystem::GetHeadPosition
	// Access:    public 
	// Returns:   VCEXPORT POSITION
	// Qualifier:
	//************************************
	VCEXPORT POSITION GetHeadPosition();
	//************************************
	// Method:    ��ȡָ���Ľڵ�λ��
	// FullName:  CSystem::FindIndex
	// Access:    public 
	// Returns:   VCEXPORT POSITION
	// Qualifier:
	// Parameter: int i
	//************************************
	VCEXPORT POSITION FindIndex(int i);
	//************************************
	// Method:    �����ϴ�����
	// FullName:  CSystem::Refresh
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	VCEXPORT bool Refresh();
	//************************************
	// Method:    �ͷ���Դ
	// FullName:  CSystem::Free
	// Access:    public 
	// Returns:   VCEXPORT static void
	// Qualifier:
	// Parameter: CSystem* psystem
	//************************************
	VCEXPORT static void Free(CSystem*psystem);
	//************************************
	// Method:    ����ʵ��
	// FullName:  CSystem::Free
	// Access:    public 
	// Returns:   VCEXPORT static void
	// Qualifier:
	// Parameter: CSystem* psystem
	//************************************
	VCEXPORT static CSystem* Create(bool bEnforce=false);
	//************************************
	// Method:    ��ʼ��¼�䶯�豸
	// FullName:  CSystem::StartRecord
	// Access:    public 
	// Returns:   VCEXPORT  void
	// Qualifier:
	//************************************
	VCEXPORT void StartRecord();
	//************************************
	// Method:    ��ȡ��һ����¼���豸
	// FullName:  CSystem::GetRecordNext
	// Access:    public 
	// Returns:   VCEXPORT  CAssembleEnvironment*
	// Qualifier:
	// Parameter: POSITION& pos
	//************************************
	VCEXPORT CAssembleEnvironment* GetRecordNext(POSITION&pos);
	
	//************************************
	// Method:    ��ȡ��¼�豸 ������
	// FullName:  CSystem::GetRecordCount
	// Access:    public 
	// Returns:   VCEXPORT int
	// Qualifier:
	//************************************
	VCEXPORT int GetRecordCount();
	//************************************
	// Method:    ��ȡ��¼�豸 ��ͷ�ڵ�
	// FullName:  CSystem::GetRecordHeadPosition
	// Access:    public 
	// Returns:   VCEXPORT POSITION
	// Qualifier:
	//************************************
	VCEXPORT POSITION GetRecordHeadPosition();
	//************************************
	// Method:    ��������
	// FullName:  CSystem::BatchDownDriverOnly
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	// Parameter: const char* szStart  ʱ�䣺2010-01-01 00:00:00
	// Parameter: const char* szPath   ����·��
	// Parameter: const char* szKey    
	//************************************
	HIGHEXPORT bool BatchDown(const char*szStart,const char* szPath,const char*szKey);
	//************************************
	// Method:    �����ϴ�
	// FullName:  CSystem::BatchDownDriverOnly
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	// Parameter: const char* szStart  ʱ�䣺2010-01-01 00:00:00
	// Parameter: const char* szPath   �ϴ��ļ�·��
	// Parameter: const char* szKey    
	//************************************
	HIGHEXPORT bool BatchUpload(const char*szPath,const char*szKey);
	//************************************
	// Method:    �����ϴ�����
	// FullName:  CSystem::BatchDriver
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	// Parameter: const char* szKey    
	//************************************
	HIGHEXPORT bool BatchDriver(const char* szKey);
	//************************************
	// Method:    ��������
	// FullName:  CSystem::BatchDo
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	// Parameter: const char* szKey    
	//************************************
	HIGHEXPORT bool BatchDo(const char* szKey);
	//************************************
	// Method:    ����
	// FullName:  CSystem::Unlock
	// Access:    public 
	// Returns:   VCEXPORT void
	// Qualifier:
	//************************************
	VCEXPORT void Unlock();
	
    //************************************
	// Method:    ����,Refresh��Ч
	// FullName:  CSystem::Lock
	// Access:    public 
	// Returns:   VCEXPORT void
	// Qualifier:
	//************************************
	VCEXPORT void Lock();
	//************************************
	// Method:    �������������ļ�
	// FullName:  CSystem::BatchDownDriverOnly
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	// Parameter: const char* szStart  ʱ�䣺2010-01-01 00:00:00
	// Parameter: const char* szPath   ����·��
	// Parameter: const char* szKey    ����������
	//************************************
	HIGHEXPORT bool BatchDownDriverOnly(const char* szStart,const char* szPath,const char*szKey);
	//************************************
	// Method:    IPhone��������
	// FullName:  CSystem::Iphone
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	HIGHEXPORT bool Iphone();
	//************************************
	// Method:    ǿ��ˢ�£���������豸��Ϣ���»�ȡ�豸��Ϣ��
	// FullName:  CSystem::ForceRefresh
	// Access:    public 
	// Returns:   VCEXPORT bool
	// Qualifier:
	//************************************
	VCEXPORT bool ForceRefresh();

	//************************************
	// Method:    ϵͳ����
	// FullName:  CSystem::Snapshot
	// Access:    public 
	// Returns:   VCEXPORT void
	// Qualifier:
	//************************************
	VCEXPORT void Snapshot();

	//************************************
	// FullName:  CSystem::SetDriverFolderPath
	// Summary:	  ����������Ŀ¼·��
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
	// Method:    ���������豸��Ϣ
	// FullName:  CDeviceService::BatchDownEquipmentInfo
	// Access:    public 
	// Returns:   VCEXPORT  bool
	// Qualifier:
	// Parameter: const char * szStartTimer
	// Parameter: const char * szPath
	//************************************
	VCEXPORT static bool BatchDownEquipmentInfo(const char* szStartTimer,const char*szPath);
	
	//************************************
	// Method:    �ϴ�������Ϣ
	// FullName:  CEquipmentService::UploadEquipmentInfo_Trustworthy
	// Access:    public 
	// Returns:   VCEXPORT  bool
	// Qualifier:
	// Parameter: const char * szHardID
	// Parameter: const char * szPath
	//************************************
	VCEXPORT static bool UploadEquipmentInfo_Trustworthy(const char *szHardID, const char *szPath);
	//************************************
	// Method:    ��ѯ�豸��Ϣ�����ţ���ѯ���
	// FullName:  CEquipmentService::SearchEquipmentSearchInfo
	// Access:    virtual private static 
	// Returns:   VCEXPORT  bool
	// Qualifier:
	// Parameter: char * szStartTimer
	// Parameter: char * szPath
	//************************************
	VCEXPORT static bool SearchEquipmentSearchInfo(const char *szStartTimer,const char *szPath);
	//************************************
	// Method:    ��ѯ������Ϣ��ѯ���
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
