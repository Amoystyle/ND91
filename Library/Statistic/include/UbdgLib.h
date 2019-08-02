#pragma once

#ifdef	UBDGLIB_EXPORTS
#define UBDGLIB_API __declspec(dllexport)
#else
#define UBDGLIB_API __declspec(dllimport)
#endif


typedef void* UbdgLibHandle;		// 接口句柄

struct CUSTOMER_DATA
{
	long		 userid;			// (必填)用户91账号ID
	char		 imei[100];			// (必填)手机IMEI	
	char		 imsi[100];			// (可空)手机IMSI	
	int			 type;				// (必填)手机平台类型
	char		 mobileVersion[50]; // (可空)手机固件版本号
	char  		 mobileModel[50];   // (可空)手机型号    
	char  		 userName[50];		// (可空)用户名
	char  		 phoneNumber[50];	// (可空)手机名
	char  		 comment[100];      // (可空)备注
	int			 jailbroken;		// (可空)是否越狱[-1:未知,0:未越狱(root),1:已越狱(root)] 空为-1

	CUSTOMER_DATA ()
	{
		userid			= -1;			
		type			= 0;			
		jailbroken		= -1;		
	}
};	

extern "C" {
/************************************************************************/
/*  初始化，返回句柄													*/
/************************************************************************/
UBDGLIB_API UbdgLibHandle __cdecl OpenHandle( unsigned short version,				// 协议版本(=1)
											  const CUSTOMER_DATA* pData = NULL);	// 用户数据
									 




/************************************************************************/
/* Act=3  获取数据收集规则，返回的data应该包含JSON字符串				*/
/************************************************************************/

struct UBDGLIB_RESPONSE   // 服务端返回数据
{
	int         errorno;    // 错误代码
	const char* data;       // 数据：仅GetCustomerInfo会返回数据
};


UBDGLIB_API UBDGLIB_RESPONSE __cdecl GetCustomerInfo(UbdgLibHandle handle);


/************************************************************************/
/* Act=4   上报客户资料事件数据,返回错误代码(0 = 成功)					*/
/************************************************************************/								

UBDGLIB_API int __cdecl SendCustomerData(UbdgLibHandle handle); 

// 安装,下载软件信息列表
struct APPOPERATE_INFO
{
	char		identifier[50];		// (必填)app标识符
	char		softName[50];		// (可空)app名称	
	char		softVersion[50];	// (可空)app版本	
	char		channel[50];		// (可空)渠道标识 
	char		hashValue[50];		// (必填)文件hash值
	int  		operateType;		// (可空)1.安装方式 (0未知,1一键装机,2软件库安装) 2.下载 (平台信息) 
	long  		operateTime;		// (必填)安装时间

	APPOPERATE_INFO ()
	{		
		operateType	= 0;	
		operateTime = 0;
	}
};

// 手机端应用下载记录
struct APPDOWNLOAD_RECORD
{
	int			userid;				// (必填)91帐号ID
	char		mac[50];			// (必填)PC唯一标识符	
	char		pcVersion[50];		// (可空)PC操作系统版本
	char		appVersion[50];		// (必填)装机助手版本号
	long  		sendTime;			// (必填)发送时间(当前本地时间)
	const APPOPERATE_INFO* pData;	// APPOPERATE_INFO数组指针
	int			length;				// APPOPERATE_INFO数组个数

	APPDOWNLOAD_RECORD ()
	{		
		userid		= 0;	
		sendTime	= 0;
		pData		= NULL;
		length		= 0;
	}
};

/************************************************************************/
/* Act=5  上报软件下载记录												*/
/************************************************************************/								

UBDGLIB_API int __cdecl SendAppDownloadRecord(UbdgLibHandle handle, const APPDOWNLOAD_RECORD* pData ); 


// 设备列表
struct DEVICE_LIST
{
	char		imei[100];			// (必填)手机串号
	char		imsi[50];			// (可空)IMSI	
	int			platform;			// (必填)手机平台类型
	int			connectMode;		// (可空)助手与手机的链接方式(0未知,10 wifi,20 usb)
	char  		mobileVersion[50];	// (可空)手机固件版本
	char  		mobileModel[50];	// (可空)手机型号
	int			jailbroken;			// (可空)是否越狱[-1:未知,0:未越狱(root),1:已越狱(root)] 空为-1
	const APPOPERATE_INFO* pData;	// APP_INSTALL_INFO数组指针
	int			length;				// APP_INSTALL_INFO数组个数

	DEVICE_LIST ()
	{		
		platform	= 0;	
		connectMode = 0;
		jailbroken	=-1;
		pData		=NULL;
		length		=0;
	}
};

// 手机应用安装记录
struct APPINSTALL_RECORD
{
	int			userid;				// (必填)91帐号ID
	char		mac[50];			// (必填)PC唯一标识符	
	char		pcVersion[50];		// (可空)PC操作系统版本
	char		appVersion[50];		// (必填)装机助手版本号
	long  		sendTime;			// (必填)发送时间(当前本地时间)
	const DEVICE_LIST*	pData;		// DEVICE_LIST数组指针
	int			length;				// DEVICE_LIST数组个数

	APPINSTALL_RECORD ()
	{		
		userid		= 0;	
		sendTime	= 0;
		pData		= NULL;
		length		= 0;
	}
};


/************************************************************************/
/* Act=6  上报软件安装记录												*/
/************************************************************************/								

UBDGLIB_API int __cdecl SendAppInstallRecord(UbdgLibHandle handle, const APPINSTALL_RECORD* pData ); 


/************************************************************************/
/*  Act=7  批量获取安装包信息,返回错误代码(0 = 成功)					*/
/************************************************************************/								
		
UBDGLIB_API UBDGLIB_RESPONSE __cdecl GetInstallPackageInfos( UbdgLibHandle handle, 
															 int nLegal,			// 是否正版APP	0=非正版 1=正版
															 int userid,			// 用户91账号ID	
															 int nPlatform);		// 手机平台类型


/************************************************************************/
/*  Act=8  获取单个安装包软件信息列表   返回错误代码(0 = 成功)			*/
/************************************************************************/								

UBDGLIB_API UBDGLIB_RESPONSE __cdecl GetAppItemsFromPackage( UbdgLibHandle handle, 
															 int nLegal,			// 是否正版APP  0=非正版 1=正版
															 long nPackageID,		// 安装包ID 	
															 int nPlatform);		// 手机平台类型	


// 装机助手安装记录
struct INSTALL_RECORD
{
	char		 diskid[50];		// (必填)硬盘ID
	char		 appVersion[50];	// (必填)装机助手版本号	
	char		 updateTime[50];	// (必填)升级时间	
	char		 osType[50];		// (必填)操作系统
	char  		 doNet[10];			// (必填).net版本    
	char  		 mac[50];			// (必填)MAC地址
	int			 isFirst;			// (必填)是否首次安装[0:首次,1:升级]

	INSTALL_RECORD ()
	{
		memset(doNet, '\0', 10);
		memcpy(doNet, ".NET 2.0", 10);			
		isFirst	 = -1;		
	}
};

/************************************************************************/
/*  Act=9  上报91装机助手安装量  返回错误代码(0 = 成功)	                    */
/************************************************************************/								

UBDGLIB_API int __cdecl SendInstallSum(UbdgLibHandle handle, const INSTALL_RECORD* pData );


/************************************************************************/
/*  关闭句柄，结束														*/
/************************************************************************/

UBDGLIB_API void __cdecl Close(UbdgLibHandle);

}
