#include "pch_module.h"

#if defined( MODULE_APP )

#include "GetAppIconCommand.h"
#include "Module/AndroidDefine.h"

GetAppIconCommand::GetAppIconCommand(const DeviceData* pDevice, wstring pkgName, wstring strSavePath)
: BaseCommand(pDevice)
{
	_packageName = pkgName;
	_strSavePath = strSavePath;
    //Android
    _nAndroidChannelID = APPLICATION;
    _nAndroidCommandID = GET_APPICON;
}

GetAppIconCommand::~GetAppIconCommand()
{
}

void ND91Assistant::GetAppIconCommand::Execute()
{
	if (_pDevice && _pDevice->GetAndroidDeamonMode())
		DefaultExecute();
}

std::string ND91Assistant::GetAppIconCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;

    writer.WriteInt(_nAndroidChannelID);
    writer.WriteChar((char)_nAndroidCommandID);
	writer.WriteString(CCodeOperation::UnicodeToUTF_8(_packageName));

    return writer.GetBuffer();
}

//特殊的命令返回格式
bool GetAppIconCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
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

void ND91Assistant::GetAppIconCommand::SetAndroidReturnData( std::string data )
{
    BinaryBufferReader reader(data.c_str(), data.length());
    if (!parseAndroidPackageHead(&reader))
        return;

	int nIconSize = 0;
    reader.ReadInt32(nIconSize);
	char* pIconData = new char[nIconSize];
    reader.ReadBuffer(pIconData, nIconSize);

	_success = SaveIconData(_strSavePath, pIconData, nIconSize);
    _result = _success ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;

	SAFE_DELETE(pIconData);
}

bool ND91Assistant::GetAppIconCommand::SaveIconData( wstring strFilePath, const char* pIconData, int nIconSize)
{
	return CFileOperation::WriteBinFile(strFilePath, pIconData, nIconSize);	
}

#endif