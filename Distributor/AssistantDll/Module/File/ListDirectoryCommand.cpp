#include "stdafx.h"

#include "ListDirectoryCommand.h"

#include "Core/DeviceData.h"
#include "Device/IosWiFiFileSystem.h"
#include "Device/AFCAppFileSystem.h"
#include "Device/AdbListDirCommand.h"
#include "Module/AndroidDefine.h"

#define SpacialDir   L"lost+found"

ListDirectoryCommand::ListDirectoryCommand(const DeviceData* pDevice, NodeData* pNode)
: BaseCommand(pDevice)
{
    _rh.SetNode(pNode);
	_success = false;
	// Android
	_nAndroidChannelID = SHELL;
	_nAndroidCommandID = ExecShell;
}

ListDirectoryCommand::~ListDirectoryCommand()
{
}

void ND91Assistant::ListDirectoryCommand::SetAndroidReturnData(std::string data)
{

	string shellresult = "";
	BinaryBufferReader reader(data.c_str(),data.length());
	if( !parseAndroidPackageHead(&reader))
		return;
	bool issuccess = false;
	reader.ReadBool(issuccess);
	if(issuccess)
	{
		int lenth = 0;
		reader.ReadInt32(lenth);
		reader.ReadString(shellresult, lenth);

		//LOG->WriteDebug(CCodeOperation::GB2312ToUnicode(shellresult));

		_success = ParseResult(shellresult);
	}

}

std::string ListDirectoryCommand::GetAndroidBodyData()
{

	string command = (_pDevice->SupportColor()?"ls -l -a --color=never \"":"ls -l -a \"")+ 
					 CCodeOperation::UnicodeToUTF_8(_rh.GetNode()->GetFullPath())+"\"";
	int value = 0;

	BinaryBufferWriter BufferWriter;
	BufferWriter.Clear();
	BufferWriter.WriteInt(_nAndroidChannelID);
	BufferWriter.WriteInt(_nAndroidCommandID);
	BufferWriter.WriteInt(command.length());
	BufferWriter.WriteBuffer(command.c_str(), command.length());
	return BufferWriter.GetBuffer();
}

void ND91Assistant::ListDirectoryCommand::Execute()
{
	_success = false;

    RETURN_IF(!_rh.GetNode());

    RETURN_IF(_pDevice == NULL);
	if ( _pDevice->GetDevInfo()->_deviceType == Ios)
	{
        DirData* pDir = dynamic_cast<DirData*>(_rh.GetNode());
        if ( pDir )
        {
            if ( _pDevice->GetDevInfo()->_connectType == WIFI)
            {
                IosWiFiFileSystem ioswifiFile(_pDevice->GetDevInfo()->_ios_wifi_handle);
                _success = ioswifiFile.Ios_GetFileInfos(pDir, eFileGetType_All);
            }
            else if (_pDevice->GetDevInfo()->_connectType == USB)
            {
                if (!_pDevice->GetShareAppIdentifier().empty())
                {
                    AFCFileSysemBase* pFileStream =  AFCAppFileSystem::StartAPPHouseArrestService(_pDevice->GetShareAppIdentifier(), (DeviceInfo*)_pDevice->GetDevInfo());
                    _success = pFileStream->Ios_GetFileInfos(pDir, eFileGetType_All);
                    SAFE_DELETE(pFileStream);
                }
                else
                {
                    AFCFileSysemBase iosUsbFile(_pDevice->GetDevInfo());
                    _success = iosUsbFile.Ios_GetFileInfos(pDir, eFileGetType_All);
                }
            }
        }
	}
	else if (_pDevice->GetDevInfo()->_deviceType == Android)
	{
        wstring path = _rh.GetNode()->GetFullPath();
        RETURN_IF(path.find(SpacialDir) != wstring::npos);

		if(_pDevice->GetDevInfo()->_connectType == USB)
		{
            AdbListDirCommand cmd(_pDevice->GetDevInfo()->_strSerialNumber, path, _pDevice->SupportColor());
            cmd.Execute();
            _success = ParseResult(cmd.GetResultData());
		}
		else
			DefaultExecute();
	}
}

bool  ListDirectoryCommand::ParseResult(const string& result)
{
    return _rh.ParseResultListDirectoryCommand(result);
}

bool ND91Assistant::ListDirectoryCommand::DoneSuccess()
{
    return _success;
}

void ND91Assistant::ListDirectoryCommand::SetNode( NodeData* pNode )
{
    _rh.SetNode(pNode);
    _success = false;
}
