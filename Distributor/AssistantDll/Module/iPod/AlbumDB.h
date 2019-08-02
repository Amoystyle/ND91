#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ipod����item(��Ŀ)�� ������(��D��ͷ)
	class DAlbumDB : public IDataRow
	{
	public:
		DAlbumDB();
		virtual ~DAlbumDB(){}

		__int64	Pid;					//Item artist��PID
		__int32	Kind;					//δ֪������=NULL
		__int32	artwork_status;			//δ֪������=NULL
		__int64	artwork_item_pid;		//artwork����
		__int64	artist_pid;				//album_artist�ֶε�pid
		__int32	user_rating;			//�û�����
		wstring	Name;					//ר������
		__int32	name_order;				//ר�����������ֶ�����
		__int32	all_compilations;		//Album�Ƿ�iTunes�޸Ĺ����޸Ĺ�=1������=0
		wstring	feed_url;				//δ֪�ֶ�
		__int32	season_number;			//δ֪�ֶ�
		__int32	name_order_section;		//Sort�����
		wstring	sort_name;				//ר�����������ֶ�
		wstring	Artist;					//ר����Ӧ�ĸ質��
		wstring sort_artist;			//ר����Ӧ�ĸ質������
		__int32	artist_order_section;	//Sort�����
		__int32	name_blank;				//Name�Ƿ�Ϊ�գ�Ϊ��=1������=0

		virtual const vector<wstring>* const GetValues();

		virtual wstring PrimaryKeyCondition() const;

#pragma region ����ʵ�ֵľ�̬����, ģ����BaseTable���õ�
		static wstring GetTableName();
		static DAlbumDB* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
