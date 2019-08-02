#pragma once

#include "Module/Driver/BaseDriverController.h"

// ��ȡ�������豸ID,������������򷵻�����ID; �豸������������������豸,���򷵻��豸ID
namespace ND91Assistant
{   
    class CLASSINDLL_CLASS_DECL GetDriverUrlID : public BaseDriverController
    {
    public:
        // pDriver ��ǰ����
        GetDriverUrlID(Driver* pDriver);
        ~GetDriverUrlID();

        // �ύ����
        bool Action();

        // ȡ�û�õ��豸ID
        string GetDriverID() { return _driverID;};
        // ȡ�û�õ�����ID
        string GetDeviceID() { return _deviceID;};

    private:        
        string _driverID;  // �õ����豸ID
        string _deviceID;  // �õ�������ID
    };
}