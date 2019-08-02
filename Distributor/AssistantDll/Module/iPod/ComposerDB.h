#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ipod管理item(曲目)表 数据类(以D开头)
	class DComposer : public IDataRow
	{
	public:
		DComposer();
		virtual ~DComposer(){}

		__int64	Pid;
		wstring	Name;				    //作曲家名称
		__int32	name_order;				//sort_name 字段优先
		wstring	sort_name;				//作曲家名称排序
		__int32	name_order_section;		//Sort区域号
		__int32	name_blank;				//name是否为空，为空=1，否则=0

		virtual const vector<wstring>* const GetValues();

		virtual wstring PrimaryKeyCondition() const;

#pragma region 必须实现的静态方法, 模板类BaseTable中用到
		static wstring GetTableName();
		static DComposer* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
