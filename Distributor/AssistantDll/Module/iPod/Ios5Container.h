#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ipod管理播放列表信息
	class DContainer5 : public IDataRow
	{
	public:
		DContainer5();
		virtual ~DContainer5(){}
			__int64 container_pid;
			int distinguished_kind;
			int date_created;
			int date_modified;
			wstring name;
			__int64 name_order;
			int parent_pid;
			int media_kinds;
			int workout_template_id;
			int is_hidden;
			int description;
			int smart_is_folder;
			int smart_is_dynamic;
			int smart_is_filtered;
			int smart_is_genius;
			int smart_enabled_only;
			int smart_is_limited;
			int smart_limit_kind;
			int smart_limit_order;
			int smart_limit_value;
			int smart_reverse_limit_order;
			string smart_criteria;
			int play_order;
			int is_reversed;
			int album_field_order;
			int repeat_mode;
			int shuffle_items;
			int has_been_shuffled;
			wstring filepath;
			int is_saveable;
			int is_src_remote;
			int is_ignored_syncing;
			int container_type;
			int is_container_type_active_target;
			int orig_date_modified;
			int store_cloud_id;


		virtual const vector<wstring>* const GetValues();

		virtual wstring PrimaryKeyCondition() const;

#pragma region 必须实现的静态方法, 模板类BaseTable中用到
		static wstring GetTableName();
		static DContainer5* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
