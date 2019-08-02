#include "stdafx.h"

#include "Module/Server/GetDeviceImageInfo.h"

#include "Core/DeviceData.h"
#include "Common/NetOperation.h"
#include "Common/rapidxml/rapidxml.hpp"
using namespace rapidxml;

CGetDeviceImageInfo::CGetDeviceImageInfo(wstring strVID, wstring strMobiletype, 
									wstring strIMEI, wstring strNum)
{
	_VID		= strVID;
	_Mobiletype	= strMobiletype;
	_IMEI		= strIMEI;
	_Num		= strNum;

    _startX = 0;
    _startY = 0;
    _width  = 0;
    _height = 0;
    _url	   = L"";
}

CGetDeviceImageInfo::CGetDeviceImageInfo( DeviceData* pDevice )
{
    if (pDevice)
    { 
        if (pDevice->GetDevInfo()->_deviceType == Ios)
        {
            _VID = L"VID_05AC";
        }
        else if (pDevice->GetDevInfo()->_deviceType == Android)
        {
            _VID = L"VID_0BB4";
        }
        if (pDevice->GetDevInfo()->_deviceType == Ios)
        {
//             _Mobiletype = pDevice->GetDevInfo()->GetDevModelName() + pDevice->GetDevInfo()->_ios_deviceColor;                        
//             _Mobiletype = CStrOperation::toLowerW(_Mobiletype);
//             _Mobiletype = CStrOperation::ReplaceW(_Mobiletype, L"iphone4s", L"iphone 4s");
            //接口变更            
            wstring strColor = pDevice->GetDevInfo()->_ios_deviceColor;
            if (strColor.length() == 0)
                strColor = L"black";
            _Mobiletype = pDevice->GetDevInfo()->_deviceProductType + strColor;
        }
        else if (pDevice->GetDevInfo()->_deviceType == Android)
        {
            _Mobiletype = CCodeOperation::GB2312ToUnicode(CNetOperation::URLEncode(pDevice->GetDevInfo()->GetDevModelName()));
        }
        _IMEI = pDevice->GetDevInfo()->_deviceIMEI;
        _Num = L"";
    }
}

CGetDeviceImageInfo::~CGetDeviceImageInfo(void)
{
}

bool CGetDeviceImageInfo::GetDeviceImageInfo()
{
    unsigned char key[8] = { 0x14, 0x73, 0x12, 0x4D, 0x68, 0x59, 0xDD, 0x4C };
    wstring url = L"http://mobilesnap.sj.91.com/controller.ashx?action=getmobielsnap";

	RETURN_FALSE_IF ( _VID.empty() || _Mobiletype.empty());

	// 组合URL参数
    wstring tnetUrl = L"vid=" + _VID + L"&clientversion=3&hash=&mobiletype=" + _Mobiletype + L"&imei=" + _IMEI;

	if ( !_Num.empty() )
	{
		tnetUrl += L"&num=" + _Num;
	}

	string netUrl = CCodeOperation::UnicodeToUTF_8(tnetUrl);
	
	unsigned char ybuf[8] = {0};
	for ( int i=0;i<8;i++ )
	{
		ybuf[i] = MyStrOperation::Random(1,254);
	}
	string ystr((char*)ybuf, 8);

	unsigned char* data = new unsigned char[netUrl.length()];
	for (int n = 0; n < (int)netUrl.length();)
	{
		for (int m = 0; m < 8; m++,n++)
		{
			if ( n >= (int)netUrl.length() )
			{
				break;
			}
			data[n] = (netUrl[n] ^ ystr[m]);
		}
	}
	string strdata((char*)data, netUrl.length());
	SAFE_DELETE(data);

	// Des加密，得到加密体
	string keytData = CStrOperation::EncryptDes(ystr, key);
	string postData = keytData + strdata;

	// 调用Post加密体后获取数据
	string imginfo	= CNetOperation::Post(CCodeOperation::UnicodeToUTF_8(url), postData);

	// 解析数据
    return ParseInfo(imginfo);
}

bool CGetDeviceImageInfo::ParseInfo( string imginfo )
{
    xml_document<char> doc;
    doc.parse<0>((char*)imginfo.c_str());

    rapidxml::xml_node<char>* root = doc.first_node();            //xml文件的根节点
    if (!root)
        return false;
    for (rapidxml::xml_node<char>* node = root->first_node(); node!=NULL; node = node->next_sibling())
    {
        string strname	= node->name();
        rapidxml::xml_node<char>* valuenode = node->first_node();
        if (!valuenode)
            continue;
        string strVal = valuenode->value();

        if ( "ImageUrl" == strname )
        {
			_url = CCodeOperation::UTF_8ToUnicode(strVal);
        }
        else if ( "ScreenWidth" == strname )
        {
            _width = atoi(strVal.c_str());
        }
        else if ( "ScreenHeight" == strname )
        {
            _height = atoi(strVal.c_str());
        }
        else if ( "ScreenStartX" == strname )
        {
            _startX = atoi(strVal.c_str());
        }
        else if ( "ScreenStartY" == strname )
        {
            _startY = atoi(strVal.c_str());
        }
        else
            continue;
    }
    return true;
}
