#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ios5 ipod����������ֿ�
	class DBaseLocaltion5 : public IDataRow
	{
	public:
		DBaseLocaltion5();
		virtual ~DBaseLocaltion5(){}

		__int64 base_location_id;//����pid
		wstring path;			 //���豸�ϵı���λ��

		virtual const vector<wstring>* const GetValues();

		virtual wstring PrimaryKeyCondition() const;

#pragma region ����ʵ�ֵľ�̬����, ģ����BaseTable���õ�
		static wstring GetTableName();
		static DBaseLocaltion5* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
