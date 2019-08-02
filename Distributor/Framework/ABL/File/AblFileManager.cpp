#include "stdafx.h"
#include "AblFileManager.h"

#include "Thread/CreateFolderThread.h"
#include "Thread/DeleteFileThread.h"
#include "Thread/DownloadFileThread.h"
#include "Thread/LoadFileListThread.h"
#include "Thread/MoveFileThread.h"
#include "Thread/RenameFileThread.h"
#include "Thread/UploadFileThread.h"
#include "Thread/SearchFileThread.h"
#include "Thread/GetNewFileNameThread.h"
#include "Common/UserMessage/abl/AblFileMessage.h"
#include "Thread/AblFileOperation.h"
#include "Thread/GetSDPath_Android.h"
#include "Singleton.h"
#include "EventManager.h"
#include "LOGX.h"


CRITICAL_SECTION AblFileManager::_csLock_Load;

AblFileManager::AblFileManager()
{
    InitializeCriticalSection(&AblFileManager::_csLock_Load);
}

AblFileManager::~AblFileManager()
{
	
}


bool AblFileManager::Handle( RequestRunThread* msg )
{
	BaseThread* thread = Singleton::GetThreadManager()->GetThread(msg->nRequestID);
	thread->Run();
	return true;
}



bool AblFileManager::Handle( RequestUploadFileMsg* msg )
{
    int nID = Singleton::GetThreadManager()->GetGUID();
    string threadID = "UploadFileThread-" + CStrOperation::Int2string(nID);

    UploadFileThread *pThread = new UploadFileThread(threadID, nID, msg->deviceID);
	RequestUploadFileMsg* temp = new RequestUploadFileMsg;
	*temp = *msg;
	pThread->_requestUploadFileMsg = temp;
    //pThread->Run();
 
    msg->nRequestID = nID;
    return true;
}

bool AblFileManager::Handle( RequestDownloadFileMsg* msg )
{
    int nID = Singleton::GetThreadManager()->GetGUID();
    string threadID = "DownloadFileThread-" + CStrOperation::Int2string(nID);

    DownloadFileThread *pThread = new DownloadFileThread(threadID, nID, msg->deviceID);
    pThread->SetParam(&(msg->strDevicePathList), msg->strPcDir);
    pThread->Run();

    msg->nRequestID = nID;

    return true;
}

bool AblFileManager::Handle( RequestCreateFolderMsg* msg )
{
    int nID = Singleton::GetThreadManager()->GetGUID();
    string threadID = "CreateFolderThread-" + CStrOperation::Int2string(nID);

    CreateFolderThread *pThread = new CreateFolderThread(threadID, nID, msg->deviceID);
	RequestCreateFolderMsg* temp = new RequestCreateFolderMsg;
	*temp = *msg;
	pThread->_requestCreateFolderMsg = temp;
    pThread->Run();

    msg->nRequestID = nID;

    return true;
}
bool AblFileManager::Handle( RequestCheckFoldersMsg* msg )
{
	int nID = Singleton::GetThreadManager()->GetGUID();
    string threadID = "CreateFolderThread-" + CStrOperation::Int2string(nID);

    CreateFolderThread *pThread = new CreateFolderThread(threadID, nID, msg->deviceID);
	RequestCheckFoldersMsg* temp = new RequestCheckFoldersMsg;
	*temp = *msg;
	pThread->_requestCheckFoldersMsg = temp;
    pThread->Run();

    msg->nRequestID = nID;

    return true;
}
bool AblFileManager::Handle( RequestDeleteFileMsg* msg )
{
    int nID = Singleton::GetThreadManager()->GetGUID();
    string threadID = "DeleteFileThread-" + CStrOperation::Int2string(nID);
    DeleteFileThread *pThread = new DeleteFileThread(threadID, nID, msg->deviceID);
	RequestDeleteFileMsg* temp = new RequestDeleteFileMsg;
	*temp = *msg;
	pThread->_requestDeleteFileMsg = temp;
    pThread->Run();

    msg->nRequestID = nID;
    return true;
}

