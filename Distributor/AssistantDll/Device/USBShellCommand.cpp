
#include "USBShellCommand.h"

using namespace ND91Assistant;


USBShellCommand::USBShellCommand()
{
	_isShellCommand = true;
    _bSizeInBody = false;
}

USBShellCommand::~USBShellCommand(void)
{
}

void ND91Assistant::USBShellCommand::Execute()
{
	_returnData = "";
	ConnectToDevice(_serialNumber);
	CommonExecute(_command);
}

bool ND91Assistant::USBShellCommand::ParseReturnData()
{
	return false;
}