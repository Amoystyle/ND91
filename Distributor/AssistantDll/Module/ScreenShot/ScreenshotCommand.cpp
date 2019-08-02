#include "pch_module.h"

#ifdef MODULE_SCREENSHOT

#include "ScreenshotCommand.h"
#include "ScreenImageData.h"
#include "GetScreenImageInfoCommand.h"

#include "Module/Common/Synchronizer/ScreenshotClient.h"
#include "Device/AdbScreenShotCommand.h"
#include "Device/NoDeamonInterface/NoDeamonInterface.h"
#include "Common/Path.h"
#include "Module/AndroidDefine.h"

ScreenshotCommand::ScreenshotCommand(const DeviceData* pDevice, wstring strSavePath)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = SHELL;
    _nAndroidCommandID = GET_SCREENSHOT;

	_strSavePath = strSavePath;
	_strIosCmdFileName = L"screensnap.0";
	_ScreenImagebinaryData = "";
}



ScreenshotCommand::~ScreenshotCommand()
{

}

// 执行命令
void ScreenshotCommand::Execute()
{
    if (_pDevice->GetDevInfo()->_connectType == WIFI)
        WifiExecute();
    else if (_pDevice->GetDevInfo()->_connectType == USB)
        if (_pDevice->GetDevInfo()->_deviceType == Android)
            AndroidUSBExecute();
        else
            IosUSBExecute();
}

void ScreenshotCommand::WifiExecute()
{
    ScreenshotClient client(_pDevice,NULL);
    client.InstallPrerequisite(L"E:\\macshare\\5.1.itd");
    wstring filepath=client.TakeScreenshotWithoutConvertTiff2PNG();
    if(!filepath.empty())
        SetIosReturnFile(filepath);
    else
        DefaultExecute();
}

void ScreenshotCommand::AndroidUSBExecute()
{
    RETURN_IF(_pDevice->GetAndroidDeamonMode() == 0);// 无守护模式

    AdbScreenShotCommand adbcmd(_pDevice->GetDevInfo()->_strSerialNumber,_pDevice->GetDevInfo()->_ios_deviceIdentifier);
    _ScreenImagebinaryData = adbcmd.GetScreenImageData();
    if (!_strSavePath.empty())
    {
        _success = SaveScreenShot(_strSavePath);
        _result = _success ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
    }
    _success = !_ScreenImagebinaryData.empty();
    _result = _success ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
}

void ScreenshotCommand::IosUSBExecute()
{
    BYTE *pngBytesPtr = NULL;
    UINT pngLength = 0;
	EnterCriticalSection(&((DeviceInfo*)_pDevice->GetDevInfo())->_csIOSSessionLock);
	int ret =0;
	try
	{
		 ret =  NoDeamonInterface::Screenshot(_pDevice->GetDevInfo()->_ios_devptr, &pngBytesPtr, &pngLength); 
	}
	catch(...)
	{

	}
	LeaveCriticalSection(&((DeviceInfo*)_pDevice->GetDevInfo())->_csIOSSessionLock);
    if(ret != 0)
    {
        _success = false;
        _result = _success ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
        return ;
    }

    FILE *fp = NULL;
    if(_tfopen_s(&fp, _strSavePath.c_str(), _T("w+b")) == 0)
    {
        fwrite(pngBytesPtr, 1, pngLength, fp);
        fclose(fp);
    }

    NoDeamonInterface::Delete(pngBytesPtr);
    _success = true;
    _result = _success ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
}

wstring ScreenshotCommand::GetIosResultFilePath()
{
    return Path::IosScreensnapFile();
}

// 解析返回文件，解析后删除文件
void ScreenshotCommand::SetIosReturnFile(std::wstring filename)
{
    CFileOperation::DeleteFile(_strSavePath);

	_success = CFileOperation::IsFileExist(filename.c_str());
	_result = _success ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
	RETURN_IF(!_success);

	CFileOperation::MakeLocalDir(CFileOperation::GetFileDir(_strSavePath));

    CFileOperation::CopyFile(filename, _strSavePath, false);
    CFileOperation::DeleteFile(filename);
}

std::string ScreenshotCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;

    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);

    return writer.GetBuffer();
}

void ScreenshotCommand::SetAndroidReturnData( std::string data )
{
    BinaryBufferReader reader(data.c_str(), data.length());

    if (!parseAndroidPackageHead(&reader))
	{
		_success = false;
		_result =  COMMAND_EXECUTE_FAIL;
        return;
	}
    int nSize = 0;		
    reader.ReadInt32(nSize);
   // char* pScreenShot = new char[nSize+1];
   // reader.ReadBuffer(pScreenShot, nSize);

	reader.ReadString(_ScreenImagebinaryData, nSize);
	//_ScreenImagebinaryData = string(pScreenShot,nSize);

	if (!_strSavePath.empty())
	{
		_success = SaveScreenShot(_strSavePath);
		_result = _success ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
	}

	//SAFE_DELETE_ARRAY(pScreenShot);

    _success = true;
    _result = COMMAND_EXECUTE_SUCCESS;
}

//特殊的命令返回格式
bool ScreenshotCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
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

bool ScreenshotCommand::SaveScreenShot( wstring strFilePath)
{
	ScreenImageData* screenData = new ScreenImageData();
	GetScreenImageInfoCommand cmd(_pDevice);
	cmd.SetParam(screenData);
	cmd.Execute();
	screenData->SetScreenBinaryData(_ScreenImagebinaryData,false);
	CFileOperation::MakeLocalDir(CFileOperation::GetFileDir(strFilePath));
	screenData->SaveToFile(strFilePath, _pDevice);
	SAFE_DELETE(screenData);
	return true;
	/*CFileOperation::MakeLocalDir(CFileOperation::GetFileDir(strFilePath));
    return CFileOperation::WriteBinFile(strFilePath, pScreenShot, nSize);*/	
}


#endif