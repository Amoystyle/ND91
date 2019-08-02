#include "stdafx.h"

#include "Module/Server/GetPackageInfoforIos.h"

#include "Core/DeviceData.h"
#include "Common/NetOperation.h"

CGetPackageInfoforIos::CGetPackageInfoforIos(DeviceData* pDevice)
:CGetWebInfoBase(pDevice)
{
}

CGetPackageInfoforIos::~CGetPackageInfoforIos(void)
{

}

bool CGetPackageInfoforIos::GetPackageInfoforIos(wstring verType)
{
	wstring deviceMt, version, fwversion;

	RETURN_FALSE_IF ( !_pDevice || Ios != _pDevice->GetDevInfo()->_deviceType );

	deviceMt	= GetDeviceMT();
	fwversion	= GetFwVersion();
	version		= GetVersion();	

	wstring tnetUrl = L"http://sjupdate.sj.91.com/user.ashx?act=5&mt=" + deviceMt + L"&versionName=" + version + \
						L"&RequestSource=91assiatant" + L"&fwversion=" + fwversion + L"&versionType=" + verType + L"&Divide=0";

	string netUrl = CCodeOperation::UnicodeToUTF_8(tnetUrl);
	string xmlData	= CNetOperation::Get(netUrl);

	return ParseXml( xmlData );
}
