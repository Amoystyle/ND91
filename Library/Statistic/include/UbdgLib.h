#pragma once

#ifdef	UBDGLIB_EXPORTS
#define UBDGLIB_API __declspec(dllexport)
#else
#define UBDGLIB_API __declspec(dllimport)
#endif


typedef void* UbdgLibHandle;		// �ӿھ��

struct CUSTOMER_DATA
{
	long		 userid;			// (����)�û�91�˺�ID
	char		 imei[100];			// (����)�ֻ�IMEI	
	char		 imsi[100];			// (�ɿ�)�ֻ�IMSI	
	int			 type;				// (����)�ֻ�ƽ̨����
	char		 mobileVersion[50]; // (�ɿ�)�ֻ��̼��汾��
	char  		 mobileModel[50];   // (�ɿ�)�ֻ��ͺ�    
	char  		 userName[50];		// (�ɿ�)�û���
	char  		 phoneNumber[50];	// (�ɿ�)�ֻ���
	char  		 comment[100];      // (�ɿ�)��ע
	int			 jailbroken;		// (�ɿ�)�Ƿ�Խ��[-1:δ֪,0:δԽ��(root),1:��Խ��(root)] ��Ϊ-1

	CUSTOMER_DATA ()
	{
		userid			= -1;			
		type			= 0;			
		jailbroken		= -1;		
	}
};	

