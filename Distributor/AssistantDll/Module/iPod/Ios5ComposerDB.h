#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ipod�������б�� ������(��D��ͷ)
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
#pragma region ����ʵ�ֵľ�̬����, ģ����BaseTable���õ�
		static wstring GetTableName();
		static DComposer5* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
