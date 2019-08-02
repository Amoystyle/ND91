#pragma once

#define CONFIG_HANDLE CConfig::GetInstance()
#define CONFIG(e)	CConfig::GetInstance()->e

struct GetConfigMsg;

// ϵͳ�����࣬��Ҫ��������һЩ�̶�����
class SINGLETON_API CConfig
{
private:
	static CConfig* _pInstance;   // ����
public:
	// ȡ�õ�������
	static CConfig* GetInstance();
	// �ͷŵ�������
	static void Release();

	//�����Ƿ��һ������
	void SetFirstTimeRun( bool val );

	//��ȡ�Ƿ��һ������
	bool GetFirstTimeRun();

	//��ȡ�Ƿ���ʾ������ԴNewͼ��
	bool GetShowWebNew();

	//�����Ƿ���ʾ������ԴNewͼ��
	void SetShowWebNew(bool val);

	//��������·��
	void SetDownLoadPath( int platform, wstring downpath );

	//�����Ƿ��Զ�ɾ���Ѿ��������
	void SetAutoDeleteTask(bool del);

	static std::wstring GetIniFileValue( wstring strIniFile, wstring strGroup, wstring strKey );

	static bool SetIniFileValue( wstring strIniFile, wstring strGroup, wstring strKey, wstring strValue );

private:
	CConfig();
	virtual ~CConfig();

    void LoadDefault();
    void LoadOption(wstring strIniFilePath);
	void LoadUserSetting();
public:
    wstring _strUserDataPath;   // �û�����Ŀ¼
    wstring _strWorkPath;       // ����Ŀ¼    
    wstring _strTemp;           // ��ʱĿ¼
    wstring _strInfoFile;       // ��Ϣ�ļ�
    wstring _strDumpPath;       // Dum�ļ�����Ŀ¼
    wstring _strDriverPath;     // �����ļ�����Ŀ¼
    wstring _strStatisticPath;  // ͳ��Ŀ¼
    wstring _strDownTempPath;   // ������ʱĿ¼
    wstring _strBackupPath;     // ����Ŀ¼
    wstring _strDownloadPath;   // ����Ŀ¼
    wstring _strDevicePhotoPath;// �豸ͼƬĿ¼
    wstring _strSoftwarePath;   // �������Ŀ¼    
    wstring _strAssistPath;     // �ػ�����Ŀ¼
    wstring _strUpdatePath;     // �����ļ�Ŀ¼
    wstring _strIconPath;       // ͼ��Ŀ¼
	wstring _strWebLegalSoft;	// ����Ӧ��Ŀ¼
    wstring _strOptionIni;      // Option.ini�ļ�·��,�����������
    wstring _strUserSettingIni; // �û������ļ�
	wstring _strCachePath;      // ����Ŀ¼
    wstring _strLogPath;        // Log·��
    wstring _strIosDownPath;      // ios���Ĭ������·��
    wstring _strAndroidDownPath;  // android���Ĭ������·��
	wstring _strIos_CpaDownPath;      // ios Cpa���Ĭ������·��
	wstring _strAndroid_CpaDownPath;  // android Cpa���Ĭ������·��
	wstring _strBizDictionaryIni; // �����ֵ����ŷ����õļ�ֵ��ֵ
	wstring _strSHSHPath;		//SHSH����·��

	wstring _strDriverInstallIni;	//�����������������������ĵ�
	wstring _strDriverInstallXML;	//�������ÿɰ�װ���������ĵ�

	wstring _strTrayIconPath;		//����ͼ���Ӧ��ַ��    

    bool _bEnableAndroid;       // �Ƿ񼤻�Android
    bool _bEnableIos;           // �Ƿ񼤻�Ios
	bool _bFirstTimeRun;		//�Ƿ��һ������
	bool _bShowWebNew;			//�Ƿ���ʾ������ԴNewͼ��

	bool _bAppStart;			//�����Ƿ�����
	bool _bAppClose;			//�ر�ʱ�ǹرջ�����С��������
	bool _bDownLoadMsg;			//��������ʼʱ�Ƿ������ʾ
	bool _bSoftInstallMsg;		//�����װ���ʱ�Ƿ������ʾ
	bool _bAutoDeleteTask;		//�Ƿ��Զ�ɾ���Ѿ��������
	bool _bAutoDelUpdatedApp;	//�������Ƿ��Զ�ɾ���Ͱ汾��Ӧ��
};

