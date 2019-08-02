#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"
	extern int G_DBver;
namespace ND91Assistant
{

	// ipod管理item(曲目)表 数据类(以D开头)
	class DAlbumArtistDB : public IDataRow
	{
	public:
		DAlbumArtistDB();
		virtual ~DAlbumArtistDB(){}

		__int64	Pid;
		__int32	Kind;					//未知，总是=NULL
		__int32	artwork_status;			//未知，总是=NULL
		__int64	Artwork_album_pid;		//artwork对象
		wstring	Name;				    //歌唱家名称
		__int32	name_order;				//sort_name 字段优先
		wstring	sort_name;				//歌唱家名称排序
		__int32	name_order_section;		//Sort区域号
		__int32	name_blank;				//name是否为空，为空=1，否则=0

		virtual const vector<wstring>* const GetValues();

		virtual wstring PrimaryKeyCondition() const;

#pragma region 必须实现的静态方法, 模板类BaseTable中用到
		static wstring GetTableName();
		static DAlbumArtistDB* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
