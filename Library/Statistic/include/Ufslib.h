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
// Summary  : ��ʼ��ͳ�ƽӿ� (��Ʒ����ʱ����) �ӷ�����ȡ���ù����ʼ�������ļ�·��
// Returns  : UfslibHandle		��ʼ���ɹ����ز��������ʧ�ܷ���NULL
// Parameter: const wchar_t * dir			���������ͳ������Ŀ¼	
// Parameter: int id						�������ƷID
// Parameter: const char * key				�������ƷKey
// Parameter: const char * appVersion		�������Ʒ�汾��
// Parameter: const char * channel			���ɿգ�����ID
// Parameter: const char * lang				���������	����("zh-cn")
// Parameter: int  session					������Ự��ʶ
UFSLIB_API UfslibHandle Init( const wchar_t*	dir,		
								int				id,			
								const char*		key,		
								const char*		appVersion, 
								const char*		channel,    
								const char*		lang, 
								int				session );
											  	


// Summary  : ����upLoad.exe�ϱ�ͳ������ (UpLoadInitStatic,UpLoadErrorStatic, SendCompetingInfo)
//			  ( �ϱ���Ʒ����, �ϱ�Ӧ��������������Ϣ���ϴ�ʹ��ʱ��,�ϱ��쳣��־ )			���ϱ��쳣��־��δʵ�֡�
//			  ( ���ڴ��е����ݱ�����info.dat, ������upLoad.exe�ϱ��ļ��й���ͳ������ )  				
// Returns  : FUNCSTATICPROVIDER_API int
// Parameter: UfslibHandle handle
// Parameter: int nNeed							�Ƿ���Ҫ�ϱ���Ʒ����		( nNeed=0 ����Ҫ�ϱ�; nNeed=1 ��Ҫ�ϱ� )
// Parameter: const wchar_t * ErrLogFileName	������־�ļ���(����еĻ�)
UFSLIB_API bool SendData(UfslibHandle handle, int nNeed = 0, const wchar_t* ErrLogFileName = NULL);

struct DEVICE_DATA
{
	char	imei[100];			// ���ɿգ��ֻ�imei(���豸���ӵ�ʱΪ�ֻ�imei)	
	int		platForm;			// ���ɿգ��ֻ�ƽ̨����
	int		connectType;		// ���ɿգ��ֻ���PC�����ӷ�ʽ(usb/wifi)
	char	osVersion[50];		// ���ɿգ�0-50���ַ����̼��汾��
	char	romVersion[50];     // ���ɿգ�0-50���ַ���Rom�汾��
	char	model[50];          // ���ɿգ�0-50���ַ����ֻ��ͺ�
	int		jailbroken;			// ���ɿգ�ios�Ƿ�Խ��/android�Ƿ�root

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

// Summary  : �ϱ������豸��Ϣ	( ���豸����ʱ����)
// Returns  : FUNCSTATICPROVIDER_API int	 �ϱ��������
// Parameter: UfslibHandle handle			
// Parameter: const DEVICE_DATA * pdata		�豸����
UFSLIB_API int SendDevInfo(UfslibHandle handle, const DEVICE_DATA* pdata);


// Summary  : �����㹦��ͳ��
// Returns  : FUNCSTATICPROVIDER_API int	�ϱ��������
// Parameter: UfslibHandle handle
// Parameter: const char * FuncID			����ID(������Ʒ������Э���ƶ�)
// Parameter: const DEVICE_DATA * pdata		��Ӧ�豸����
// Parameter: const char * label			���ܱ�ǩ��ͬһ���ܿɸ��ݲ�ͬ�ı�ǩ����ͳ��
UFSLIB_API bool FunctionCount( UfslibHandle handle, 
								  const char* FuncID, 
								  const DEVICE_DATA* pdata=NULL, 
								  const char* label=NULL);
										


}