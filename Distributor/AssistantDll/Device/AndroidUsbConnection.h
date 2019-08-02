///////////////////////////////////////////////////////////
//  AndroidUsbConnection.h
//  Implementation of the Class AndroidUsbConnection
//  Created on:      01-六月-2011 9:42:48
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
	// Android USB连接处理
    class AndroidUsbConnection : public IConnection
    {

    public:
        AndroidUsbConnection(DeviceData* pDeviceData);
        virtual ~AndroidUsbConnection();

        // 连接准备
        virtual bool PreActivate();

        // 激活
        virtual bool Activate();

        // 连接
        virtual bool Connect();

        // 断开
        virtual void Disconnect();

        // 返回连接成功后获得的硬件信息
        virtual const DeviceInfo* GetDeviceInfo();

        //获取设备属性
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
