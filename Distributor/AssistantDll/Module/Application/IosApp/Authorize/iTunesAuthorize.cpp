#include "stdafx.h"
#include "iTunesAuthorize.h"
#include "rand.h"
#include "des.h"
#include "Device/IosPublicFunc.h"
#include "Device/IosUsbSync.h"

iTunesAuthorize::iTunesAuthorize()
{

}
int iTunesAuthorize::set_env()
{
	DWORD regValueSize = MAX_PATH;
	TCHAR appleApplicationSupportPath[MAX_PATH] = {0};
	TCHAR appleMobileDeviceSupportPath[MAX_PATH] = {0};
	DWORD regValueTpye = REG_SZ;
	HKEY regkey;
	int ret;
	int envSize = 1024;
	PTCHAR envPath;
	PTCHAR newEnvPath;

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Apple Inc.\\Apple Application Support"), 0, KEY_READ, &regkey) == ERROR_SUCCESS)
	{
		if(RegQueryValueEx(regkey, _T("InstallDir"), NULL, &regValueTpye, (LPBYTE)appleApplicationSupportPath, &regValueSize) != ERROR_SUCCESS)
		{
			RegCloseKey(regkey);
			return -1;
		}
		RegCloseKey(regkey);
	}
	else 
	{
		return -1;
	}

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Apple Inc.\\Apple Mobile Device Support"), 0, KEY_READ, &regkey) == ERROR_SUCCESS)
	{
		regValueSize = MAX_PATH;
		if(RegQueryValueEx(regkey, _T("InstallDir"), NULL, &regValueTpye, (LPBYTE)appleMobileDeviceSupportPath, &regValueSize) != ERROR_SUCCESS)
		{
			RegCloseKey(regkey);
			return -1;
		}
		RegCloseKey(regkey);
	}
	else 
	{
		return -1;
	}


	envPath = new TCHAR[envSize];
	memset(envPath, 0x00, envSize*sizeof(TCHAR));
	ret = GetEnvironmentVariable(TEXT("PATH"), envPath, envSize);
	if(ret > envSize)
	{
		delete [] envPath;
		envPath = new TCHAR[ret];
		ret = GetEnvironmentVariable(TEXT("PATH"), envPath, ret);
	}

	newEnvPath = new TCHAR[lstrlen(appleApplicationSupportPath) + lstrlen(appleMobileDeviceSupportPath) + ret + 256];

	lstrcpy(newEnvPath, appleApplicationSupportPath);
	lstrcat(newEnvPath, TEXT(";"));
	lstrcat(newEnvPath, appleMobileDeviceSupportPath);
	lstrcat(newEnvPath, TEXT(";"));

	if(ret > 0 && lstrlen(envPath) > 0)
	{
		lstrcat(newEnvPath, envPath);
	}

	SetEnvironmentVariable(TEXT("PATH"), newEnvPath);

	delete [] envPath;
	delete [] newEnvPath;

	return 0;
}
long iTunesAuthorize::IsAssistance()
{
	return (long)ND_IsAssistance;
}
int iTunesAuthorize::Authorize(/*struct am_device *iPodDev,*/bool authorizationFromServer, string icInfoPath, 
                               vector<ND_UInt32> serverNumbers, ND_UInt64* ids, int count, /*char* szuuid, ND_UInt32 iOSVersion, ND_UInt32 afcConn*/const DeviceInfo* pDevInfoPtr)
{
    struct am_device* iPodDev = (struct am_device*)pDevInfoPtr->_ios_devptr;
    string deviceIdentifier = CCodeOperation::UnicodeToUTF_8(pDevInfoPtr->_ios_deviceIdentifier);
    char* szuuid = (char*)deviceIdentifier.c_str();
    ND_UInt32 iOSVersion = IosPublicFunc::GetVerNum(pDevInfoPtr->_deviceProductVersion);
    ND_UInt32 afcConn = (ND_UInt32)pDevInfoPtr->_ios_afc1Handle;

    LOG_DEBUG(L"set_env()");
 	set_env();
 	unsigned char securityData[HELPER_SECURITY_LENGTH];
 	setSecurityBytes(securityData);
 
    LOG_DEBUG(L"set TransUserSetting");
 	TransUserSetting us;
 	memset(&us, 0x00, sizeof(TransUserSetting));
 	us.m_iPodDev = (struct am_device *)iPodDev;
 	us.m_iTimeout = 50000;
 	us.m_iDevStatus = 0xffffffff;
 	us.m_szUUID[0] = 0;
 	memcpy(us.m_szUUID,szuuid,strlen(szuuid));
 	us.m_bSCInfoFromServer = (BOOL)authorizationFromServer;
 	strcpy(us.m_icinfoPath,icInfoPath.c_str());
    us.m_iOSVersion = iOSVersion;
    us.m_AFCConn = afcConn;
 
 	if (us.m_bSCInfoFromServer)
 	{
        LOG_DEBUG(L"set m_serverNumber");
 		if (serverNumbers.size()!=12)
 			return -1;
 
 		for (int i = 0; i < 6; i++)
 		{			
 			memcpy(us.m_serverNumber1+i,&serverNumbers.at(i),sizeof(ND_UInt32));
 			memcpy(us.m_serverNumber2+i,&serverNumbers.at(i+6),sizeof(ND_UInt32));
 		}
 	}
 
 	us.m_serverAppCount = count;
 	us.m_serverApplicationDSID = ids;

    IosUsbSync usbSync(pDevInfoPtr);
    usbSync.StartSync();

    int ret = -9101;
    try
    {
        TransUserSetting *tus = setTransUserSetting(&us, securityData);
        RETURN_VALUE_IF(tus == NULL, ret);

        TransUserSetting *tustmp =(TransUserSetting *)(((unsigned char *)tus) + 64);
        //us.m_iDevStatus = 0xffffffff;
        ret = ND_Assistance((TransUserSetting *)(((unsigned char *)tus) + 64));
        clearTransUserSetting(tus);
    }
    catch (...)
    {
    }
 
    usbSync.StopSync();
 	return ret;

}
int iTunesAuthorize::setSecurityBytes(unsigned char *data)
{
	DES_cblock key;	
	DES_cblock ivec = {0xA5, 0x8C, 0x71, 0x5A, 0xB5, 0x06, 0xA4, 0x27};
	DES_key_schedule keysched;	
	unsigned char randBytes[HELPER_ENCRYPT_BYTES_LEN];
	unsigned char encryptBytes[HELPER_ENCRYPT_BYTES_LEN];
	DES_cblock seed;
	time_t currentTime;

	if(data == NULL)
		return -1;

	// 随机种子
	srand((unsigned)time(NULL));
#ifdef WIN32
	_snprintf_s((char *)seed, sizeof(DES_cblock), _TRUNCATE, "%X%X", rand(),rand());
#else
	snprintf((char *)seed, sizeof(DES_cblock), "%X%X", rand(),rand());
#endif
	RAND_seed(seed, sizeof(DES_cblock));

	//随机密钥
	DES_random_key(&key);
	DES_set_odd_parity(&key);
	DES_set_key_unchecked(&key, &keysched);

	//随机校验字串
	memset(randBytes, 0x00, sizeof(randBytes));
	memset(encryptBytes, 0x00, sizeof(encryptBytes));
	RAND_bytes(randBytes, sizeof(randBytes));

	currentTime = time(NULL); //1970到现在的秒数,4个字节,使用这个时间比较,data在10秒内使用有效
	memcpy(randBytes+(sizeof(randBytes)/2), &currentTime, sizeof(time_t)); //4个字节替换为当期时间

	//随机校验字串加密 cbc模式
	DES_ncbc_encrypt(randBytes, encryptBytes, HELPER_ENCRYPT_BYTES_LEN, &keysched, &ivec, DES_ENCRYPT); //ivec使用后会改变

	//组合加密二进制串
	//结构为:|-- 随机校验字串前半 --|-- 随机密钥后半 --|-- 加密校验字串后半 --|-- 随机密钥前半 --|-- 加密校验字串前半 --|-- 随机校验字串后半 --|
	memcpy(data, randBytes, sizeof(randBytes)/2);
	memcpy(data+(sizeof(randBytes)/2), key+(sizeof(DES_cblock)/2), sizeof(DES_cblock)/2);
	memcpy(data+(sizeof(randBytes)/2)+(sizeof(DES_cblock)/2), encryptBytes+(sizeof(encryptBytes)/2), sizeof(encryptBytes)/2);
	memcpy(data+(sizeof(randBytes)/2)+(sizeof(DES_cblock)/2)+(sizeof(encryptBytes)/2), key, sizeof(DES_cblock)/2);
	memcpy(data+(sizeof(randBytes)/2)+(sizeof(DES_cblock)/2)+(sizeof(encryptBytes)/2)+(sizeof(DES_cblock)/2), encryptBytes, sizeof(encryptBytes)/2);
	memcpy(data+(sizeof(randBytes)/2)+(sizeof(DES_cblock)/2)+(sizeof(encryptBytes)/2)+(sizeof(DES_cblock)/2)+sizeof(encryptBytes)/2, randBytes+(sizeof(randBytes)/2), sizeof(randBytes)/2);

	return 0;
}