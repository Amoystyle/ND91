///////////////////////////////////////////////////////////
//  GetDeviceInfoCommand.h
//  Implementation of the Class GetDeviceInfoCommand
//  Created on:      01-ÁùÔÂ-2011 9:42:55
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_9C2C8FB4_2EFE_4801_90CB_721C4A156985__INCLUDED_)
#define EA_9C2C8FB4_2EFE_4801_90CB_721C4A156985__INCLUDED_

#include "Module/BaseCommand.h"

namespace ND91Assistant
{

    class GetDeviceInfoCommand : public BaseCommand
    {

    public:
        GetDeviceInfoCommand(const DeviceData* pDevice);
        virtual ~GetDeviceInfoCommand();

    };
}

#endif // !defined(EA_9C2C8FB4_2EFE_4801_90CB_721C4A156985__INCLUDED_)
