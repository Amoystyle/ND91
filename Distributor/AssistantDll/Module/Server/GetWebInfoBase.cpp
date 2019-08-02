#include "stdafx.h"

#include "Module/Server/GetWebInfoBase.h"
#include "Core/DeviceData.h"
#include "Device/IosPublicFunc.h"
#include "Common/NDBase64.h"
#include "Common/rapidxml/rapidxml.hpp"
using namespace rapidxml;

CGetWebInfoBase::CGetWebInfoBase()
{

}

CGetWebInfoBase::CGetWebInfoBase(DeviceData* pDevice)
{
	_pDevice = pDevice;

	_nFileSize	= -1;	
	_nResult	= -1;	
	_nNeedForce	= -1;

}

CGetWebInfoBase::~CGetWebInfoBase(void)
{

}

wstring CGetWebInfoBase::GetDeviceMT()
{
	if ( L"iPhone" == _pDevice->GetDevInfo()->_ios_deviceClass )
		return L"1";
	else if (L"iPad" == _pDevice->GetDevInfo()->_ios_deviceClass)
		return L"7";
	else if (Android == _pDevice->GetDevInfo()->_deviceType)
		return L"4";
	else 
		return L"0";
}

wstring CGetWebInfoBase::GetVersion()
{
	wstring version;
	int nDaemonVer = IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_daemonVer)/100;
	switch(nDaemonVer)
	{
	case 1:
		version = L"1.0";break;
	case 2:
		version = L"2.0";break;
	case 3:
		version = L"3.0";break;
	case 4:
		version = L"4.0";break;
	default:
		version = L"0.0";break;
	}
	return version;
}

wstring CGetWebInfoBase::GetFwVersion()
{
	wstring fwversion;
	int nDeviceVer = IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion)/100;
	switch(nDeviceVer)
	{
	case 1:
		fwversion = L"1.x";break;
	case 2:
		fwversion = L"2.x";break;
	case 3:
		fwversion = L"3.x";break;
	case 4:
		fwversion = L"4.x";break;
	case 5:
		fwversion = L"5.x";break;
	default:
		fwversion = L"0.x";break;
	}
	return fwversion;
}

bool CGetWebInfoBase::ParseXml( string xmlData )
{
	string strName, strVal;

	xml_document<char> doc;
	doc.parse<0>((char*)xmlData.c_str());

	rapidxml::xml_node<char>* root = doc.first_node();            //xml文件的根节点
	RETURN_FALSE_IF (!root);

	for (rapidxml::xml_node<char>* node = root->first_node(); node!=NULL; node = node->next_sibling())
	{
		if ( !node )
			continue;

		string name = node->name();
		if ( "resultState" == name )
		{
			rapidxml::xml_node<char>* rNode = node->first_node();
			RETURN_FALSE_IF (!rNode);

			strVal = rNode->value();
			_nResult = atoi(strVal.c_str());

		}
		else if ( "data" == name )
		{
			for (rapidxml::xml_node<char>* listnode = node->first_node(); listnode!=NULL; listnode = listnode->next_sibling())
			{
				strName = listnode->name();
				strVal  = listnode->value();

				if ( "version" == strName )
				{
					_strVersion = CCodeOperation::UTF_8ToUnicode(strVal);
				}
				else if ( "size" == strName )
				{
					_nFileSize = atoi(strVal.c_str());
				}
				else if ( "updatetime" == strName )
				{
					_updateTime = CDealTime::StringToTime(CCodeOperation::UTF_8ToUnicode(strVal));
				}
				else if ( "updateAtOnce" == strName)
				{
					_nNeedForce = atoi(strVal.c_str());
				}
				else if ( "filelist" == strName )
				{
					rapidxml::xml_node<char>* xnode = listnode->first_node();
					RETURN_FALSE_IF (!xnode);

					strVal  = xnode->value();
					_strUrl = CCodeOperation::UTF_8ToUnicode(strVal);
				}
				else if ( "content" == strName)
				{
					_content = CCodeOperation::UTF_8ToUnicode(strVal);
				}
				else
					continue;
			}
		}
		else
			continue;
	}
	return true;
}

string CGetWebInfoBase::GetPhoneInfo()
{
	RETURN_VALUE_IF (!_pDevice, "");
	string data, firmwareVersion, imei;

	firmwareVersion = CCodeOperation::UnicodeToUTF_8(_pDevice->GetDevInfo()->_deviceProductVersion);

	imei = CCodeOperation::UnicodeToUTF_8(_pDevice->GetDevInfo()->_deviceIMEI);
	if (imei.empty())
		imei = "000000000000000";

	if ( _pDevice->GetDevInfo()->_deviceType == Ios )
	{	
		if ( _pDevice->GetDevInfo()->IsPad() )
		{
			firmwareVersion = "ipad" + firmwareVersion;
		}

		data = GetPhoneData(firmwareVersion, imei, "iphone");
	}
	else if ( _pDevice->GetDevInfo()->_deviceType == Android )
	{
		data = GetPhoneData(firmwareVersion, imei, "android" );
	}
	else
		return "";

	return data;
}

