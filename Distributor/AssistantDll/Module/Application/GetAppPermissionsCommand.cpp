#include "pch_module.h"

#if defined( MODULE_APP )

#include "GetAppPermissionsCommand.h"
#include "Module/AndroidDefine.h"

GetAppPermissionsCommand::GetAppPermissionsCommand(const DeviceData* pDevice, wstring pkgName)
: BaseCommand(pDevice)
{
	_packageName = pkgName;
	//Android
	_nAndroidChannelID = APPLICATION;
	_nAndroidCommandID = GET_APPPERMISSIONS;
}

GetAppPermissionsCommand::~GetAppPermissionsCommand()
{
}

void ND91Assistant::GetAppPermissionsCommand::Execute()
{
	if (_pDevice && _pDevice->GetAndroidDeamonMode())
		DefaultExecute(); 
}

std::string ND91Assistant::GetAppPermissionsCommand::GetAndroidBodyData()
{
	BinaryBufferWriter writer;

	writer.WriteInt(_nAndroidChannelID);
	writer.WriteChar((char)_nAndroidCommandID);
	writer.WriteString(CCodeOperation::UnicodeToUTF_8(_packageName));

	return writer.GetBuffer();
}

bool ND91Assistant::GetAppPermissionsCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
{
	pReader->ReadBuffer(_pReturnHead, 4);
	pReader->ReadShort(_nReturnFlag);
	pReader->ReadShort(_nReturnLongConnectSign);
	pReader->ReadInt32(_nReturnBodySize);
	pReader->ReadInt32(_nReturnChannelID);
	string strOkay;
	pReader->ReadString(strOkay, 4);
	//如果返回的数据包信息不符合本Command的信息，则跳过
	if (_nReturnChannelID != _nAndroidChannelID || strOkay != ANDROID_PACKAGE_OKAY)
		return false;
	return true;
}

void ND91Assistant::GetAppPermissionsCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead(&reader))
		return;

	int nSize;
	reader.ReadInt32(nSize);
	if (nSize > 100)
		return;

	_PermissionList.clear();
	for (int i=0; i<nSize; i++)
	{
		string strPermission;
		reader.ReadString(strPermission);
		if (!strPermission.empty())
		{
			wstring strPermissionUnicode = CCodeOperation::GB2312ToUnicode(strPermission);
			_PermissionList.push_back(strPermissionUnicode);
		}
	}
	_success = true;
    _result = COMMAND_EXECUTE_SUCCESS;
}

#endif