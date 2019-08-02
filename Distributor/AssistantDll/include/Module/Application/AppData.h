///////////////////////////////////////////////////////////
//  AppData.h
//  Implementation of the Class AppData
//  Created on:      01-����-2011 9:42:49
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_7330FA27_D4AD_40d8_A454_2C41E4A6C565__INCLUDED_)
#define EA_7330FA27_D4AD_40d8_A454_2C41E4A6C565__INCLUDED_

#include "Module/BaseData.h"
#include "Core/DeviceData.h"
#include "Common/IosPngConver.h"
#include <deque>
#include <string>
#include <vector>
using namespace std;

#pragma warning(disable : 4251)

namespace ND91Assistant
{
#define PXL_APP_INFO_FOLDER					L"/private/var/root/Media/PXL/DB/"
#define IPA_APP_INFO_FILE					L"/private/var/mobile/Library/Caches/com.apple.mobile.installation.plist"
#define IPA_APP_INFO_FILE_NAME				L"ipaList.plist"
#define INSTALLER_APP_INFO_FOLDER			L"/private/var/mobile/Library/Installer/"// �Ͱ汾�Ĵ���ļ���·����ͬ
#define INSTALLER_APP_INFO_FOLDER_LOW		L"/private/var/root/Library/Installer/"
#define INSTALLER_APP_INFO_FILE_NAME		L"LocalPackages.plist"
#define PANDA_APP_INFO_FILE					L"/private/var/root/Media/TQINSTALLER/installed.plist"
#define PANDA_APP_INFO_FILE_NAME			L"ndInstaller.plist"
#define DEB_APP_INFO_FILE					L"/private/var/lib/dpkg/status"
#define DEB_APP_INFO_FILE_NAME				L"cydialist"
#define IPA_APP_METADATA_FILE				L"iTunesMetadata.plist"

    class CLASSINDLL_CLASS_DECL AppData : public BaseData
    {
    friend class GetAppListCommand;
	friend class GetAppInfoCommand;
	friend class AdbPMListCommand;
    public:
        AppData();
		AppData(const AppData* pAppData);
        virtual ~AppData();

		/// <summary>
		/// apk�����ļ���Ҫ��װ��·��
		/// android:installLocation=["auto" | "internalOnly" | "preferExternal"] >
		/// </summary>
		
		enum AndroidInstallLocation
		{
			None = -1,           //�汾��2.2һ��û��������Ի�����2.2�汾�����ø�����
			Auto = 0,           //0 The application may be installed on the external storage, but the system will install the application on the internal storage by default. If the internal storage is full, then the system will install it on the external storage. Once installed, the user can move the application to either internal or external storage through the system settings.
			InternalOnly = 1,       // 1 The application must be installed on the internal device storage only. If this is set, the application will never be installed on the external storage. If the internal storage is full, then the system will not install the application. This is also the default behavior if you do not define android:installLocation.
			PreferExternal = 2,       // 2 The application prefers to be installed on the external storage (SD card). There is no guarantee that the system will honor this request. The application might be installed on internal storage if the external media is unavailable or full, or if the application uses the forward-locking mechanism (not supported on external storage). Once installed, the user can move the application to either internal or external storage through the system settings.
		};

		// ��װ����Ϣ
		std::wstring _strAppPCPath;		// ��װ��·�� (PC��) 
		bool _bLockApp;				// ��װʱ�Ƿ�Ҫ����App
		bool _bRecover;				// �Ƿ񸲸�ԭ����
		bool _bOnSDCard;			// �Ƿ�װ��SDCard
		bool _bKeepData;			// �Ƿ����û����� (ж��ʱʹ��)
		//AndroidInstallLocation   _APKInstallLocation; // apk�����ļ���Ҫ��װ��·��


		// ������Ϣ
		wstring _packageName;		// ��װ���� ISO��ʾ�����Ψһ��ʶ
        int     _nVersionCode;      // android�ڲ��汾�ţ����������                 
		wstring _strVersion;		// �汾����                   
		wstring _AppName;			// Ӧ�ó�����             
		wstring _Description;		// ����
		wstring _Url;				// ���ӵ�ַ
		wstring _strAppPhonePath;	// ����·�� (�ֻ���)              

		wstring		_strIconPhonePath;	// ͼ��·�� (�ֻ���)
		wstring		_strIconPCPath;		// ͼ��·�� (PC��)
		APP_TYPE	_eAppType;			// ������ࣨpxl,ipa,installer,panda,deb��
		APP_OWNER	_eAppOwner;			// �������ͣ�ϵͳ���û���
		bool		_bShare;			// �ļ��Ƿ���
		wstring		_strState;          // ����״̬
		wstring		_ExtName;			//��չ��-��װexe�ļ�ʱ��

		wstring _strPurchaser;          // ������
        int64_t _applicationDSID;       // ������ID ���ֻ���

		wstring _DebEssential;
		wstring _DebDepends;
		vector<wstring> _DeviceFamily;//֧���豸�б� 1 ipod itouch iphone 2 ipad
		wstring _sdkVer;//ʹ�ÿ������汾
		int _isLegalSoft;//�Ƿ�����
		deque<wstring>  _PermissionList;     // Ȩ���б�
		
        int64_t _dynamicDiskUsage;      // �ĵ���С
        int64_t _staticDiskUsage;       // �����С
        wstring _minimumOSVersion;      // ��͹̼��汾Ҫ��

		wstring _strExecutable;//����

        // �Ƿ���91�˻������IPA,�ǵĻ�,������Ӧ����Ȩ
        bool _is91IPA;

		bool _isEnterpriseApp;//�Ƿ�����ҵ��Ӧ��

        // 91IPA����˻���Ϣ
        wstring _acountInfo;

		int _itemId; //itunes id

        wstring _strBundleExecutableName;
        wstring _strIconName;

	public:

		// ��ȡ����ͼ��,����Ϊ�ļ�(strPCPath)�����ص�ͼ���ļ��ɽ��渺��ɾ��
		// Ҫ���ֵ�ͼ����
		bool LoadAppIcon(const DeviceData* pDeviceData, wstring strPCPath);
		// ��������Ȩ����Ϣ
		bool LoadPermissions(const DeviceData* pDeviceData);
        // ��ȡӦ�õ�Ȩ���б�
        bool GetPermissions(vector<wstring>& permissionList);
		// �鿴�Ƿ��û�����
		bool IsUserApp();
        wstring GetAppExt();

        // ��ȡӦ�ù�����
        bool GetAppPurchaser(const DeviceData* pDeviceData);

    private:
        size_t ParseAndroidData(const char* pData, size_t nSize);
		bool   ParseAndroidAdbData(string strLine);

        // ��ȡӦ��ICON·��
        bool GetAppIconPhonePath(const DeviceData* pDeviceData);

    };
}

#endif // !defined(EA_7330FA27_D4AD_40d8_A454_2C41E4A6C565__INCLUDED_)
