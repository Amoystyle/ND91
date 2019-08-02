#include "stdafx.h"

#include "CopyFileCommand.h"
#include "DirectoryOperationCommand.h"
#include "Module/File/FileHelper.h"

#include "Core/DeviceData.h"
#include "Device/AdbDirOperateCommand.h"
#include "Module/AndroidDefine.h"

CopyFileCommand::CopyFileCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
	_retCopyInfo._mDuration = -1;
	_retCopyInfo._mSpeed = -1;
	_retCopyInfo._mTotalTransfered =-1;

    _doneSuccess = false;

    _source = L"";
    _dest = L"";

	// Android
	_nAndroidChannelID = FILEMANAGER;
	_nAndroidCommandID = 4;
	_adbCmd =NULL;
	_pDirCmd = NULL;
}

CopyFileCommand::~CopyFileCommand(void)
{
	SAFE_DELETE(_adbCmd);
    SAFE_DELETE(_pDirCmd);
}

std::string CopyFileCommand::GetAndroidBodyData()
{
    /*string command = "dd if=\"" + CCodeOperation::UnicodeToUTF_8(_source) +
					 "\" of=\"" + CCodeOperation::UnicodeToUTF_8(_dest) + "\"";*/

/*	string command = "cp ";
// 	if(_IsOverWrite)
// 		command += "-f ";

	command += "\""+CCodeOperation::UnicodeToUTF_8(_source) +
				"\" \"" + CCodeOperation::UnicodeToUTF_8(_dest) + "\"";*/

    BinaryBufferWriter  writer;
    writer.Clear();
    writer.WriteInt(_nAndroidChannelID);
    writer.WriteChar(_nAndroidCommandID);
    writer.WriteStringAsUTF8(_source);
    writer.WriteStringAsUTF8(_dest);
	writer.WriteBool(false);
    return writer.GetBuffer();
}

void ND91Assistant::CopyFileCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader   bufReader(data.c_str(), data.length());
	if(!parseAndroidPackageHead(&bufReader))
       return;

	string  result;
	bool issuccess = false;
	bufReader.ReadBool(issuccess);
	_doneSuccess = issuccess;
	return;
}


void ND91Assistant::CopyFileCommand::setFileNames( const wstring remoteFromPath, const wstring remoteToPath)
{
	_source = remoteFromPath;
	 _dest = remoteToPath;
}

void ND91Assistant::CopyFileCommand::Execute()
{
	_retCopyInfo._mSpeed = 0;
    _doneSuccess = false;

    // ios设备无此功能: 不支持在设备上复制文件
    RETURN_IF( _pDevice->GetDevInfo()->_deviceType == Ios );

	// 存在同名文件检查
	if( !CheckExistFile() )
	{
		_doneSuccess = true;
		return;
	}

    if(_pDevice->GetDevInfo()->_connectType == WIFI)
		DefaultExecute();
	else
	{
		// 同路径文件拷贝，直接跳过
		if (_source == _dest)
		{
			_doneSuccess = true;
			return;
		}
		if (!_adbCmd)
			_adbCmd = new AdbDirOperateCommand(_pDevice->GetDevInfo()->_strSerialNumber);
		string command = "dd if=\"" + CCodeOperation::UnicodeToUTF_8(_source) +
					"\" of=\"" + CCodeOperation::UnicodeToUTF_8(_dest) + "\"";
		_adbCmd->setParam(command);
		_adbCmd->Execute();
		ParseResult(_adbCmd->GetReturnData());
	}

    ProgressObserver* pObserver = NULL;
    if (!_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

    IF_CANCEL(pObserver)
    {
        FileHelper::Delete(_pDevice, _dest);
        _doneSuccess = false;
    }

	if(_retCopyInfo._mSpeed > 0)
		_doneSuccess = true;
	else
		_doneSuccess = CheckFileExist(_dest);    
}

bool CopyFileCommand::CheckFileExist(const wstring& aRemotefile)
{
	return  DirectoryOperationCommand(_pDevice).Exist(aRemotefile);
}

void CopyFileCommand::ParseResult(const string& result)
{
    _rh.ParseResultCopyFileCommand(result, _retCopyInfo);
}

bool CopyFileCommand::CheckExistFile()
{
	RETURN_TRUE_IF ( ! GetDirCmd()->Exist(_dest) );

	switch(AskUser(_dest))
	{
	case CANCEL:
		return false;

	case OVERWRITE:
	case OVERWRITE_ALL:
		// GetDirCmd()->DeleteFileLink(_fileonphone);
		return true;

	case RENAME:
	case RENAME_ALL:
		_dest = DirData::PhoneFileRename(_dest, GetDirCmd());
		return true;

    case RENAME_COPY:
    case RENAME_COPY_ALL:
        _dest = DirData::PhoneFileRenameCopy(_dest, GetDirCmd());
        return true;

	default:
		return false;
	}
}

DirectoryOperationCommand* ND91Assistant::CopyFileCommand::GetDirCmd()
{
	if ( ! _pDirCmd )
		_pDirCmd = new DirectoryOperationCommand(_pDevice);

	return _pDirCmd;
}