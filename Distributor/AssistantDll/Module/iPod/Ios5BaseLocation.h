#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ios5 ipod管理基础音乐库
	class DBaseLocaltion5 : public IDataRow
	{
	public:
		DBaseLocaltion5();
		virtual ~DBaseLocaltion5(){}

		__int64 base_location_id;//歌曲pid
		wstring path;			 //在设备上的保存位置

		virtual const vector<wstring>* const GetValues();

		virtual wstring PrimaryKeyCondition() const;

#pragma region 必须实现的静态方法, 模板类BaseTable中用到
		static wstring GetTableName();
		static DBaseLocaltion5* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
