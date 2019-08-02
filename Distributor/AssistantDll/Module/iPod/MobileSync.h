#ifndef __MOBILESYNC_H
#define __MOBILESYNC_H
//#include "Msg_Status.h"
#pragma once
#ifdef __cplusplus
extern "C" {
#endif




	//music_name:		22499165516719152
	//music_loacl_path:	e:\music\18. Secret (慢板).mp3
	//music_store_path	iTunes_Control/Music/F10/DCAA.mp3
	typedef struct  {
		DWORD iStatus;
		char *pszMessage;	
	}notification_callback_info;



	typedef struct
	{ 
		char music_name[32];			//music_name 
		char music_local_path[256];		//music_local_path
		char music_store_path[256];		//music_store_path 
		char artwork_local_path[256];	  	//专辑封面地址，无专辑传null	e:\\sync_data\\4620114000510352043.jpg
	}sMusicData ;

	typedef struct {
		int hWnd;
		int AFcConnection; 
		int OperateMode; 
		int ReleaseFlag;	//0:No 1:yes  
		int ItemCount;	
		char *uuid;
		int CigAddr;
		int iPodDev;
		//	sMusicData music_struct_addr[];
		//sMusicData music_struct_addr;//int *music_struct_addr;		
	}sConnDevice;

	typedef struct {
		int plist_size;
		int *plist_buffer;	
	}sPlistInfo;


	//接收消息号
	enum
	{
		Msg_Status_init = 102400,	
		Msg_Status_Begin_Sync,
		Msg_Status_Syncing,				
		Msg_Status_Require_plist,		
		Msg_Status_Sync_finish,
		Msg_Status_Sync_error,
		Msg_Status_Sync_timeout,
		Msg_Status_Copy_Music,			
		Msg_Status_Copy_Music_error,	
		Msg_Status_Get_Plist_timeout,	
		Msg_Status_iTunesPrefsID,		
		Msg_Status_dll_load_fail,		
		Msg_Status_AssetMediaID,			
		Msg_Status_Copy_Progress,	
		Msg_Status_Copy_Music_finish,	
		//同步出错的音乐Track ID
		Msg_Status_sync_error_trackID,	
	};

	//Release status
	enum
	{
		Release_Status_Normal = 0,	
		Release_Status_Device_Disconnect,	
		Release_Status_Exit,	
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
		MUSIC_DEL = 0,
		MUSIC_ADD,	
		PLAYLIST_DEL,
		PLAYLIST_ADD,
		PLAYLIST_UPDATE,
		VOICEMEMO_ADD,
		VOICEMEMO_DEL,
		RINGTONE_ADD,
		RINGTONE_DEL,
		PHOTO_ADD,
		PHOTO_DEL,
		BOOK_ADD,
		BOOK_DEL,
		APP_ADD,
		APP_DEL,	
	};


	typedef void (*notification_callback)(notification_callback_info *);

	int ND_IOS5_SyncMusic(sConnDevice *data,sMusicData music[], notification_callback callback, notification_callback_info **notification);
	int ND_SendPlistAddr(sPlistInfo *data);
	int ND_ReturnHwndAdd(int hWnd);
	int ND_ReturnMusicStruct(sConnDevice *data);
	//int ND_ReturnMusic(sConnDevice *Conn,sMusicData data[]);
	char *ND_IOS5_CheckDLL();
	void ND_IOS5_Free();


#ifdef __cplusplus
}
#endif

#endif

