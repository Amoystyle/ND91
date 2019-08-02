#pragma once
#include "Common/BaseAblThread.h"
#include "Common/UserMessage/abl/AblFileMessage.h"


class GetNewFileNameThread : public BaseAblThread 
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    GetNewFileNameThread( string threadID, int nRequestID, wstring deviceID );
    ~GetNewFileNameThread();

	RequestExist_ABL* _requestExist_ABL;
	ResponseGetNewName_ABL* _responseGetNewName_ABL;

protected:
    // ִ�о���Ĳ���
    virtual bool Action(DeviceData* pDevice);
	void HandleRequestExist_ABL(DeviceData* pDevice);
	void HandleRequestGetNewName_ABL(DeviceData* pDevice);
};