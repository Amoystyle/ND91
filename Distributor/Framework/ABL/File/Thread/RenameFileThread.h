#pragma once
#include "Common/BaseAblThread.h"
#include "Common/UserMessage/abl/AblFileMessage.h"

/*! @class
********************************************************************************
<PRE>
������   : RenameFileThread
����     : �����б�
�쳣��   :  
--------------------------------------------------------------------------------
��ע     : �̴߳������Զ��ͷţ������лᷢ��Ϣ֪ͨ�ⲿ���� 
�����÷� : 
</PRE>
*******************************************************************************/
class RenameFileThread : public BaseAblThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    RenameFileThread( string threadID, int nRequestID, wstring deviceID );
    ~RenameFileThread();

	RequestRenameFileMsg* _requestRenameFileMsg;
protected:
    // ִ�о���Ĳ���
    virtual bool Action(DeviceData* pDevice);
	void Handle_RequestRenameFileMsg(DeviceData* pDevice);

    wstring _strDevicePath;
    wstring _strTargetName;
};