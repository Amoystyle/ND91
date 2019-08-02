#pragma once
#include "Common/BaseAblThread.h"
#include "Common/UserMessage/abl/AblFileMessage.h"
/*! @class
********************************************************************************
<PRE>
类名称   : DeleteFileThread
功能     : 删除文件
异常类   :  
--------------------------------------------------------------------------------
备注     : 线程处理完自动释放，运行中会发消息通知外部进度 
典型用法 : 
</PRE>
*******************************************************************************/
class DeleteFileThread : public BaseAblThread
{
public:
    // 构造函数 threadID线程唯一标识
    DeleteFileThread( string threadID, int nRequestID, wstring deviceID );
    ~DeleteFileThread();

    RequestDeleteFileMsg* _requestDeleteFileMsg;
	RequestDeleteFileByPathMsg* _requestDeleteFileByPathMsg;
protected:
    // 执行具体的操作
    virtual bool Action(DeviceData* pDevice);

	void Handle_RequestDeleteFileByPathMsg(DeviceData* pDevice);
	void Handle_RequestDeleteFileMsg(DeviceData* pDevice);

	bool find(DirData* par_data, NodeData* sub_data);
};