string CGetWebInfoBase::GetSendInfo(int deviceType)
{
	string strType = deviceType == 1 ? "iphone" : "android";

	string data = GetPhoneData("", "", strType);

	return data;
}


string CGetWebInfoBase::EncodeInfo( string phoneData )
{
    //和网站约定的密钥，网站收到数据时候会验证数据的正确性
    string strKeys[10] = { "5a@4a$0e","$3%#&1dc","a^l!92(z","e(~jv|6+","|<)@1tz=",
        "o{u#*a@2","&~6gmd*g","r$n[w(8b","]#sk9(b%","*7~u1}t|" };

    int num = MyStrOperation::Random(0, 9);
    string strKey = strKeys[num];

    //---   加密(base64 + 前后倒置 + 替换特殊字符)   ---

    // base64加密
    int newLen = CNDBase64::Base64encode_len(phoneData.length());
    char* buffer = new char[newLen];
    memset(buffer, 0, newLen);

    CNDBase64::Base64encode(buffer, phoneData.c_str(), phoneData.length());
    string strData(buffer);

    // 字符串前后倒置, 如str = ABCD 转换为 str=CDAB
    if ( strData.length() > 2 )
    {
        strData = strData.substr( strData.length() / 2 ) + strData.substr(0, strData.length() / 2);
    }

    strData = CStrOperation::Replace( strData, "+", "!" );
    strData = CStrOperation::Replace( strData, "/", "@" );
    strData = CStrOperation::Replace( strData, "=", "$" );

    //得到md5加密后的key,这个key是网站为了验证发送过去的phone信息是不是正确的
    string strMd5 = CStrOperation::GetMD5( strKey + strData );
	SAFE_DELETE(buffer);

    string sData = "i=" + strData + "&ks=" + CStrOperation::Int2string(num) + "&k=" + strMd5;

    return sData;

}

string CGetWebInfoBase::GetPhoneData( string phoneVersion, string phoneSerial, string platForm )
{
    //// 手机的信息：

    //===旧的接口 
    //utpversion="1.0.0"				    // Web接口的升级文件版本，便于你们软件升级时下面参数格式不一样的时候用 
    //cmd="checkupdate"				    // 软件检查更新的标志
    //source="iphonepcsuite";			// 来源的标志位，默认为"iphonepcsuite"
    //sourceversion="";				    // 来源的版本号，即当前PC Suite软件的版本号
    //phone="iphone"					    // 默认就好了，表示iphone
    //iccid=""                           // 来源的唯一ID，即ICCID
    //fwversion=""					    // 固件版本，like 1.0.1
    //return="none"					    // 返回值，默认不返回、直接显示HTML页面，xml：返回XML
    //=====================================================================================================


    //=== 现在的接口
    //src                                // 来源的标志位，默认为"iphonepcsuite" ====
    //srcver                             // 来源的版本号，即当前PC Suite软件的版本号
    //phone                              // 默认就好了，表示iphone
    //psys                               // 操作系统，对iPhone来说目前都一样，留空即可
    //psysver    `       				// 固件版本，like 2.1
    //uniqueid           				// 来源的的唯一ID，即SN/IMEI
    //==================================================================================


    //Dim phoneData As String = String.Format("src={0}&srcver={1}&phone={2}&psys={3}&psysver={4}&uniqueid={5}&resolution={6}&timestamp={7}", _
    //                                        platForm & "pcsuite", My.Application.Info.Version.ToString(4), platForm, "", phoneVersion, phoneSerial, "320x480", System.Web.HttpUtility.HtmlEncode(DateTime.Now.ToString()))
    //因为未设置程序版本号， 会影响程序管理的使用，所以写死 3.0.0.1

    char szDaytime[20] = {0};
    string strDayTime;
    time_t t = time(NULL);

    CDealTime dt(t);
    sprintf(szDaytime,"%d-%d-%d %d:%d:%d",
        dt.GetYear(),
        dt.GetMonth(),
        dt.GetDay(),
        8+dt.GetHour(),
        dt.GetMinute(),
        dt.GetSecond());
    strDayTime = szDaytime;

    string phoneData = "src=" + platForm + "pcsuite"
        + "&srcver=" + "3.0.0.1"
        + "&phone=" + platForm
        + "&psys=" + ""
        + "&psysver=" + phoneVersion
        + "&uniqueid=" + phoneSerial
        + "&resolution=" + "320x480"
        + "&timestamp=" + CStrOperation::HtmlEncode(strDayTime);

    phoneData = EncodeInfo(phoneData);

    return phoneData;
}
