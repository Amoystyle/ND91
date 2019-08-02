#pragma once
#include "Common\UserMessage\abl\AblFileMessage.h"

// abl �ļ����������̵߳��Զ���
#define ABLFile_AutoLock CSLock lockCS(&AblFileManager::_csLock_Load);

/*! @class
********************************************************************************
<PRE>
������   : AblFileManager
����     : �ļ������������FileManger
�쳣��   :  
--------------------------------------------------------------------------------
��ע     : 
�����÷� : 
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
    // �ϴ�
    bool Handle( RequestUploadFileMsg* msg );
    // ����
    bool Handle( RequestDownloadFileMsg* msg );
    // �����ļ���
	bool Handle( RequestCreateFolderMsg* msg );
	// ����ļ����б�
	bool Handle( RequestCheckFoldersMsg* msg );
	// ɾ��
	bool Handle( RequestDeleteFileMsg* msg );

	bool Handle( RequestDeleteFileByPathMsg* msg );
	// �ƶ�
	bool Handle( RequestMoveFileMsg* msg );
	// ������
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
	static CRITICAL_SECTION _csLock_Load; //�����б�Ļ���
};