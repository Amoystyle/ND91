#pragma once
class CheckCanInstallAppsync : public BaseThread
{
public:
    // 构造函数 threadID线程唯一标识
    CheckCanInstallAppsync();
	~CheckCanInstallAppsync();

	static bool CanInstall(wstring sn);


	static bool Install(wstring sn, wstring& errorMsg);
private:
	static bool CheckNodeExist(wstring sn, wstring path);
};