#include "stdafx.h"

#include "DirectoryOperationCommand.h"
#include "Core/DeviceData.h"

#include "Device/AdbDirOperateCommand.h"
#include "Device/IosWiFiFileSystem.h"
#include "Device/IIosFileSystem.h"
#include "Device/AFCAppFileSystem.h"
#include "Device/NoDeamonInterface/NoDeamonInterface.h"
#include "Module/AndroidDefine.h"

DirectoryOperationCommand::DirectoryOperationCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _doneSuccess = false;
	_pAdbDirOp	 = NULL;

    _commandType	= EDirExist;
    _androidCommand = "";
    _mode			= "";
    _source			= L"";
    _dest			= L"";
	_fileSize		= -1;
	// Android
	_nAndroidChannelID = SHELL;
	_nAndroidCommandID = ExecShell;
}

DirectoryOperationCommand::~DirectoryOperationCommand()
{
	SAFE_DELETE(_pAdbDirOp);
}

void DirectoryOperationCommand::SetAndroidReturnData(std::string data)
{
     _doneSuccess = false;

	 if(ECreateDir == _commandType && _createDirbyShellFail)
	 {
		 BinaryBufferReader reader(data.c_str(),data.length());
		 if( ! parseAndroidPackageHead_Top12(&reader) )
			 return;
		reader.ReadInt32(_nReturnChannelID);
		if(_nReturnChannelID != FILEMANAGER)
			return;
		reader.ReadBool(_doneSuccess);
		return;
	 }
    string result ="";
    if( ExtractAndroidResult(data, result))
    {
		ParseResult(result);
    }
}

void DirectoryOperationCommand::ParseResult(const string result)
{
	switch(_commandType)
	{
	case EDeleteDir:
	case EMoveRemotefile:
	case ERenameRemotefile:
	case EChmod:
		_doneSuccess = result.empty();
		break;
	case EDeleteFile:
		_doneSuccess =  (result.empty() || (int)result.find("No such file or directory") >= 0 );
		break;

	case EDirExist:
		_doneSuccess =  !((int)result.find("No such file or directory") >= 0 
			|| (int)result.find("Permission denied") >= 0 
			|| (int)result.find("Not a directory") >= 0);
		break;

	case ECreateDir:
		_doneSuccess = result.empty() 
			|| (int)result.find("mkdir: can't set permissions of directory") >= 0
			|| (int)result.find("File exists") >= 0 ;
		break;
	case ESupportColor:
		_doneSuccess = !((int) result.find("No such file or directory") >= 0 
						|| (int)result.find("Permission denied") >= 0
						|| (int)result.find("unrecognized option") >= 0
						|| (int)result.find("Unknown option") >= 0 );
		break;

	default:
		break;
	}
}


bool DirectoryOperationCommand::ExtractAndroidResult(const string& receivedData, string& shellresult)
{
    BinaryBufferReader reader(receivedData.c_str(),receivedData.length());

    if( ! parseAndroidPackageHead(&reader) )
        return false;

    bool issuccess = false;
    reader.ReadBool(issuccess);

    if(issuccess)
    {
        int lenth = 0;
        reader.ReadInt32(lenth);
        reader.ReadString(shellresult, lenth);
    }

    return issuccess;
}

std::string DirectoryOperationCommand::GetAndroidBodyData()
{
		BinaryBufferWriter  bufWrite;
		bufWrite.Clear();
		if(ECreateDir == _commandType && _createDirbyShellFail)
		{
			_nAndroidChannelID=FILEMANAGER;
			bufWrite.WriteInt(FILEMANAGER);
			bufWrite.WriteChar(6);
			bufWrite.WriteStringAsUTF8(_source);
		}
		else
		{
			_nAndroidChannelID=SHELL;
			bufWrite.WriteInt(_nAndroidChannelID);
			bufWrite.WriteInt(_nAndroidCommandID);
			bufWrite.WriteInt(_androidCommand.length());
			bufWrite.WriteBuffer(_androidCommand.c_str(), _androidCommand.length());
		}
		return bufWrite.GetBuffer();
}

