#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ipod�������б�� ������(��D��ͷ)
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
#pragma region ����ʵ�ֵľ�̬����, ģ����BaseTable���õ�
		static wstring GetTableName();
		static DExtViewMembership* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
