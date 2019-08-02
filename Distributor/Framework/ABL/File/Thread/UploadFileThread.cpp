#include "stdafx.h"
#include "UploadFileThread.h"
#include "LOGX.h"
#include "AblFileOperation.h"
#include "../AblFileManager.h"

UploadFileThread::UploadFileThread( string threadID, int nRequestID, wstring deviceID )
: BaseAblThread(threadID, nRequestID, deviceID)
{
	_pProgressObserver = new BaseProgressObserver(_nGUID);
	_requestUploadFileMsg = NULL;
}

UploadFileThread::~UploadFileThread()
{
	SAFE_DELETE(_requestUploadFileMsg);
}

bool UploadFileThread::Action(DeviceData* pDevice)
{
	ABLFile_AutoLock;
    WAIT_DEVICE_ACTIVE_NORMAL;
	try
	{
		Handle_RequestUploadFileMsg(pDevice);
	}
	catch (...)
	{
		
	}
	
    return true;
}

void UploadFileThread::Handle_RequestUploadFileMsg( DeviceData* pDevice )
{
	RETURN_IF(!_requestUploadFileMsg);
	 
	ResponseUploadFileMsg msg;
	msg.nRequestID = _nGUID;
	try
	{
		FileListItem* par_item = (FileListItem*)_requestUploadFileMsg->par_node;
		DirData* par_data = NULL;

		NodeData* ndata = AblFileOperation::GetNode(par_item->data);
		if(ndata && ndata->_type == NodeData::DIR)
			par_data = dynamic_cast<DirData*>(ndata);

		if(!par_data || !FileHelper::ExistInDevice(par_data->GetFullPath(), pDevice))
		{ 
			msg.res = Err_NodeEmpty;
			msg.SendToUI();
			return;
		}

		vector<FileListItem*>* items_add = &msg.items;


		if(AblFileOperation::CheckWritable(pDevice, par_data))
		{
			msg.res = Err_Unwritable;
			msg.SendToUI();
			return;
		}

		TransferResult tr = Transfer_Cancel;

		// ���ʣ��ռ�
		int nodeSum = 0;
		__int64 sizeSum = 0;

		DWORD tw = GetTickCount();
		int iCountType = 0;
		OPTION->Get(GlobalOption::INT_FILETASK_COUNTMODE, iCountType);
		for (vector<wstring>::iterator it = _requestUploadFileMsg->strPcPathList.begin(); it != _requestUploadFileMsg->strPcPathList.end(); it++)
		{
			if(_pProgressObserver && _pProgressObserver->IsCancel())
				break;

			int nodeSum_tmp = 0;
			__int64 sizeSum_tmp = 0;

			wstring path = *it;

			if(CFileOperation::IsFileExist(path.c_str()))
			{
				nodeSum += 1;
				sizeSum += CFileOperation::GetFileSizeW(path);
			}
			else
			{
				AblFileOperation::GetDirSizeW(path, nodeSum_tmp, sizeSum_tmp, NULL, _pProgressObserver);
				if(iCountType > 0)
					nodeSum++;
			}

			nodeSum += nodeSum_tmp;
			sizeSum += sizeSum_tmp;
		}
		tw = GetTickCount() - tw;
		LOGX::Write(L"ͳ���ϴ��ļ���С����ʱ�� ", CStrOperation::IntToWString(tw));

		bool size_flag = true; // flase ��ʾʣ��ռ䲻��
		int64_t nLeftSize = 0;
		if (pDevice->GetDevInfo()->_deviceType == Android)
		{
			if(!(_pProgressObserver && _pProgressObserver->IsCancel()))
			{
				int64_t nUsedSize = 0;
				DWORD tw = GetTickCount();
				pDevice->GetRootData()->GetAndroidSpaceInfo(par_data->GetFullPath(), nUsedSize, nLeftSize);
				tw = GetTickCount() - tw;
				LOGX::Write(L"��ȡ android ʣ��ռ��С����ʱ�� ", CStrOperation::IntToWString(tw));
				if (sizeSum > nLeftSize)
				{
					wchar_t msg[1024] = {0};
					wsprintf(msg,_T("Android--------�ϴ��ļ��пռ䣺%d  �豸�ܿռ䣺%d"),sizeSum,nLeftSize);
					LOGX::Write(msg);
					//������С�����ϴ�
					size_flag = false;
				}
			}
		}
		else if (pDevice->GetDevInfo()->_deviceType == Ios)
		{
			if(!(_pProgressObserver && _pProgressObserver->IsCancel()))
			{
				SpaceInfo _si;
				DWORD tw = GetTickCount();
				pDevice->GetRootData()->GetSpaceInfo(RootData::PHONEDATA, &_si);
				tw = GetTickCount() - tw;
				LOGX::Write(L"��ȡ ios ʣ��ռ��С����ʱ�� ", CStrOperation::IntToWString(tw));
				nLeftSize = _si._nAmountDataAvailable;
				if (sizeSum > nLeftSize)
				{
					wchar_t msg[1024] = {0};
					wsprintf(msg,_T("IOS--------�ϴ��ļ��пռ䣺%d  �豸�ܿռ䣺%d"),sizeSum,_si._nAmountDataAvailable);
					LOGX::Write(msg);
					//������С�����ϴ�
					size_flag = false;
				}
			}
		}

		if(!_pProgressObserver || !_pProgressObserver->IsCancel())
		{
			if(size_flag)
			{
				// ע���ϴ������ʹ��ˢ�£� �ײ㲻֧������
				// ��������ϴ���Ҫѡ��Ŀ��Ļ��� ��֧������ѡ�С� ��Ϊ�������Ҳ���Ŀ�ꡣ
				tr = par_data->UploadDir(_requestUploadFileMsg->strPcPathList, _pProgressObserver, nodeSum, sizeSum, false);
				if(tr != Transfer_Success ) 
					LOGX::Write(L"upload fail or cancel");

				//UI�Լ�����ˢ��
				//AblFileOperation::Refresh(par_item);
			}
			else
				tr = Transfer_Lack_Of_Space;
		}

		if(tr == Transfer_Cancel || (_pProgressObserver && _pProgressObserver->IsCancel()))
			msg.res = Err_Cancel;
		else if(!size_flag)
		{
			msg.res = 2;
			msg.nFileSize = sizeSum;
			msg.nLeftSize = nLeftSize;
		}
		else
			msg.res = (int)tr;
	}
	catch (...)
	{
		msg.res = Err_Unknown;
		LOGX::Write(L";;;;;;;;;;;;;;;;;UploadFileThread catch;;;;;;;;;;;;;;;;;;");
	}
	msg.SendToUI();
	NotifyModuleSpaceChangedMsg spaceMsg;
	spaceMsg.deviceID = _deviceID;
	spaceMsg.nModule = eSpaceInfo_Module_File;
	spaceMsg.PostToUI();
}