void DirectoryOperationCommand::Execute()
{
	_doneSuccess = false;
	if ( _pDevice->GetDevInfo()->_deviceType == Ios)
	{
		if(_source.length()<1)
			return;

        IIosFileSystem* pIosFs = NULL;
        if (_pDevice->GetDevInfo()->_connectType == WIFI)
            pIosFs = (IIosFileSystem*)new IosWiFiFileSystem(_pDevice->GetDevInfo()->_ios_wifi_handle);
        else
            if (!_pDevice->GetShareAppIdentifier().empty())
                pIosFs =  AFCAppFileSystem::StartAPPHouseArrestService(_pDevice->GetShareAppIdentifier(), 
                                                                       (DeviceInfo*)_pDevice->GetDevInfo());
            else
                pIosFs = (IIosFileSystem*)new AFCFileSysemBase(_pDevice->GetDevInfo());

		switch(_commandType)
		{
		case EDirExist:
			_doneSuccess = pIosFs->Ios_Exists(_source);
			break;
		case ECreateDir:
			_doneSuccess = pIosFs->Ios_CreateDirectory(_source);
			break;
		case EDeleteDir:
			_doneSuccess = pIosFs->Ios_DeleteEmptyDirectory(_source);
			break;
		case EDeleteFile:
			_doneSuccess = pIosFs->Ios_DeleteFile(_source);
			break;
		case ERenameRemotefile:
			_doneSuccess = pIosFs->Ios_Rename(_source, _dest);
			break;
		case EGetFileSize:
			_fileSize	 = pIosFs->Ios_FileSize(_source);
			_doneSuccess = true; //add by wengcunmin为了防范flase的返回会导致未知bug这里我加了一行。
			break;
		case EChmod:
			DefaultExecute();
			_doneSuccess = true;//add by wengcunmin 未越狱机器如果这里返回失败会导致上传文件夹不能成功
			break;
		default:
			break;
		}

		SAFE_DELETE(pIosFs);
	}
	else if (_pDevice->GetDevInfo()->_deviceType == Android)
	{
		_doneSuccess = false;
		if(_androidCommand.empty())
			return;
		if(_pDevice->GetDevInfo()->_connectType == WIFI || (_commandType ==ECreateDir && _createDirbyShellFail))
		{
			DefaultExecute();
		}
		else
		{
			if(!_pAdbDirOp)
				_pAdbDirOp = new AdbDirOperateCommand(_pDevice->GetDevInfo()->_strSerialNumber);
			_pAdbDirOp->setParam(_androidCommand);
			_pAdbDirOp->Execute();
			if(!_pAdbDirOp->IsOk())
				return;
			ParseResult(_pAdbDirOp->GetReturnData());
		}
	}
	else
	{
		THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);
	}
}

void DirectoryOperationCommand::SetParams(COMMAND_TYPE t, const wstring source, const wstring dest)
{
	_androidCommand = "";
    _commandType = t;
	_source = source;
	_dest = dest;
	if ( this->_pDevice->GetDevInfo()->_deviceType == Android )
    {
        string sourceFile = "\"" + CCodeOperation::UnicodeToUTF_8(source) + "\"";
        string destinationFile = "\"" + CCodeOperation::UnicodeToUTF_8(dest) + "\"";

        switch(_commandType)
        {
        case EDeleteDir:
            _androidCommand = "rmdir " + sourceFile;
            break;

        case EDeleteFile:
            _androidCommand = "rm "+ sourceFile;
            break;

        case EMoveRemotefile:
            _androidCommand = "mv " + sourceFile + " " + destinationFile;
            break;

        case ERenameRemotefile:
            _androidCommand = "mv "+ sourceFile + " " + destinationFile;
            break;

        case EChmod:
            _androidCommand = "chmod " + _mode + " " + sourceFile;
            break;

        case EDirExist:
            _androidCommand = _pDevice->SupportColor() ? "ls -l -a --color=never " : "ls -l -a ";
            _androidCommand.append(sourceFile);
            break;

        case ECreateDir:
            _androidCommand = "mkdir " + sourceFile;
            break;
		case ESupportColor:
			_androidCommand = "ls --color=never";
			break;

        default:
            break;
        }
    }
	// 统一在这里增加字符转义处理
	if (_pDevice->GetDevInfo()->_connectType == USB)
	{
		_androidCommand = CStrOperation::Replace(_androidCommand, "$", "\\$");
		_androidCommand = CStrOperation::Replace(_androidCommand, "`", "\\`");
	}
}

