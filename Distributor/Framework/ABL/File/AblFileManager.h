#pragma once
#include "Common\UserMessage\abl\AblFileMessage.h"

// abl 文件管理所有线程的自动锁
#define ABLFile_AutoLock CSLock lockCS(&AblFileManager::_csLock_Load);

/*! @class
********************************************************************************
<PRE>
类名称   : AblFileManager
功能     : 文件管理，负责调度FileManger
异常类   :  
--------------------------------------------------------------------------------
备注     : 
典型用法 : 
</PRE>
*******************************************************************************/
class AblFileManager
	: public BaseGetData<RequestRunThread>
    , public BaseGetData<RequestUploadFileMsg>
    , public BaseGetData<RequestDownloadFileMsg>
    , public BaseGetData<RequestCreateFolderMsg>
    , public BaseGetData<RequestDeleteFileMsg>
    , public BaseGetData<RequestMoveFileMsg>
    , public BaseGetData<RequestRenameFileMsg>
	, public BaseGetData<RequestReleaseFileListItem>

	, public BaseGetData<RequestLoadItemsByPath_ABL>
	, public BaseGetData<RequestLoadItemsByItem_ABL>
	, public BaseGetData<RequestSearchFileMsg>
	, public BaseGetData<RequestSearchFileMsg_Do>
	, public BaseGetData<RequestJump2Path_ABL>
	, public BaseGetData<RequestExist_ABL>
	, public BaseGetData<RequestGetNewName_ABL>
	, public BaseGetData<RequestReloadNode_ABL>
	, public BaseGetData<RequestReleaseItemsMsg>
	, public BaseGetData<ReqeustSDPathMsg>
	, public BaseGetData<RequestAppDisplayNameMsg>
	, public BaseGetData<RequestDeleteFileByPathMsg>
	, public BaseGetData<RequestReleaseNodesMsg>
, public BaseGetData<RequestCheckFoldersMsg>
	
{
public:
    AblFileManager();
    ~AblFileManager();

protected:

		  bool Handle( RequestRunThread* msg );
    // 上传
    bool Handle( RequestUploadFileMsg* msg );
    // 下载
    bool Handle( RequestDownloadFileMsg* msg );
    // 创建文件夹
	bool Handle( RequestCreateFolderMsg* msg );
	// 检查文件夹列表
	bool Handle( RequestCheckFoldersMsg* msg );
	// 删除
	bool Handle( RequestDeleteFileMsg* msg );

	bool Handle( RequestDeleteFileByPathMsg* msg );
	// 移动
	bool Handle( RequestMoveFileMsg* msg );
	// 重命名
	bool Handle( RequestRenameFileMsg* msg );

	bool Handle( RequestReleaseFileListItem* msg );

	bool Handle( RequestLoadItemsByPath_ABL* msg );

	bool Handle( RequestLoadItemsByItem_ABL* msg );

	bool Handle( RequestSearchFileMsg* msg );

	bool Handle( RequestSearchFileMsg_Do* msg );

	bool Handle( RequestJump2Path_ABL* msg );

	bool Handle( RequestExist_ABL* msg );

	bool Handle( RequestGetNewName_ABL* msg );
	
	bool Handle( RequestReloadNode_ABL* msg );

	bool Handle( RequestReleaseItemsMsg* msg );
	
	bool Handle( ReqeustSDPathMsg* msg );

		bool Handle( RequestAppDisplayNameMsg* msg );

			bool Handle( RequestReleaseNodesMsg* msg );
		
public:
	static CRITICAL_SECTION _csLock_Load; //加载列表的互斥
};