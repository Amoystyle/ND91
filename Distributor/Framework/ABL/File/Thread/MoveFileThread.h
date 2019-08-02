#pragma once
#include "Common/BaseAblThread.h"

/*! @class
********************************************************************************
<PRE>
类名称   : MoveFileThread
功能     : 加载列表
异常类   :  
--------------------------------------------------------------------------------
备注     : 线程处理完自动释放，运行中会发消息通知外部进度 
典型用法 : 
</PRE>
*******************************************************************************/
class MoveFileThread : public BaseAblThread
{
public:
    // 构造函数 threadID线程唯一标识
    MoveFileThread( string threadID, int nRequestID, wstring deviceID );
    ~MoveFileThread();

    void SetParam(vector<wstring>* strSourceDevicePathList, wstring strTargetDeviceDir, bool bCopy)
    {
        _strSourceDevicePathList = *strSourceDevicePathList;
        _strTargetDeviceDir = strTargetDeviceDir;
        _bCopy = bCopy;
    };
protected:
    // 执行具体的操作
    virtual bool Action(DeviceData* pDevice);

    vector<wstring> _strSourceDevicePathList;   // 要移动的节点
    wstring _strTargetDeviceDir;    // 移动到这里
    bool _bCopy;    // 是否拷贝，是就拷贝，否就移动
};