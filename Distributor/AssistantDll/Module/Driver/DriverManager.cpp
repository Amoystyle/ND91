#include "stdafx.h"

#ifdef MODULE_DRIVER

#include "Module/Driver/DriverManager.h"
#include "DeviceHardIDList.h"
#include "DriverFunction.h"
#include "Module/Driver/Driverdef.h"
#include "Module/Driver/SearchDriverInfo.h"
#include "HttpDriver.h"
#include "Common/SystemInfo.h"
#define DRIVER_AVOID_CLASS  ",CDROM,COMPUTER,DISKDRIVE,DISPLAY,FDC,HDC,HIDCLASS,KEYBOARD,LEGACYDRIVER,MEDIA,MONITOR,MOUSE,PROCESSOR,SCSIADAPTER,SYSTEM,VOLUME,LEGACYDRIVER"
#define DRIVER_KEYWORD_ADB			L"ADB"
#define DRIVER_KEYWORD_ANDROID		"ANDROID"

DriverManager* DriverManager::_pInstance = NULL;

#include <cfgmgr32.h> 
#pragma comment(lib, "cfgmgr32.lib")

DriverManager* DriverManager::GetInstance()
{
	if (_pInstance)
		return _pInstance;

	_pInstance = new DriverManager();

	return _pInstance;
}

void DriverManager::Release()
{
	SAFE_DELETE(DriverManager::_pInstance);
}

DriverManager::DriverManager()
{
    pDeviceHardIDList = new DeviceHardIDList();
}
 
DriverManager::~DriverManager()
{
	ReleaseDriverList();
    SAFE_DELETE(pDeviceHardIDList);
}

void DriverManager::ReleaseDriverList()
{
	for (vector<Driver*>::iterator it = _DriverList.begin(); it != _DriverList.end(); it++)
	{
		SAFE_DELETE(*it);
	}
	_DriverList.clear();
}

int ND91Assistant::DriverManager::ScanHardware(wstring wstrHardID)
{
	return ScanHardware(wstrHardID, L"", L"", L"");
}

int ND91Assistant::DriverManager::ScanHardware(wstring wstrHardID, wstring wstrVID, wstring wstrPID, wstring wstrMI)
{
	string strHardID = CCodeOperation::UnicodeToGB2312(CStrOperation::toUpperW(wstrHardID));
	string strVID = CCodeOperation::UnicodeToGB2312(CStrOperation::toUpperW(wstrVID));
	string strPID = CCodeOperation::UnicodeToGB2312(CStrOperation::toUpperW(wstrPID));
	string strMI  = CCodeOperation::UnicodeToGB2312(CStrOperation::toUpperW(wstrMI));
	bool bCheckHardID = strHardID.length() > 0;
	bool bCheckVID	  = strVID.length() > 0;

	ReleaseDriverList();

    //全局设备信息 
#ifdef WIN32    
    HDEVINFO hDevinfo;

    try
    {
        //获得所有设备信息
        hDevinfo=SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES|DIGCF_PRESENT);
        if(hDevinfo==NULL)
            return 0;
        SP_DEVINFO_DATA device;
        device.cbSize=sizeof(device);
    
        pDeviceHardIDList->_netSearch_HardID_List.clear();
        for(int i=0;SetupDiEnumDeviceInfo(hDevinfo,i,&device);i++)
        {
            string strHardIDTemp = CStrOperation::toUpper(DriverFunction::getProperty(hDevinfo, device, SPDRP_HARDWAREID));
            if ( strnicmp(strHardIDTemp.c_str(), "usb", 3) )    // 仅处理USB设备
            {
                continue;
            }
            if (bCheckHardID || bCheckVID)
			{
				if (bCheckHardID && strHardIDTemp != strHardID)
					continue;
				else
				{
					int n1 = strHardIDTemp.find(strVID);
					int n2 = strHardIDTemp.find(strPID);
					int n3 = strHardIDTemp.find(strMI);
					if (!(n1 > 0 && n2 > n1 && (n3 > n2 || strMI.length() == 0)))
						continue;
				}
			} 

            string strDriverDesc = DriverFunction::getProperty(hDevinfo, device, Ep_DEVICEDESC);
            HARDWARE_TYPE nType = GetHardwareType(	DriverFunction::getProperty(hDevinfo, device, Ep_MFG),
													strDriverDesc, strHardIDTemp);
            if ( nType == PHONE_ANDROIDTemp )
            {
                nType = PHONE_UNKNOWN;
                unsigned long nState = 0;
                ULONG ulState;
                if( CM_Get_DevNode_Status(&ulState, &(nState), device.DevInst, NULL))
                {
                    if (nState != 0)
                        nType = PHONE_ANDROID;
                }                   
            }

			if (nType == PHONE_UNKNOWN)
			{
				if (CStrOperation::toUpper(strDriverDesc) == DRIVER_KEYWORD_ANDROID)
					nType = PHONE_ANDROID;
			}			

            if (nType != PHONE_UNKNOWN) 
            {
				HardwareInfo* pHardwareInfo = DriverFunction::CreateHardwareInfo(nType, hDevinfo, device);
                Driver* pDriver = new Driver(pHardwareInfo);

				// 检查是否有同一设备
				bool bFound = false;
				for (vector<Driver*>::iterator it = _DriverList.begin(); it != _DriverList.end(); it++)
				{
					const HardwareInfo* pInfoBefore = (*it)->getHardwareInfo();

					// 重置驱动状态，HardID完全一致，代表完全一样的设备
					if (pHardwareInfo->_strHardID == pInfoBefore->_strHardID)
						(*it)->SetDriverState(pHardwareInfo->_nState);

					// 发现有同型设备，这里指同一个设备可能会产生的多个设备
					if (pInfoBefore->_strKey == pHardwareInfo->_strKey)
					{
						if ((int)(CStrOperation::toUpperW(pInfoBefore->_strHardID).find(DRIVER_KEYWORD_ADB)) > 0)
						{
							//LOG->WriteDebug(L"驱动检测：发现重复，丢弃当前" + pInfoBefore->_strHardID);
							bFound = true;
							break;
						}
						else if ((int)(CStrOperation::toUpperW(pHardwareInfo->_strHardID).find(DRIVER_KEYWORD_ADB)) > 0)
						{
							//LOG->WriteDebug(L"驱动检测：发现重复，丢弃原有" + pHardwareInfo->_strHardID);
							SAFE_DELETE(*it);
							_DriverList.erase(it);
							break;
						}
					}
				}

                if (!bFound && pDriver)
				{
					LOG->WriteDebug(L"驱动检测：发现设备" + pHardwareInfo->_strHardID);
					_DriverList.push_back(pDriver);
				}
				else if (pDriver)
				{
					SAFE_DELETE(pDriver);
				}
            }   // if (nType != PHONE_UNKNOWN) 

        } // for(int i=0;SetupDiEnumDeviceInfo(hDevinfo,i,&device);i++)

		// 释放
		SetupDiDestroyDeviceInfoList(hDevinfo);
    }
    catch(...)
    {
		//LOG->WriteError(L"驱动检测错误！");
    }
