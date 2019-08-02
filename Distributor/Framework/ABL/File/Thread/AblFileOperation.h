#pragma once
#include "Common\UserMessage\abl\AblFileMessage.h"

#define Err_Unwritable	1001
#define Err_Cancel		1002
#define Err_Unknown		1003
#define Err_NodeEmpty	1004
/*! @class
********************************************************************************
<PRE>
������   : AblFileOperation
����     : �ṩ�ļ���ͨ�ò���
�쳣��   :  
--------------------------------------------------------------------------------
��ע     : �̴߳������Զ��ͷţ������лᷢ��Ϣ֪ͨ�ⲿ���� 
�����÷� : 
</PRE>
*******************************************************************************/
class AblFileOperation
{
public:
    static vector<NodeData*> GetNodes(DirData* pRoot, vector<wstring> devicePathList, bool bFindLinkTarget = true, bool bExactMatch = true, ProgressObserver* pProgressObserver = NULL);

    static TransferResult DownloadFile(DeviceData* pDevice, wstring strMobilePath, wstring strPcDir, ProgressObserver* pProgressObserver);

	static FileListItem* NewItem(NodeData* node, FileListItem* par_item);
	// checkAll = true: �ݹ��������Ӽ��� false:  ֻ���һ���Ӽ�
	static FileListItem* GetItem(NodeData* node, FileListItem* par_item, bool checkAll = false);
	static void ReleaseItem(FileListItem* item);
	static void ClearSubitems(FileListItem* item);
	static void RemoveFromChildrens(FileListItem* item, bool b_release_data_abi = false);
	static void Refresh(FileListItem* item);
	static void ReloadData_ABL(NodeData* cur_node);
	// 0: ��ʾ��д; ����ֵ��ʾ�쳣
	static int CheckWritable(DeviceData* pDevice, DirData* data);
	static NodeData* GetNode(int data);
	// ��� path ·���ϵ����нڵ㡣 true:��ʾ�л�ȡ���ڵ㣬 ����û�нڵ�
	static bool GetNodes( DirData* pRoot, wstring path, vector<NodeData*>& nodes, bool bFindLinkTarget, bool bExactMatch);
	// Ϊ��֧�����ϴ���ʱ�����ȡ����
	static void GetDirSizeW( const wstring& dir, int& nodeSum, __int64& sizeSum, vector<wstring>* fileExts, ProgressObserver* pProgressObserver = NULL);
};