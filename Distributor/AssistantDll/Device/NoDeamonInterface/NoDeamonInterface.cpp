#include "stdafx.h"

#include "NoDeamonInterface.h"

#include "Device/iPhoneInterfaceDef.h"
#include "Device/NoDeamonInterface/ActionCallback/itunesInterface.h"
#include "iphoneInterface.h"

#include "Module/Application/IosApp/Authorize/des.h"
#include "Module/Application/IosApp/Authorize/rand.h"

#include "Common/SystemInfo.h"

wstring NoDeamonInterface::backDir = CFileOperation::GetCurrWorkingDir()+L"backfile";
wstring NoDeamonInterface::deamonDir = CFileOperation::GetCurrWorkingDir()+L"daemon";

void NoDeamonInterface::Uninitialize()
{
    return iphoneinterface_uninitialize();
}

bool NoDeamonInterface::Initialize()
{
    wstring appleApplicationSupportPath = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, DLLCoreFoundationReg, _T("InstallDir"));
    wstring appleMobileDeviceSupportPath = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, DLLiTunesMobileDeviceReg, _T("InstallDir"));
    if ( *appleApplicationSupportPath.rbegin() == '\\' ) appleApplicationSupportPath.erase(appleApplicationSupportPath.size() - 1);
    if ( *appleMobileDeviceSupportPath.rbegin() == '\\' ) appleMobileDeviceSupportPath.erase(appleMobileDeviceSupportPath.size() - 1);
    LOG->WriteDebug(L"iphoneinterface_initialize Begin");
    LOG->WriteDebug(L"appleApplicationSupportPath = " + appleApplicationSupportPath);
    LOG->WriteDebug(L"appleMobileDeviceSupportPath = " + appleMobileDeviceSupportPath);
    wstring strLog;
    OPTION->Get(GlobalOption::STR_91HELPERDEAMON_LOG, strLog);
	
	int res =-1;
	if (!CSystemInfo::IsNeedInstalliTunes(_T("10.5.0.0")))
	{	
		//增加判断版本号>10.05才去初始化
		res = iphoneinterface_initialize((wchar_t*)appleMobileDeviceSupportPath.c_str(), 
                                         (wchar_t*)appleApplicationSupportPath.c_str(), 
                                         (wchar_t*)(strLog.c_str()));
	}

    LOG->WriteDebug(L"iphoneinterface_initialize Finished");
    return res == 0;
}

#define HELPER_ENCRYPT_BYTES_LEN			56 //必须为8的倍数
#define HELPER_SECURITY_LENGTH				(sizeof(DES_cblock) + (HELPER_ENCRYPT_BYTES_LEN * 2))
int setHelperSecurityBytes(unsigned char *data)
{
    DES_cblock key;	
    DES_cblock ivec = {0x61, 0xD2, 0x93, 0xDA, 0x37, 0x8C, 0xC5, 0xA6};
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

pIphoneInterfaceType GetIphoneInterfaceType(void* devicePointer, DWORD iosIp,wstring backDir=L"",wstring deamonDir=L"")
{
    unsigned char securityData[HELPER_SECURITY_LENGTH];
    setHelperSecurityBytes(securityData);

	wstring lbackDir,ldeamonDir;

	if(backDir.length() == 0)
		lbackDir = CFileOperation::GetCurrWorkingDir()+L"backfile";
	else
		lbackDir = backDir;
	if(deamonDir.length() == 0)
		ldeamonDir = CFileOperation::GetCurrWorkingDir()+L"daemon";
	else
		ldeamonDir = deamonDir;

    pIphoneInterfaceType iType = NULL;
    if (iosIp == 0)
        iType = setIphoneInterfaceTypeUsb(securityData, (struct am_device *)devicePointer, (wchar_t*)ldeamonDir.c_str(), (wchar_t*)lbackDir.c_str());
    else
        iType = setIphoneInterfaceTypeWifi(securityData, iosIp);

    return iType;
}

int NoDeamonInterface::Screenshot(void* devicePointer, BYTE** pngBytesPtr, UINT* pngLength, DWORD iosIp)
{
    return iphoneinterface_screenshot(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir), pngBytesPtr, pngLength);
}

void NoDeamonInterface::Delete(void* buffer)
{
    return iphoneinterface_delete(buffer);
}

int NoDeamonInterface::Reload(void* devicePointer, DWORD iosIp)
{
    return iphoneinterface_reload(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir));
}

int NoDeamonInterface::Reboot(void* devicePointer, DWORD iosIp)
{
    return iphoneinterface_reboot(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir));
}

