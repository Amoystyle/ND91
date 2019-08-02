#pragma  once

#include "AssistantDll.h"
#include <string>
#include <GdiPlus.h>

#include "Module/iPod/IosiPodData.h"
#include "Module/iPod/IosCBDStruct.h"
#include "Module/iPod/iPodDbBase.h"
#include "Module/iPod/PlaylistDB.h"


using namespace std;
using namespace Gdiplus;
namespace ND91Assistant
{
// 	enum
// 	{
// 		Msg_Status_init = 102400,		//不需处理
// 		Msg_Status_Begin_Sync,			//不需处理
// 		Msg_Status_Syncing,				//不需处理
// 		Msg_Status_Require_plist,			//请求plist消息，收到该消息时，需要调用ND_SendPlistAddr，发送plist信息。10s内未收到有效信息，将返回超时。
// 		Msg_Status_Sync_finish,				//同步完成
// 		Msg_Status_Sync_error,				//同步错误
// 		Msg_Status_Sync_timeout,		//不需处理
// 		Msg_Status_Copy_Music,				//当前拷贝的音视频文件		e.g. e:\music\18. Secret (慢板).mp3
// 		Msg_Status_Copy_Music_error,		//当前拷贝出错的文件		e.g. e:\music\18. Secret (慢板).mp3
// 		Msg_Status_Get_Plist_timeout,		//请求plist文件超时
// 		Msg_Status_iTunesPrefsID,		//不需处理
// 		Msg_Status_dll_load_fail,			//dll加载失败			e.g. itunes.dll
// 		Msg_Status_AssetMediaID,		//不需处理	
// 		Msg_Status_Copy_Progress,			//当前拷贝进度，以KB为单位
// 	};

	//iPod4音乐管理类
	class CLASSINDLL_CLASS_DECL ciPodManager : public iPodDbBase
	{	
		// iPod管理类
		//数据库所在文件夹 必须是'\'结尾的
	protected:
		ciPodManager(wstring cdbFilePath);
		~ciPodManager();
	public:
		//单键
		static ciPodManager* GetInstance();
		static	 void   DestroyInstance();
		static void Init(wstring cdbFilePath,int VersionNUM);
	
		// 新建音乐
		CIosCDBTrackData* InsertNewTrack(wstring FileName);

		//    删除音乐
		void DeleteTrack(CIosCDBTrackData *pTrackData);

		//  修改音乐 
		// _alltracklist列表中获得，获得后只允许修改字符串类型值,ID等数据不需修改 
		//修改接口 AlbumArtistName 并入CIosCDBTrackData 因此这个参数废弃
		void ModifyTrack(CIosCDBTrackData *pTrackData ,wstring ArtWorkFilePath);
		
		//   新增播放列表
		void CreatePlayList(wstring PlayListName);

		//根据播放列表名删除播放列表
		void DeletePlayList(wstring PlayListName);

		//根据播放列表ID删除播放列表
		void DeletePlayList(__int64 PlayListId);

		//根据旧播放列表名修改播放列表	
		void ModifyPlayList(wstring PlayListOldName,wstring PlayListNewName);

		//修改播放列表名
		void ModifyPlayList(__int64 PlayListId,wstring PlayListNewName);

		// 添加到播放列表
		void AddToPlayList(__int64 trackid,wstring PlayListName);

		//从播放列表中移除一首歌曲
		void RemoveFromPlayList(__int64 trackid,wstring PlayListName);

		const CIosiPodData* GetPiPodCDBData() const { return piPodCDBDataList; }

		CIosCDBTrackData* GetTrackByID(__int64 pid){return piPodCDBDataList->FindTrackDataByID64(pid);}

		wstring GetDevFilePathByID(__int64 PlayListId);

		void testDB(){TestDb3();}
		//根据歌曲pid将封面图片导出到一个文件文件名为titlt+"_"+pid+".jpg"
		BOOL GetAlbumCover(__int64 TrackID,wstring TrackTitle);
		//重新加载播放列表
		void ReloadPlayListDB();
		//测试CBD文件完整性
		void TestPackCDBFile();
	private:

		//查找专辑数据库id
		__int64 find_Album_PID(int  trackid);
		//查找艺术家数据库id
		__int64 find_artist_PID(int  trackid);
		//保存封面图片的编码格式
		int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
		bool FindTrackByMp3Info(IiPodMediaParser * mediainfo);
	private:
			//自身实例
		  static ciPodManager* m_pInstance;
		  CIosiPodData* piPodCDBDataList;	//CBD文件操作数据类	
		  wstring _cdbFilePath;				//文件路径

		  int m_Hashtype;					//校验类型
		  GdiplusStartupInput _gdiplusStartupInput;
		  ULONG_PTR _gdiplusToken;
		  cIosPlayListDB* pIosPlayListDB;

	};
}