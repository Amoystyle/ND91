#include "pch_module.h"

//#if defined( MODULE_APP )

#include "Module/Application/AppData.h"
#include "Module/Application/GetAppIconCommand.h"
#include "Module/Application/GetAppPermissionsCommand.h"
#include "Module/Application/IosApp/IosIPAAppOperation.h"

AppData::AppData()
{
	_strAppPCPath = L"";		// ��װ��·�� (PC��) 
	_bLockApp = false;				// ��װʱ�Ƿ�Ҫ����App
	_bRecover = false;				// �Ƿ񸲸�ԭ����
	_bOnSDCard = false;			// �Ƿ�װ��SDCard
	_bKeepData = false;			// �Ƿ����û����� (ж��ʱʹ��)

	_packageName = L"";		// ��װ���� ISO��ʾ�����Ψһ��ʶ
	_nVersionCode = -1;      // android�ڲ��汾�ţ����������                 
	_strVersion = L"";		// �汾����                   
	_AppName = L"";			// Ӧ�ó�����             
	_Description = L"";		// ����
	_Url = L"";				// ���ӵ�ַ
	_strAppPhonePath = L"";	// ����·�� (�ֻ���)              

	_strIconPhonePath = L"";	// ͼ��·�� (�ֻ���)
	_strIconPCPath = L"";		// ͼ��·�� (PC��)
	_eAppType = APP_TYPE_UNKNOW;			// ������ࣨpxl,ipa,installer,panda,deb��
	_eAppOwner = APP_OWNER_UNKNOW;			// �������ͣ�ϵͳ���û���
	_bShare = false;			// �ļ��Ƿ���
	_strState = L"";          // ����״̬

	_strPurchaser = L""; //������
    _applicationDSID = 0;

	_DebEssential = L"";
	_DebDepends = L"";
	_minimumOSVersion = L"";

    _dynamicDiskUsage = 0;      // �ĵ���С
    _staticDiskUsage = 0;       // �����С
	_isLegalSoft=0;
	_itemId=0;

    _is91IPA = false;

	_isEnterpriseApp = false;
}

AppData::AppData(const AppData* pAppData)
{
	_strAppPCPath = pAppData->_strAppPCPath;		// ��װ��·�� (PC��) 
	_bLockApp = pAppData->_bLockApp;				// ��װʱ�Ƿ�Ҫ����App
	_bRecover = pAppData->_bRecover;				// �Ƿ񸲸�ԭ����
	_bOnSDCard = pAppData->_bOnSDCard;			// �Ƿ�װ��SDCard
	_bKeepData = pAppData->_bKeepData;			// �Ƿ����û����� (ж��ʱʹ��)

	_packageName = pAppData->_packageName;		// ��װ���� ISO��ʾ�����Ψһ��ʶ
	_nVersionCode = pAppData->_nVersionCode;      // android�ڲ��汾�ţ����������                 
	_strVersion = pAppData->_strVersion;		// �汾����                   
	_AppName = pAppData->_AppName;			// Ӧ�ó�����             
	_Description = pAppData->_Description;		// ����
	_Url = pAppData->_Url;				// ���ӵ�ַ
	_strAppPhonePath = pAppData->_strAppPhonePath;	// ����·�� (�ֻ���)              

	_strIconPhonePath = pAppData->_strIconPhonePath;	// ͼ��·�� (�ֻ���)
	_strIconPCPath = pAppData->_strIconPCPath;		// ͼ��·�� (PC��)
	_eAppType = pAppData->_eAppType;			// ������ࣨpxl,ipa,installer,panda,deb��
	_eAppOwner = pAppData->_eAppOwner;			// �������ͣ�ϵͳ���û���
	_bShare = pAppData->_bShare;			// �ļ��Ƿ���
	_strState = pAppData->_strState;          // ����״̬

	_DebEssential = pAppData->_DebEssential;
	_DebDepends = pAppData->_DebDepends;

	_strPurchaser = pAppData->_strPurchaser;

	_PermissionList = pAppData->_PermissionList;

    _dynamicDiskUsage = pAppData->_dynamicDiskUsage;
    _staticDiskUsage = pAppData->_staticDiskUsage;
	_isLegalSoft = pAppData->_isLegalSoft;
    _is91IPA = pAppData->_is91IPA;
	_minimumOSVersion = pAppData->_minimumOSVersion;
	_itemId=pAppData->_itemId;

    _strBundleExecutableName = pAppData->_strBundleExecutableName;
    _strIconName = pAppData->_strIconName;

	_isEnterpriseApp = pAppData->_isEnterpriseApp;
    _DeviceFamily.insert(_DeviceFamily.begin(), pAppData->_DeviceFamily.begin(), pAppData->_DeviceFamily.end());
}

AppData::~AppData()
{
	//�������ǳ���_PermissionList�ĵ�ַ��0����������ʱ������,��ʱ����Ҳ�������ֱ��ע��
	/*int p = (int)&_PermissionList;
	if(p!=0)
		_PermissionList.clear();*/
}

bool AppData::GetAppPurchaser(const DeviceData* pDeviceData)
{
    RETURN_FALSE_IF(NULL == pDeviceData || pDeviceData->GetDevInfo()->_deviceType == Android || _eAppType != APP_TYPE_IPA);
    IosIPAAppOperation operation(pDeviceData, NullProgressSubject::Instance());
    return operation.GetAppPurchaser(this);
}

