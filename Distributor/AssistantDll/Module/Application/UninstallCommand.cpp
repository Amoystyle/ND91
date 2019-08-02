#include "pch_module.h"

#if defined( MODULE_APP )

#include "Module/Application/UninstallCommand.h"
#include "Module/Application/AppData.h"

#include "Device/AdbUninstallAppCommand.h"
#include "IosApp/IosAppOperationFactory.h"
#include "Module/AndroidDefine.h"

UninstallCommand::UninstallCommand(const DeviceData* pDevice, const AppData* appData)
: BaseCommand(pDevice)
{
	//Android
	_nAndroidChannelID = APPLICATION;
	_nAndroidCommandID = UNINSTALL_APP;
	_appData = appData;
}



UninstallCommand::~UninstallCommand()
{
}

void ND91Assistant::UninstallCommand::Execute()
{
	if (!_pDevice || !_pDevice->GetDevInfo())
		return ;

    SendPrompt(MESSAGE_INSTALL_UNINSTALL);
    SendProgress();
	if (_pDevice->GetDevInfo()->_deviceType == Android)
	{
		if (_pDevice->GetDevInfo()->_connectType == WIFI)
		{
			DefaultExecute();
		}
		else if (_pDevice->GetDevInfo()->_connectType == USB)
		{//USB方式优先使用Adb
			AdbUninstallAppCommand uninstallCommand(_pDevice->GetDevInfo()->_strSerialNumber, _appData->_packageName, _appData->_bKeepData);
			uninstallCommand.Execute();
			_success = uninstallCommand.Success();
		}
	}
	else if (_pDevice->GetDevInfo()->_deviceType == Ios)
	{
		IIosAppOperation* pOperation = IosAppOperationFactory::GetAppOperation(_pDevice, _appData->_eAppType, *this);
		if (pOperation)
		{
			_success = pOperation->UnInstallApp(_appData->_packageName);
			delete pOperation;
		}
		pOperation = NULL;
	}

    _result = _success ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
}

#pragma region Android
bool ND91Assistant::UninstallCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
{
	pReader->ReadBuffer(_pReturnHead, 4);
	pReader->ReadShort(_nReturnFlag);
	pReader->ReadShort(_nReturnLongConnectSign);
	pReader->ReadInt32(_nReturnBodySize);
	pReader->ReadInt32(_nReturnChannelID);
	//如果返回的数据包信息不符合本Command的信息，则跳过
	if (_nReturnChannelID != _nAndroidChannelID) // || _nReturnCommonType != _nAndroidCommandID)
		return false;
	string strOkay;
	pReader->ReadString(strOkay, 4);
	if (strOkay != ANDROID_PACKAGE_OKAY)
		return false;
	return true;
}

std::string ND91Assistant::UninstallCommand::GetAndroidBodyData()
{
	BinaryBufferWriter writer;

	string strPackageName = CCodeOperation::UnicodeToUTF_8(_appData->_packageName);

	writer.WriteInt(_nAndroidChannelID);
	writer.WriteChar((char)_nAndroidCommandID);
	writer.WriteString(strPackageName);
	writer.WriteBool(_appData->_bKeepData);
	writer.WriteInt( strPackageName == ADB_DEMON_PACKAGE_NAME ? 2 : 1);

	return writer.GetBuffer();
}

void ND91Assistant::UninstallCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead(&reader))
	{
		_success = false;
		return;
	}
	_success = true;
    _result = _success ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
}

bool ND91Assistant::UninstallCommand::SimpleUninstallApp( const DeviceData* pDevice, wstring strPackageName, APP_TYPE appType, bool bKeepData )
{
	AppData* pData = new AppData();
	pData->_packageName  = strPackageName;
	pData->_eAppType = APP_TYPE_APK;
	pData->_bKeepData = bKeepData;

	UninstallCommand uninstallCmd(pDevice, pData);
	uninstallCmd.Execute();

	SAFE_DELETE(pData);

	return uninstallCmd.Success();
}
#pragma endregion

#endif