void ND91Assistant::DirectoryOperationCommand::SetMode( const string& mode )
{
    _mode = mode;
}

bool ND91Assistant::DirectoryOperationCommand::AndroidCreateDir( const wstring & path )
{
	wstring  tempPath = CStrOperation::trimRightW(path, L"/");
	int pos = tempPath.substr(0,tempPath.length()-1).find_last_of(L"/");
	tempPath=tempPath.substr(0, pos+1);
	if(!Exist(tempPath) && !AndroidCreateDir(tempPath))
	{
		return false;
	}
	return MakeDir(path);
}

bool ND91Assistant::DirectoryOperationCommand::Move( wstring sourceDir, wstring destDir )
{
    // 【mv /sdcard/TestCopy/ /sdcard/TestMove/TestCopy_1】会出错
    // 【mv /sdcard/TestCopy /sdcard/TestMove/TestCopy_1】可以成功，所以去掉右边的/

	// 同路径文件拷贝，直接跳过
    sourceDir = CStrOperation::trimRightW(sourceDir, L"/");
    SetParams(EMoveRemotefile, sourceDir, destDir);
    Execute();

	bool succss = _doneSuccess;
	ChangeMode(destDir, "644");
    return succss;
}

bool ND91Assistant::DirectoryOperationCommand::Exist( wstring dir )
{
    SetParams(EDirExist, dir);
    Execute();
    return _doneSuccess;
}

bool ND91Assistant::DirectoryOperationCommand::MakeDir( wstring dir)
{
	_createDirbyShellFail = false;
    SetParams(ECreateDir, dir);
    Execute();

	if(!_doneSuccess)
	{
		_createDirbyShellFail = true;
		SetParams(ECreateDir, dir);
		Execute();
	}

	if(_doneSuccess)
    {
		ChangeMode(dir, "755");     // 此操作可能失败
        // 就算是ChangeMode失败也认为成功
        _doneSuccess = true;
    }


    return _doneSuccess;
}

bool ND91Assistant::DirectoryOperationCommand::DeleteDir( wstring dir )
{
    SetParams(EDeleteDir, dir);
    Execute();
    return _doneSuccess;
}

bool ND91Assistant::DirectoryOperationCommand::DeleteFileLink( wstring file )
{
    SetParams(EDeleteFile, file);
    Execute();
    return _doneSuccess;
}

bool ND91Assistant::DirectoryOperationCommand::Rename( wstring sourceDir, wstring destDir )
{
    SetParams(ERenameRemotefile, sourceDir, destDir);
    Execute();
    return _doneSuccess;
}

bool ND91Assistant::DirectoryOperationCommand::GetMode(wstring dir, string& mode, wstring& owner)
{
    INT64 size = 0;
    UINT16 mod = 0;
    UINT32 uid = 0;
    UINT32 gid = 0;
    LONG mtime = 0;
    LONG birthtime = 0;
    if (_pDevice->GetDevInfo()->_deviceType == Ios 
        && _pDevice->GetDevInfo()->_connectType == USB 
        && _pDevice->GetDevInfo()->_ios_bJailBreaked)
    {
		SetCSLock(((DeviceInfo*)_pDevice->GetDevInfo())->_csIOSSessionLock);
        int res = NoDeamonInterface::GetFileInfo(_pDevice->GetDevInfo()->_ios_devptr,
                                                 (wchar_t*)dir.c_str(),
                                                 &size, &mod, &uid, &gid, &mtime, &birthtime);

        //size=%lld mode=%ho uid=%u gid=%u mtime=%ld birthtime=%ld"
        char tempMod[128] = {0};
        sprintf(tempMod, "%ho", mod);
        mode = tempMod;
        wchar_t tempOwner[128] = {0};
        wsprintf(tempOwner, L"%u", uid);
        owner = tempOwner;

        return (res == 0);
    }

    return false;
}

