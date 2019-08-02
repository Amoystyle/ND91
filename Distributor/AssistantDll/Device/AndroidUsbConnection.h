///////////////////////////////////////////////////////////
//  AndroidUsbConnection.h
//  Implementation of the Class AndroidUsbConnection
//  Created on:      01-����-2011 9:42:48
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_BE4B9CF9_B643_4bd6_B097_C80FDC5BC05B__INCLUDED_)
#define EA_BE4B9CF9_B643_4bd6_B097_C80FDC5BC05B__INCLUDED_

#include <vector>
#include <string>

#include "Device/IConnection.h"
#include "Core/DeviceData.h"

namespace ND91Assistant
{
	// Android USB���Ӵ���
    class AndroidUsbConnection : public IConnection
    {

    public:
        AndroidUsbConnection(DeviceData* pDeviceData);
        virtual ~AndroidUsbConnection();

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

		virtual void InitDeviceData();

		DWORD WINAPI ND91Assistant::AndroidUsbConnection::InitDeviceOtherData(/*LPVOID lpParameter*/);
    private:
		
	
        bool StartDeamon();
        bool StopDeamon();
        bool InstallDeamon();
    private:
        DeviceData* _pDeviceData;
		HANDLE	_hThread;
    };
}

#endif // !defined(EA_BE4B9CF9_B643_4bd6_B097_C80FDC5BC05B__INCLUDED_)
