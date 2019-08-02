#pragma once
#include "Common/BaseAblThread.h"

/*! @class
********************************************************************************
<PRE>
������   : DownloadFileThread
����     : �����ļ�
�쳣��   :  
--------------------------------------------------------------------------------
��ע     : �̴߳������Զ��ͷţ������лᷢ��Ϣ֪ͨ�ⲿ���� 
�����÷� : 
</PRE>
*******************************************************************************/
class DownloadFileThread : public BaseAblThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    DownloadFileThread( string threadID, int nRequestID, wstring deviceID );
    ~DownloadFileThread();

    void SetParam(vector<wstring>* strDevicePathList, wstring strPcDir)
    {
        _strDevicePathList = *strDevicePathList;
        _strPcDir = strPcDir;
    };
protected:
    // ִ�о���Ĳ���
    virtual bool Action(DeviceData* pDevice);

    vector<wstring> _strDevicePathList;
    wstring _strPcDir;
};