#include "stdafx.h"

#include "Module/Server/GetIPhoneSellInfo.h"
#include "Core/DeviceData.h"
#include "Common/NetOperation.h"
#include "Common/rapidxml/rapidxml.hpp"
using namespace rapidxml;

#define     POST_KEY     "$#@!4321"
#define     GET_KEY      "!@#ASD12"

CGetIPhoneSellInfo::CGetIPhoneSellInfo(DeviceData* pDevice)
:CGetWebInfoBase(pDevice)
{
    _nIsRegistered      = -1;
    _nActivationStatus  = -1;
    _nPHHasCoverage     = -1;
    _nHWHasCoverage     = -1;
    _bSuccess           = false;
	
	//_bSuccess = GetDeviceSellInfo();
    _bSuccess = GetWarrantInfo();
}

CGetIPhoneSellInfo::~CGetIPhoneSellInfo()
{

}

bool CGetIPhoneSellInfo::GetWarrantInfo()
{
    string wHtml;
    int ret = RequestWarrantyFrom91Server(wHtml);
    if (ret == 0)
        Analytic91Html(wHtml);

    RETURN_TRUE_IF(ret == 0)
    ret = RequestWarrantyFromAppleServer(wHtml);
    if (ret == 0)
        AnalyticAppleHtml(wHtml);

    return ret == 0;
}

int CGetIPhoneSellInfo::RequestWarrantyFrom91Server(string& retWarrantyStr)
{
    time_t nowTime;
    ::time(&nowTime);

    string serial = CCodeOperation::UnicodeToUTF_8(_pDevice->GetDevInfo()->_strSerialNumber);
    string strContent = serial+"xml"+GET_KEY;
    string strHash = CStrOperation::GetMD5(strContent);

    string strUrl = "http://device.sj.91.com/controller.ashx?Action=getphonedevice&sn="+serial
                    +"&datatype=xml&checkcode="+strHash
                    +"&now="+CCodeOperation::UnicodeToUTF_8(CStrOperation::Int64ToWString(nowTime));
    
    int intResult = -1;
    try
    {
        retWarrantyStr = CNetOperation::Get(strUrl);
        if (retWarrantyStr.empty())
        {   // 重试1次
            Sleep(3000);
            retWarrantyStr = CNetOperation::Get(strUrl);
        }

        if (retWarrantyStr.empty())
            intResult = -2;
        else
            intResult = 0;
    }
    catch (...)
    {
        intResult = -3;
        retWarrantyStr = "";
    }

    return intResult;
}

/* 91服务器反馈信息模板
<?xml version="1.0"?>
<Result>
<SN><![CDATA[8781532RWH8]]></SN>  //设备序列号
<PurchCountry><![CDATA[]]></PurchCountry>///发售国家
<LastUnbrick><![CDATA[]]></LastUnbrick>//上次解锁
<IsRegistered>False</IsRegistered>//是否注册
<ActivationStatus>False</ActivationStatus>//激活状态
<PHEndDate><![CDATA[]]></PHEndDate>//电话服务结束时间
<PurchaseDate><![CDATA[]]></PurchaseDate>销售日期
<PHHasCoverage>False</PHHasCoverage>//是否有电话服务
<HWHasCoverage>False</HWHasCoverage>//是否硬件保修
<CovEndDate><![CDATA[]]></CovEndDate>//保修结束时间
<Hash><![CDATA[2E49FE6D4EDB6E9A81DE46A68826D79A]]></Hash> //服务端存储apple获取信息hash值
<Color><![CDATA[Black]]></Color>
</Result>
*/
void CGetIPhoneSellInfo::Analytic91Html(string htmlStr)
{
    string strName, strVal;

    xml_document<char> doc;
    doc.parse<0>((char*)htmlStr.c_str());

    rapidxml::xml_node<char>* root = doc.first_node();            //xml文件的根节点
    RETURN_IF (!root);

    for (rapidxml::xml_node<char>* node = root->first_node(); node!=NULL; node = node->next_sibling())
    {
        if ( !node )
            continue;

        strName = node->name();
        rapidxml::xml_node<char>* rNode = node->first_node();
        if ( !rNode )
            continue;

        strVal = rNode->value();

        if ( "SN" == strName )
            _strSn = CCodeOperation::UTF_8ToUnicode(strVal);
        else if ( "PurchCountry" == strName )
            _strPurchCountry = CCodeOperation::UTF_8ToUnicode(strVal);
        else if ( "LastUnbrick" == strName )
            _strLastUnbrick = CCodeOperation::UTF_8ToUnicode(strVal);
        else if ( "IsRegistered" == strName)
            _nIsRegistered = (CStrOperation::toLower(strVal) == "true" ? 1 : 0);
        else if ( "ActivationStatus" == strName )
            _nActivationStatus = (CStrOperation::toLower(strVal) == "true" ? 1 : 0);
        else if ( "PHEndDate" == strName)
            _strPHEndDate = CCodeOperation::UTF_8ToUnicode(strVal);
        else if ( "PurchaseDate" == strName)
            _strPurchaseDate = CCodeOperation::UTF_8ToUnicode(strVal);
        else if ( "PHHasCoverage" == strName)
            _nPHHasCoverage = (CStrOperation::toLower(strVal) == "true" ? 1 : 0);
        else if ( "HWHasCoverage" == strName)
            _nHWHasCoverage = (CStrOperation::toLower(strVal) == "true" ? 1 : 0);
        else if ( "CovEndDate" == strName)
            _strCovEndDate = CCodeOperation::UTF_8ToUnicode(strVal);
        else if ( "Hash" == strName)
            _strHash = CCodeOperation::UTF_8ToUnicode(strVal);
        else if ( "Color" == strName)
            _strColor = CCodeOperation::UTF_8ToUnicode(strVal);
    }
}

