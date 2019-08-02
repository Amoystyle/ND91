#pragma once

#define WebResourceModuleCONFIG_HANDLE WebResourceModuleCConfig::GetInstance()
#define WebResourceModuleCONFIG(e)	WebResourceModuleCConfig::GetInstance()->e

// 系统配置类，主要用来配置一些固定参数
class WebResourceModuleCConfig  
{
private:
	static WebResourceModuleCConfig* _pInstance;   // 单例
public:
	// 取得单例对象
	static WebResourceModuleCConfig* GetInstance();
	// 释放单力对象
	static void Release();



private:
	WebResourceModuleCConfig();
	virtual ~WebResourceModuleCConfig();

    void LoadDefault();
    void LoadOption(wstring strIniFilePath);
	void LoadUserSetting();
public:
  
	wstring _strUserDataPath;	// 用户数据目录
	wstring _strWebLegalSoft;	// 正版应用目录
	wstring _strUserSettingIni; // 用户配置文件

	wstring _strIosDownPath;      // ios软件默认下载路径

	wstring _strAndroidDownPath; // android 软件默认下载路径

	wstring _strbatchdownloadPath;		//批量下载的xml存放路径

};

