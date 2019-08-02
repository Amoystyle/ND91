#include "stdafx.h"

#ifdef MODULE_DRIVER

#include "HttpDriver.h"
#include "Module/Driver/HardwareInfo.h"

#include "Common/SystemInfo.h"
#include "Common/w3c.h"

#define DRIVER_HOST			"http://drivers.sj.91.com"
#define URL_DEVICE_INFO		"/driversload/DeviceInfoPage.aspx"
#define URL_DEVICE_DOWN		"/driversload/DriverDownload.aspx"
#define URL_ITURNS_64		L"http://show.sj.91.com/Launch/Download.aspx?os=0&b=1&lan=0&sn=7"
#define URL_ITURNS_32		L"http://show.sj.91.com/Launch/Download.aspx?os=0&b=0&lan=0&sn=7"
#define URL_DEVICE_DRIVER	L"http://drivers.sj.91.com/driversload/DriverDownload.aspx?id=1&isX86="

HttpDriver::HttpDriver(const HardwareInfo* pHardware)
{
	_pHardware = pHardware;
#ifdef WIN32	
	_isX86 = !CSystemInfo::is64();
#endif		
}

HttpDriver::~HttpDriver()
{
}

string HttpDriver::Post(string postData)
{
	string strReturn;
#ifdef WIN32
	W3Client w3;
	try
	{
	
		if(w3.Connect(DRIVER_HOST))
		{
			w3.SetPostData(postData);
			if(w3.Request(URL_DEVICE_INFO, W3Client::reqPost))
			{
				char buf[1024] = "\0";
				char p1[10240] = {'\0'};
				int nSize = 0;
				int nIndex = 0;
				while((nSize = w3.Response(reinterpret_cast<unsigned char *>(buf), 1024)) > 0)
				{
					for (int i=0; i<nSize; i++)
					{
						if (nIndex < sizeof(p1))
							p1[nIndex++] = buf[i];
					}
				}
				strReturn.assign(p1, nIndex);
			}
		}
		w3.Close();
	}
	catch (...)
	{
	}
#endif	
	return strReturn;
}

std::wstring ND91Assistant::HttpDriver::GetDriverUrl()
{
	//TODO: Ios 直接返回简版iTurns，考虑该地址加入为OPTION配置或外部直接处理
	if (_pHardware->_nType == PHONE_IPHONE)
	{
		return GetITunesUrl();
	}

	wstring strUrl = L"";
	unsigned char key[8] = {0x44,0x73,0x12,0x4D,0x68,0x59,0xDD,0x4C};

	ostringstream ss;
	ss << "USB\\VID_" << _pHardware->_strVID << "&amp;PID_" << _pHardware->_strPID;
	string strID = ss.str();
	// strID = "USB\\VID_22B8&amp;PID_2D76";

	ostringstream oss;
	oss << "<Parameter><HardwareId>" << strID << "</HardwareId>\
	<Classguid></Classguid>\
	<CompatibleIDs></CompatibleIDs>\
	<DeviceDesc></DeviceDesc>\
	<FriendlyName></FriendlyName>\
	<Mfg></Mfg>\
	<LocationInformation></LocationInformation>\
	<IsX86>" << (_isX86 ? "true" : "false") << "</IsX86>\
	</Parameter>";

	string strData = CCodeOperation::GB2312ToUTF_8(oss.str());
    string strDes  = CStrOperation::EncryptDes(strData, key);

#ifdef _DEBUG
	CFileOperation::WriteBinFile(L"C:\\PostData.txt", strDes.c_str(), strDes.length());
#endif

	string strReturn = Post(strDes);
    strReturn = CCodeOperation::UTF_8ToGB2312(CStrOperation::EncryptDes(strReturn, key));
	int nReturn = strReturn.find("<ReturnCode>0</ReturnCode>");
	int n1 = strReturn.find("<DriverId>");
	int n2 = strReturn.find("</DriverId>");
	if (nReturn > 0)
	{
		string strID = "1";
		if (n1 > 0 && n2 > n1)
		{
			strID = strReturn.substr(n1 + 10, n2 - n1 - 10);
		}
		else
		{
			LOG->WriteAlert(CCodeOperation::StringToWstring("没有找到相应驱动"));//，以默认驱动尝试；" + CCodeOperation::GB2312ToUnicode(strID));
			return L"";
		}				
		ostringstream oss;				
		oss << DRIVER_HOST << URL_DEVICE_DOWN << "?id=" << strID << "&isX86=" << (_isX86 ? "true" : "false");
		strUrl = CCodeOperation::GB2312ToUnicode(oss.str());
		LOG->WriteAlert(CCodeOperation::StringToWstring("得到驱动地址：") + strUrl);
	}

	return strUrl;
}

std::wstring ND91Assistant::HttpDriver::GetDefaultAndroidDriverUrl()
{
	wstring strUrl = URL_DEVICE_DRIVER;
	strUrl += (_isX86 ? L"true" : L"false");
	return strUrl;
}

std::wstring ND91Assistant::HttpDriver::GetITunesUrl()
{
#ifdef WIN32	
	if (CSystemInfo::is64())
		return URL_ITURNS_64;
#endif		
	return URL_ITURNS_32;
}

#endif