#endif    

    return _DriverList.size();
}



ND91Assistant::HARDWARE_TYPE ND91Assistant::DriverManager::GetHardwareType(string strClassName, string strDesc, string strHardID)
{
	string strNoAllowClass = DRIVER_AVOID_CLASS;
    strClassName    = CStrOperation::toUpper(strClassName);
    strDesc         = CStrOperation::toUpper(strDesc);
    strHardID       = CStrOperation::toUpper(strHardID);

    if (strHardID.length() == 0 || strClassName.length() > 0 && (int)strNoAllowClass.find(strClassName) >= 0)
        return PHONE_UNKNOWN;

	//LOG->WriteDebug(CCodeOperation::GB2312ToUnicode(strClassName + " | " + strHardID));
    	
	return GetHardwareType(strHardID);
}

HARDWARE_TYPE ND91Assistant::DriverManager::GetHardwareType(string strHardID)
{
	strHardID = CStrOperation::toUpper(strHardID);

	int nVID = CStrOperation::Hex2Int(DriverFunction::GetVID(strHardID));
    int nPID = CStrOperation::Hex2Int(DriverFunction::GetPID(strHardID));

    //判断是不是Android设备
    //特殊范围判断
    if (nVID == 0x0BB4 && (nPID >= 0x0C01 && nPID <= 0x0EFF) && (int)strHardID.find("&MI_01") > 0)
        return PHONE_ANDROID;

	vector<string>* pAndroid_HardID_List = pDeviceHardIDList->GetAndroid_HardID_List();
	for (vector<string>::iterator it = pAndroid_HardID_List->begin(); it != pAndroid_HardID_List->end(); it++)
    {
		int nIndex = (*it).find("&MI_");
		if (nIndex > 0 && nIndex + 6 <= (int)(*it).length() && (*it).find("USB\\VID_") == 0)
		{
			string strID = "USB\\VID_" + DriverFunction::GetVID((*it)) + "&PID_" + DriverFunction::GetPID((*it));
			string strMI =  (*it).substr(nIndex, 6);
			if (strHardID.find(strID) == 0 && (int)strHardID.find(strMI) > 0)
				return PHONE_ANDROID;
		}
		else
		{
			if (strHardID.find((*it)) == 0)	// 待确定
				return PHONE_ANDROID;
		}
    }

    //判断是不是iPhone设备
	vector<string>* pIos_HardID_List = pDeviceHardIDList->GetIos_HardID_List();
	for (vector<string>::iterator it = pIos_HardID_List->begin(); it != pIos_HardID_List->end(); it++)
	{
		if (strHardID.find((*it)) == 0)
			return PHONE_IPHONE;
	}

    // 通过网络判断是不是
    if ((int)strHardID.find("VID") > 0 && (int)strHardID.find("PID") > 0)
    {
        // 主设备处理
        string strParentHardID = DriverFunction::GetUSDVIDPID(strHardID);
        if (strParentHardID.length() > 0)
        {
            if (!pDeviceHardIDList->IsSearched(strParentHardID))
            {
                SearchDriverInfo search2(strParentHardID);
                if (search2.GetResult())
                    return PHONE_ANDROIDTemp;
                pDeviceHardIDList->_netSearch_HardID_List.push_back(strParentHardID);
            }
        }
        SearchDriverInfo search(strHardID);
        if (search.GetResult())
            return PHONE_ANDROIDTemp;
    }
    return PHONE_UNKNOWN;
	
}

