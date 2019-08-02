#pragma once
#include "Common/BaseAblThread.h"
#include "Common/UserMessage/abl/AblFileMessage.h"


/*! @class
********************************************************************************
<PRE>
类名称   : GetSDPath_Android
功能     : 创建文件夹
异常类   :  
--------------------------------------------------------------------------------
备注     : 线程处理完自动释放，运行中会发消息通知外部进度 
典型用法 : 
</PRE>
*******************************************************************************/
class GetSDPath_Android : public BaseAblThread
{
public:
    // 构造函数 threadID线程唯一标识
    GetSDPath_Android( string threadID, int nRequestID, wstring deviceID );
    ~GetSDPath_Android();
	

	ReqeustSDPathMsg* _reqeustSDPathMsg;
protected:
    // 执行具体的操作
    virtual bool Action(DeviceData* pDevice);
};