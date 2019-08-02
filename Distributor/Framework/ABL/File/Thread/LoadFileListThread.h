#pragma once
#include "Common/BaseAblThread.h"
#include "Common/UserMessage/abl/AblFileMessage.h"


/*! @class
********************************************************************************
<PRE>
������   : LoadFileListThread
����     : �����б�
�쳣��   :  
--------------------------------------------------------------------------------
��ע     : �̴߳������Զ��ͷţ������лᷢ��Ϣ֪ͨ�ⲿ���� 
�����÷� : 
</PRE>
*******************************************************************************/
class LoadFileListThread : public BaseAblThread 
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    LoadFileListThread( string threadID, int nRequestID, wstring deviceID );
    ~LoadFileListThread();

	RequestLoadItemsByPath_ABL* _requestLoadItemsByPath_ABL;
	RequestLoadItemsByItem_ABL* _requestLoadItemsByItem_ABL;
	RequestJump2Path_ABL*		_requestJump2Path_ABL;
	RequestReloadNode_ABL*		_requestReloadNode_ABL;
	RequestAppDisplayNameMsg*	_requestAppDisplayNameMsg;
protected:
    // ִ�о���Ĳ���
    virtual bool Action(DeviceData* pDevice);
	void Handle_RequestLoadItemsByPath_ABL(DeviceData* pDevice);
	void Handle_RequestLoadItemsByItem_ABL(DeviceData* pDevice);
	void Handle_RequestJump2Path_ABL(DeviceData* pDevice);
	void Handle_RequestReloadNode_ABL(DeviceData* pDevice);
		void Handle_RequestAppDisplayNameMsg(DeviceData* pDevice);
		void Handle_RequestReleaseNodesMsg(DeviceData* pDevice);
	void RemoveFromChildrens(FileListItem* item);
};