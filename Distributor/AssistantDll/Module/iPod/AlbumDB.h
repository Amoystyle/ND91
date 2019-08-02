#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ipod管理item(曲目)表 数据类(以D开头)
	class DAlbumDB : public IDataRow
	{
	public:
		DAlbumDB();
		virtual ~DAlbumDB(){}

		__int64	Pid;					//Item artist的PID
		__int32	Kind;					//未知，总是=NULL
		__int32	artwork_status;			//未知，总是=NULL
		__int64	artwork_item_pid;		//artwork对象
		__int64	artist_pid;				//album_artist字段的pid
		__int32	user_rating;			//用户评价
		wstring	Name;					//专辑名称
		__int32	name_order;				//专辑名称排序字段优先
		__int32	all_compilations;		//Album是否被iTunes修改过，修改过=1，否则=0
		wstring	feed_url;				//未知字段
		__int32	season_number;			//未知字段
		__int32	name_order_section;		//Sort区域号
		wstring	sort_name;				//专辑名称排序字段
		wstring	Artist;					//专辑对应的歌唱家
		wstring sort_artist;			//专辑对应的歌唱家排序
		__int32	artist_order_section;	//Sort区域号
		__int32	name_blank;				//Name是否为空，为空=1，否则=0

		virtual const vector<wstring>* const GetValues();

		virtual wstring PrimaryKeyCondition() const;

#pragma region 必须实现的静态方法, 模板类BaseTable中用到
		static wstring GetTableName();
		static DAlbumDB* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
