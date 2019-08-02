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

//��ȡVID
string DriverFunction::GetVID(const string strHardID)
{
	string strVID;
	int nIndex = (int)strHardID.find("VID_");
	if (nIndex > 0 && nIndex + 8 < (int)strHardID.length())
		strVID = strHardID.substr(nIndex + 4, 4);
	return strVID;
}
//��ȡPID
string DriverFunction::GetPID(const string strHardID)
{
	string strPID;
	int nIndex = (int)strHardID.find("PID_");
	if (nIndex > 0 && nIndex + 8 < (int)strHardID.length())
		strPID = strHardID.substr(nIndex + 4, 4);
	return strPID;
}
//��ȡMI
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
// �Ƶ�systeminfo
// bool ND91Assistant::DriverFunction::IsNeedInstalliTunes()
// {
// 	bool bNeed = false;
// #ifdef WIN32	
// 	//���iTunes.exe�汾
// 	wstring strItunesPath = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, IS_SOFT_ITUNES, IS_SOFT_ITUNES_PATH);
// 	//ĳЩ�汾iTunesʹ��Ĭ��ֵΪpathֵ
// 	if(strItunesPath.empty())
// 			strItunesPath = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, IS_SOFT_ITUNES, _T(""));
// 	LOG->WriteDebug(L"ȡ��iTunes�������ַ��" + strItunesPath);
// 	if (strItunesPath.length() > 0 && CFileOperation::IsDirExist(strItunesPath.c_str()))
// 	{//�Ѱ�װiTunes������
// 		CVersion iTunesVer = CVersion::GetFileVersion(strItunesPath.c_str());
// 		LOG->WriteDebug(L"ȡ��iTunes�汾��" + iTunesVer.ToStringW());
// 		CVersion minVer(7,5,0,20);
// 		if (iTunesVer.CompareTo(&minVer) < 0)//(CFileOperation::CompareFileVersion(&iTunesVer, &minVer) < 0)
// 		{//�汾������СҪ��
// 			LOG->WriteDebug(L"�汾�������Ҫ��" + minVer.ToStringW());
// 			bNeed = true;
// 		}
// 	}
// 	else
// 	{//δ��װiTunes������, 
// 		LOG->WriteDebug(L"δ��װiTunes������,���򻯰���Ϣ");
// 		// ���
// 		wstring strItunesVer = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, IS_SOFT_APPLE_MOBILE_DEVICE, IS_SOFT_APPLE_MOBILE_VERSION);
// 		LOG->WriteDebug(L"ȡ��iTunes�汾��" + strItunesVer);
// 		if (strItunesVer.length() > 0)
// 		{
// 			CVersion curVer(CCodeOperation::UnicodeToGB2312(strItunesVer));
// 			CVersion minVer(CCodeOperation::UnicodeToGB2312(APPLE_DEVICE_VERSION));
// 			if (curVer.CompareTo(&minVer) < 0)
// 			{
// 				LOG->WriteDebug(L"�򻯰�ItunesС����С�汾Ҫ��" + minVer.ToStringW());
// 				bNeed = true;
// 			}
// 		}
// 		else
// 		{// û���ҵ�ֵ��˵��û�а�װ
// 			LOG->WriteDebug(L"û�а�װiTunes!");
// 			bNeed = true;
// 		}
// 	}
// #endif	
// 	return bNeed;
// }

unsigned long ND91Assistant::DriverFunction::GetDriverState( HARDWARE_TYPE nType, SP_DEVINFO_DATA device )
{
	//�ж��Ƿ���Ҫ��װ����
	unsigned long nState = 0;
#ifdef WIN32	
	ULONG ulState;
	if( CR_SUCCESS != CM_Get_DevNode_Status(&ulState, &(nState), device.DevInst, NULL))
		nState = -10;

	if (nType == PHONE_IPHONE)
	{// iPhone�ж��Ƿ��а�װiTunes
		LOG->WriteDebug(L"********** ��⵽Ios�豸������Ios�������******************************");
		if (CSystemInfo::IsNeedInstalliTunes())
			nState = -20;
		else
			nState = 0; // Ios�豸ֻ�ж�iTunes����
		LOG->WriteDebug(L"Ios�����������" + CStrOperation::IntToWString(nState));
	}
    return nState;
	if (nType == PHONE_ANDROID && nState == 0)
	{// ͨ��Adb �ж��Ƿ���Ҫ��װ����
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