#pragma once
#include "Common/BaseAblThread.h"
#include "Common/UserMessage/abl/AblFileMessage.h"


/*! @class
********************************************************************************
<PRE>
类名称   : CreateFolderThread
功能     : 创建文件夹
异常类   :  
--------------------------------------------------------------------------------
备注     : 线程处理完自动释放，运行中会发消息通知外部进度 
典型用法 : 
</PRE>
*******************************************************************************/
class CreateFolderThread : public BaseAblThread
{
public:
    // 构造函数 threadID线程唯一标识
    CreateFolderThread( string threadID, int nRequestID, wstring deviceID );
    ~CreateFolderThread();
	

	RequestCreateFolderMsg* _requestCreateFolderMsg;
	RequestCheckFoldersMsg* _requestCheckFoldersMsg;
protected:
    // 执行具体的操作
    virtual bool Action(DeviceData* pDevice);

private:
	void Handle_RequestCreateFolderMsg_ABL(DeviceData* pDevice);
	void Handle_RequestCheckFoldersMsg_ABL(DeviceData* pDevice);
};