#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ipod管理播放列表表 数据类(以D开头)
	class DAlbum5 : public IDataRow
	{
	public:
		DAlbum5();
		virtual ~DAlbum5(){}
		__int64	album_pid;
		wstring	album;
		wstring	sort_album;
		__int64	album_artist_pid;
		__int64	representative_item_pid;
		string	grouping_key;
		int	cloud_status;
		__int64	artwork_cache_id;
		int	user_rating;
		int	all_compilations;
		wstring	feed_url;
		int	season_number;

		virtual const vector<wstring>* const GetValues();
		virtual wstring PrimaryKeyCondition() const;
#pragma region 必须实现的静态方法, 模板类BaseTable中用到
		static wstring GetTableName();
		static DAlbum5* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
