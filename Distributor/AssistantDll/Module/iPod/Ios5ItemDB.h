#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ios5 ipod������Ϣ
	class DItem5 : public IDataRow
	{
	public:
		DItem5();
		virtual ~DItem5(){}
		__int64 item_pid;		 //����pid
		int media_type;			 //ý������
		__int64 title_order;	 //��������
		int title_order_section;
		__int64 item_artist_pid;	//������pid
		__int64 item_artist_order;
		int item_artist_order_section;
		__int64 series_name_order;
		int series_name_order_section;
		__int64 album_pid;			//ר��pid
		__int64 album_oder;
		int album_oder_section;
		__int64 album_artist_pid;	//ר��������pid
		__int64 album_artist_order;
		int album_artist_order_section;
		__int64 composer_pid;		//������pid
		__int64 compser_order;
		int compser_order_sectionn;
		__int64 genre_id;			//���
		__int64 genre_order;
		int genre_order_section;
		int disk_number;
		int track_number;
		__int64 location_kind_id;	//�������ͣ����ػ������磩
		__int64 episode_sort_id;
		__int64 store_link_id;
		__int64 category_id;
		__int64 base_location_id;	//������pid

		virtual const vector<wstring>* const GetValues();

		virtual wstring PrimaryKeyCondition() const;

#pragma region ����ʵ�ֵľ�̬����, ģ����BaseTable���õ�
		static wstring GetTableName();
		static DItem5* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
