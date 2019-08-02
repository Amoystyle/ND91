#pragma once
#include "Common/BaseAblThread.h"
#include "Common/UserMessage/abl/AblFileMessage.h"

/*! @class
********************************************************************************
<PRE>
类名称   : UploadFileThread
功能     : 上传
异常类   :  
--------------------------------------------------------------------------------
备注     : 线程处理完自动释放，运行中会发消息通知外部进度 
典型用法 : 
</PRE>
*******************************************************************************/
class UploadFileThread 
    : public BaseAblThread
{
public:
    // 构造函数 threadID线程唯一标识
    UploadFileThread( string threadID, int nRequestID, wstring deviceID );
    ~UploadFileThread();

	RequestUploadFileMsg* _requestUploadFileMsg;
protected:
    // 执行具体的操作
    virtual bool Action(DeviceData* pDevice);
	void Handle_RequestUploadFileMsg(DeviceData* pDevice);
};