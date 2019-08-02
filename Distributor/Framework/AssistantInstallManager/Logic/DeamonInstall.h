#pragma once


#define IOSDEAMON		L"DEAMON"

#define IPHONEASSAT		L"IPHONE"
#define IPADASSAT		L"IPAD"
#define IOSLEGALASSAT	L"LEGAL"
#define ANDROIDASSAT	L"ANDROID"
#define OFFICIALASSAT	L"OFFICIAL"

#define LEGALASSATID	L"netdragon.mobile.iphone.91pandaspace"
#define IOSASSATID		L"netdragon.mobile.iphone.91space"
#define IPADASSATID		L"netdragon.mobile.ipad.91space"
#define ANDROIDASSATID	L"com.dragon.android.pandaspace"

const int PandaMinSize = 1024 * 1024 * 3;

// ��װ״̬
enum DEAMON_INSTALLSTATE
{
	INSTALLSTATE_WAITTING,
	INSTALLSTATE_RUNNING,
	INSTALLSTATE_COMPLETED,
	INSTALLSTATE_FAIL,
};

class CDeamonInstall
{
public:

	CDeamonInstall(wstring deviceId = L"");
	~CDeamonInstall();


	// Summary  : ��ʼ������
	// Returns  : void
	void Init();

	// Summary  : �����û���¼��Ϣ
	// Returns  : void
	// Parameter: bool bLogin
	// Parameter: bool bCpaUser
	// Parameter: std::string channelID
	void SetUserInfo( bool bLogin, bool bCpaUser, std::string channelID );

	// Summary  : ��鱾�����ְ�װ���Ƿ��������Ƿ���Ҫ����(IOSר��)
	// Returns  : bool
	virtual bool CheckDeamonFilesPerfect(int nDeviceVer = 5, bool isPad = false) = 0;

	// Summary  : �����ػ�(IOSר��)
	// Returns  : bool
	virtual bool DownloadDeamon() = 0;

	// Summary  : ��װ�ػ�(IOSר��)
	// Returns  : DEAMON_INSTALLSTATE
	virtual DEAMON_INSTALLSTATE InstallDeamon() = 0;


	// Summary  : ��鱾�����ְ�װ���Ƿ��������Ƿ���Ҫ����
	// Returns  : bool
	// Parameter: int nDeviceVer
	// Parameter: bool isPad
	virtual bool CheckAssistFilesPerfect( int nDeviceVer, bool isJailBreaked=false, bool isPad=false, bool isCpa=false ) = 0;

	// Summary  : �����ֻ�������
	// Returns  : bool
	virtual bool DownloadAssistant(bool isJailBreaked=false, bool isPad=false, bool isCpa=false) = 0;

	// Summary  : ��װ����
	// Returns  : DEAMON_INSTALLSTATE
	virtual DEAMON_INSTALLSTATE InstallAssistant() = 0;


	// Summary  : �ж��Ƿ���Ҫ��װ���߸����ػ�
	// Returns  : bool
	virtual bool NeedDeamonInstall() = 0;

	// Summary  : �ж��Ƿ���Ҫ��װ���߸�������
	// Returns  : bool
	virtual bool NeedAssistInstall() = 0;


	bool _bDownLoadFinish;					// �����Ƿ��������


protected:

	//���汾
	bool CompareVersion(vector<wstring> LocalVersionList,vector<wstring> serviceVersionList,size_t pos);



	wstring _strIniPath;

	wstring _strIosDir;
	wstring _strIosAssistantDir;
	wstring _strIosDeamonDir;

	wstring _strDeamonPath;					// ios�豸���̼��汾·��
	wstring _strIphoneAssistPath;
	wstring _strIpadAssistantPath;

	wstring _strLegalAssistPath;			// �����ֻ�����


	wstring _strAndDir;
	wstring _strAndAssistantDir;
	wstring _strAssistantAndPath;			// android�豸���̼��汾·��
	wstring _strAssOfficialAndPath;			// android�豸�ٷ�������ID�汾

	wstring _strServiceVer;					// Deamon��������°汾
	wstring _strServiceUrl;					// Deamon���ص�ַ	


	DEAMON_INSTALLSTATE _InstallState;

	wstring		_deviceId;
	DeviceData* _pDevData;

	bool _bLogin;
	bool _bCpaUser;
	std::string _strChannelID;


private:
	
	bool		_bHasInit;

		

};
