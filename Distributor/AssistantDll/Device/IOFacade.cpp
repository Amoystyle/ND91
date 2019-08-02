///////////////////////////////////////////////////////////
//  ChannelFacade.cpp
//  Implementation of the Class ChannelFacade
//  Created on:      01-六月-2011 9:42:51
//  Original author: michael
///////////////////////////////////////////////////////////

#include "IOFacade.h"

#include "IosUsbIO.h"
#include "IosWifiIO.h"
#include "AndroidUsbIO.h"
#include "AndroidWifiIO.h"

#include "Common/AssistantException.h"

using namespace ND91Assistant;

IOFacade::IOFacade()
{

}

IOFacade::~IOFacade()
{

}

void ND91Assistant::IOFacade::Execute( BaseCommand* pCommand )
{
    const DeviceInfo* pDeviceInfo = pCommand->GetDeviceInfo();

    switch( pDeviceInfo->_deviceType)
    {
    case Ios:
        {
            IosFileIO* pIO = NULL;
            switch( pDeviceInfo->_connectType)
            {
            case USB:
                pIO = new IosUsbIO();
                break;

            case WIFI:
                pIO = new IosWifiIO();
                break;

            default:
                THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);
                break;
            }

            if ( pCommand->IsAsync() )
            {
                pIO->SendAsync(pCommand);
                // TODO: 异步下面如何处理？
            }
            else
            {
                pIO->Send(pCommand);
            }
            delete pIO;
        }

        break;
    case Android:
        {
            AndroidTcpIO* pIO = NULL;
            switch( pDeviceInfo->_connectType)
            {
            case USB:
                pIO = new AndroidUsbIO();
                break;

            case WIFI:
                pIO = new AndroidWifiIO();
                break;

            default:
                THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);
                break;
            }

            if ( pCommand->IsAsync() )
            {
                pIO->SendAsync(pCommand);
                // TODO: 异步下面如何处理？
            }
            else
            {
                pIO->Send(pCommand);
            }
            delete pIO;
        }
        break;
    default:
        THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);
        break;
    }
}