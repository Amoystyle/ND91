#pragma once
#include "Common/BaseAblThread.h"
#include "Common/UserMessage/abl/AblFileMessage.h"


/*! @class
********************************************************************************
<PRE>
������   : GetSDPath_Android
����     : �����ļ���
�쳣��   :  
--------------------------------------------------------------------------------
��ע     : �̴߳������Զ��ͷţ������лᷢ��Ϣ֪ͨ�ⲿ���� 
�����÷� : 
</PRE>
*******************************************************************************/
class GetSDPath_Android : public BaseAblThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    GetSDPath_Android( string threadID, int nRequestID, wstring deviceID );
    ~GetSDPath_Android();
	

	ReqeustSDPathMsg* _reqeustSDPathMsg;
protected:
    // ִ�о���Ĳ���
    virtual bool Action(DeviceData* pDevice);
};