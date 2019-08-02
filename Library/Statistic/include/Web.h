#pragma once

#include "UbdgLib.h"
#include "RevertData.h"
#include <vector>

using namespace std;

class PostData;
class ReturnData;
class UBDGLIB_API Web
{
public:
    Web(unsigned short	version,				// 协议版本，现在是1
		const CUSTOMER_DATA*  pdata = NULL		// 用户数据
		);
    ~Web();

    // Summary  : 获取客户资料
    // Returns  : UBDGLIB_RESPONSE
    UBDGLIB_RESPONSE GetCustomerInfo();

	// Summary  : 批量获取安装包信息
	// Returns  : UBDGLIB_RESPONSE
	UBDGLIB_RESPONSE GetPackageInfos(int userid, int nLegal, int nPlatform, bool bIsCpa=false);

	// Summary  : 获取单个安装包里面的软件列表信息
	// Returns  : UBDGLIB_RESPONSE
	UBDGLIB_RESPONSE GetAppInfosByPackage(long nPackageID, int nLegal, int nPlatform);

	// Summary  : 获取客户资料
	// Returns  : UserData*
	UserData* GetCustomerInfoC();

	// Summary  : 批量获取安装包信息(C++调用接口)
	// Returns  : vector<PackageData*>
	vector<PackageData*> GetPackageInfosC(int userid, int nLegal, int nPlatform, bool bIsCpa=false);

	// Summary  : 获取单个安装包里面的软件列表信息(C++调用接口)
	// Returns  : vector<SoftData*>
	vector<SoftData*> GetAppInfosByPackageC(long nPackageID, int nLegal, int nPlatform);

	// Summary  : 上报客户资料
	// Returns  : int
	int PostCustomerData();

	// Summary  : 上报应用下载记录
	// Returns  : int
	int PostAppDownloadRecord(const APPDOWNLOAD_RECORD* pData);

	// Summary  : 上报应用安装记录
	// Returns  : int
	int PostAppInstallRecord(const APPINSTALL_RECORD* pData);

	// Summary  : 上报91装机助手安装量
	// Returns  : int
	int PostInstallSum(const INSTALL_RECORD* pData);

private:

    PostData*				_pPost;
    char*					_userInfo;  // 获取数据收集规则返回的数据(JSON格式)
	const CUSTOMER_DATA*	_pdata;		// 用户数据
};
