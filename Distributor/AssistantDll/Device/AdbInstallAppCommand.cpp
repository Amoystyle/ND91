#include "stdafx.h"
#include "AdbInstallAppCommand.h"
#include "Device/AndroidAdbHelper.h"

using namespace ND91Assistant;

ND91Assistant::AdbInstallAppCommand::AdbInstallAppCommand( wstring sn, wstring strPhonePath, bool bLock/*=true*/, bool bRecover/*=true*/, bool bOnSDCard/*=true*/ )
{
	_sn = CCodeOperation::UnicodeToUTF_8(sn);

	_strPhonePath = strPhonePath;
	_bLock        = bLock;
	_bRecover     = bRecover;
	_bOnSDCard    = bOnSDCard;
}

wstring ND91Assistant::AdbInstallAppCommand::GetInstallFailError()
{
	if(!_failerr.empty())
		_failerr=_failerr.substr(0,_failerr.find("]"));
	return CCodeOperation::UTF_8ToUnicode(_failerr);
}

void ND91Assistant::AdbInstallAppCommand::ExecuteAgain( bool sdCardOption )
{
	_success = ConnectToDevice(_sn);
	if (!_success)
		return;
	_success = false;
	GenerateCommand(sdCardOption);
	CommonExecute(_strCommand);
}

void ND91Assistant::AdbInstallAppCommand::InternalExecute()
{
	ExecuteAgain(true);

	//增加INSTALL_FAILED_CONTAINER_ERROR错误时的重试机制，先删除/mnt/secure/asec/smdl2tmp1.asec再重新安装可能会成功
	if ( !_returnData.empty() &&_returnData.find("INSTALL_FAILED_CONTAINER_ERROR")!=string::npos )
	{
			if (_bOnSDCard)
			{
				LOG->WriteDebug(L" returnData: INSTALL_FAILED_CONTAINER_ERROR");
				_pBaseSocket->close();
				SAFE_DELETE( _pBaseSocket );
				_pBaseSocket = new CBaseSocket();
				_pBaseSocket->create(ADB_COMMAND_HOST, ADB_COMMAND_PORT,true,3000);
				string cmdTmp;
				cmdTmp.append("rm \"/mnt/secure/asec/smdl2tmp1.asec\"");
				CommonExecute(cmdTmp);
				ExecuteAgain(true);
			}
		
	}

	if ( _returnData.empty() || _returnData.find("Error: Unknown option") != string::npos )
	{
		LOG->WriteDebug(L" returnData: install -null");
		_pBaseSocket->close();
		SAFE_DELETE( _pBaseSocket );
		_pBaseSocket = new CBaseSocket();
		_pBaseSocket->create(ADB_COMMAND_HOST, ADB_COMMAND_PORT,true,3000);
		
		ExecuteAgain(false);
	}
}

void ND91Assistant::AdbInstallAppCommand::GenerateCommand( bool sdCardOption )
{
	char buffer[512];
	sprintf(buffer, ADB_CMD_INSTALL_APP,
		_bLock ? "-l" : "",
		_bRecover ? "-r" : "",
		sdCardOption ? (_bOnSDCard ? "-s" : "-f") : "",
		CCodeOperation::UnicodeToUTF_8(_strPhonePath).c_str());
	_strCommand = buffer;
	LOG->WriteDebug(L"AdbInstallAppCommand : " + CCodeOperation::GB2312ToUnicode(_strCommand));
}

bool ND91Assistant::AdbInstallAppCommand::ParseReturnData()
{
    string returnData = _returnData;
	int j=0;
	while(j<3)
	{
		LOG->WriteDebug(L"AdbInstallAppCommand Return : " + CCodeOperation::GB2312ToUnicode(returnData));
		if(returnData.find(CCodeOperation::UnicodeToUTF_8(_strPhonePath))!=string::npos)
		{
			vector<string> retlines=CStrOperation::parseLines(returnData);
			if(retlines.size()>1)
			{
				_success=returnData.find("Success") != string::npos;
				if(!_success&&retlines[1].find("[")!=string::npos)
					_failerr=retlines[1].substr(retlines[1].find("[")+1);

              //  if (!returnData.empty())
                    _returnData = returnData;

				return _success;
			}
			else
			{
				int i=1;
				returnData=_pBaseSocket->receiveData();
				while(returnData.empty() && i < 10)
				{
					returnData=_pBaseSocket->receiveData();
					i++;
				}
				LOG->WriteDebug(L"AdbInstallAppCommand retry data : "+_strPhonePath + CCodeOperation::GB2312ToUnicode(returnData));
				LOG->WriteDebug(L"AdbInstallAppCommand retry : " + CStrOperation::IntToWString(i));
				_success=returnData.find("Success") != string::npos;
				if(!_success&&!returnData.empty()&&returnData.find("[")!=string::npos)
					_failerr=returnData.substr(returnData.find("[")+1);

              //  if (!returnData.empty())
                    _returnData = returnData;

				return _success;
			}
			break;
		}
		returnData=_pBaseSocket->receiveData();
		j++;
	}

    if (!returnData.empty())
        _returnData = returnData;
	
	return false;
}