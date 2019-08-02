#pragma once

//�����������ݿ������

INSTALLCASE_BEGIN

class InstallsCaseDBEx
{
public:
	InstallsCaseDBEx();
	~InstallsCaseDBEx();

	//��������
	void LoadData(InstallCaseManagerEx* caseManager);

	//�����½�����
	void InsertNewCase(InstallsCaseData* casedata);

	//����������Ӧ��
	void InsertCaseFile(InstallsCaseData* casedata,InstallsAppInfo* appdata);

	//�������Ƴ�Ӧ��
	void RemoveCaseFile(InstallsCaseData* casedata,InstallsAppInfo* appdata);

	//�Ƴ��������������
	void RemoveAllCaseFile(InstallsCaseData* casedata);

	//�Ƴ�����
	void RemoveCase(InstallsCaseData* casedata);

	//��������
	void RenameCase(InstallsCaseData* casedata,wstring newName);

	//���·���
	void UpdateCase(InstallsCaseData* casedata);

	//�������ļ�
	void InsertAppToLib(InstallsCaseData* casedata,InstallsAppInfo* appdata);

	//�����Ƴ��ļ�
	void RemoveAppFromLib(InstallsCaseData* casedata,InstallsAppInfo* appdata);

	//������Ϣ��
	void CreateTableProfile();

	//��ȡ��󷽰�id
	int  GetCaseMaxId();

	int  GetAppMaxId();

	int  GetInstallPlanFileMaxId();

	void UpdateItemIDByPath(wstring tableName, wstring path, int itemID);
protected:
	//����
	void CreateTableSoftLibFolder();
	void CreateTableSoftLibFile();
	void CreateTableInstallPlan();
	void CreateTableInstallPlanFile();
	void InitTable();
	//���±�
	void UpdateTableInstallPlanFile();
	void UpdateTableSoftLibFile();
	void UpdateTableInstallPlan();
	void UpdateTableInstallPlanFile2();
	void UpdateTableSoftLibFile2();
	void UpdateTableInstallPlanFile3();
	void UpdateTableSoftLibFile3();
	//wstring m_dbName;

	void LoadLibApps(InstallsCaseData* casedata);
	void LoadCaseApps(InstallsCaseData* casedata);

private:
	int idFix;
public:
	bool needReAnalyzeApps;
};

INSTALLCASE_END