int CGetIPhoneSellInfo::RequestWarrantyFromAppleServer(string& retWarrantyStr)
{
    string serial = CCodeOperation::UnicodeToUTF_8(_pDevice->GetDevInfo()->_strSerialNumber);
    string strUrl = "https://selfsolve.apple.com/warrantyChecker.do?sn="+serial+"&cb=crossDomainAjax.success";

    int intResult = -1;
    try
    {
        retWarrantyStr = CNetOperation::Get(strUrl);
        if (retWarrantyStr.empty())
        {   // 重试1次
            Sleep(3000);
            retWarrantyStr = CNetOperation::Get(strUrl);
        }

        if (!retWarrantyStr.empty())
            intResult = 0;

        if (!CStrOperation::startsWith(retWarrantyStr, "crossDomainAjax.success"))
        {
            intResult = -2;
            retWarrantyStr = "";
        }
        else
        {
            //crossDomainAjax.success({"ERROR_CODE":"PD04","ERROR_KEY":"wc.check.err.usr.pd04.invalidserialnumber","ERROR_DESC":"We&#039;re sorry, but this serial number is not valid. Please check your information and try again."})
            if (retWarrantyStr.find("ERROR_CODE") != string::npos 
                && retWarrantyStr.find("ERROR_KEY") != string::npos)
            {
                intResult = -4;
                retWarrantyStr = "";
            }
        }
    }
    catch (...)
    {
        intResult = -3;
        retWarrantyStr = "";
    }

    return intResult;
}

