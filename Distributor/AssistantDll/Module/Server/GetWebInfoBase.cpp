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

	rapidxml::xml_node<char>* root = doc.first_node();            //xml�ļ��ĸ��ڵ�
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
    //����վԼ������Կ����վ�յ�����ʱ�����֤���ݵ���ȷ��
    string strKeys[10] = { "5a@4a$0e","$3%#&1dc","a^l!92(z","e(~jv|6+","|<)@1tz=",
        "o{u#*a@2","&~6gmd*g","r$n[w(8b","]#sk9(b%","*7~u1}t|" };

    int num = MyStrOperation::Random(0, 9);
    string strKey = strKeys[num];

    //---   ����(base64 + ǰ���� + �滻�����ַ�)   ---

    // base64����
    int newLen = CNDBase64::Base64encode_len(phoneData.length());
    char* buffer = new char[newLen];
    memset(buffer, 0, newLen);

    CNDBase64::Base64encode(buffer, phoneData.c_str(), phoneData.length());
    string strData(buffer);

    // �ַ���ǰ����, ��str = ABCD ת��Ϊ str=CDAB
    if ( strData.length() > 2 )
    {
        strData = strData.substr( strData.length() / 2 ) + strData.substr(0, strData.length() / 2);
    }

    strData = CStrOperation::Replace( strData, "+", "!" );
    strData = CStrOperation::Replace( strData, "/", "@" );
    strData = CStrOperation::Replace( strData, "=", "$" );

    //�õ�md5���ܺ��key,���key����վΪ����֤���͹�ȥ��phone��Ϣ�ǲ�����ȷ��
    string strMd5 = CStrOperation::GetMD5( strKey + strData );
	SAFE_DELETE(buffer);

    string sData = "i=" + strData + "&ks=" + CStrOperation::Int2string(num) + "&k=" + strMd5;

    return sData;

}

string CGetWebInfoBase::GetPhoneData( string phoneVersion, string phoneSerial, string platForm )
{
    //// �ֻ�����Ϣ��

    //===�ɵĽӿ� 
    //utpversion="1.0.0"				    // Web�ӿڵ������ļ��汾�����������������ʱ���������ʽ��һ����ʱ���� 
    //cmd="checkupdate"				    // ��������µı�־
    //source="iphonepcsuite";			// ��Դ�ı�־λ��Ĭ��Ϊ"iphonepcsuite"
    //sourceversion="";				    // ��Դ�İ汾�ţ�����ǰPC Suite����İ汾��
    //phone="iphone"					    // Ĭ�Ͼͺ��ˣ���ʾiphone
    //iccid=""                           // ��Դ��ΨһID����ICCID
    //fwversion=""					    // �̼��汾��like 1.0.1
    //return="none"					    // ����ֵ��Ĭ�ϲ����ء�ֱ����ʾHTMLҳ�棬xml������XML
    //=====================================================================================================


    //=== ���ڵĽӿ�
    //src                                // ��Դ�ı�־λ��Ĭ��Ϊ"iphonepcsuite" ====
    //srcver                             // ��Դ�İ汾�ţ�����ǰPC Suite����İ汾��
    //phone                              // Ĭ�Ͼͺ��ˣ���ʾiphone
    //psys                               // ����ϵͳ����iPhone��˵Ŀǰ��һ�������ռ���
    //psysver    `       				// �̼��汾��like 2.1
    //uniqueid           				// ��Դ�ĵ�ΨһID����SN/IMEI
    //==================================================================================


    //Dim phoneData As String = String.Format("src={0}&srcver={1}&phone={2}&psys={3}&psysver={4}&uniqueid={5}&resolution={6}&timestamp={7}", _
    //                                        platForm & "pcsuite", My.Application.Info.Version.ToString(4), platForm, "", phoneVersion, phoneSerial, "320x480", System.Web.HttpUtility.HtmlEncode(DateTime.Now.ToString()))
    //��Ϊδ���ó���汾�ţ� ��Ӱ���������ʹ�ã�����д�� 3.0.0.1

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
