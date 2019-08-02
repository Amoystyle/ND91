#pragma once
#include "Module/Common/IosPList/IosPlistHelper.h"

#include <deque>
using namespace std;
using namespace rapidxml;

namespace ND91Assistant
{

	class BookmarkFolder;
	class DeviceData;
	class IosBookmarkPlistHelper
	{
	public:
		IosBookmarkPlistHelper(void);
		~IosBookmarkPlistHelper(void);

		// Summary  : ��ȡ��ǩplist�ļ�
		// Returns	: BookmarkFolder* ���ظ��ڵ�
		// Parameter: wstring path
		// Parameter: const DeviceData * pDevice
		static BookmarkFolder* ParseBookMark(wstring path, const DeviceData* pDevice);


		// Summary  : ����ǩд��plist�ļ�
		// Returns	: void
		// Parameter: wstring path
		// Parameter: BookmarkFolder * rootFolder ���ڵ�
		static void SaveBookMarkToPlist(wstring path, BookmarkFolder* rootFolder);

	private:

		// Summary  : �����ļ��нڵ�
		// Returns	: void
		// Parameter: rapidxml::xml_node<char> * itemnode
		// Parameter: BookmarkFolder * pfolder
		// Parameter: const DeviceData * pDevice
		static void ParseArrayNode(rapidxml::xml_node<char>* itemnode, BookmarkFolder* pfolder, const DeviceData* pDevice);

		// Summary  : �����ļ��нڵ�
		// Returns	: void
		// Parameter: BookmarkFolder * pfolder
		// Parameter: XML_NODE_INDEX & index
		// Parameter: int & level
		// Parameter: IosPlistHelper & helper
		static void CreateChildrenNodes(BookmarkFolder* pfolder, XML_NODE_INDEX& index, int level, IosPlistHelper& helper);
	};

}
