#pragma once

#define SETTINGCFG_HANDLE SettingCfg::GetInstance()
#define SETTINGCFG(e)	SettingCfg::GetInstance()->e


class SettingCfg  
{
private:
	static SettingCfg* _pInstance;   // ����
public:
	// ȡ�õ�������
	static SettingCfg* GetInstance();
	// �ͷŵ�������
	static void Release();
private:
	SettingCfg();
	virtual ~SettingCfg();

public:
	void WriteIni();
};

