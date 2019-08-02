#pragma once
#include "Module/BaseCommand.h"
#include <deque>

using namespace std;
namespace ND91Assistant
{
class BookmarkFolder;
class BookmarkData;
// 书签命令
class BookmarkOperationCommand :public BaseCommand
{
public:
	BookmarkOperationCommand(const DeviceData* pDevice);
	virtual ~BookmarkOperationCommand(void);

	
	// Summary  : 获取书签
	// Returns	: const deque<BookMarkCategory*>
	BookmarkFolder* LoadBookMarks();

	// Summary  : 添加书签
	// Returns	: bool
	// Parameter: BookmarkData * pBookmark
	bool AddBookMark(BookmarkData* pBookmark);

	// Summary  : 删除书签
	// Returns	: bool
	// Parameter: BookmarkData * pBookmark
	bool DeleteBookMark(BookmarkData* pBookmark);

	// Summary  : 更新书签
	// Returns	: bool
	// Parameter: BookmarkData * pBookmark
	bool UpdateBookMark(BookmarkData* pBookmark);

	// Summary  : 添加书签分类
	// Returns	: bool
	// Parameter: BookMarkCategory * pFolder
	bool AddCategory(BookmarkFolder* pFolder);

	// Summary  : 更新书签分类
	// Returns	: bool
	// Parameter: BookMarkCategory * pFolder
	bool UpdateCategory(const BookmarkFolder* pFolder);

	// Summary  : 删除书签分类
	// Returns	: bool
	// Parameter: BookMarkCategory * pFolder
	bool DeleteCategory(const BookmarkFolder* pFolder);

private:
	// Summary  : 解析某分类下书签
	// Returns	: void
	// Parameter: BinaryBufferReader & reader
	// Parameter: BookMarkCategory * pFolder
	void ParseAndroidBookMarks(BinaryBufferReader& reader,BookmarkFolder* pFolder);

private:

	void Execute();

	/********************* from BaseCommand*********************/
	// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
	virtual std::string GetAndroidBodyData();

	// 解析返回数据包
	virtual void SetAndroidReturnData(std::string data);

	// 解析ios设备的返回文件（全路径, unicode）并删除文件
	virtual void SetIosReturnFile(std::wstring filename){}

private:
	enum BookMarkActionType
	{
		Action_LoadBookMark = 1, // 获取书签
		Action_AddBookMark,		 // 添加书签
		Action_DeleteBookMark,	 // 删除书签
		Action_UpdateBookMark,	 // 更新书签
		Action_AddCategory,		 // 添加书签分类
		Action_UpdateCategory,	 // 更新书签分类
		Action_DeleteCategory	 // 删除书签分类
	};
 
	BookmarkData*  _pSourceBookMark;

	const BookmarkFolder*	_pSrcBookMarkCategory;

	BookmarkFolder*			_pAddBookMarkCategory;

	BookmarkFolder*			_pBookMarkRoot;

};

}
