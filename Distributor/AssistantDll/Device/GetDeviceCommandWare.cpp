#include "stdafx.h"
#include "GetDeviceCommandWare.h"
#include "AdbGetDeviceCommand.h"
#include "core/DeviceManager.h"
#include "Device/AndroidAdbHelper.h"
#include "Common/AssistantException.h"

C_EXTERN CLASSINDLL_CLASS_DECL const char* GetIosDevices()
{

	string devices = DEVICE_MANAGER->GetInstance()->GetIosDevice();
	char* pBuffer = new char[devices.size() + 1];
	strcpy(pBuffer,  devices.c_str());
	pBuffer[devices.size()] = 0;
	return pBuffer;
}
C_EXTERN CLASSINDLL_CLASS_DECL void KillAdb()
{
	AndroidAdbHelper aah;
	aah.stopServer();
}
C_EXTERN CLASSINDLL_CLASS_DECL const char* GetAdbDevices()
{
	string cmdretdata;
	try
	{
		AdbGetDeviceCommand getDeviceCmd("host:nddevices");
		getDeviceCmd.Execute();
		cmdretdata = getDeviceCmd.GetReturnData();
		int retrycount=0;
		while(cmdretdata.find("error:") != string::npos)
		{
			getDeviceCmd.Execute();
			cmdretdata = getDeviceCmd.GetReturnData();
			retrycount++;
			if(retrycount>5)
				cmdretdata = "error";
		}	
	}
	catch(AssistantException e)
	{
		//if(e.GetErrorCode() == AssistantException::ADB_COMMAND_NO_NDDEVICE)
		{
			AdbGetDeviceCommand getDeviceCmd1("host:devices");
			getDeviceCmd1.Execute();
			cmdretdata = getDeviceCmd1.GetReturnData();
			int retrycount=0;
			while(cmdretdata.find("error:") != string::npos)
			{
				getDeviceCmd1.Execute();
				cmdretdata = getDeviceCmd1.GetReturnData();
				retrycount++;
				if(retrycount>5)
					cmdretdata = "error";
			}	
		}
	}
	
	
	char* pBuffer = new char[cmdretdata.size() + 1];
	strcpy(pBuffer, cmdretdata.c_str());
	pBuffer[cmdretdata.size()] = 0;

	return pBuffer;

}
C_EXTERN CLASSINDLL_CLASS_DECL int IosInit()
{
	return (int)DEVICE_MANAGER->GetInstance()->IosInit();
}
C_EXTERN CLASSINDLL_CLASS_DECL void ReleaseCharPtr(char* p)
{
	if(p)
	{
		delete p;
		p=NULL;
	}
}

// 获取adb_usb.ini文件路径
wstring AdbUsbIni() 
{
    // 取 ADB.EXE 路径
    wstring path;
    RETURN_VALUE_IF ( ! OPTION->Get(GlobalOption::STR_ADB_FILE_APTH, path), L"");
    RETURN_VALUE_IF(path.empty(), L"");

    // 路径转换
    wstring::size_type pos = path.rfind('\\');
    RETURN_VALUE_IF ( pos == wstring::npos, L"");
    path.erase(pos);
    path.append(L"\\.android\\adb_usb.ini");

    // 检测存在和大小
    RETURN_VALUE_IF(_waccess(path.c_str(), 0), L"");
    RETURN_VALUE_IF(CFileOperation::GetFileSizeW(path) > 102400, L"");
    return path;
}

C_EXTERN CLASSINDLL_CLASS_DECL bool ModADBIni( int vid )
{
    std::wstring path = AdbUsbIni();
    RETURN_FALSE_IF(path.empty());

    string strVid = "0x" + CStrOperation::Int2Hex(vid);

    string iniFile = CFileOperation::ReadFile(path, true);

    if ( ! iniFile.empty() )
    {
        // 已在ini中: 则返回true
        RETURN_TRUE_IF ( string::npos != iniFile.find(strVid) );
        RETURN_TRUE_IF ( string::npos != iniFile.find(CStrOperation::toUpper(strVid)) );

        if ( *iniFile.rbegin() != '\x0A' )
        {
            iniFile.append("\x0D\x0A");
        }
    }

    iniFile.append(strVid);
    iniFile.append("\x0D\x0A");

    RETURN_FALSE_IF( ! CFileOperation::WriteBinFile(path, iniFile.c_str(), iniFile.size()));
    return true;
}
