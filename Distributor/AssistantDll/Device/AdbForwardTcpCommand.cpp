#include "stdafx.h"

#include "AdbForwardTcpCommand.h"

AdbForwardTcpCommand::AdbForwardTcpCommand(const std::wstring sn,wstring id,
                                           int pcPort,
                                           int devicePort)
:AdbRemoteCommand(id) 
,_pcPort(pcPort)
, _devicePort(devicePort)
{
	_sn = CCodeOperation::UnicodeToUTF_8(sn);
    _isShellCommand = false;
    _bSizeInBody = false;
}
void AdbForwardTcpCommand::InternalExecute()
{
    if (!ConnectToDevice(_sn))
        return;

    // …˙≥…√¸¡Ó
    char buffer[256];
    //sprintf_s(buffer, 256, ADB_CMD_FORWARD_TCP, _sn.c_str(), _pcPort, _devicePort);
    sprintf(buffer, ADB_CMD_FORWARD_TCP, _sn.c_str(), _pcPort, _devicePort);

    CommonExecute(buffer); 
}

bool AdbForwardTcpCommand::ParseReturnData()
{
    return ! _returnData.empty();
}
