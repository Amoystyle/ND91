#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ipod管理播放列表表 数据类(以D开头)
	class DExtViewMembership : public IDataRow
	{
	public:
		DExtViewMembership();
		virtual ~DExtViewMembership(){}
		__int64	item_pid;
		int	movie_mbr;
		int	movie_rental_mbr;
		virtual const vector<wstring>* const GetValues();
		virtual wstring PrimaryKeyCondition() const;
#pragma region 必须实现的静态方法, 模板类BaseTable中用到
		static wstring GetTableName();
		static DExtViewMembership* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
