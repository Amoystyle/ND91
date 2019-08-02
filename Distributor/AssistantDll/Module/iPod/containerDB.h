#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ipod管理播放列表 数据类(以D开头)
	class DContainerDB : public IDataRow
	{
	public:
		DContainerDB();
		virtual ~DContainerDB(){}

		__int64 Pid;
		__int32	distinguished_kind;
		__int32	date_created;
		__int32	date_modified;
		wstring	name;
		__int32	name_order;
		__int32	parent_pid;
		__int32	media_kinds;
		__int32	workout_template_id;
		__int32	is_hidden;
		__int32	smart_is_folder;
		__int32	smart_is_dynamic;
		__int32	smart_is_filtered;
		__int32	smart_is_genius;
		__int32	smart_enabled_only;
		__int32	smart_is_limited;
		__int32	smart_limit_kind;
		__int32	smart_limit_order;
		__int32	smart_limit_value;
		__int32	smart_reverse_limit_order;
		wstring	smart_criteria;
		wstring	Description;


	virtual const vector<wstring>* const GetValues();

	virtual wstring PrimaryKeyCondition() const;

#pragma region 必须实现的静态方法, 模板类BaseTable中用到
	static wstring GetTableName();
	static DContainerDB* FromValue(const vector<const char*>* pValues);
	static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
