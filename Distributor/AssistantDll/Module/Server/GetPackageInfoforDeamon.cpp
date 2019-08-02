#include "stdafx.h"

#include "Module/Server/GetPackageInfoforDeamon.h"

#include "Core/DeviceData.h"
#include "Common/NetOperation.h"

CGetPackageInfoforDeamon::CGetPackageInfoforDeamon(DeviceData* pDevice)
:CGetWebInfoBase(pDevice)
{
}

CGetPackageInfoforDeamon::~CGetPackageInfoforDeamon(void)
{

}

bool CGetPackageInfoforDeamon::GetPackageInfoFromWeb()
{
	wstring deviceMt, fwversion, version;
	RETURN_FALSE_IF ( !_pDevice );

	deviceMt	= GetDeviceMT();
	fwversion	= GetFwVersion();
	version		= GetVersion();

	wstring tnetUrl = L"http://sjupdate.sj.91.com/index.ashx?mt=" + deviceMt + L"&softid=999&fwversion=" + fwversion + L"&version=" + version + L"&RequestSource=91assiatant";

	string netUrl = CCodeOperation::UnicodeToUTF_8(tnetUrl);
	string xmlData	= CNetOperation::Get(netUrl);

	return ParseXml( xmlData );
}
