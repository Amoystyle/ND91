#pragma once
#include "Common/BaseAblThread.h"

/*! @class
********************************************************************************
<PRE>
������   : MoveFileThread
����     : �����б�
�쳣��   :  
--------------------------------------------------------------------------------
��ע     : �̴߳������Զ��ͷţ������лᷢ��Ϣ֪ͨ�ⲿ���� 
�����÷� : 
</PRE>
*******************************************************************************/
class MoveFileThread : public BaseAblThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    MoveFileThread( string threadID, int nRequestID, wstring deviceID );
    ~MoveFileThread();

    void SetParam(vector<wstring>* strSourceDevicePathList, wstring strTargetDeviceDir, bool bCopy)
    {
        _strSourceDevicePathList = *strSourceDevicePathList;
        _strTargetDeviceDir = strTargetDeviceDir;
        _bCopy = bCopy;
    };
protected:
    // ִ�о���Ĳ���
    virtual bool Action(DeviceData* pDevice);

    vector<wstring> _strSourceDevicePathList;   // Ҫ�ƶ��Ľڵ�
    wstring _strTargetDeviceDir;    // �ƶ�������
    bool _bCopy;    // �Ƿ񿽱����ǾͿ���������ƶ�
};