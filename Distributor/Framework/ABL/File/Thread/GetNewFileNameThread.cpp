#include "stdafx.h"
#include "GetNewFileNameThread.h"
#include "../AblFileManager.h"
#include <stack>
#include "LOGX.h"
#include "..\AblFileManager.h"

GetNewFileNameThread::GetNewFileNameThread( string threadID, int nRequestID, wstring deviceID )
: BaseAblThread(threadID, nRequestID, deviceID)
{
	_responseGetNewName_ABL = NULL;
	_requestExist_ABL = NULL;
}

GetNewFileNameThread::~GetNewFileNameThread()
{
	SAFE_DELETE(_responseGetNewName_ABL);
	SAFE_DELETE(_requestExist_ABL);
}


bool GetNewFileNameThread::Action(DeviceData* pDevice)
{
	ABLFile_AutoLock;
	WAIT_DEVICE_ACTIVE_NORMAL;
	HandleRequestGetNewName_ABL(pDevice);
	HandleRequestExist_ABL(pDevice);


	return true;
}

void GetNewFileNameThread::HandleRequestExist_ABL( DeviceData* pDevice )
{
	RETURN_IF(!_requestExist_ABL);
	ResponseExist_ABL msg;
	msg.path = _requestExist_ABL->path;
	msg.nRequestID = _nGUID;
	msg.res = FileHelper::ExistInDevice(_requestExist_ABL->path, pDevice) ? 0 : 1;
	msg.SendToUI();
}

void GetNewFileNameThread::HandleRequestGetNewName_ABL( DeviceData* pDevice )
{
	RETURN_IF(!_responseGetNewName_ABL);
	//ResponseGetNewName_ABL msg;
	//	msg.folder = _requestGetNewName->folder;
	//	// TODO 实现获取不重名的名字
	//
	//	/*int i = 0;
	//	wchar_t buffer[10];
	//	wstring destName;*/
	//
	//	/*do
	//	{
	//		CStrOperation::MyItows(++i, buffer, 10);
	//		wstring copyName = International::GetString(FILE_COPY_NAME);
	//		if (name.find(copyName)!=wstring::npos)
	//			destName = path + copyName + buffer + L" " + filename.substr(copyName.length(), filename.length()) + ext;
	//		else
	//			destName = path + filename + buffer + ext;
	//	}
	//	while( pCmd->Exist(destName) );*/
	//
	//	msg.SendToUI();
}
