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
    Web(unsigned short	version,				// Э��汾��������1
		const CUSTOMER_DATA*  pdata = NULL		// �û�����
		);
    ~Web();

    // Summary  : ��ȡ�ͻ�����
    // Returns  : UBDGLIB_RESPONSE
    UBDGLIB_RESPONSE GetCustomerInfo();

	// Summary  : ������ȡ��װ����Ϣ
	// Returns  : UBDGLIB_RESPONSE
	UBDGLIB_RESPONSE GetPackageInfos(int userid, int nLegal, int nPlatform, bool bIsCpa=false);

	// Summary  : ��ȡ������װ�����������б���Ϣ
	// Returns  : UBDGLIB_RESPONSE
	UBDGLIB_RESPONSE GetAppInfosByPackage(long nPackageID, int nLegal, int nPlatform);

	// Summary  : ��ȡ�ͻ�����
	// Returns  : UserData*
	UserData* GetCustomerInfoC();

	// Summary  : ������ȡ��װ����Ϣ(C++���ýӿ�)
	// Returns  : vector<PackageData*>
	vector<PackageData*> GetPackageInfosC(int userid, int nLegal, int nPlatform, bool bIsCpa=false);

	// Summary  : ��ȡ������װ�����������б���Ϣ(C++���ýӿ�)
	// Returns  : vector<SoftData*>
	vector<SoftData*> GetAppInfosByPackageC(long nPackageID, int nLegal, int nPlatform);

	// Summary  : �ϱ��ͻ�����
	// Returns  : int
	int PostCustomerData();

	// Summary  : �ϱ�Ӧ�����ؼ�¼
	// Returns  : int
	int PostAppDownloadRecord(const APPDOWNLOAD_RECORD* pData);

	// Summary  : �ϱ�Ӧ�ð�װ��¼
	// Returns  : int
	int PostAppInstallRecord(const APPINSTALL_RECORD* pData);

	// Summary  : �ϱ�91װ�����ְ�װ��
	// Returns  : int
	int PostInstallSum(const INSTALL_RECORD* pData);

private:

    PostData*				_pPost;
    char*					_userInfo;  // ��ȡ�����ռ����򷵻ص�����(JSON��ʽ)
	const CUSTOMER_DATA*	_pdata;		// �û�����
};
