#pragma once

#include "Module/IManager.h"

namespace ND91Assistant
{
    class DeviceData;

	//网络数据获取管理类
	class CGetAppUpdateInfo;			
	class CGetDeviceImageInfo;		
	class CGetIPhoneSellInfo;			
	class CGetPackageInfoforAndroid;	
	class CGetPackageInfoforDeamon;	
	class CGetPackageInfoforIos;		
	class CLASSINDLL_CLASS_DECL ServerManager : public IManager
	{
	public:
		ServerManager(const DeviceData* pDevice);
		~ServerManager();

		CGetAppUpdateInfo* GetAppUpdateInfoManager();

		CGetDeviceImageInfo* GetDeviceImageInfoManager();

		CGetIPhoneSellInfo* GetIPhoneSellInfoManager();

		CGetPackageInfoforAndroid* GetPackageInfoforAndroidManager();

		CGetPackageInfoforDeamon* GetPackageInfoforDeamonManager();

		CGetPackageInfoforIos* GetPackageInfoforIosManager();

	private:

		CGetAppUpdateInfo*			_pGetAppUpdateInfo;
		CGetDeviceImageInfo*		_pGetDeviceImageInfo;
		CGetIPhoneSellInfo*			_pGetIPhoneSellInfo;
		CGetPackageInfoforAndroid*	_pGetPackageInfoforAndroid;
		CGetPackageInfoforDeamon*	_pGetPackageInfoforDeamon;
		CGetPackageInfoforIos*		_pGetPackageInfoforIos;

	};
}

