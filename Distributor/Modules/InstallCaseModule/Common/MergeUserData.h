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
    // 加载旧版本数据
    bool LoadOldData();
    bool LoadOldData_Folder();  // 加载方案库文件夹
    bool LoadOldData_Soft();    // 加载应用列表
    bool LoadOldData_Case();    // 加载方案数据
    bool LoadOldData_CaseFile();// 加载方案文件数据

    // 加载新版本数据
    bool LoadNewData();
    bool LoadNewData_Folder();  // 加载方案库文件夹
    bool LoadNewData_Soft();    // 加载应用列表
    bool LoadNewData_Case();    // 加载方案数据
    bool LoadNewData_CaseFile();// 加载方案文件数据

    // 插入数据到新版本中
    bool MergeData(); 
    bool MergeData_Folder();   // 导入方案库文件夹
    bool MergeData_Case();     // 导入方案数据        

    // 释放数据
    void ReleaseData();
};

INSTALLCASE_END