#pragma once
//#include "Data/BaseData.h"
#include <xstring>

#include "Module/BaseData.h"

using namespace std;
namespace ND91Assistant
{

	enum BookMarkType
	{
		TypeBookMark,      // 书签
		TypeBookMarkFolder // 文件夹
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
        BookmarkData(map<string,void*> mapEntities);	// Ios 用Map构造一个BookmarkData;
		BookmarkData(const BookmarkData* bookmark);     // android专用的构造函数
		virtual ~BookmarkData();

		// Ios 用BookmarkData生成一个map
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

		// Summary  : 解析命令返回的数据
		// Returns	: void
		// Parameter: BinaryBufferReader & reader
		// Parameter: bool IsOms
		void ParseAndroidData(BinaryBufferReader& reader, bool IsOms);

		// Summary  : 生成命令发送数据
		// Returns	: void
		// Parameter: BinaryBufferWriter & writer
		// Parameter: bool IsOms
		void AndroidDataToStream(BinaryBufferWriter& writer, bool IsOms);


	public:

		// android书签专用
		int				_visits;						// 访问次数
		__int64			_lastDate;						// 上次访问时间
		__int64			_createDate;					// 创建时间
		wstring			_description;					// 书签描述
		int				_bookmark;						// 是否显示在手机上

		// Ios书签专用
		string          _recordEntityName;

	protected:

		// 公共属性
		BookMarkType	_type;							// 类型

		string			_id;							// id
		string			_parentid;						// 所属类别的id
		wstring			_title;							// 书签名称
		wstring			_url;							// 书签url

		// Ios书签专用
// 		wstring         _ios_Guid;	
// 		wstring         _ios_Parent_Guid;				// 父节点uid 
        int             _position;    
		// Ios >=320
		int             _childrenCount;  				// 
		bool            _editable;		 				// 是否可编辑
		bool            _deleteable;	 				// 是否可删除
		string         _external_guid;	 				// 原始Guid
	};

}