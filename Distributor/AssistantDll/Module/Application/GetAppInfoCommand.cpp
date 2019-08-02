#include "pch_module.h"

#if defined( MODULE_APP )

#include "GetAppInfoCommand.h"
#include "Module/AndroidDefine.h"

GetAppInfoCommand::GetAppInfoCommand(const DeviceData* pDevice, wstring strPackageName)
: BaseCommand(pDevice)
{
	_strPackageName = strPackageName;
	//Android
	_nAndroidChannelID = APPLICATION;
	_nAndroidCommandID = GET_APPINFO;
}

GetAppInfoCommand::~GetAppInfoCommand()
{
}

void ND91Assistant::GetAppInfoCommand::Execute()
{
	if (!_pDevice || !_pDevice->GetDevInfo())
		return ;

	if ( _pDevice->GetDevInfo()->_deviceType == Android )
	{
		if (_pDevice->GetAndroidDeamonMode())
			DefaultExecute();
	}
}

std::string ND91Assistant::GetAppInfoCommand::GetAndroidBodyData()
{
	BinaryBufferWriter writer;

	writer.WriteInt(_nAndroidChannelID);
	writer.WriteChar((char)_nAndroidCommandID);
	writer.WriteStringAsUTF8(_strPackageName);

	return writer.GetBuffer();
}

void ND91Assistant::GetAppInfoCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead_Top16(&reader))
		return;
	string strOkay;
	reader.ReadString(strOkay, 4);
	if (strOkay != ANDROID_PACKAGE_OKAY)
	{
		return;
	}
	_success = true;
    _result = COMMAND_EXECUTE_SUCCESS;
	const char* pData = data.c_str() + reader.GetOffset();
	int nLeftDataSize = data.length() - (pData - data.c_str());
	_appData.ParseAndroidData(pData, nLeftDataSize);
}

const AppData* ND91Assistant::GetAppInfoCommand::GetAppInfo( wstring strPackageName )
{
		_strPackageName = strPackageName;
		Execute();
		return &_appData;
}
#endif