bool AppData::GetAppIconPhonePath(const DeviceData* pDeviceData)
{
    RETURN_FALSE_IF(NULL == pDeviceData || pDeviceData->GetDevInfo()->_deviceType == Android || _eAppType != APP_TYPE_IPA);
    IosIPAAppOperation operation(pDeviceData, NullProgressSubject::Instance());
    return operation.GetAppIconPhonePath(this);
}

bool AppData::LoadAppIcon(const DeviceData* pDeviceData, wstring strPCPath)
{
#ifdef MODULE_APP
	RETURN_FALSE_IF( ! pDeviceData);

	if (pDeviceData->GetDevInfo()->_deviceType == Android)
	{
		GetAppIconCommand cmd(pDeviceData, _packageName, strPCPath);
		cmd.Execute();
		return cmd.Success();
	}	
	else
	{
        GetAppIconPhonePath(pDeviceData);
		RETURN_FALSE_IF(_strIconPhonePath.length() <= 0);

		AFCFileSysemBase* pFileStream = NULL;
		if (pDeviceData->GetDevInfo()->_ios_bJailBreaked)//(devInfo->_ios_bJailBreaked)
			pFileStream = new AFCFileSysemBase(pDeviceData->GetDevInfo());
		else
			pFileStream =  AFCAppFileSystem::StartAPPHouseArrestService(_packageName, (DeviceInfo*)pDeviceData->GetDevInfo());

		bool bRet = false;
		if (pFileStream)
		{
			TransferResult result = pFileStream->Ios_DownFromPhone(_strIconPhonePath, strPCPath);

			if (result == Transfer_Success)
           {          	
#ifdef WIN32
				IosPngConver::ConverApplePngToNormal(strPCPath.c_str(), strPCPath.c_str());
#endif
                bRet = true;
           }
			delete pFileStream;
		}

		return bRet;
	}
#else
    return false;
#endif
}

bool AppData::LoadPermissions(const DeviceData* pDeviceData)
{
#ifdef MODULE_APP
	GetAppPermissionsCommand cmd(pDeviceData, _packageName);
	cmd.Execute();

	if (cmd.Success())
	{
		vector<wstring>* pList = cmd.GetPerminssionList();
		_PermissionList.clear();
		_PermissionList.insert(_PermissionList.begin(), pList->begin(), pList->end());
	}
	return cmd.Success();
#else
    return false;
#endif
}

bool AppData::IsUserApp()
{
	return _eAppOwner == APP_OWNER_USER;
}

#pragma region Android ���
size_t AppData::ParseAndroidData(const char* pData, size_t nSize)
{
    BinaryBufferReader reader(pData, nSize);
    reader.ReadStringFromUTF8(_packageName);
    reader.ReadInt32(_nVersionCode);
    reader.ReadStringFromUTF8(_strVersion);
	reader.ReadStringFromUTF8(_strAppPhonePath);
    reader.ReadStringFromUTF8(_AppName);
	bool isSystem;
	reader.ReadBool(isSystem);
	_eAppOwner = isSystem?APP_OWNER_SYS:APP_OWNER_USER;
    reader.ReadInt64(_staticDiskUsage);

    wstring temp = CStrOperation::toLowerW(_strAppPhonePath);
    if (!CStrOperation::startsWith(temp, L"/system/app")
        && !CStrOperation::startsWith(temp, L"/data/app/")
        && !CStrOperation::startsWith(temp, L"/data/app-private")
        && !CStrOperation::startsWith(temp, L"/opl/apps"))
        _bOnSDCard = true;

    return reader.GetOffset();
}
#pragma endregion

bool ND91Assistant::AppData::ParseAndroidAdbData( string strLine)
{
    _eAppType = APP_TYPE_APK;

	string strTop = "package:";
	int n1 = strLine.find(strTop);
	int n2 = strLine.find_last_of("=");
	if (n1 == 0 && n2 > n1)
	{
		_strAppPhonePath = CCodeOperation::UTF_8ToUnicode(strLine.substr(n1 + strTop.length(), n2 - (n1 + strTop.length())));
        wstring temp = CStrOperation::toLowerW(_strAppPhonePath);
        if (!CStrOperation::startsWith(temp, L"/system/app")
            && !CStrOperation::startsWith(temp, L"/data/app/")
            && !CStrOperation::startsWith(temp, L"/data/app-private")
            && !CStrOperation::startsWith(temp, L"/opl/apps"))
            _bOnSDCard = true;

		_packageName = CCodeOperation::UTF_8ToUnicode(strLine.substr(n2 + 1));
	}
	else
		return false;

// 	if (_strAppPhonePath.find(L"/system/") == 0)
// 		_eAppOwner = APP_OWNER_SYS;
// 	else
// 		_eAppOwner = APP_OWNER_USER;    
	return true;
}

std::wstring ND91Assistant::AppData::GetAppExt()
{
    switch (_eAppType)
    {
    case APP_TYPE_APK:
        return L"apk";
        break;
    case APP_TYPE_PXL:
        return L"pxl";
        break;
    case APP_TYPE_IPA:
        return L"ipa";
        break;
    case APP_TYPE_DEB:
        return L"deb";
        break;
    }
    return L"";
}

bool ND91Assistant::AppData::GetPermissions( vector<wstring>& permissionList )
{
    permissionList.clear();
    for (deque<wstring>::iterator it = _PermissionList.begin(); it != _PermissionList.end(); it++)
        permissionList.push_back(*it);
    return true;
}

//#endif