int NoDeamonInterface::Shutdown(void* devicePointer, DWORD iosIp)
{
    return iphoneinterface_shutdown(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir));
}

int NoDeamonInterface::IconsRepair(void* devicePointer, DWORD iosIp)
{
    return iphoneinterface_iconsRepair(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir));
}

int NoDeamonInterface::GetImsi(void* devicePointer, PTCHAR* imsi, DWORD iosIp)
{
    return iphoneinterface_getImsi(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir), imsi);
}

int NoDeamonInterface::GetType(void* devicePointer, PTCHAR* type, DWORD iosIp)
{
    return iphoneinterface_getType(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir), type);
}

int NoDeamonInterface::Kill(void* devicePointer, PTCHAR processName, DWORD iosIp)
{
    return iphoneinterface_kill(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir), processName);
}

void NoDeamonInterface::SetPath(wstring lbackupfile,wstring ldeamonDir)
{
	backDir = lbackupfile;
	deamonDir = ldeamonDir;
}
extern "C" int pxlCallback(CFDictionaryRef dict);
int NoDeamonInterface::PxlInstall(void* devicePointer, PTCHAR remotePxlFile, DWORD iosIp)
{
    return iphoneinterface_pxlInstall(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir),
                                      remotePxlFile/*,
                                      true,
                                      pxlCallback*/);
}

int NoDeamonInterface::PxlUninstall(void* devicePointer, PTCHAR CFBundleIdentifier, DWORD iosIp)
{
    return iphoneinterface_pxlUninstall(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir),
                                        CFBundleIdentifier/*,
                                        pxlCallback*/);
}

int NoDeamonInterface::Install91HelperService(void* devicePointer, DWORD iosIp)
{
    return iphoneinterface_install91HelperService(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir));
}

int NoDeamonInterface::RunProgram(void* devicePointer, const PTCHAR program, long waitSeconds, int* status, DWORD iosIp)
{
    return iphoneinterface_runProgram(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir), 
                                      program, 
                                      waitSeconds, 
                                      status);
}

int NoDeamonInterface::SpecialRunProgram(void* devicePointer, const PTCHAR program, long waitSeconds, int* status, DWORD iosIp)
{
    return iphoneinterface_special_runProgram(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir), 
                                              program, 
                                              waitSeconds, 
                                              status);
}

extern "C" int chmodChownCallback(CFDictionaryRef dict);
int NoDeamonInterface::Chmod(void* devicePointer, PTCHAR remoteFile, PTCHAR mode, bool recursive, DWORD iosIp)
{
    return iphoneinterface_chmod(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir), 
                                 remoteFile, 
                                 mode, 
                                 recursive/*, 
                                 chmodChownCallback*/);
}

int NoDeamonInterface::Chown(void* devicePointer, PTCHAR remoteFile, PTCHAR uid, PTCHAR gid, bool recursive, DWORD iosIp)
{
	if(!devicePointer || !remoteFile || !uid || !gid )
		return 0;
    return iphoneinterface_chown(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir), 
                                 remoteFile, 
                                 _wtoi(uid), 
                                 _wtoi(gid), 
                                 recursive/*, 
                                 chmodChownCallback*/);
}

int NoDeamonInterface::SendSmsSingle(void* devicePointer, PTCHAR aTelephoneNumber, PTCHAR content, DWORD iosIp)
{
    return iphoneinterface_sendSmsSingle(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir), 
                                         aTelephoneNumber, 
                                         content);
}

int NoDeamonInterface::SmsRepair(void* devicePointer, DWORD iosIp)
{
    return iphoneinterface_smsRepair(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir));
}

int NoDeamonInterface::ChangeCallRingtone(void* devicePointer, DWORD iosIp)
{
    return iphoneinterface_changeCallRingtone(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir));
}

int NoDeamonInterface::SetSmsFlag(void* devicePointer, PTCHAR smsid, PTCHAR flag, DWORD iosIp)
{
    return iphoneinterface_setSmsFlag(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir), smsid, flag);
}

int NoDeamonInterface::GetFileInfo(void* devicePointer, PTCHAR remoteFile, 
                                   INT64 *size, UINT16 *mode, UINT32 *uid, UINT32 *gid, LONG *mtime, LONG *birthtime, DWORD iosIp)
{
    return iphoneinterface_getFileInfo(GetIphoneInterfaceType(devicePointer, iosIp,backDir,deamonDir), 
                                       remoteFile, 
                                       size, mode, uid, gid, mtime, birthtime);
}
