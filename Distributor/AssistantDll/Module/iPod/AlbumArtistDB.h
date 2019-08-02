#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"
	extern int G_DBver;
namespace ND91Assistant
{

	// ipod����item(��Ŀ)�� ������(��D��ͷ)
	class DAlbumArtistDB : public IDataRow
	{
	public:
		DAlbumArtistDB();
		virtual ~DAlbumArtistDB(){}

		__int64	Pid;
		__int32	Kind;					//δ֪������=NULL
		__int32	artwork_status;			//δ֪������=NULL
		__int64	Artwork_album_pid;		//artwork����
		wstring	Name;				    //�質������
		__int32	name_order;				//sort_name �ֶ�����
		wstring	sort_name;				//�質����������
		__int32	name_order_section;		//Sort�����
		__int32	name_blank;				//name�Ƿ�Ϊ�գ�Ϊ��=1������=0

		virtual const vector<wstring>* const GetValues();

		virtual wstring PrimaryKeyCondition() const;

#pragma region ����ʵ�ֵľ�̬����, ģ����BaseTable���õ�
		static wstring GetTableName();
		static DAlbumArtistDB* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
