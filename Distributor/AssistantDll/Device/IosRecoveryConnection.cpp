#include "stdafx.h"

#include "IosRecoveryConnection.h"
#include "Device/ITunesMobileDevice.h"
#include "Device/CFConverter.h"

IosRecoveryConnection::IosRecoveryConnection(DeviceInfo* devInfo)
{
	m_devInfo = devInfo;
}

IosRecoveryConnection::~IosRecoveryConnection(void)
{
}

bool IosRecoveryConnection::Connect()
{
	if (!m_devInfo || !(m_devInfo->_ios_devptr))
		return false;

	void* ptr = CITunesMobileDevice::iTunes_AMRecoveryModeDeviceCopyIMEI(m_devInfo->_ios_devptr);
	if (ptr != NULL)
		m_devInfo->_deviceIMEI = GetDeviceValueAsString(ptr);

	ptr = CITunesMobileDevice::iTunes_AMRecoveryModeDeviceCopySerialNumber(m_devInfo->_ios_devptr);
	if (ptr != NULL)
		m_devInfo->_ios_deviceIdentifier = GetDeviceValueAsString(ptr);

	INT32 nType = CITunesMobileDevice::iTunes_AMRecoveryModeDeviceGetProductType(m_devInfo->_ios_devptr);
	m_devInfo->_deviceProductType = GetDevTypeString(nType);

	INT64 nECID = 0;
	memcpy(&nECID, ((char*)(m_devInfo->_ios_devptr))+88, 8);

	m_devInfo->_ios_bAFCConn			= true;
	m_devInfo->_ios_bJailBreaked		= false;
	m_devInfo->_ios_deviceActivated		= false;
	m_devInfo->_deviceName				= L"";
	m_devInfo->_deviceProductVersion	= L"";
	m_devInfo->_strSerialNumber			= L"";
	m_devInfo->_ios_daemonInstalled		= false;
	m_devInfo->_ios_deviceECID			= nECID;
    m_devInfo->_ios_isSimInserted       = false;
	m_devInfo->_deviceType = Ios;
		
	return true;
}

void IosRecoveryConnection::Disconnect()
{
	if (m_devInfo && m_devInfo->_ios_devptr)
	{
		CITunesMobileDevice::iTunes_AMDeviceRelease(m_devInfo->_ios_devptr);
		m_devInfo->_ios_devptr = NULL;
	}
}

wstring IosRecoveryConnection::GetDeviceValueAsString(void* cfstring)
{
	string s;
    CCFConverter::GetIosData(cfstring, s);
	return CCodeOperation::GB2312ToUnicode(s);
}   

wstring IosRecoveryConnection::GetDevTypeString(int type)
{
	wstring strResult = L"";

	switch (type)
	{
	case 0x2008930:
		strResult = L"iPad1,1"; // iPad wifi
		break;
    //case 0x6008930:       // 0x6009930是猜测的 by liuheqi
    case 0x4008940:         // 经过测试应该是这个
	    strResult = L"iPad2,1"; // iPad2 wifi
		break;
	case 0x6008940:
		strResult = L"iPad2,2"; // iPad2 wifi + 3G(w)
		break;
	case 0x6008950:       // 0x6009950是猜测的 by liuheqi
		strResult = L"iPad2,3"; // iPad2 wifi + 3G(c)
		break;
	case 0x8720:
		strResult = L"iPod2,1"; // Touch 2
		break;
	case 0x8730:          // 0x8730是猜测的 by liuheqi
		strResult = L"iPod3,1"; // Touch 3
		break;
	case 0x8008930:
		strResult = L"iPod4,1"; // Touch 4
		break;
	case 0x12801100:
		strResult = L"iPhone1,1"; // iphone
		break;
	case 0x12803100:
		strResult = L"iPhone1,2"; // iphone 3G
		break;
	case 0x8920:
		strResult = L"iPhone2,1"; // iphone 3Gs
		break;
	case 0x8930:
		strResult = L"iPhone3,1"; // iphone 4
		break;
	case 0x6008930:
	case 0x7008930:
		strResult = L"iPhone3,3"; // iPhone 4 cdma
		break;
	case 0x08008940:
		strResult = L"iPhone4,1";//iPhone4s
		break;
	case 0x00008945:
		strResult = L"iPad3,1";
		break;
	case 0x02008945:
		strResult = L"iPad3,2";
		break;
	case 0x4008945:
		strResult = L"iPad3,3";
		break;
	case 0x0008950:
		strResult = L"iPhone5,1";
		break;
	case 0x2008950:
		strResult = L"iPhone5,2";
		break;
	case 0x4008950:
		strResult = L"iPhone5,3";
		break;
	case 0x73c94b0:
		strResult = L"iPhone4,1";
			break;
	case 0xA008942:
		strResult = L"iPad2,5";
			break;
	default:
		strResult += type;
		break;
	}

	return strResult;
}

DeviceInfo* IosRecoveryConnection::GetDeviceInfo()
{
    return m_devInfo;
}

void IosRecoveryConnection::RebootToNormal()
{
	if (!m_devInfo || !(m_devInfo->_ios_devptr))
		return ;

	if (0 == CITunesMobileDevice::iTunes_AMRecoveryModeDeviceSetAutoBoot(m_devInfo->_ios_devptr, 1))
		CITunesMobileDevice::iTunes_AMRecoveryModeDeviceReboot(m_devInfo->_ios_devptr);
}
