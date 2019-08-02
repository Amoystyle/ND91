#pragma once
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif
#ifdef UFSLIB_EXPORTS
#define UFSLIB_API EXTERNC __declspec(dllexport)
#else
#define UFSLIB_API EXTERNC __declspec(dllimport)
#endif
typedef void* UfslibHandle;


extern "C" {
// Summary  : 初始化统计接口 (产品启动时调用) 从服务器取配置规则初始化配置文件路径
// Returns  : UfslibHandle		初始化成功返回操作句柄，失败返回NULL
// Parameter: const wchar_t * dir			（必填）保存统计数据目录	
// Parameter: int id						（必填）产品ID
// Parameter: const char * key				（必填）产品Key
// Parameter: const char * appVersion		（必填）产品版本号
// Parameter: const char * channel			（可空）渠道ID
// Parameter: const char * lang				（必填）语言	中文("zh-cn")
// Parameter: int  session					（必填）会话标识
UFSLIB_API UfslibHandle Init( const wchar_t*	dir,		
								int				id,			
								const char*		key,		
								const char*		appVersion, 
								const char*		channel,    
								const char*		lang, 
								int				session );
											  	


// Summary  : 启动upLoad.exe上报统计数据 (UpLoadInitStatic,UpLoadErrorStatic, SendCompetingInfo)
//			  ( 上报竞品数据, 上报应用启动、渠道信息、上次使用时段,上报异常日志 )			【上报异常日志暂未实现】
//			  ( 将内存中的数据保存至info.dat, 后启动upLoad.exe上报文件中功能统计数据 )  				
// Returns  : FUNCSTATICPROVIDER_API int
// Parameter: UfslibHandle handle
// Parameter: int nNeed							是否需要上报竞品数据		( nNeed=0 不需要上报; nNeed=1 需要上报 )
// Parameter: const wchar_t * ErrLogFileName	错误日志文件名(如果有的话)
UFSLIB_API bool SendData(UfslibHandle handle, int nNeed = 0, const wchar_t* ErrLogFileName = NULL);

struct DEVICE_DATA
{
	char	imei[100];			// （可空）手机imei(有设备连接的时为手机imei)	
	int		platForm;			// （可空）手机平台类型
	int		connectType;		// （可空）手机与PC的连接方式(usb/wifi)
	char	osVersion[50];		// （可空：0-50个字符）固件版本号
	char	romVersion[50];     // （可空：0-50个字符）Rom版本号
	char	model[50];          // （可空：0-50个字符）手机型号
	int		jailbroken;			// （可空）ios是否越狱/android是否root

	DEVICE_DATA()
	{
		platForm	= 0;
		connectType	= 0;
		jailbroken	= -1;
		memset(imei, '\0', 100);
		memset(osVersion, '\0', 50);
		memset(romVersion, '\0', 50);
		memset(model, '\0', 50);
	}
};

// Summary  : 上报新增设备信息	( 新设备连接时调用)
// Returns  : FUNCSTATICPROVIDER_API int	 上报错误参数
// Parameter: UfslibHandle handle			
// Parameter: const DEVICE_DATA * pdata		设备数据
UFSLIB_API int SendDevInfo(UfslibHandle handle, const DEVICE_DATA* pdata);


// Summary  : 缓存打点功能统计
// Returns  : FUNCSTATICPROVIDER_API int	上报错误参数
// Parameter: UfslibHandle handle
// Parameter: const char * FuncID			功能ID(各个产品与服务端协商制定)
// Parameter: const DEVICE_DATA * pdata		对应设备数据
// Parameter: const char * label			功能标签：同一功能可根据不同的标签进行统计
UFSLIB_API bool FunctionCount( UfslibHandle handle, 
								  const char* FuncID, 
								  const DEVICE_DATA* pdata=NULL, 
								  const char* label=NULL);
										


}