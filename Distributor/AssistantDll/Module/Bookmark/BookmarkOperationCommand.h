#pragma once
#include "Module/BaseCommand.h"
#include <deque>

using namespace std;
namespace ND91Assistant
{
class BookmarkFolder;
class BookmarkData;
// ��ǩ����
class BookmarkOperationCommand :public BaseCommand
{
public:
	BookmarkOperationCommand(const DeviceData* pDevice);
	virtual ~BookmarkOperationCommand(void);

	
	// Summary  : ��ȡ��ǩ
	// Returns	: const deque<BookMarkCategory*>
	BookmarkFolder* LoadBookMarks();

	// Summary  : �����ǩ
	// Returns	: bool
	// Parameter: BookmarkData * pBookmark
	bool AddBookMark(BookmarkData* pBookmark);

	// Summary  : ɾ����ǩ
	// Returns	: bool
	// Parameter: BookmarkData * pBookmark
	bool DeleteBookMark(BookmarkData* pBookmark);

	// Summary  : ������ǩ
	// Returns	: bool
	// Parameter: BookmarkData * pBookmark
	bool UpdateBookMark(BookmarkData* pBookmark);

	// Summary  : �����ǩ����
	// Returns	: bool
	// Parameter: BookMarkCategory * pFolder
	bool AddCategory(BookmarkFolder* pFolder);

	// Summary  : ������ǩ����
	// Returns	: bool
	// Parameter: BookMarkCategory * pFolder
	bool UpdateCategory(const BookmarkFolder* pFolder);

	// Summary  : ɾ����ǩ����
	// Returns	: bool
	// Parameter: BookMarkCategory * pFolder
	bool DeleteCategory(const BookmarkFolder* pFolder);

private:
	// Summary  : ����ĳ��������ǩ
	// Returns	: void
	// Parameter: BinaryBufferReader & reader
	// Parameter: BookMarkCategory * pFolder
	void ParseAndroidBookMarks(BinaryBufferReader& reader,BookmarkFolder* pFolder);

private:

	void Execute();

	/********************* from BaseCommand*********************/
	// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
	virtual std::string GetAndroidBodyData();

	// �����������ݰ�
	virtual void SetAndroidReturnData(std::string data);

	// ����ios�豸�ķ����ļ���ȫ·��, unicode����ɾ���ļ�
	virtual void SetIosReturnFile(std::wstring filename){}

private:
	enum BookMarkActionType
	{
		Action_LoadBookMark = 1, // ��ȡ��ǩ
		Action_AddBookMark,		 // �����ǩ
		Action_DeleteBookMark,	 // ɾ����ǩ
		Action_UpdateBookMark,	 // ������ǩ
		Action_AddCategory,		 // �����ǩ����
		Action_UpdateCategory,	 // ������ǩ����
		Action_DeleteCategory	 // ɾ����ǩ����
	};
 
	BookmarkData*  _pSourceBookMark;

	const BookmarkFolder*	_pSrcBookMarkCategory;

	BookmarkFolder*			_pAddBookMarkCategory;

	BookmarkFolder*			_pBookMarkRoot;

};

}
