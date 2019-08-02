#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ipod管理播放列表表 数据类(以D开头)
	class DComposer5 : public IDataRow
	{
	public:
		DComposer5();
		virtual ~DComposer5(){}
		__int64 composer_pid;
		wstring	composer;
		wstring	sort_composer;
		string	grouping_key;
		int	cloud_status;
		__int64	representative_item_pid;

		virtual const vector<wstring>* const GetValues();
		virtual wstring PrimaryKeyCondition() const;
#pragma region 必须实现的静态方法, 模板类BaseTable中用到
		static wstring GetTableName();
		static DComposer5* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
