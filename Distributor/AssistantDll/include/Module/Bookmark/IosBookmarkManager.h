 #pragma once

#include "BookmarkManager.h"

namespace ND91Assistant
{
	// Ios��ǩ�ļ�����
	class DeviceData;
	class BookmarkData;
	class BookMarksSynchronizer;
	class CLASSINDLL_CLASS_DECL IosBookmarkManager : public BookmarkManager
	{
		friend class IosBookMarkDB;
		friend class IosBookMarkPlistHelper;
	public:
		IosBookmarkManager(const DeviceData* pDevice);
		virtual ~IosBookmarkManager(void);

//         IosBookmarkManager(map<string,void*> mapEntities,const DeviceData* pDevice);

		virtual BookmarkFolder* LoadBookMarks();

		// Summary  : �����ǩ
		// Returns	: bool
		// Parameter: wstring tittle ����
		// Parameter: wstring url    ��ַ, Ios�ɿգ���ʾ����ļ���
		// Parameter: wstring des	 ����
		virtual BookmarkData* AddBookMark(wstring tittle, BookmarkFolder* pFolder, wstring url=L"", wstring des=L"");

		// Summary  : �����ǩ������
		// Returns	: bool
		// Parameter: BookmarkData * pBookMark
		virtual bool ImportBookMark(BookmarkData* pBookmark, BookmarkFolder* pFolder);

		// Summary  : ɾ����ǩ
		// Returns	: bool
		// Parameter: BookmarkData * pBookMark
		virtual bool DeleteBookMark(BookmarkData* pBookMark);

		// Summary  : ������ǩ
		// Returns	: bool
		// Parameter: BookmarkData * pBookMark
		virtual bool UpdateBookMark(BookmarkData* pBookMark);

		// Summary  : ͬ�����ֻ��������ڵ����
		// Returns	: bool
		virtual bool  SyncToPhone();

		// Summary  : ��������
		// Returns	: bool  
		// Parameter: BookmarkData * pChildBookMark �ӽڵ�
		// Parameter: bool Up ����
		bool MoveBookMarkNode(BookmarkData* pChildBookMark, bool Up);


	private:
		enum DBOperateType //���ݿ��������
		{
			EDelete,  // ɾ��
			EUpdate,  // ����
			EAdd	  // ���
		};
		// Summary  : ����������ӽڵ��˳��Id
		// Returns	: int
		int  GetChildOrderIndex();

		// Summary  : 320���ϰ汾���ݲ���
		// Returns	: void
		// Parameter: DBOperateType type
		// Parameter: BookmarkData * pBookMark
		bool DBOperation(DBOperateType type, BookmarkData* pBookMark);

		BookmarkData* FindNode(BookmarkData* pBookMark);


	private:

		int                     _version; //�汾��

		BookmarksSynchronizer* _pBookmarkSync;
	};

}