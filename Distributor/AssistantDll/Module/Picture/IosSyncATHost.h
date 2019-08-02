#pragma once

#include "AssistantDll.h"

#define BUFFER_SIZE_MEDIA		32
#define BUFFER_SIZE_PREFSID		32
#define BUFFER_SIZE_PLISTCIG	21

typedef struct {
	int OperateMode; 
	int ItemCount;	
	const DeviceInfo* DevInfo;
}sConnDeviceInfo;

typedef struct
{ 
	char* photo_uuid;			    //photo_uuid, photo_name 
	wstring photo_local_path;		//photo_local_path
	wstring photo_remote_path;      //photo remote path
}sPhotoData ;

typedef struct {
	int plist_size;
	int *plist_buffer;	
}sPlistInfos;

typedef struct
{ 
	string photoAssetIDuuid;			//photo_uuid 
}sErrorFileData ;

typedef struct {
	unsigned int null_1;
	unsigned int null_2;
	unsigned int null_3;
	unsigned int str2_1;
	unsigned int str1_1;
	unsigned int str2_2;
	unsigned int str1_2;
} t_PrefsID;

enum
{
	PHOTO_DEL = 0,
	PHOTO_ADD,	
	ALBUM_RENAME,
};

//NDMT_PostMessage status
enum
{
	Msg_Status_init = 102400,
	Msg_Status_Begin_Sync,
	Msg_Status_Syncing,				//
	Msg_Status_Require_plist,		//Message plist revision	e.g. 0
	Msg_Status_Sync_finish,
	Msg_Status_Sync_error,
	Msg_Status_Sync_timeout,
	Msg_Status_Copy_Photo,			//Message: File Path			
	Msg_Status_Copy_Photo_error,	//Message: File Path			e.g. e:\\itunes_proj\\photo\\photo_folder\\2011-12-5\\00000000-0000-0000-0000-000000000067
	Msg_Status_Get_Plist_timeout,	
	Msg_Status_iTunesPrefsID,		//Message: ID			e.g. E000AA88B69DEB93
	Msg_Status_dll_load_fail,		//Message: dll name			e.g. itunes.dll
	Msg_Status_AssetMediaID,			//Message: AssetID
};

enum
{
	CALL_SUCCESS = 0,
	CALL_INPUT_PARAM_ERROR,	
	CALL_DLL_INIT_ERROR,
	CALL_ATH_CONN_ERROR,
	CALL_AFC_OPEN_ERROR,
	CALL_UNKOWN_ERROR,	
	CALL_ITUNES_VERSION_INVAILD,	
};

enum
{
	STATUS_INITIALIZE = 0,
	STATUS_SYNCALLOWED,
	STATUS_READYFORSYNC,
	STATUS_SYNCFAILED,
	STATUS_SYNCFINISHED,
	STATUS_ASSETMANIFEST,
	STATUS_SEND_FILE_ERROR,	
	STATUS_NULL_MESSAGE,
	STATUS_CLOSE_THREAD,
};

enum
{
	STATUS_NO = 0,
	STATUS_SEND,
	STATUS_SEND_FINISH,
};

int StartSyncPhoto(sConnDeviceInfo* sDeviceData, map<wstring, wstring>* photoData, vector<char*> pictUuids);
bool StartConnectDevice(const DeviceInfo* devInfo);
char* ConverWstringtoChar(wstring source);
string ConverWstringtoString(wstring source);

static int g_iStatus = 0;					//=0表示原始态，=1表示syncAllowed，=2表示ReadyForSync，=3表示SyncFailed，=4表示SyncFinished，=5表示AssetManifest
static int g_SendRequest = STATUS_NO;		//0:no 1:yes

static int g_photo_count;		//No of photos to add
static int g_PhotoOperate;

static sPhotoData *g_pPhotoLibrary;
static HANDLE g_hCon = 0;
static HANDLE g_hMediaDictionarData;
static sErrorFileData * g_pErrorFileData;
static int g_error_file_count = 0;

static HANDLE g_AFcConnection;
static char *g_piTunesPrefsID;
static char *g_pHostName = 0;
static char *g_pItunesVersion = 0;
static char *g_pMediaBuffer;
static char *g_BplistBuffer = 0;
static int g_BplistSize = 0;
static wstring g_uuid;

static HANDLE g_hThread_Recv;
static HANDLE g_hThread_Send;

static const DeviceInfo* g_devInfo;