bool ND91Assistant::DirectoryOperationCommand::ChangeMode( wstring dir, string mode, wstring owner)
{
    SetMode(mode);
    SetParams(EChmod, dir);
    _strOwner = owner;
    if(_pDevice->GetDevInfo()->_deviceType == Ios)
    {
        if (_pDevice->GetDevInfo()->_connectType == USB 
            && _pDevice->GetDevInfo()->_ios_bJailBreaked)
        {
			SetCSLock(((DeviceInfo*)_pDevice->GetDevInfo())->_csIOSSessionLock);
            wstring smode = FormatMode(mode);
            NoDeamonInterface::Chmod(_pDevice->GetDevInfo()->_ios_devptr, (wchar_t*)dir.c_str(), (wchar_t*)smode.c_str());

            if (_strOwner.empty())
                _strOwner = L"501";
            NoDeamonInterface::Chown(_pDevice->GetDevInfo()->_ios_devptr, (wchar_t*)dir.c_str(), (wchar_t*)_strOwner.c_str());
            _doneSuccess = true;
            return _doneSuccess;
        }
        else
        {
            if (mode.empty())
            {
                if(_strOwner.empty())
                    _strOwner = L"501";
                _strIosCmdFileName = L"Chown." + CFileOperation::GetGuidW();
            }
            else
            {
                _strIosCmdFileName = L"SetAttribute." + CFileOperation::GetGuidW();
            }
        }
    }
    Execute();
    return _doneSuccess;
}

// 转换权限格式（例如："755" 转换为 "rwxr-xr-x"）
wstring ND91Assistant::DirectoryOperationCommand::FormatMode(string mode)
{
    wstring res;
    if (mode.size() == 0)
        mode = "000";
    else if (mode.size() == 1)
        mode = "00" + mode;
    else if (mode.size() == 2)
        mode = "0" + mode;

    for (size_t i = 0; i < mode.size(); i++)
    {
        if (mode.at(i) == '7')
            res += L"rwx";
        else if (mode.at(i) == '6')
            res += L"rw-";
        else if (mode.at(i) == '5')
            res += L"r-x";
        else if (mode.at(i) == '4')
            res += L"r--";
        else if (mode.at(i) == '3')
            res += L"-wx";
        else if (mode.at(i) == '2')
            res += L"-w-";
        else if (mode.at(i) == '1')
            res += L"--x";
        else if (mode.at(i) == '0')
            res += L"---";
    }
    return res;
}

bool ND91Assistant::DirectoryOperationCommand::IsSupportColor()
{
	SetParams(ESupportColor,L"");
	Execute();
	return _doneSuccess;
}

int ND91Assistant::DirectoryOperationCommand::IosGetFileSize( wstring file )
{
	SetParams(EGetFileSize, file);
	Execute();
	return _fileSize;
}

void ND91Assistant::DirectoryOperationCommand::SetIosReturnFile( std::wstring filename )
{
	if (CFileOperation::IsFileExist(filename.c_str()))
		_doneSuccess = true;

    CFileOperation::DeleteFile(filename);
}

std::wstring ND91Assistant::DirectoryOperationCommand::GetIosCommandFileContent()
{
	int nVer = IosPublicFunc::GetVerNum(GetDeviceInfo()->_deviceProductVersion);
	wstring strCmdMemo = L"";

	if (_mode.empty())
	{
		strCmdMemo += _source + L"\t" + _strOwner;
		return strCmdMemo;
	}

	wstring mode = CCodeOperation::UTF_8ToUnicode(_mode);
	if (_strOwner.empty())
	{
		if (nVer >= 103)
			_strOwner = L"501";
		else
			_strOwner = L"0";
	}
	strCmdMemo += _strOwner + L"\t" + _source + L"\t" + mode + L"\n";

	return strCmdMemo;

}

bool ND91Assistant::DirectoryOperationCommand::IsMountSystem()
{
    return _rh.ParseIsMountSystem(_pDevice->GetDevInfo()->_strSerialNumber);
}