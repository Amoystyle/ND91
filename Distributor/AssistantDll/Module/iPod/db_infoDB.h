#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ios5 ipod数据库信息
	class DDbInfo : public IDataRow
	{
	public:
		DDbInfo();
		virtual ~DDbInfo(){}

		__int64 db_pid;
		__int64	primary_container_pid;
		wstring	media_folder_url;
		int	audio_language;
		int	subtitle_language;
		int	genius_cuid;
		string	bib;
		string	rib;

		virtual const vector<wstring>* const GetValues();

		virtual wstring PrimaryKeyCondition() const;

#pragma region 必须实现的静态方法, 模板类BaseTable中用到
		static wstring GetTableName();
		static DDbInfo* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
