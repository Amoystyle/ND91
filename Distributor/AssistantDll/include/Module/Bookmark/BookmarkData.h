#pragma once
//#include "Data/BaseData.h"
#include <xstring>

#include "Module/BaseData.h"

using namespace std;
namespace ND91Assistant
{

	enum BookMarkType
	{
		TypeBookMark,      // ��ǩ
		TypeBookMarkFolder // �ļ���
	};

	class DeviceData;
	class BinaryBufferReader;
	class BinaryBufferWriter;

	class CLASSINDLL_CLASS_DECL BookmarkData : public BaseData
	{
		friend class IosBookmarkDB;
		friend class IosBookmarkPlistHelper;
		friend class BookmarkOperationCommand;
		friend class BookmarksSynchronizer;
	public:
		BookmarkData();
        BookmarkData(map<string,void*> mapEntities);	// Ios ��Map����һ��BookmarkData;
		BookmarkData(const BookmarkData* bookmark);     // androidר�õĹ��캯��
		virtual ~BookmarkData();

		// Ios ��BookmarkData����һ��map
        void ConvertToDictInfos(map<string, void*>& infoss);   

		BookmarkData* Clone();

//         void SetId(int id){_id = id;}
// 		int GetId(){return _id;}
// 
// 		void SetCatId(int catID){_catID = catID;}
// 		int GetCatId(){return _catID;}

//         void SetIosGuid(wstring guid){_ios_Guid = guid;}
// 		wstring GetIosGuid(){return _ios_Guid;}
// 
// 		void SetIosParentGuid(wstring guid){_ios_Parent_Guid = guid;}
//         wstring GetIosParentGuid(){ return _ios_Parent_Guid; }

		void	SetId(string id)				{_id = id;}
		string	GetId()							{return _id;}

		void	SetParentId(string parentid)	{_parentid = parentid;}
		string	GetParentId()					{ return _parentid; }

		void	SetUrl(wstring url)				{_url = url;}
		wstring GetUrl()						{return _url;}

		void	SetTitle(wstring title)			{_title = title;}
		wstring GetTitle()						{return _title;}


		void	SetPosition(int position)		{_position = position;}
		int		GetPosition()					{return _position;}

		BookMarkType GetType()					{return _type;}


	private:

		// Summary  : ��������ص�����
		// Returns	: void
		// Parameter: BinaryBufferReader & reader
		// Parameter: bool IsOms
		void ParseAndroidData(BinaryBufferReader& reader, bool IsOms);

		// Summary  : �������������
		// Returns	: void
		// Parameter: BinaryBufferWriter & writer
		// Parameter: bool IsOms
		void AndroidDataToStream(BinaryBufferWriter& writer, bool IsOms);


	public:

		// android��ǩר��
		int				_visits;						// ���ʴ���
		__int64			_lastDate;						// �ϴη���ʱ��
		__int64			_createDate;					// ����ʱ��
		wstring			_description;					// ��ǩ����
		int				_bookmark;						// �Ƿ���ʾ���ֻ���

		// Ios��ǩר��
		string          _recordEntityName;

	protected:

		// ��������
		BookMarkType	_type;							// ����

		string			_id;							// id
		string			_parentid;						// ��������id
		wstring			_title;							// ��ǩ����
		wstring			_url;							// ��ǩurl

		// Ios��ǩר��
// 		wstring         _ios_Guid;	
// 		wstring         _ios_Parent_Guid;				// ���ڵ�uid 
        int             _position;    
		// Ios >=320
		int             _childrenCount;  				// 
		bool            _editable;		 				// �Ƿ�ɱ༭
		bool            _deleteable;	 				// �Ƿ��ɾ��
		string         _external_guid;	 				// ԭʼGuid
	};

}