bool AblFileManager::Handle( RequestDeleteFileByPathMsg* msg )
{
	int nID = Singleton::GetThreadManager()->GetGUID();
	string threadID = "DeleteFileThread-" + CStrOperation::Int2string(nID);
	DeleteFileThread *pThread = new DeleteFileThread(threadID, nID, msg->deviceID);
	RequestDeleteFileByPathMsg* temp = new RequestDeleteFileByPathMsg;
	*temp = *msg;
	pThread->_requestDeleteFileByPathMsg = temp;
	pThread->Run();

	msg->nRequestID = nID;
	return true;
}

bool AblFileManager::Handle( RequestMoveFileMsg* msg )
{
    int nID = Singleton::GetThreadManager()->GetGUID();
    string threadID = "MoveFileThread-" + CStrOperation::Int2string(nID);

    MoveFileThread *pThread = new MoveFileThread(threadID, nID, msg->deviceID);
    pThread->SetParam(&(msg->strSourceDevicePathList), msg->strTargetDeviceDir, msg->bCopy);
    pThread->Run();

    msg->nRequestID = nID;

    return true;
}

bool AblFileManager::Handle( RequestRenameFileMsg* msg )
{
    int nID = Singleton::GetThreadManager()->GetGUID();
    string threadID = "RenameFileThread-RequestRenameFileMsg" + CStrOperation::Int2string(nID);

    RenameFileThread *pThread = new RenameFileThread(threadID, nID, msg->deviceID);
	RequestRenameFileMsg* temp = new RequestRenameFileMsg;
	*temp = *msg;
	pThread->_requestRenameFileMsg = temp;
    pThread->Run();

    msg->nRequestID = nID;

    return true;
}

bool AblFileManager::Handle( RequestReleaseFileListItem* msg )
{
	AblFileOperation::ReleaseItem(msg->item);
	return false;
}
bool AblFileManager::Handle( RequestLoadItemsByPath_ABL* msg )
{
	int nID = Singleton::GetThreadManager()->GetGUID();
	string threadID = "LoadFilesThread.RequestLoadNodes_ABL-" + CStrOperation::Int2string(nID);

	LoadFileListThread *pThread = new LoadFileListThread(threadID, nID, msg->deviceID);
	RequestLoadItemsByPath_ABL* temp = new RequestLoadItemsByPath_ABL;
	*temp = *msg;
	pThread->_requestLoadItemsByPath_ABL = temp;
	pThread->Run();
	
	msg->nRequestID = nID;

	return true;
}

bool AblFileManager::Handle( RequestLoadItemsByItem_ABL* msg )
{
	int nID = Singleton::GetThreadManager()->GetGUID();
	string threadID = "LoadFilesThread.RequestLoadNodes_ABL-" + CStrOperation::Int2string(nID);

	LoadFileListThread *pThread = new LoadFileListThread(threadID, nID, msg->deviceID);
	RequestLoadItemsByItem_ABL* temp = new RequestLoadItemsByItem_ABL;
	*temp = *msg;
	pThread->_requestLoadItemsByItem_ABL = temp;
	pThread->Run();

	msg->nRequestID = nID;
	return true;
}

bool AblFileManager::Handle( RequestSearchFileMsg* msg )
{
	int nID = Singleton::GetThreadManager()->GetGUID();
	string threadID = "SearchFileThread.RequestSearchFileMsg-" + CStrOperation::Int2string(nID);

	SearchFileThread *pThread = new SearchFileThread(threadID, nID, msg->deviceid);
	RequestSearchFileMsg* temp = new RequestSearchFileMsg;
	*temp = *msg;
	pThread->_requestSearchFileMsg = temp;
	pThread->Run();

	msg->nRequestID = nID;
	return true;
}

bool AblFileManager::Handle( RequestSearchFileMsg_Do* msg )
{
	int nID = msg->nRequestID;
	string threadID = "SearchFileThread.RequestSearchFileMsg-" + CStrOperation::Int2string(nID);
	SearchFileThread* pThread = (SearchFileThread*)Singleton::GetThreadManager()->GetThread(threadID);
	if(pThread)
	{
		if(msg->type == 0)
			pThread->Pause();
		else if(msg->type == 1)
			pThread->Resume();
		else if(msg->type == 2)
		{
			pThread->Pause();
			pThread->Exit();
		}
	}
	return true;
}