void CGetIPhoneSellInfo::AnalyticAppleHtml(string htmlStr)
{
    wstring strData = CCodeOperation::UTF_8ToUnicode(htmlStr);

    int nIndex1 = strData.find(L"{");
    int nIndex2 = strData.find_last_of(L"}");
    if (nIndex2 > 0 && nIndex2 > 0)
        strData = strData.substr(nIndex1+1, nIndex2-nIndex1-1);
    vector<wstring> attrs = CStrOperation::parseStringsW(strData, L"\",\"");
    for (vector<wstring>::iterator it = attrs.begin(); it != attrs.end(); it++)
    {
        vector<wstring> data = CStrOperation::parseStringsW(*it, L"\":\"");
        if (data.size() != 2)
            continue;

        wstring strName = data.at(0);
        wstring strValue= data.at(1);
        if ( L"SN" == strName )
            _strSn = strValue;
        else if ( L"PURCH_COUNTRY" == strName )
            _strPurchCountry = strValue;
        else if ( L"LAST_UNBRICK_DT" == strName )
            _strLastUnbrick = strValue;
        else if ( L"IS_REGISTERED" == strName)
            _nIsRegistered = strValue == L"Y" ? 1 : 0;
        else if ( L"ACTIVATION_STATUS" == strName )
            _nActivationStatus = strValue == L"Y" ? 1 : 0;
        else if ( L"PH_END_DATE" == strName)
            _strPHEndDate = strValue;
        else if ( L"PURCHASE_DATE" == strName)
            _strPurchaseDate = strValue;
        else if ( L"PH_HAS_COVERAGE" == strName)
            _nPHHasCoverage = strValue == L"Y" ? 1 : 0;
        else if ( L"HW_HAS_COVERAGE" == strName)
            _nHWHasCoverage = L"Y" ? 1 : 0;
        else if ( L"COV_END_DATE" == strName)
            _strCovEndDate = strValue;
    }
}
/*
bool CGetIPhoneSellInfo::GetDeviceSellInfo()
{
	string strUrl = "https://selfsolve.apple.com/warrantyChecker.do?sn=" + CCodeOperation::UnicodeToUTF_8(_pDevice->GetDevInfo()->_strSerialNumber) + "&cb=crossDomainAjax.success";
	//string strUrl = "https://selfsolve.apple.com/warrantyChecker.do?sn=5K136VGMA4S&cb=crossDomainAjax.success";
	string strUTF8 = CNetOperation::Get(strUrl);
	
	if ( strUTF8.empty() )
	{// 重试1次
		Sleep(3000);
		strUTF8 = CNetOperation::Get(strUrl);
	}
	RETURN_FALSE_IF (strUTF8.empty());
	wstring strData = CCodeOperation::UTF_8ToUnicode(strUTF8);

	int nIndex1 = strData.find(L"{");
	int nIndex2 = strData.find_last_of(L"}");
	if (nIndex2 > 0 && nIndex2 > 0)
		strData = strData.substr(nIndex1+1, nIndex2-nIndex1-1);
	vector<wstring> attrs = CStrOperation::parseStringsW(strData, L"\",\"");
	for (vector<wstring>::iterator it = attrs.begin(); it != attrs.end(); it++)
	{
		vector<wstring> data = CStrOperation::parseStringsW(*it, L"\":\"");
		if (data.size() == 2)
		{
			wstring strName = data.at(0);
			wstring strValue= data.at(1);
			if (strName == L"IS_REGISTERED")    //注册状态
				_nRegedit = strValue == L"Y" ? 1 : 0;
			else if (strName == L"ACTIVATION_STATUS")   //激活状态
				_nRegedit = strValue == L"Y" ? 1 : 0;
			else if (strName == L"PURCH_COUNTRY")   //销售地区
				_strCountry = strValue;
			else if (strName == L"LAST_UNBRICK_DT") //激活日期
				_strActiveDate = strValue;
			else if (strName == L"PURCHASE_DATE")   //销售日期
				_strPurchaseDate = strValue;
			else if (strName == L"COV_END_DATE")    //保修日期
				_strWarrantyDate = strValue;
			else if (strName == L"HW_HAS_COVERAGE") //保修是否到期
				_nWarrantyOut = strValue == L"N" ? 1 : 0; 
		}
	}
	if (attrs.size() < 5)
        return false;
	return true;
}

std::wstring CGetIPhoneSellInfo::GetWarranty()
{
    if (_strWarrantyDate.length() > 0)
        return _strWarrantyDate;
    return _nWarrantyOut == -1 ? L"未知" : _nWarrantyOut == 0 ? L"未到期" : L"已到期";
}

bool CGetIPhoneSellInfo::LoadFrom91()
{
    string strPostKey = "$#@!4321";
    string strGetKey = "!@#ASD12";
    string strCheckCode = CStrOperation::GetMD5(CCodeOperation::UnicodeToUTF_8(_pDevice->GetDevInfo()->_strSerialNumber) + "xml" + strGetKey);
    string strUrl = "http://device.sj.91.com/controller.ashx?Action=getphonedevice&sn="
        + CCodeOperation::UnicodeToUTF_8(_pDevice->GetDevInfo()->_strSerialNumber)
        + "&datatype=xml&checkcode="
        + strCheckCode;

    string strUTF8 = CNetOperation::Get(strUrl);
    return true;
}*/