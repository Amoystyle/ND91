#pragma once
#include "Common/BaseAblThread.h"
#include "Common/UserMessage/abl/AblFileMessage.h"
/*! @class
********************************************************************************
<PRE>
������   : DeleteFileThread
����     : ɾ���ļ�
�쳣��   :  
--------------------------------------------------------------------------------
��ע     : �̴߳������Զ��ͷţ������лᷢ��Ϣ֪ͨ�ⲿ���� 
�����÷� : 
</PRE>
*******************************************************************************/
class DeleteFileThread : public BaseAblThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    DeleteFileThread( string threadID, int nRequestID, wstring deviceID );
    ~DeleteFileThread();

    RequestDeleteFileMsg* _requestDeleteFileMsg;
	RequestDeleteFileByPathMsg* _requestDeleteFileByPathMsg;
protected:
    // ִ�о���Ĳ���
    virtual bool Action(DeviceData* pDevice);

	void Handle_RequestDeleteFileByPathMsg(DeviceData* pDevice);
	void Handle_RequestDeleteFileMsg(DeviceData* pDevice);

	bool find(DirData* par_data, NodeData* sub_data);
};