#pragma  once

#include "AssistantDll.h"
#include "IosiPodData.h"
#include "Common/SqlAccess/BaseTable.h"

#ifndef _CSHARPDLL
#include "Core/DeviceData.h"
#endif

using namespace std;
namespace ND91Assistant
{
    class SqliteAccess;
    class DLocation;
    class DItem;

	//iPod音乐管理数据库操作类
	class CLASSINDLL_CLASS_DECL iPodDbBase
	{
		friend class ciPodManager;
		friend class cIosPlayListDB;
	public:
			iPodDbBase(wstring sqlDbPath);
			~iPodDbBase();

		//************************************
		// Method:    随机生成64位ID
		// Returns:   __int64 ID号
		//************************************
		static __int64 Create64BitID();

		//得到一个随机的mp3文件名
		static string gen_file_name( );

	private:

//新增音乐数据插入数据库
		void InsertSqlDBNewTrack(STU_CREATE_DBID * pstudb,IiPodMediaParser * pMP3Data);
		//数据库更新音乐数据
		wstring UpdateSqlDbTrack(CIosCDBTrackData * ptrackData,STU_CREATE_DBID* pDBID,wstring ArtWorkFilePath,wstring ArtWorkDBPath);
		//数据库删除音乐数据
		wstring DeleteSqlDbtrack(CIosCDBTrackData * ptrackData,int& ArtWorkID);	
		//从数据库获得信息
		wstring getInfoFromSqlDB(int infotype);

		//查找专辑表演者数据库ID
		__int64 find_Album_artist_PID(SqliteAccess *sql,__int64 trackpid);
		//检查新建mp3文件名是否在用
		bool CheckNewName(wstring filename,BaseTable<DLocation>* plocaltion);
		//创建数据库新ID
		void GetAndCreateAllDBID(IiPodMediaParser* pMP3data,STU_CREATE_DBID * pstudb);
		//取得文件名
		wstring GetTrackPath(__int64 pid);
		//删除Localtion表的数据
		wstring LocationDBDeltrack(__int64 pid);
		//删除专辑表对应记录连带专辑表演者表清理
		void DeleteAlbumArtistDbRow(__int64 pid,SqliteAccess * sql);
		//根据名称获得作曲家ID，没有就新建一个
		__int64 GetComposerIdByName(SqliteAccess* pSql,wstring ComperName);
		//根据名称获得专辑ID，没有就新建一个
		__int64 GetAlbumIDByName(SqliteAccess* pSql,wstring AlbumName,STU_CREATE_DBID * pstudb);
		//根据名称获得专辑艺术家ID，没有就新建一个
		__int64 GetAlbumArtistIdByName(SqliteAccess* pSql,wstring AlbumArtistName);
		//根据名称获得艺术家ID，没有就新建一个
		__int64 GetArtistIdByName(SqliteAccess* pSql,wstring ArtistName);
		//更新数据库作曲者信息
		__int64 UpdateComposerDB(SqliteAccess* pSql,CIosCDBTrackData * ptrackData);
		//更新艺术家
		__int64 UpdateArtistDB(SqliteAccess* pSql,CIosCDBTrackData * ptrackData);
		//更新专辑
		__int64 UpdateAlbumDB(SqliteAccess* pSql,CIosCDBTrackData * ptrackData,STU_CREATE_DBID* pDBID);
		//更新专辑表演者
		__int64 UpdateAlbumArtistDB(SqliteAccess* pSql,__int64 oldAlbumid,wstring AlbumArtistName);

		//查找作曲数据库id
		__int64 find_composer_PID(SqliteAccess *sql,__int64  trackpid);
		//创建触发器函数
		static void CreateiPhoneKeyFunction(CppSQLite3DB* db);

		__int64 find_artist_PID(CIosiPodData* piPodCDBDataList,int  trackid);
		//根据歌曲pid获得封面图片id
		int GetArtworkid(__int64 pid);

		virtual __int64 find_Album_PID(int  trackid) = 0;
		virtual	__int64 find_artist_PID(int  trackid) = 0;

		void TestDb3();

		wstring GetNewDevFileName();

	private:
		wstring _sqlDbPath;				//pc上itunes所用的数据库所在路径

	};

}