#pragma once
#include "Common/BaseAblThread.h"
#include "Common/UserMessage/abl/AblFileMessage.h"


/*! @class
********************************************************************************
<PRE>
类名称   : LoadFileListThread
功能     : 加载列表
异常类   :  
--------------------------------------------------------------------------------
备注     : 线程处理完自动释放，运行中会发消息通知外部进度 
典型用法 : 
</PRE>
*******************************************************************************/
class LoadFileListThread : public BaseAblThread 
{
public:
    // 构造函数 threadID线程唯一标识
    LoadFileListThread( string threadID, int nRequestID, wstring deviceID );
    ~LoadFileListThread();

	RequestLoadItemsByPath_ABL* _requestLoadItemsByPath_ABL;
	RequestLoadItemsByItem_ABL* _requestLoadItemsByItem_ABL;
	RequestJump2Path_ABL*		_requestJump2Path_ABL;
	RequestReloadNode_ABL*		_requestReloadNode_ABL;
	RequestAppDisplayNameMsg*	_requestAppDisplayNameMsg;
protected:
    // 执行具体的操作
    virtual bool Action(DeviceData* pDevice);
	void Handle_RequestLoadItemsByPath_ABL(DeviceData* pDevice);
	void Handle_RequestLoadItemsByItem_ABL(DeviceData* pDevice);
	void Handle_RequestJump2Path_ABL(DeviceData* pDevice);
	void Handle_RequestReloadNode_ABL(DeviceData* pDevice);
		void Handle_RequestAppDisplayNameMsg(DeviceData* pDevice);
		void Handle_RequestReleaseNodesMsg(DeviceData* pDevice);
	void RemoveFromChildrens(FileListItem* item);
};