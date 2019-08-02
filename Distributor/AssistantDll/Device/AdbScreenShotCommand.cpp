#include "stdafx.h"

#include "AdbScreenShotCommand.h"
#include "Common/BinaryBufferReader.h"
#include "Common/BinaryBufferWriter.h"

AdbScreenShotCommand::AdbScreenShotCommand(const wstring sn,wstring id)
:AdbRemoteCommand(id)
{
	_sn             = CCodeOperation::UnicodeToUTF_8(sn);
	_isShellCommand = false;
	_IsInfo          = false;
}

AdbScreenShotCommand::~AdbScreenShotCommand(void)
{
}

void ND91Assistant::AdbScreenShotCommand::InternalExecute()
{
	_returnData = "";
	_bpp = 0;
	if (!ConnectToDevice(_sn))
        return;

	string command = FormAdbRequest("framebuffer:");
	_pBaseSocket->sendData(command.c_str(), command.length());
	int size = 0;
	if(ReceiveOkFlag())
	{
		
		string result = _pBaseSocket->receiveData(4);
		BinaryBufferReader  bppreader(result.c_str(), result.length());
		bppreader.ReadInt32(_bpp);
		if(_bpp == 1)
		{
			_returnData = _pBaseSocket->receiveData(48);
			if (_IsInfo)
				return;
			BinaryBufferReader  reader(_returnData.c_str(), _returnData.length());
			int a;
			reader.ReadInt32(a);
			reader.ReadInt32(size);
		}
		else
		{
			_returnData = _pBaseSocket->receiveData(12);
			if (_IsInfo)
				return;
			BinaryBufferReader  reader(_returnData.c_str(), _returnData.length());
			reader.ReadInt32(size);
		}
	}
	BinaryBufferWriter bufwriter;
	bufwriter.WriteChar(0);
	if(!_pBaseSocket->sendData(bufwriter.GetBuffer().c_str(), bufwriter.GetBuffer().length()))
	{
		_returnData ="";
		return;
	}
	_returnData = _pBaseSocket->receiveData(size);
}

string ND91Assistant::AdbScreenShotCommand::GetScreenImageInfoData(int& bpp)
{
	_IsInfo = true;
	Execute();
	bpp = _bpp;
	return _returnData;
}

string ND91Assistant::AdbScreenShotCommand::GetScreenImageData()
{
	_IsInfo = false;
	Execute();
	return _returnData;
}