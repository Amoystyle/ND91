#pragma once

#define SETTINGCFG_HANDLE SettingCfg::GetInstance()
#define SETTINGCFG(e)	SettingCfg::GetInstance()->e


class SettingCfg  
{
private:
	static SettingCfg* _pInstance;   // 单例
public:
	// 取得单例对象
	static SettingCfg* GetInstance();
	// 释放单力对象
	static void Release();
private:
	SettingCfg();
	virtual ~SettingCfg();

public:
	void WriteIni();
};

