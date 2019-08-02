///////////////////////////////////////////////////////////
//  UsbConnection.h
//  Implementation of the Class UsbConnection
//  Created on:      01-ÁùÔÂ-2011 9:43:04
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_9B6A2974_3CA8_47ca_BD2E_09C2C1BA6A14__INCLUDED_)
#define EA_9B6A2974_3CA8_47ca_BD2E_09C2C1BA6A14__INCLUDED_

#include <vector>
#include <core/DeviceInfo.h>

namespace ND91Assistant
{

    class UsbConnection
    {

    public:
        UsbConnection();
        virtual ~UsbConnection();

        std::vector<DeviceInfo> ScanUsbBus();
        int Connect(DeviceInfo di);
        void Disconnect(DeviceInfo di);

    };
}

#endif // !defined(EA_9B6A2974_3CA8_47ca_BD2E_09C2C1BA6A14__INCLUDED_)
