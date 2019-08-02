#pragma once
#include "Common/rapidxml/rapidxml.hpp"
#include "Common/rapidxml/rapidxml_utils.hpp"
#include "Common/rapidxml/rapidxml_print.hpp"
#include "Common/rapidxml/rapidxml_iterators.hpp"
#include <string>
#include "Common/SqlAccess/SqliteAccess.h"
using namespace std;
using namespace rapidxml;
#include "Module/iPod/IosiPodData.h"

namespace ND91Assistant
{
	//class CIosCDBPlayList;
	//class SqliteAccess;

	// 播放列表DB（Library.itdb）读写类
	class CLASSINDLL_CLASS_DECL cIosPlayListDB
	{
		friend class ciPodManager;
		friend class CIosiPodData;
		friend class cIPodData5;

	public:
		cIosPlayListDB(wstring _iTunesPath);
		~cIosPlayListDB();
	private:
		//将歌曲添加到播放列表，传入歌曲pid，播放列表pid，播放列表所在文件夹路径
			wstring addTrackToPlist(__int64 trackid,wstring PlayListName  );

		//根据列表名获得列表pid
		 __int64 GetPlayPidByName(wstring PlayListName );

		//根据播放列表名添加歌曲到播放列表
		 void addTrackToPlayListDB(__int64 trackid,wstring PlayListName );

		//根据播放列表ID添加歌曲到播放列表
		 void addTrackToPlayListDB(__int64 trackid,__int64 ContainerID);

		//用于新增的歌曲，将歌曲添加到设备音乐列表
		 __int64 addTrackToTotalPlayListDB(__int64 trackid );
		 void addTrackToPlayListDB(CONST vector<__int64>* trackid,__int64 ContainerID);

		//根据播放列表名从播放列表里面移除一个歌曲
		 void RemoveTrackFromPlayListDB(__int64 trackid,wstring PlayListName);

		//根据播放列ID从播放列表里面移除一个歌曲
		 void RemoveTrackFromPlayListDB(__int64 trackid,__int64 ContainerID);
		 void RemoveTrackFromPlayListDB(CONST vector<__int64>* trackid,__int64 ContainerID);

		//数据库新增播放列表
		 __int64 InsertSqlDbPlayList(wstring PlayListName);

		//数据库删除播放列表
		 void DeleteSqlDbPlayList(__int64 PlayListId);
		 void DeleteSqlDbPlayList(wstring PlayListName);

		//数据库更新播放列表
		 void UpdateSqlDbPlayList(__int64 PlayListId,wstring PlayListNewName);
		 void UpdateSqlDbPlayList(wstring PlayListOldName,wstring PlayListNewName);

		//获得所有播放列表信息
		 void GetAllPlayList(vector<CIosCDBPlayList*>* pVecPlayList);
		
		 //播放列表是否能用
		 bool CheckDBActive(){return true;}
		
		 //打开数据库
		 void OpenPlsDB();

		 //关闭数据库
		 void ClosePlsDB();
		
	private:
		wstring _DbFileName;
		SqliteAccess sql;
		bool bDbActive;

	};
}