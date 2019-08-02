#pragma once
#include "Common\UserMessage\abl\AblFileMessage.h"

#define Err_Unwritable	1001
#define Err_Cancel		1002
#define Err_Unknown		1003
#define Err_NodeEmpty	1004
/*! @class
********************************************************************************
<PRE>
类名称   : AblFileOperation
功能     : 提供文件的通用操作
异常类   :  
--------------------------------------------------------------------------------
备注     : 线程处理完自动释放，运行中会发消息通知外部进度 
典型用法 : 
</PRE>
*******************************************************************************/
class AblFileOperation
{
public:
    static vector<NodeData*> GetNodes(DirData* pRoot, vector<wstring> devicePathList, bool bFindLinkTarget = true, bool bExactMatch = true, ProgressObserver* pProgressObserver = NULL);

    static TransferResult DownloadFile(DeviceData* pDevice, wstring strMobilePath, wstring strPcDir, ProgressObserver* pProgressObserver);

	static FileListItem* NewItem(NodeData* node, FileListItem* par_item);
	// checkAll = true: 递归检查所有子集， false:  只检查一级子集
	static FileListItem* GetItem(NodeData* node, FileListItem* par_item, bool checkAll = false);
	static void ReleaseItem(FileListItem* item);
	static void ClearSubitems(FileListItem* item);
	static void RemoveFromChildrens(FileListItem* item, bool b_release_data_abi = false);
	static void Refresh(FileListItem* item);
	static void ReloadData_ABL(NodeData* cur_node);
	// 0: 表示可写; 其他值表示异常
	static int CheckWritable(DeviceData* pDevice, DirData* data);
	static NodeData* GetNode(int data);
	// 检查 path 路径上的所有节点。 true:表示有获取到节点， 否则没有节点
	static bool GetNodes( DirData* pRoot, wstring path, vector<NodeData*>& nodes, bool bFindLinkTarget, bool bExactMatch);
	// 为了支持在上传的时候可以取消。
	static void GetDirSizeW( const wstring& dir, int& nodeSum, __int64& sizeSum, vector<wstring>* fileExts, ProgressObserver* pProgressObserver = NULL);
};