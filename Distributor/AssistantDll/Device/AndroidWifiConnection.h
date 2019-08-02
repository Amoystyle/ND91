///////////////////////////////////////////////////////////
//  AndroidWifibConnection.h
//  Implementation of the Class AndroidWifibConnection
//  Created on:      01-����-2011 9:42:48
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_C1637CC2_B865_4502_A25B_6E2AB1293B56__INCLUDED_)
#define EA_C1637CC2_B865_4502_A25B_6E2AB1293B56__INCLUDED_
#include "Common/BaseSocket.h"
#include "Core/DeviceData.h"
#include "IConnection.h"
namespace ND91Assistant
{
    // Android WIFI���Ӵ���
    class AndroidWifiConnection : public IConnection
    {

    public:
        AndroidWifiConnection(DeviceData* pDeviceData);
        virtual ~AndroidWifiConnection();

        // ����׼��
        virtual bool PreActivate();

        // ����
        virtual bool Activate();

        // ����
        virtual bool Connect();

        // �Ͽ�
        virtual void Disconnect();

        // �������ӳɹ����õ�Ӳ����Ϣ
        virtual const DeviceInfo* GetDeviceInfo();

        //��ȡ�豸����
        void GetAndroidProperty();

    private:
        DeviceData* _pDeviceData;
   };
}

#endif // !defined(EA_C1637CC2_B865_4502_A25B_6E2AB1293B56__INCLUDED_)
