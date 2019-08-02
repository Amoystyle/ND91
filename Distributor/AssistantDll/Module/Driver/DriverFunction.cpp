#include "stdafx.h"

#ifdef MODULE_DRIVER

#include "DriverFunction.h"
#include "DriverDef.h"
#include "Device/AdbGetDeviceCommand.h"
#include "Common/version.h"
#include "Common/SystemInfo.h"

// #define	IS_SOFT_ITUNES					_T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\iTunes.exe")
// #define	IS_SOFT_ITUNES_PATH				_T("Path")
// #define	IS_SOFT_APPLE_MOBILE_DEVICE		_T("Software\\Apple Inc.\\Apple Mobile Device Support")
// #define	IS_SOFT_APPLE_MOBILE_VERSION	_T("Version")
// #define ITUNES_VERSION					_T("7.5.0.20")
// #define APPLE_DEVICE_VERSION			_T("3.1.0.62")
// #define APPLE_MOBILE_DEVICE_SUPPORT		_T("Apple Mobile Device Support")
// #define ITUNES_URL						_T("http://show.sj.91.com/launch/Download.aspx?os=0&b=%d&lan=0&sn=%d")

string DriverFunction::getProperty( HDEVINFO hDevinfo, SP_DEVINFO_DATA device, int nInfo)
{
#ifdef WIN32	
	DWORD nTem;
	UCHAR szTem[4000];
	memset(szTem,0,sizeof(szTem));
	SetupDiGetDeviceRegistryPropertyA(hDevinfo, &device, nInfo, NULL, szTem, sizeof(szTem), (PDWORD)&nTem);
	string str((char*)szTem);
	return str;
#else
	return "";
#endif		
}

//获取VID
string DriverFunction::GetVID(const string strHardID)
{
	string strVID;
	int nIndex = (int)strHardID.find("VID_");
	if (nIndex > 0 && nIndex + 8 < (int)strHardID.length())
		strVID = strHardID.substr(nIndex + 4, 4);
	return strVID;
}
//获取PID
string DriverFunction::GetPID(const string strHardID)
{
	string strPID;
	int nIndex = (int)strHardID.find("PID_");
	if (nIndex > 0 && nIndex + 8 < (int)strHardID.length())
		strPID = strHardID.substr(nIndex + 4, 4);
	return strPID;
}
//获取MI
string DriverFunction::GetMI(const string strHardID)
{
	string strMI;
	int nIndex = strHardID.find("&MI_");
	if (nIndex > 0 && nIndex + 6 <= (int)strHardID.length())
		strMI = strHardID.substr(nIndex + 1, 5);
	return strMI;
}


