#include "stdafx.h"

#include "Core/DeviceData.h"
#include "Module/Server/GetPackageInfoforAndroid.h"
#include "Common/NetOperation.h"

CGetPackageInfoforAndroid::CGetPackageInfoforAndroid(DeviceData* pDevice)
:CGetWebInfoBase(pDevice)
{
}

CGetPackageInfoforAndroid::~CGetPackageInfoforAndroid(void)
{

}

bool CGetPackageInfoforAndroid::GetPackageInfoforAndroid()
{
	wstring deviceMt, version, fwversion;

	RETURN_FALSE_IF ( !_pDevice || Android != _pDevice->GetDevInfo()->_deviceType );

	deviceMt	= GetDeviceMT();
	fwversion	= GetFwVersion();
	version		= GetVersion();

	wstring tnetUrl = L"http://sjupdate.sj.91.com/user.ashx?act=5&mt=" + deviceMt + L"&versionName=" + version + \
					    L"&versionType=pxl" + L"&RequestSource=91assiatant" + L"&fwversion=" + fwversion;

	string netUrl = CCodeOperation::UnicodeToUTF_8(tnetUrl);
	string xmlData	= CNetOperation::Get(netUrl);

	return ParseXml( xmlData );
}
