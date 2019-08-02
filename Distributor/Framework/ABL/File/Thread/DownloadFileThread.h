#pragma once
#include "Common/BaseAblThread.h"

/*! @class
********************************************************************************
<PRE>
类名称   : DownloadFileThread
功能     : 下载文件
异常类   :  
--------------------------------------------------------------------------------
备注     : 线程处理完自动释放，运行中会发消息通知外部进度 
典型用法 : 
</PRE>
*******************************************************************************/
class DownloadFileThread : public BaseAblThread
{
public:
    // 构造函数 threadID线程唯一标识
    DownloadFileThread( string threadID, int nRequestID, wstring deviceID );
    ~DownloadFileThread();

    void SetParam(vector<wstring>* strDevicePathList, wstring strPcDir)
    {
        _strDevicePathList = *strDevicePathList;
        _strPcDir = strPcDir;
    };
protected:
    // 执行具体的操作
    virtual bool Action(DeviceData* pDevice);

    vector<wstring> _strDevicePathList;
    wstring _strPcDir;
};