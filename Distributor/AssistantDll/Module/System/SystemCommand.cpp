#include "stdafx.h"
#ifdef MODULE_SYSTEM

#include "SystemCommand.h"
#include <sstream>
#include "Module/AndroidDefine.h"

using namespace ND91Assistant;

std::string SystemCommand::GetAndroidDatagram(short port)
{
    BinaryBufferWriter _writer;

    _writer.Clear();
    _writer.WriteBuffer("NDPC", 4);
    _writer.WriteShort(Client | NeedPostBack | CreateLongConnection);
    _writer.WriteShort(port);
    _writer.WriteInt(4);
    _writer.WriteInt(SYSTEM);

    return _writer.GetBuffer();
}

void SystemCommand::ParseturnDatagram( std::string data )
{
    BinaryBufferReader reader(data.c_str(), data.size());

    memset(_pHead, 0, 5);
    reader.ReadBuffer(_pHead, 4);
    reader.ReadInt32(_nFlag);
    reader.ReadInt32(_nDataLength);
    reader.ReadInt32(_nCommandID);
}

bool SystemCommand::VerifyPackageHead()
{
    return ! strncmp(_pHead, "NDPC", 4);
}

std::wstring SystemCommand::ToString()
{
    std::wostringstream woss;

    woss << " receive datagram : head=" << _pHead << ", flag=" << _nFlag
        << ", length" << _nDataLength << ", command id=" << _nCommandID;

    return woss.str();
}



#endif