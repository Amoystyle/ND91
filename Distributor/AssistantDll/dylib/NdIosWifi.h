
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the NDIPHONENET_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// NDIPHONENET_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef WIN32

	#ifdef NDIPHONENET_EXPORTS
		#define NDIPHONENET_API __declspec(dllexport)
	#else
		#define NDIPHONENET_API __declspec(dllimport)
	#endif

	#define WIFI_HANDLE unsigned int // 64bit in Mac, 32bit in Win
#else
	#define NDIPHONENET_API 

	#define WIFI_HANDLE unsigned long // 64bit in Mac, 32bit in Win
#endif


extern "C"
{

NDIPHONENET_API void ConnectionState( WIFI_HANDLE handle, bool* connected, char* ip, char* phoneversion );

NDIPHONENET_API WIFI_HANDLE Connect( char* ip, char* pasw, int* errcode );
NDIPHONENET_API bool Rename( WIFI_HANDLE handle, char* sourceName, char* destName );
NDIPHONENET_API bool DeletePhoneFile( WIFI_HANDLE handle, char* path );
NDIPHONENET_API bool DeleteDirectory( WIFI_HANDLE handle, char* path, bool recursive );

// Summary  : ȡָ��Ŀ¼�µ��ļ�
// Returns  : buffer����: 0��ʾ������, ����0��Ҫ����FreeBuffer�ͷ�*buff
//                        -1 ��ʾ�������, -2��ʾbuffΪNULL, -3 ��ʾpath̫��
// Parameter: WIFI_HANDLE handle (connect�������ص�)���Ӿ��
// Parameter: char * path ָ��Ŀ¼
// Parameter: char ** buff �������, ������/�ָ��ļ������ַ���ָ��
NDIPHONENET_API long GetFiles( WIFI_HANDLE handle, char* path, char** buff );

// Summary  : ȡָ��Ŀ¼�µ�Ŀ¼
// Returns  : buffer����: 0 : ������, > 0 : ��������, ����FreeBuffer�ͷ�*buff
//                        -1 : �������, -2 : buffΪNULL, -3 : path̫��
// Parameter: WIFI_HANDLE handle (connect�������ص�)���Ӿ��
// Parameter: char * path ָ��Ŀ¼
// Parameter: char ** buff �������, ������/�ָ�Ŀ¼�����ַ���ָ��
NDIPHONENET_API long GetDirectories( WIFI_HANDLE handle, char* path, char** buff );

NDIPHONENET_API int FileSize( WIFI_HANDLE handle, char* path );
NDIPHONENET_API bool CreatePhoneDirectory( WIFI_HANDLE handle, char* path );
NDIPHONENET_API int HasDirectories( WIFI_HANDLE handle, char* path );
NDIPHONENET_API int Exists( WIFI_HANDLE handle, char* path );
NDIPHONENET_API	int IsDirectory( WIFI_HANDLE handle, char* path );
NDIPHONENET_API bool CopyToPhone( WIFI_HANDLE handle, char* sourceOnComputer, char* destinationOnPhone );
NDIPHONENET_API unsigned int CopyToPhoneAsync( WIFI_HANDLE handle, char* sourceOnComputer, char* destinationOnPhone );
NDIPHONENET_API bool DownFromPhone( WIFI_HANDLE handle, char* sourceOnPhone, char* destinationOnComputer );
NDIPHONENET_API unsigned int DownFromPhoneAsync( WIFI_HANDLE handle, char* sourceOnPhone, char* destinationOnComputer );

NDIPHONENET_API void GetTransProgress( WIFI_HANDLE handle, int* pFileSize, int* pCompleteSize );
NDIPHONENET_API void CloseTransHandle( WIFI_HANDLE handle );

NDIPHONENET_API bool Chown( WIFI_HANDLE handle, char* file, char* owner );
NDIPHONENET_API bool SetAttribute( WIFI_HANDLE handle, char* owner, char* path, char* rights, char* result );
NDIPHONENET_API char* GetAttribute( WIFI_HANDLE handle, char* path );
NDIPHONENET_API long GetSpaceSize( WIFI_HANDLE handle, char* disk );
NDIPHONENET_API long GetSize( WIFI_HANDLE handle, char* path );
NDIPHONENET_API bool RestartiPod( WIFI_HANDLE handle );
NDIPHONENET_API bool ScreenSnap( WIFI_HANDLE handle );
NDIPHONENET_API bool Shutdown( WIFI_HANDLE handle );
NDIPHONENET_API bool Reboot( WIFI_HANDLE handle );
NDIPHONENET_API bool Restart( WIFI_HANDLE handle );

// Summary  : LOG���ƺ���
//            log�ļ������size=6291456���������½�һ��
//            δ������֧��LOG SERVER�����Ӳ���
// Parameter: bool on: true��, false��(��ʱ�����������)
// Parameter: int level:  log����, 0~8(�μ�cplog.hxx)
// Parameter: const char * filename : ָ��log�ļ�����·��
NDIPHONENET_API void EnableLog(bool on, int level, const char* filename);

// Summary  : �ͷ��ڴ�
// Parameter: char * pBuffer �ڴ�ָ��
NDIPHONENET_API void FreeBuffer(char* pBuffer);

}
