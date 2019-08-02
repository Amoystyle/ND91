#pragma once
#include "Common/BaseAblThread.h"
#include "Common/UserMessage/abl/AblFileMessage.h"

/*! @class
********************************************************************************
<PRE>
������   : UploadFileThread
����     : �ϴ�
�쳣��   :  
--------------------------------------------------------------------------------
��ע     : �̴߳������Զ��ͷţ������лᷢ��Ϣ֪ͨ�ⲿ���� 
�����÷� : 
</PRE>
*******************************************************************************/
class UploadFileThread 
    : public BaseAblThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    UploadFileThread( string threadID, int nRequestID, wstring deviceID );
    ~UploadFileThread();

	RequestUploadFileMsg* _requestUploadFileMsg;
protected:
    // ִ�о���Ĳ���
    virtual bool Action(DeviceData* pDevice);
	void Handle_RequestUploadFileMsg(DeviceData* pDevice);
};