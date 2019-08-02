#pragma  once
#include "AssistantDll.h"
#include <GdiPlus.h>
#include <string>

#include "Module/iPod/PlaylistDB.h"
#include "Module/iPod/MobileSync.h"
#include "Module/iPod/Ios5iPodData.h"
#include "Module/iPod/Ios5BaseLocation.h"


using namespace std;
namespace ND91Assistant
{
	//音乐结构体
// 	typedef struct
// 	{ 
// 		char music_name[32];			//music_name 
// 		char music_local_path[256];		//music_local_path
// 		char music_store_path[256];		//music_store_path 
// 	}sMusicData ;
// 	//sConnDevice结构体
// 	typedef struct {
// 		int hWnd;			//窗口句柄，同步函数会发送同步消息给该窗口
// 		int AFcConnection;	//AFC连接句柄
// 		int OperateMode;	//见 //PsConnDevice->OperateMode参数值 
// 		int ReleaseFlag;	//见 //PsConnDevice->ReleaseFlag 参数值
// 		int music_count;	//音视频个数
// 		char *uuid;			//设备UUID
// 	}sConnDevice;
// 	//Plist信息结构
// 	typedef struct {
// 		int plist_size;		//plist大小
// 		int *plist_buffer;	//plist内存地址	
// 	}sPlistInfo;
// 	//回调函数
// 	typedef struct  {
// 		char  *str; 
// 		unsigned int msg;      
// 	}notification_callback_info;
// 
// 	typedef void(*notification_callback)(notification_callback_info *);
// 	typedef int (*SyncMusic)(sConnDevice *,sMusicData[], notification_callback , notification_callback_info **);
// 	typedef int (*SendPlistAddr)(sPlistInfo *data);
	//ipod5音乐管理类

	class CLASSINDLL_CLASS_DECL ciPodManager5
	{
	public:

		ciPodManager5(wstring sqlDbPath,int handle);
		~ciPodManager5();
		//获得ipod数据类
		const CIosiPodData* GetPiPodCDBData() const { return piPodCDBDataList; }

		//单独的ios5模块不做这些操作


		// 新建音乐
		CIosCDBTrackData* InsertNewTrack(wstring FileName);

		//    删除音乐
		void DeleteTrack(CIosCDBTrackData *pTrackData);

		//   新增播放列表
		void CreatePlayList(wstring PlayListName);

		//根据播放列表名删除播放列表
		void DeletePlayList(wstring PlayListName);

		//根据播放列表ID删除播放列表
		void DeletePlayList(__int64 PlayListId);

		// 修改播放列表
		void ModifyPlayList(wstring PlayListName,wstring NewFileName);

		//增加到播放列表
		void AddToPlayList(wstring PlayListName,vector<__int64> trackid,wstring NewName = L"");
		//从播放列表里移除
		void DelFromPlayList(wstring PlayListName,vector<__int64> trackid,wstring NewName = L"" );

		//生成plist字符串
		static string CreatePlistWithXML(wstring xmlstr);
		//发生plist报文
		static void SendPlistAdd(char* revision);

		static DWORD WINAPI SYNCMusicThread(LPVOID lpParameter);
		//上传CDB文件
		void UpLoadCDB();

		//获得单键实例指针
		static ciPodManager5* GetInstance();
		//销毁实例
		static	 void   DestroyInstance();
		//初始化并创建实例
		static void Init(wstring sqlDbPath,int VersionNUM,int handle);


		const cIPodData5* GetPiPodData() const { return piPodDataList; }
	private:
		//ios5添加音乐
		wstring CreatePlistAddMidea(IiPodMediaParser *pMP3Data,STU_CREATE_DBID * pstudb);
		//检查名字是否重复
		bool CheckNewName(string filename,BaseTable<DBaseLocaltion5>* pItem);
		//创建plist报文头部
		wstring CreatePlistHead();
		//创建plist报文尾部
		static wstring CreatePlistTail(char* revision);
		//创建歌曲信息的plist报文
		wstring CreatePlistAddTrackBody(IiPodMediaParser *pMP3Data,STU_CREATE_DBID * pstudb);
		//int转string
		wstring IntToStr( int i );
		//int64转string
		wstring Int64ToStr( __int64 i );
		//获得当前时间字符串yyyy-mm-ddThh:mm:ssZ格式
		static wstring getNowTimeStr();
		//获得表演者id
		__int64 GetArtistPid(wstring ArtistName,__int64 TrackPid);
		//获得专辑ID
		__int64 GetAlbumPid(wstring AlbumName,__int64 TrackPid,__int64 AlbumArtistPid);
		//同步操作
		void SyncOper(int OperateMode,sMusicData arrMusicData[]);
		//是否空的专辑
		bool IsAlbumEmpty(__int64 pid);
		//创建删除音乐文本
		wstring CreatePlistDelMidea(CIosCDBTrackData *pTrackData);
		//创建新播放列表文本
		wstring CreatePlistCreateplayList(wstring playListName,__int64& pid);
		//删除播放列表文本
		wstring CreatePlistDeleteplayList(wstring playListName,__int64& pid);
		
		//编辑播放列表文本
		wstring CreatePlistModifyPlayList(__int64 TrackPID,wstring playListName,vector<__int64> trackid);

		//删除音乐文本主体
		wstring CreatePlistDelTrackBody(CIosCDBTrackData *pTrackData);
		//创建播放列表文本主体
		wstring ciPodManager5::CreatePlistCreatePlayListBody(wstring name,__int64 pid);
		//新建播放列表ID
		__int64 CreateNewPlayListPid();
		//删除播放列表主体
		wstring CreatePlistDelPlayListBody(wstring name,__int64 pid);
		//根据播放列表名字得到pid
		__int64 FindPidByName(wstring playListName);
		//修改播放列表
		//name: 修改后的名字 pid:播放列表 pid trackid:歌曲条目
		wstring CreatePlistModifyPlayListBody(wstring name,__int64 pid,vector<__int64> trackPid);
		//查找作曲者pid
		__int64 GetComposerPid(wstring ComposerName,__int64 TrackPid);
		//查找专辑表演者
		__int64 GetAlbumArtistPid(wstring ComposerName,__int64 TrackPid);

		static void syncMusicCallBack(notification_callback_info * para);

	//	static void SyncMessage(notification_callback_info * back_info);
		BOOL GetAppVersion( wchar_t *LibName, WORD *MajorVersion, WORD *MinorVersion, WORD *BuildNumber, WORD *RevisionNumber );

		bool FindTrackByMp3Info(IiPodMediaParser * mediainfo);
	private:
		//自身实例
		static ciPodManager5* m_pInstance;
		static int m_Version;
		cIPodData5* piPodDataList;		
		wstring _sqlDbPath;				//pc上itunes所用的数据库所在路径
		 CIosiPodData* piPodCDBDataList;	//CBD文件操作数据类	
		 SqliteAccess sql;

//		SyncMusic Ios5SyncMusic;
//		SendPlistAddr Ios5SendPlistAddr;
		HINSTANCE hDLL; 
		int MessageWinHandle;
		static wstring m_plistBuffer;
		int iTunesPrvInterface;
		bool bDBActive;
		bool bSyncProcessFinsh;
		
	};
}