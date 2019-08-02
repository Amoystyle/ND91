#pragma once

#include "sqlite/CppSqlite3.h"

INSTALLCASE_BEGIN

class MergeUserData
{
public:
    MergeUserData();
    ~MergeUserData();

private:
    struct MergeSoftInfo
    {
        int id;
        int linkID;
        string name;
        wstring path;
        int size;

        string version;
        string fileFormat;
        string supportDevice;
        string appName;
        int InstallTime;

        time_t LastWriteTime;
        int bLegalSoft;
        string buyer;
        string phoneCategory;
        string FBundleVersion;

        int versionCode;
        string SDKVersion;
		int fid;
    };
    struct MergeSoftLibFolder
    {
        int id;
        string name;
        string category;
        wstring path;
        int num;
        int size;
        vector<MergeSoftInfo*> fileList;
    };
    struct MergeInstallPlan
    {
        int id;
        string category;
        string name;
        int num;
        int size;
        int runtime;
        int installnum;
        time_t lastuseTime;
        int type; //0=
        vector<MergeSoftInfo*> fileList;
    };    
private:
    vector<MergeSoftLibFolder*> _oldFolderList;
    vector<MergeSoftLibFolder*> _newFolderList;
    vector<MergeInstallPlan*> _oldPlanList;
    vector<MergeInstallPlan*> _newPlanList;    

    CppSQLite3DB m_olddb;
    bool _success;
    int _nMaxLibFolderID;
    int _nMaxLibFileID;
    int _nMaxPlanID;
    int _nMaxPlanFileID;
    wstring _iconPath;
private:
    bool Action();
    void GetSoftInfo(MergeSoftInfo* pSoft, CppSQLite3Table* pTable);    
    bool InsertLibFolder(MergeSoftLibFolder* pData);
    bool InsertLibFile(MergeSoftInfo* pData, int linkID);
    bool InsertPlan(MergeInstallPlan* pData);
    bool InsertPlanFile(MergeSoftInfo* pData, int linkID);
    bool CopyIcon(MergeSoftInfo* pSoft);  
    string FormatValue(string str);
public:
    // ���ؾɰ汾����
    bool LoadOldData();
    bool LoadOldData_Folder();  // ���ط������ļ���
    bool LoadOldData_Soft();    // ����Ӧ���б�
    bool LoadOldData_Case();    // ���ط�������
    bool LoadOldData_CaseFile();// ���ط����ļ�����

    // �����°汾����
    bool LoadNewData();
    bool LoadNewData_Folder();  // ���ط������ļ���
    bool LoadNewData_Soft();    // ����Ӧ���б�
    bool LoadNewData_Case();    // ���ط�������
    bool LoadNewData_CaseFile();// ���ط����ļ�����

    // �������ݵ��°汾��
    bool MergeData(); 
    bool MergeData_Folder();   // ���뷽�����ļ���
    bool MergeData_Case();     // ���뷽������        

    // �ͷ�����
    void ReleaseData();
};

INSTALLCASE_END