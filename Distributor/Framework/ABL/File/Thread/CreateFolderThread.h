#pragma once
#include "Common/BaseAblThread.h"
#include "Common/UserMessage/abl/AblFileMessage.h"


/*! @class
********************************************************************************
<PRE>
������   : CreateFolderThread
����     : �����ļ���
�쳣��   :  
--------------------------------------------------------------------------------
��ע     : �̴߳������Զ��ͷţ������лᷢ��Ϣ֪ͨ�ⲿ���� 
�����÷� : 
</PRE>
*******************************************************************************/
class CreateFolderThread : public BaseAblThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    CreateFolderThread( string threadID, int nRequestID, wstring deviceID );
    ~CreateFolderThread();
	

	RequestCreateFolderMsg* _requestCreateFolderMsg;
	RequestCheckFoldersMsg* _requestCheckFoldersMsg;
protected:
    // ִ�о���Ĳ���
    virtual bool Action(DeviceData* pDevice);

private:
	void Handle_RequestCreateFolderMsg_ABL(DeviceData* pDevice);
	void Handle_RequestCheckFoldersMsg_ABL(DeviceData* pDevice);
};