bool AblFileManager::Handle( RequestJump2Path_ABL* msg )
{

	int nID = Singleton::GetThreadManager()->GetGUID();
	string threadID = "LoadFilesThread.RequestJump2Path_ABL-" + CStrOperation::Int2string(nID);

	LoadFileListThread *pThread = new LoadFileListThread(threadID, nID, msg->deviceID);
	RequestJump2Path_ABL* temp = new RequestJump2Path_ABL;
	*temp = *msg;
	pThread->_requestJump2Path_ABL = temp;
	pThread->Run();

	msg->nRequestID = nID;
	return true;
}

bool AblFileManager::Handle( RequestExist_ABL* msg )
{
	int nID = Singleton::GetThreadManager()->GetGUID();
	string threadID = "GetNewFileNameThread.RequestExist_ABL-" + CStrOperation::Int2string(nID);

	GetNewFileNameThread *pThread = new GetNewFileNameThread(threadID, nID, msg->deviceID);
	RequestExist_ABL* temp = new RequestExist_ABL;
	*temp = *msg;
	pThread->_requestExist_ABL = temp;
	pThread->Run();

	msg->nRequestID = nID;
	return false;
}

bool AblFileManager::Handle( RequestGetNewName_ABL* msg )
{
	

	return false;
}

bool AblFileManager::Handle( RequestReloadNode_ABL* msg )
{

	int nID = Singleton::GetThreadManager()->GetGUID();
	string threadID = "LoadFilesThread.RequestLoadNodes_ABL-" + CStrOperation::Int2string(nID);

	LoadFileListThread *pThread = new LoadFileListThread(threadID, nID, msg->deviceID);
	RequestReloadNode_ABL* temp = new RequestReloadNode_ABL;
	*temp = *msg;
	pThread->_requestReloadNode_ABL = temp;
	pThread->Run();

	msg->nRequestID = nID;
	return true;
}

bool AblFileManager::Handle( RequestReleaseItemsMsg* msg )
{
	for (vector<int>::iterator it = msg->node_del.begin(); it != msg->node_del.end(); it ++)
		AblFileOperation::RemoveFromChildrens((FileListItem*)(*it));

	msg->nRequestID = 0;
	return false;
}

bool AblFileManager::Handle( ReqeustSDPathMsg* msg )
{
	int nID = Singleton::GetThreadManager()->GetGUID();
	string threadID = "GetSDPath_Android.ReqeustSDPathMsg-" + CStrOperation::Int2string(nID);

	GetSDPath_Android *pThread = new GetSDPath_Android(threadID, nID, msg->deviceID);
	ReqeustSDPathMsg* temp = new ReqeustSDPathMsg;
	*temp = *msg;
	pThread->_reqeustSDPathMsg = temp;
	pThread->Run();

	msg->nRequestID = nID;
	return false;
}

bool AblFileManager::Handle( RequestAppDisplayNameMsg* msg )
{
	int nID = Singleton::GetThreadManager()->GetGUID();
	string threadID = "LoadFilesThread.RequestAppDisplayNameMsg-" + CStrOperation::Int2string(nID);

	LoadFileListThread *pThread = new LoadFileListThread(threadID, nID, msg->deviceID);
	RequestAppDisplayNameMsg* temp = new RequestAppDisplayNameMsg;
	*temp = *msg;
	pThread->_requestAppDisplayNameMsg = temp;
	pThread->Run();

	msg->nRequestID = nID;
	return true;
}

bool AblFileManager::Handle( RequestReleaseNodesMsg* msg )
{// 主线程释放共享的指针资源。 背景线程可能出异常。要捕获掉  
	// TODO 指针问题好像捕获了也照样奔溃。 后期改成由线程退出后检查线程计数， 如果为0， 且界面请求过释放， 那么进行释放。
	// 注意计数要对应设备id.
	for (vector<int>::iterator it = msg->nodes.begin(); it != msg->nodes.end(); it ++)
	{
		FileListItem* item = (FileListItem*)(*it);
		AblFileOperation::RemoveFromChildrens(item, true);
	}
	return true;
}

 
