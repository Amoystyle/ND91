#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ipod����item(��Ŀ)�� ������(��D��ͷ)
	class DComposer : public IDataRow
	{
	public:
		DComposer();
		virtual ~DComposer(){}

		__int64	Pid;
		wstring	Name;				    //����������
		__int32	name_order;				//sort_name �ֶ�����
		wstring	sort_name;				//��������������
		__int32	name_order_section;		//Sort�����
		__int32	name_blank;				//name�Ƿ�Ϊ�գ�Ϊ��=1������=0

		virtual const vector<wstring>* const GetValues();

		virtual wstring PrimaryKeyCondition() const;

#pragma region ����ʵ�ֵľ�̬����, ģ����BaseTable���õ�
		static wstring GetTableName();
		static DComposer* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
