#pragma once
class CheckCanInstallAppsync : public BaseThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    CheckCanInstallAppsync();
	~CheckCanInstallAppsync();

	static bool CanInstall(wstring sn);


	static bool Install(wstring sn, wstring& errorMsg);
private:
	static bool CheckNodeExist(wstring sn, wstring path);
};