bool ND91Assistant::DriverManager::IsIosDevice(wstring wstrVID, wstring wstrPID)
{
	string strVID = CCodeOperation::UnicodeToGB2312(wstrVID);
	string strPID = CCodeOperation::UnicodeToGB2312(wstrPID);
    return IsIosDevice(strVID, strPID);
}

bool ND91Assistant::DriverManager::IsIosDevice(string strVID, string strPID)
{
    strVID = CStrOperation::toUpper(strVID);
    strPID = CStrOperation::toUpper(strPID);
    // 这里调用不精确判断，如果不要，请去掉，使用后半部分的精确判断
    if ((int)strVID.find("05AC") >= 0 && (int)strPID.find("1") >= 0)
        return true;
    else
        return false;
    ////////////////////
    vector<string>* pIos_HardID_List = pDeviceHardIDList->GetIos_HardID_List();
    for (vector<string>::iterator it = pIos_HardID_List->begin(); it != pIos_HardID_List->end(); it++)
    {        
        if ((int)(*it).find(strVID) >= 0 && (int)(*it).find(strPID) >= 0)
            return true;
    }
    return false;
}

Driver* ND91Assistant::DriverManager::GetDriver( wstring wstrHardID )
{
	// 在已有的列表里查找
	ScanHardware(wstrHardID, L"", L"", L"");
	wstrHardID = CStrOperation::toUpperW(wstrHardID);
	for (vector<Driver*>::iterator it = _DriverList.begin(); it != _DriverList.end(); it++)
	{
		const HardwareInfo* pInfo = (*it)->getHardwareInfo();
		if (pInfo->_strHardID == wstrHardID)
			return *it;
	}
	return NULL;
}

Driver* ND91Assistant::DriverManager::GetDriver( wstring wstrVID, wstring wstrPID, wstring wstrMI/*=L""*/ )
{
	// 在已有的列表里查找
	ScanHardware(L"", wstrVID, wstrPID, wstrMI);
	wstrVID = CStrOperation::toUpperW(wstrVID);
	wstrPID = CStrOperation::toUpperW(wstrPID);
	wstrMI  = CStrOperation::toUpperW(wstrMI);
	for (vector<Driver*>::iterator it = _DriverList.begin(); it != _DriverList.end(); it++)
	{
		const HardwareInfo* pInfo = (*it)->getHardwareInfo();
		int n1 = pInfo->_strHardID.find(wstrVID);
		int n2 = pInfo->_strHardID.find(wstrPID);
		int n3 = pInfo->_strHardID.find(wstrMI);
		if (n1 > 0 && n2 > n1 && (n3 > n2 || wstrMI.length() == 0))
			return *it;
	}
	return NULL;
}

bool ND91Assistant::DriverManager::IsNeedInstalliTunes()
{
	return CSystemInfo::IsNeedInstalliTunes();
}

int ND91Assistant::DriverManager::HasDeviceLinkin()
{
    if (_DriverList.size() == 0)
        return 0;
    Driver* pDriver = _DriverList.at(0);
    if (IsIosDevice(pDriver->getHardwareInfo()->_strVID, pDriver->getHardwareInfo()->_strPID))
        return 1;
    return 2;
}

#endif

bool ND91Assistant::DriverManager::ScanForHardwareChange()
{    
    DEVINST devInst; 
    CONFIGRET status;
    //得到设备管理树的根结点 
    status = CM_Locate_DevNode(&devInst,NULL,CM_LOCATE_DEVNODE_NORMAL); 
    if (status != CR_SUCCESS) 
    { 
        return FALSE; 
    } 
    //刷新 
    status = CM_Reenumerate_DevNode(devInst,CM_REENUMERATE_ASYNCHRONOUS); //CM_REENUMERATE_ASYNCHRONOUS 异步方式可以立即响应
    if (status != CR_SUCCESS) 
    { 
        return FALSE; 
    }
    return true; 
}

wstring ND91Assistant::DriverManager::GetITunesUrl()
{
    return HttpDriver::GetITunesUrl();
}

bool ND91Assistant::DriverManager::InstallInf( wstring strInf )
{
    Driver driver((HardwareInfo*)NULL);
    return driver.InstallInf(strInf);
}

bool ND91Assistant::DriverManager::InstallExe( wstring strExe, wstring strParam/*=L""*/ )
{
    Driver driver((HardwareInfo*)NULL);
    return driver.InstallExe(strExe, strParam);
}

bool ND91Assistant::DriverManager::InstallMsi(wstring strMsi, wstring strParam)
{
    Driver driver((HardwareInfo*)NULL);
    return driver.InstallMsi(strMsi, strParam);
}