extern "C" {
/************************************************************************/
/*  ��ʼ�������ؾ��													*/
/************************************************************************/
UBDGLIB_API UbdgLibHandle __cdecl OpenHandle( unsigned short version,				// Э��汾(=1)
											  const CUSTOMER_DATA* pData = NULL);	// �û�����
									 




/************************************************************************/
/* Act=3  ��ȡ�����ռ����򣬷��ص�dataӦ�ð���JSON�ַ���				*/
/************************************************************************/

struct UBDGLIB_RESPONSE   // ����˷�������
{
	int         errorno;    // �������
	const char* data;       // ���ݣ���GetCustomerInfo�᷵������
};


UBDGLIB_API UBDGLIB_RESPONSE __cdecl GetCustomerInfo(UbdgLibHandle handle);


/************************************************************************/
/* Act=4   �ϱ��ͻ������¼�����,���ش������(0 = �ɹ�)					*/
/************************************************************************/								

UBDGLIB_API int __cdecl SendCustomerData(UbdgLibHandle handle); 

// ��װ,���������Ϣ�б�
struct APPOPERATE_INFO
{
	char		identifier[50];		// (����)app��ʶ��
	char		softName[50];		// (�ɿ�)app����	
	char		softVersion[50];	// (�ɿ�)app�汾	
	char		channel[50];		// (�ɿ�)������ʶ 
	char		hashValue[50];		// (����)�ļ�hashֵ
	int  		operateType;		// (�ɿ�)1.��װ��ʽ (0δ֪,1һ��װ��,2����ⰲװ) 2.���� (ƽ̨��Ϣ) 
	long  		operateTime;		// (����)��װʱ��

	APPOPERATE_INFO ()
	{		
		operateType	= 0;	
		operateTime = 0;
	}
};

// �ֻ���Ӧ�����ؼ�¼
struct APPDOWNLOAD_RECORD
{
	int			userid;				// (����)91�ʺ�ID
	char		mac[50];			// (����)PCΨһ��ʶ��	
	char		pcVersion[50];		// (�ɿ�)PC����ϵͳ�汾
	char		appVersion[50];		// (����)װ�����ְ汾��
	long  		sendTime;			// (����)����ʱ��(��ǰ����ʱ��)
	const APPOPERATE_INFO* pData;	// APPOPERATE_INFO����ָ��
	int			length;				// APPOPERATE_INFO�������

	APPDOWNLOAD_RECORD ()
	{		
		userid		= 0;	
		sendTime	= 0;
		pData		= NULL;
		length		= 0;
	}
};

/************************************************************************/
/* Act=5  �ϱ�������ؼ�¼												*/
/************************************************************************/								

UBDGLIB_API int __cdecl SendAppDownloadRecord(UbdgLibHandle handle, const APPDOWNLOAD_RECORD* pData ); 


// �豸�б�
struct DEVICE_LIST
{
	char		imei[100];			// (����)�ֻ�����
	char		imsi[50];			// (�ɿ�)IMSI	
	int			platform;			// (����)�ֻ�ƽ̨����
	int			connectMode;		// (�ɿ�)�������ֻ������ӷ�ʽ(0δ֪,10 wifi,20 usb)
	char  		mobileVersion[50];	// (�ɿ�)�ֻ��̼��汾
	char  		mobileModel[50];	// (�ɿ�)�ֻ��ͺ�
	int			jailbroken;			// (�ɿ�)�Ƿ�Խ��[-1:δ֪,0:δԽ��(root),1:��Խ��(root)] ��Ϊ-1
	const APPOPERATE_INFO* pData;	// APP_INSTALL_INFO����ָ��
	int			length;				// APP_INSTALL_INFO�������

	DEVICE_LIST ()
	{		
		platform	= 0;	
		connectMode = 0;
		jailbroken	=-1;
		pData		=NULL;
		length		=0;
	}
};

// �ֻ�Ӧ�ð�װ��¼
struct APPINSTALL_RECORD
{
	int			userid;				// (����)91�ʺ�ID
	char		mac[50];			// (����)PCΨһ��ʶ��	
	char		pcVersion[50];		// (�ɿ�)PC����ϵͳ�汾
	char		appVersion[50];		// (����)װ�����ְ汾��
	long  		sendTime;			// (����)����ʱ��(��ǰ����ʱ��)
	const DEVICE_LIST*	pData;		// DEVICE_LIST����ָ��
	int			length;				// DEVICE_LIST�������

	APPINSTALL_RECORD ()
	{		
		userid		= 0;	
		sendTime	= 0;
		pData		= NULL;
		length		= 0;
	}
};


/************************************************************************/
/* Act=6  �ϱ������װ��¼												*/
/************************************************************************/								

UBDGLIB_API int __cdecl SendAppInstallRecord(UbdgLibHandle handle, const APPINSTALL_RECORD* pData ); 


/************************************************************************/
/*  Act=7  ������ȡ��װ����Ϣ,���ش������(0 = �ɹ�)					*/
/************************************************************************/								
		
UBDGLIB_API UBDGLIB_RESPONSE __cdecl GetInstallPackageInfos( UbdgLibHandle handle, 
															 int nLegal,			// �Ƿ�����APP	0=������ 1=����
															 int userid,			// �û�91�˺�ID	
															 int nPlatform);		// �ֻ�ƽ̨����


/************************************************************************/
/*  Act=8  ��ȡ������װ�������Ϣ�б�   ���ش������(0 = �ɹ�)			*/
/************************************************************************/								

UBDGLIB_API UBDGLIB_RESPONSE __cdecl GetAppItemsFromPackage( UbdgLibHandle handle, 
															 int nLegal,			// �Ƿ�����APP  0=������ 1=����
															 long nPackageID,		// ��װ��ID 	
															 int nPlatform);		// �ֻ�ƽ̨����	


// װ�����ְ�װ��¼
struct INSTALL_RECORD
{
	char		 diskid[50];		// (����)Ӳ��ID
	char		 appVersion[50];	// (����)װ�����ְ汾��	
	char		 updateTime[50];	// (����)����ʱ��	
	char		 osType[50];		// (����)����ϵͳ
	char  		 doNet[10];			// (����).net�汾    
	char  		 mac[50];			// (����)MAC��ַ
	int			 isFirst;			// (����)�Ƿ��״ΰ�װ[0:�״�,1:����]

	INSTALL_RECORD ()
	{
		memset(doNet, '\0', 10);
		memcpy(doNet, ".NET 2.0", 10);			
		isFirst	 = -1;		
	}
};

/************************************************************************/
/*  Act=9  �ϱ�91װ�����ְ�װ��  ���ش������(0 = �ɹ�)	                    */
/************************************************************************/								

UBDGLIB_API int __cdecl SendInstallSum(UbdgLibHandle handle, const INSTALL_RECORD* pData );


/************************************************************************/
/*  �رվ��������														*/
/************************************************************************/

UBDGLIB_API void __cdecl Close(UbdgLibHandle);

}
