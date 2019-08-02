#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "CloseLongConnectionCommand.h"
#include "Module/AndroidDefine.h"

CloseLongConnectionCommand::CloseLongConnectionCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _nFlag = Client | NeedPostBack | CloseCurrentConnection;
}

std::string ND91Assistant::CloseLongConnectionCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;
    writer.WriteInt(0);

    return writer.GetBuffer();
}

void ND91Assistant::CloseLongConnectionCommand::SetAndroidReturnData( std::string data )
{

}


#endif