#include "StdAfx.h"
#include "core/DeviceData.h"
#include "Activator.h"
#include <iomanip>
#include "Device/ITunesMobileDevice.h"
#include "Device/CFConverter.h"
#include "core/DeviceInfo.h"
#include "Common/plist/PlistOperation.h"
#include "module/iPod/xmltoPlist.h"

using namespace std;

CActivator::CActivator(DeviceInfo*  pDevice)
{
	m_pDevice = pDevice;
}

CActivator::~CActivator(void)
{
}

bool CActivator::Activate()
{
	//准备请求数据
	tstring strActInfo;
	try
	{
	if (!DeviceGetActivateInfoXML(strActInfo))
		return false;
	}
	catch(...)
	{
		return false;
	}
	strActInfo = GetXMLSubString(strActInfo, _T("dict"));

	map<tstring, tstring> parameters;
	char szName[256] = {0};
	gethostname(szName, 256);

	AddKeyValue(_T("machineName"), CCodeOperation::s2ws(szName), parameters);
	AddKeyValue(_T("ECID"),CStrOperation::Int64ToWString(m_pDevice->_ios_deviceECID),parameters);
	AddKeyValue(_T("activation-info"), strActInfo, parameters);
	AddKeyValue(_T("InStoreActivation"), tstring(_T("false")), parameters);

	AddNotNullKeyValue(_T("ICCID"),m_pDevice->_ios_deviceICCID, parameters);

	AddKeyValue(_T("AppleSerialNumber"),m_pDevice->_strSerialNumber, parameters);
	AddNotNullKeyValue(_T("IMEI"),m_pDevice->_deviceIMEI, parameters);
	AddNotNullKeyValue(_T("IMSI"),m_pDevice->_deviceIMSI, parameters);
	AddNotNullKeyValue(_T("CTN"),m_pDevice->_ios_devicePhoneNumber, parameters);

	tstring strRsp = PostActivationReq(parameters);
	tstring strErr;
	CFDictionaryMAP pDictActivation;
	try
	{	
		ParseAppleResponse(strRsp, strErr,pDictActivation);
	}
	catch(...)
	{
		return false;
	}
	if (pDictActivation.size()==0 )
		return false;

	if(pDictActivation.find("activation-record")==pDictActivation.end() 
		&& pDictActivation.find("ack-received")!=pDictActivation.end())
	{
		return true;
	}
	else
		return DoActivation(pDictActivation["activation-record"]);
}

bool CActivator::DeviceGetActivateInfoXML( tstring &sInfo )
{

	CFTypeRef* pInfo=NULL;

	int ret = CITunesMobileDevice::iTunes_AMDeviceConnect(m_pDevice->_ios_devptr);
	if (0  != ret)
		return false;
	ret = CITunesMobileDevice::iTunes_AMDeviceStartSession(m_pDevice->_ios_devptr);
	if (0 == ret)
	{
		pInfo =  (CFTypeRef*)CCFConverter::GetDeviceData(m_pDevice->_ios_devptr,NULL,"ActivationInfo");
		CITunesMobileDevice::iTunes_AMDeviceStopSession(m_pDevice->_ios_devptr);
	}
	CITunesMobileDevice::iTunes_AMDeviceDisconnect(m_pDevice->_ios_devptr);

	if (pInfo)
	{
		CFDictionaryMAP InfoMap;
		bool ret = CCFConverter::GetIosData(pInfo,InfoMap);
		RETURN_FALSE_IF(!ret || InfoMap.size()==0);
		//卡非法等情况时里面有个"ActivationInfoErrors" key,值是empty iccid from commcenter和 empty imsi from CommCenter"; via xyq
 		if(InfoMap.find("ActivationInfoErrors")!=InfoMap.end())
 			return false;

		string xmldata = CCFConverter::ConvertPlist(pInfo,false);

		sInfo = CCodeOperation::s2ws(xmldata);
		CCFConverter::NDCFRelease(pInfo);
		return true;
	}
	return false;
}

void CActivator::AddNotNullKeyValue( LPCTSTR sKey, const tstring& sVal, map<tstring, tstring>& dict )
{
	if (sVal.length() == 0)
		return ;
	AddKeyValue(sKey, sVal, dict);
}

