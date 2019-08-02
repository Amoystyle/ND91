#include "stdafx.h"
#include "DownloadFileThread.h"
#include "AblFileOperation.h"
#include "..\AblFileManager.h"

DownloadFileThread::DownloadFileThread( string threadID, int nRequestID, wstring deviceID )
: BaseAblThread(threadID, nRequestID, deviceID)
{
	_pProgressObserver = new BaseProgressObserver(_nGUID);
}

DownloadFileThread::~DownloadFileThread()
{
}

bool DownloadFileThread::Action(DeviceData* pDevice)
{
	ABLFile_AutoLock;
	WAIT_DEVICE_ACTIVE_NORMAL;
	try
	{
		RootData *pRootdata = pDevice->GetRootData();
		DirData* pRoot = pRootdata->GetDir();

		vector<NodeData*> nodes = AblFileOperation::GetNodes(pRoot, _strDevicePathList, true, true, _pProgressObserver);
		TransferResult nRec = Transfer_Cancel;
		if (!(_pProgressObserver && _pProgressObserver->IsCancel()))
			nRec = FileHelper::Download(&nodes, _strPcDir, _pProgressObserver);
		SAFE_DELETE(pRootdata);

		ResponseDownloadFileMsg msg;
		msg.nRequestID = _nGUID;
		if(nRec == Transfer_Success)
			msg.res = 0;
		else if(nRec == Transfer_Cancel)
			msg.res = Err_Cancel;
		else 
			msg.res = 1;
		msg.SendToUI();
		return true;
	}
	catch(AssistantException e)
	{
		return false;
	}
	catch (...)
	{
		return false;
	}    

	return true;
}
