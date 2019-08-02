#pragma once

INSTALLCASE_BEGIN
//�����������ݿ������
class InstallsCaseManager;
class InstallsCaseDB
{
public:
	InstallsCaseDB();
	~InstallsCaseDB();
	//��������
	void LoadData(InstallsCaseManager* caseManager);
	//�����½�����
	void InsertNewCase(InstallsCaseData* casedata);
	//����������Ӧ��
	void InsertCaseFile(InstallsCaseData* casedata,InstallsAppInfo* appdata);
	//�������Ƴ�Ӧ��
	void RemoveCaseFile(InstallsCaseData* casedata,InstallsAppInfo* appdata);
	void RemoveAllCaseFile(InstallsCaseData* casedata);
	//�Ƴ�����
	void RemoveCase(InstallsCaseData* casedata);
	//��������
	void RenameCase(InstallsCaseData* casedata,wstring newName);
	//�������ļ�
	void InsertAppToLib(InstallsCaseData* casedata,InstallsAppInfo* appdata);
	//�����Ƴ��ļ�
	void RemoveAppFromLib(InstallsCaseData* casedata,InstallsAppInfo* appdata);
	//������Ϣ��
	void CreateTableProfile();
protected:
	//����
	void CreateTableSoftLibFolder();
	void CreateTableSoftLibFile();
	void CreateTableInstallPlan();
	void CreateTableInstallPlanFile();
	void InitTable();
	//wstring m_dbName;

	void LoadLibApps(InstallsCaseData* casedata);
	void LoadCaseApps(InstallsCaseData* casedata);
};
INSTALLCASE_END