HardwareInfo* DriverFunction::CreateHardwareInfo(HARDWARE_TYPE nType, HDEVINFO hDevinfo, SP_DEVINFO_DATA device)
{
	HardwareInfo* pInfo = new HardwareInfo(nType);
	pInfo->_strProviderName    = CCodeOperation::UTF_8ToUnicode(DriverFunction::getProperty(hDevinfo, device, Ep_MFG));
	pInfo->_strDriverDesc      = CCodeOperation::UTF_8ToUnicode(DriverFunction::getProperty(hDevinfo, device, Ep_DEVICEDESC));
	pInfo->_strHardID          = CCodeOperation::UTF_8ToUnicode(DriverFunction::getProperty(hDevinfo, device, SPDRP_HARDWAREID));
    pInfo->_strCompareID       = CCodeOperation::UTF_8ToUnicode(DriverFunction::getProperty(hDevinfo, device, Ep_COMPATIBLEIDS));
	pInfo->_strHardID			= CStrOperation::toUpperW(pInfo->_strHardID);

	string strHardID = DriverFunction::getProperty(hDevinfo, device, SPDRP_HARDWAREID);
	strHardID = CStrOperation::toUpper(strHardID);
	pInfo->_strVID = DriverFunction::GetVID(strHardID);
	pInfo->_strPID = DriverFunction::GetPID(strHardID);
	pInfo->_vid = CStrOperation::Hex2Int(pInfo->_strVID);
	pInfo->_pid = CStrOperation::Hex2Int(pInfo->_strPID);

	pInfo->_strMI = DriverFunction::GetMI(strHardID);
	pInfo->_strKey = "USB\\VID" + pInfo->_strVID + "&PID_" + pInfo->_strPID + "&" + pInfo->_strMI;

	pInfo->_nState = GetDriverState(nType, device);

	return pInfo;
}
// 移到systeminfo
// bool ND91Assistant::DriverFunction::IsNeedInstalliTunes()
// {
// 	bool bNeed = false;
// #ifdef WIN32	
// 	//检查iTunes.exe版本
// 	wstring strItunesPath = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, IS_SOFT_ITUNES, IS_SOFT_ITUNES_PATH);
// 	//某些版本iTunes使用默认值为path值
// 	if(strItunesPath.empty())
// 			strItunesPath = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, IS_SOFT_ITUNES, _T(""));
// 	LOG->WriteDebug(L"取得iTunes完整版地址：" + strItunesPath);
// 	if (strItunesPath.length() > 0 && CFileOperation::IsDirExist(strItunesPath.c_str()))
// 	{//已安装iTunes完整版
// 		CVersion iTunesVer = CVersion::GetFileVersion(strItunesPath.c_str());
// 		LOG->WriteDebug(L"取得iTunes版本：" + iTunesVer.ToStringW());
// 		CVersion minVer(7,5,0,20);
// 		if (iTunesVer.CompareTo(&minVer) < 0)//(CFileOperation::CompareFileVersion(&iTunesVer, &minVer) < 0)
// 		{//版本低于最小要求
// 			LOG->WriteDebug(L"版本低于最低要求：" + minVer.ToStringW());
// 			bNeed = true;
// 		}
// 	}
// 	else
// 	{//未安装iTunes完整版, 
// 		LOG->WriteDebug(L"未安装iTunes完整版,检查简化版信息");
// 		// 检查
// 		wstring strItunesVer = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, IS_SOFT_APPLE_MOBILE_DEVICE, IS_SOFT_APPLE_MOBILE_VERSION);
// 		LOG->WriteDebug(L"取得iTunes版本：" + strItunesVer);
// 		if (strItunesVer.length() > 0)
// 		{
// 			CVersion curVer(CCodeOperation::UnicodeToGB2312(strItunesVer));
// 			CVersion minVer(CCodeOperation::UnicodeToGB2312(APPLE_DEVICE_VERSION));
// 			if (curVer.CompareTo(&minVer) < 0)
// 			{
// 				LOG->WriteDebug(L"简化版Itunes小于最小版本要求：" + minVer.ToStringW());
// 				bNeed = true;
// 			}
// 		}
// 		else
// 		{// 没有找到值，说明没有安装
// 			LOG->WriteDebug(L"没有安装iTunes!");
// 			bNeed = true;
// 		}
// 	}
// #endif	
// 	return bNeed;
// }

unsigned long ND91Assistant::DriverFunction::GetDriverState( HARDWARE_TYPE nType, SP_DEVINFO_DATA device )
{
	//判断是否需要安装驱动
	unsigned long nState = 0;
#ifdef WIN32	
	ULONG ulState;
	if( CR_SUCCESS != CM_Get_DevNode_Status(&ulState, &(nState), device.DevInst, NULL))
		nState = -10;

	if (nType == PHONE_IPHONE)
	{// iPhone判断是否有安装iTunes
		LOG->WriteDebug(L"********** 检测到Ios设备，进入Ios环境检测******************************");
		if (CSystemInfo::IsNeedInstalliTunes())
			nState = -20;
		else
			nState = 0; // Ios设备只判断iTunes环境
		LOG->WriteDebug(L"Ios环境检测结果：" + CStrOperation::IntToWString(nState));
	}
    return nState;
	if (nType == PHONE_ANDROID && nState == 0)
	{// 通过Adb 判断是否需要安装驱动
		AdbGetDeviceCommand cmd;
		cmd.Execute();
		if (cmd.GetAllDeviceSize() == 0)
			nState = -11;
	}
#endif	
	return nState;
}

string ND91Assistant::DriverFunction::GetUSDVIDPID( const string strHardID )
{
    string strPID;
    int nIndex = (int)strHardID.find("PID_");
    if (nIndex > 0 && nIndex + 8 < (int)strHardID.length())
        strPID = strHardID.substr(0, nIndex + 8);
    return strPID;
}

#endif