
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

// Summary  : 取指定目录下的文件
// Returns  : buffer长度: 0表示无数据, 大于0需要调用FreeBuffer释放*buff
//                        -1 表示网络错误, -2表示buff为NULL, -3 表示path太长
// Parameter: WIFI_HANDLE handle (connect函数返回的)连接句柄
// Parameter: char * path 指定目录
// Parameter: char ** buff 输出参数, 返回用/分隔文件名的字符串指针
NDIPHONENET_API long GetFiles( WIFI_HANDLE handle, char* path, char** buff );

// Summary  : 取指定目录下的目录
// Returns  : buffer长度: 0 : 无数据, > 0 : 正常返回, 需用FreeBuffer释放*buff
//                        -1 : 网络错误, -2 : buff为NULL, -3 : path太长
// Parameter: WIFI_HANDLE handle (connect函数返回的)连接句柄
// Parameter: char * path 指定目录
// Parameter: char ** buff 输出参数, 返回用/分隔目录名的字符串指针
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

// Summary  : LOG控制函数
//            log文件的最大size=6291456，超过会新建一个
//            未来若需支持LOG SERVER再增加参数
// Parameter: bool on: true开, false关(此时后二参数无用)
// Parameter: int level:  log级别, 0~8(参见cplog.hxx)
// Parameter: const char * filename : 指定log文件完整路径
NDIPHONENET_API void EnableLog(bool on, int level, const char* filename);

// Summary  : 释放内存
// Parameter: char * pBuffer 内存指针
NDIPHONENET_API void FreeBuffer(char* pBuffer);

}
