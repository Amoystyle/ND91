#pragma once

#include "Module/Common/IosDB/IosDBBase.h"

namespace ND91Assistant
{
	class BookmarkData;
	class BookmarkFolder;
	class DeviceData;

	// Ios��ǩ���ݿ����
	class IosBookmarkDB:public IosDBBase
	{
	public:
		IosBookmarkDB(IosDBPath iosDBPath);
		~IosBookmarkDB(void);

		// Summary  : ���������ǩ
		// Returns	: BookmarkFolder* ���ظ��ڵ�
		// Parameter: const DeviceData * pDevice
		BookmarkFolder* GetAllBookMark(const DeviceData* pDevice);
		
		// Summary  : ɾ����ǩ
		// Returns	: void
		// Parameter: BookmarkData * pNode
		void DeleteNode(BookmarkData* pNode);

		// Summary  : ɾ�����ӽڵ�
		// Returns	: void
		// Parameter: BookmarkFolder * parent���ڵ�
		void DeleteChildren(BookmarkFolder* parent);

		// Summary  : ������ǩ
		// Returns	: bool
		// Parameter: BookmarkData * pNode
		bool UpdateNode(BookmarkData* pNode);

		// Summary  : �����ǩ
		// Returns	: bool
		// Parameter: BookmarkData * pNewNode�½ڵ�
		bool InserBookMarkNode(BookmarkData* pNewNode);
	};

}