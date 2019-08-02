#pragma once
#include "Common/BaseAblThread.h"
#include "Common/UserMessage/abl/AblFileMessage.h"

/*! @class
********************************************************************************
<PRE>
类名称   : RenameFileThread
功能     : 加载列表
异常类   :  
--------------------------------------------------------------------------------
备注     : 线程处理完自动释放，运行中会发消息通知外部进度 
典型用法 : 
</PRE>
*******************************************************************************/
class RenameFileThread : public BaseAblThread
{
public:
    // 构造函数 threadID线程唯一标识
    RenameFileThread( string threadID, int nRequestID, wstring deviceID );
    ~RenameFileThread();

	RequestRenameFileMsg* _requestRenameFileMsg;
protected:
    // 执行具体的操作
    virtual bool Action(DeviceData* pDevice);
	void Handle_RequestRenameFileMsg(DeviceData* pDevice);

    wstring _strDevicePath;
    wstring _strTargetName;
};