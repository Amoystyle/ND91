///////////////////////////////////////////////////////////
//  WifiConnection.h
//  Implementation of the Class WifiConnection
//  Created on:      01-ÁùÔÂ-2011 9:43:05
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_A6D04D59_0FB4_4930_BE66_809E5FDB8E50__INCLUDED_)
#define EA_A6D04D59_0FB4_4930_BE66_809E5FDB8E50__INCLUDED_

#include "Core/DeviceInfo.h"

namespace ND91Assistant
{

    class WifiConnection
    {

    public:
        WifiConnection();
        virtual ~WifiConnection();

        void Listen();
        int Connect(DeviceInfo di);
        void Disconnect(DeviceInfo di);

    };
}

#endif // !defined(EA_A6D04D59_0FB4_4930_BE66_809E5FDB8E50__INCLUDED_)