void CActivator::AddKeyValue( LPCTSTR sKey, const tstring& sVal, map<tstring, tstring>& dict )
{
	dict.insert(map<tstring,tstring>::value_type(sKey, sVal));
}

tstring CActivator::PostActivationReq(map<tstring, tstring>& params)
{
	LPCTSTR szAccepts[] = {_T("*.*"), NULL};
	TCHAR* pszUserAgent = _T("iTunes/10.6.1 (Windows; Microsoft Windows 7 Ultimate Edition Service Pack 1 (Build 7601)) AppleWebKit/534.54.16");
	TCHAR* pszHost   = _T("albert.apple.com");
	TCHAR* pszMethod = _T("POST");
	TCHAR* pszObject = _T("/deviceservices/deviceActivation");
	tstring strContentType = _T("Content-Type: multipart/form-data; boundary=") + this->GenBoundary();
	tstring strTimeZone = _T("X-Apple-Tz: ") + GetStringUTCOff();
	tstring szFront = _T("X-Apple-Store-Front: 143465-2,12");
	string strContent = DictToHttpReqString(params);
	tstring strRsp;
	BOOL bRet = false;
	HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
	hSession = InternetOpen(pszUserAgent, INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);
	if (NULL == hSession)
		goto endReq;
	hConnect = InternetConnect(hSession, pszHost, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (NULL == hConnect)
		goto endReq;
	hRequest = HttpOpenRequest(hConnect, pszMethod, pszObject, NULL, NULL,  szAccepts, INTERNET_FLAG_SECURE, NULL);
	if (NULL == hRequest)
		goto endReq;
	bRet = HttpAddRequestHeaders(hRequest, strContentType.c_str(), strContentType.length(), HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
	bRet = HttpAddRequestHeaders(hRequest, strTimeZone.c_str(), strTimeZone.length(), HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
	bRet = HttpAddRequestHeaders(hRequest, szFront.c_str(), szFront.length(), HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
	bRet = HttpSendRequest(hRequest, NULL, 0, (LPVOID*)strContent.c_str(), strContent.length());
	if (!bRet)
		goto endReq;

	strRsp = ReadHttpResponseString(hRequest);
endReq:
	if (NULL == hRequest)
		InternetCloseHandle(hRequest);
	if (NULL == hConnect)
		InternetCloseHandle(hConnect);
	if (hSession)
		InternetCloseHandle(hSession);

	return strRsp;
}

bool CActivator::Deactivate()
{
 	int ret = CITunesMobileDevice::iTunes_AMDeviceConnect(m_pDevice->_ios_devptr);
 	if (0  != ret)
 		return false;
 	ret = CITunesMobileDevice::iTunes_AMDeviceStartSession(m_pDevice->_ios_devptr);
 	if (0 == ret)
 	{
 		ret = (int)CITunesMobileDevice::iTunes_AMDeviceDeactivate((am_device *)m_pDevice->_ios_devptr);
 		CITunesMobileDevice::iTunes_AMDeviceStopSession(m_pDevice->_ios_devptr);
 	}
 	CITunesMobileDevice::iTunes_AMDeviceDisconnect(m_pDevice->_ios_devptr);
 	return (0 == ret);
}

void CActivator::ParseAppleResponse( tstring strRsp, tstring& strErr,CFDictionaryMAP &DicMap )
{
	void* pDictActivation = NULL;
	tstring strPlist = GetXMLSubString(strRsp, _T("plist"));

	if (strPlist.length())
	{
		string strUTF8 = CCodeOperation::UnicodeToUTF_8(strPlist);
		unsigned char * pBuffer = new unsigned char[strUTF8.length() + 1];
		memset(pBuffer, 0, strUTF8.length() + 1);
		memcpy(pBuffer, strUTF8.c_str(), strUTF8.length());
		void* data = CCFConverter::NDCFDataCreate(NULL, pBuffer, strUTF8.length());

		void* cfPropertyList = CCFConverter::NDCFPropertyListCreateWithData
			(data, kCFPropertyListImmutable);
		CCFConverter::NDCFRelease(data);
		RETURN_IF( ! cfPropertyList);

		
		CFDictionaryMAP DictMap;
		CCFConverter::GetIosData(cfPropertyList,DictMap);
		CFDictionaryMAP* pDict=&DictMap;

		if (pDict->size())
		{
			pDictActivation = DictMap["iphone-activation"];
			if (!pDictActivation)
				pDictActivation = DictMap["device-activation"];

			if (pDictActivation)
			{

				CCFConverter::GetIosData(pDictActivation,DicMap);
				return;
			}
		}
		else
		{
			int iBegin = strRsp.find(_T("<b>"));
			int iEnd = strRsp.rfind(_T("</b>"));
			if (tstring::npos != iEnd && tstring::npos != iBegin)
			{
				iBegin += 3;
				strErr = strRsp.substr(iBegin, iEnd-iBegin);
			}
		}
		
	}
	return;
}

bool CActivator::DoActivation( void* pActRecord )
{
	if (NULL == pActRecord)
		return false;
	int ret = 0;
	ret = CITunesMobileDevice::iTunes_AMDeviceConnect(m_pDevice->_ios_devptr);
	if (ret == 0)
	{
		ret = CITunesMobileDevice::iTunes_AMDeviceStartSession(m_pDevice->_ios_devptr);
		{
			ret = CITunesMobileDevice::iTunes_AMDeviceActivate((am_device *)m_pDevice->_ios_devptr, pActRecord);
			CITunesMobileDevice::iTunes_AMDeviceStopSession(m_pDevice->_ios_devptr);
		}
		CITunesMobileDevice::iTunes_AMDeviceDisconnect(m_pDevice->_ios_devptr);
	}
	return 0 == ret;
}

tstring CActivator::ReadHttpResponseString( HINTERNET hRequest )
{
	tstring ret;
	DWORD dwSize = 0;
	HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH, NULL, &dwSize, NULL);
	BYTE *pBuf = new BYTE[dwSize];
	HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH, pBuf, &dwSize, NULL);
	LONGLONG llLength = _ttoi64((LPCTSTR)pBuf);
	SAFE_DELETE_ARRAY(pBuf);
	pBuf = new BYTE[(int)llLength + 1];
	pBuf[llLength] = 0;
	DWORD dwR;
	if (InternetReadFile(hRequest, pBuf, (int)llLength, &dwR))
		ret = CCodeOperation::UTF_8ToUnicode((char*)pBuf,dwR);
	SAFE_DELETE_ARRAY(pBuf);
	return ret;
}

string CActivator::DictToHttpReqString( const std::map<tstring, tstring>& dict)
{
	ostringstream ossContent;
	for (map<tstring,tstring>::const_iterator it = dict.begin(); it != dict.end(); it++)
	{
		ossContent << CCodeOperation::UnicodeToUTF_8(GetBoundaryStr(false)) << "Content-Disposition: form-data; name=\"";
		ossContent << CCodeOperation::UnicodeToUTF_8(it->first) << "\"\r\n\r\n";
		ossContent << CCodeOperation::UnicodeToUTF_8(it->second) << "\r\n";
	}
	ossContent << CCodeOperation::UnicodeToUTF_8(GetBoundaryStr(true));
	return ossContent.str();
}

tstring CActivator::GetStringUTCOff()
{
	wostringstream ossTimeZone;
	ossTimeZone << std::fixed << setprecision(0) <<m_pDevice->_ios_timeZoneOffsetFromUTC;
	return ossTimeZone.str();
}

tstring CActivator::GetXMLSubString( const tstring& strOri, const tstring& strNode )
{
	tstring strHead = _T("<") + strNode ;
	tstring strFoot = _T("</") + strNode + _T(">");
	int iBegin = strOri.find(strHead);
	int iEnd = strOri.rfind(strFoot);
	if (iBegin == tstring::npos || iEnd == tstring::npos)
		return _T("");
	iEnd += strFoot.length();
	return strOri.substr(iBegin, iEnd - iBegin);
}

tstring CActivator::GenBoundary()
{
	srand(GetTickCount());
	m_llbound =  rand() * rand();
	wostringstream oss;
	oss << m_llbound;
	return oss.str();
}

tstring CActivator::GetBoundaryStr( bool bEnd )
{
	wostringstream oss;
	oss << _T("--") << m_llbound;
	if (bEnd)
		oss << _T("--\r\n");
	else
		oss << _T("\r\n");
	return oss.str();
}
