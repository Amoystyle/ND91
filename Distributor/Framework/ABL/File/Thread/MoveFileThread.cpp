#include "stdafx.h"
#include "MoveFileThread.h"
#include "AblFileOperation.h"
#include "..\AblFileManager.h"

MoveFileThread::MoveFileThread( string threadID, int nRequestID, wstring deviceID )
: BaseAblThread(threadID, nRequestID, deviceID)
{
	 _pProgressObserver = new BaseProgressObserver(_nGUID);
}

MoveFileThread::~MoveFileThread()
{
}

bool MoveFileThread::Action(DeviceData* pDevice)
{
	ABLFile_AutoLock;
    WAIT_DEVICE_ACTIVE_NORMAL;
    try
    {
		ResponseMoveFileMsg msg;
		msg.nRequestID = _nGUID;
		
		RootData *pRootdata = pDevice->GetRootData();
		vector<NodeData*> nodes = AblFileOperation::GetNodes(pRootdata->GetDir(), _strSourceDevicePathList);
        NodeData* pNode = FileHelper::GetNode(pRootdata->GetDir(), _strTargetDeviceDir, false);
        DirData* pCurDir = dynamic_cast<DirData*>(pNode);;

		if(!pCurDir || !FileHelper::ExistInDevice(pCurDir->GetFullPath(), pDevice))
		{
			msg.res = Err_NodeEmpty;
			msg.SendToUI();
			return false;
		}

		if(AblFileOperation::CheckWritable(pDevice, pCurDir))
		{
			msg.res = Err_Unwritable;
			msg.SendToUI();
			return false;
		}
        bool bRec = false;
        if (pCurDir)
        {
            //if (!_bCopy)
                //bRec = pCurDir->Move(&nodes, &_pProgressObserver);
            if(_bCopy)
                bRec = pCurDir->Copy(&nodes, _pProgressObserver);
        }
		SAFE_DELETE(pRootdata);
		msg.res = bRec ? 0 : 1;
		msg.SendToUI();
        return bRec;
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
