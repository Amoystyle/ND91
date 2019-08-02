#pragma once

#define WebResourceModuleCONFIG_HANDLE WebResourceModuleCConfig::GetInstance()
#define WebResourceModuleCONFIG(e)	WebResourceModuleCConfig::GetInstance()->e

// ϵͳ�����࣬��Ҫ��������һЩ�̶�����
class WebResourceModuleCConfig  
{
private:
	static WebResourceModuleCConfig* _pInstance;   // ����
public:
	// ȡ�õ�������
	static WebResourceModuleCConfig* GetInstance();
	// �ͷŵ�������
	static void Release();



private:
	WebResourceModuleCConfig();
	virtual ~WebResourceModuleCConfig();

    void LoadDefault();
    void LoadOption(wstring strIniFilePath);
	void LoadUserSetting();
public:
  
	wstring _strUserDataPath;	// �û�����Ŀ¼
	wstring _strWebLegalSoft;	// ����Ӧ��Ŀ¼
	wstring _strUserSettingIni; // �û������ļ�

	wstring _strIosDownPath;      // ios���Ĭ������·��

	wstring _strAndroidDownPath; // android ���Ĭ������·��

	wstring _strbatchdownloadPath;		//�������ص�xml���·��

};

