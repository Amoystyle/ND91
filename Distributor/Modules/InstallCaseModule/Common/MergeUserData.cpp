#include "stdafx.h"
#include "MergeUserData.h"
INSTALLCASE_BEGIN

MergeUserData::MergeUserData()
{INTERFUN;
    _nMaxLibFolderID = 0;
    _nMaxLibFileID = 0;
    _nMaxPlanID = 0;
    _nMaxPlanFileID = 0;

    _success = false;
    _success = Action();
}

MergeUserData::~MergeUserData()
{INTERFUN;

}

bool MergeUserData::Action()
{INTERFUN;
    INITTICKCHECKER;
    if (!LoadOldData()) return false;
    TICKCHECKER;
    if (!LoadNewData()) return false;
    TICKCHECKER;

    CFileOperation::MakeLocalDir(CONFIG(_strUserDataPath) + L"icon");
	
    _iconPath = CONFIG(_strUserDataPath) + L"icon\\";
    if (!MergeData()) return false;
    TICKCHECKER;
    return true;
}

// Old Data
bool MergeUserData::LoadOldData()
{    INTERFUN;
    try
    {
        wstring strOldDBPath = MyFileOperation::GetMyDocumentPath() + L"\\Distributor\\UserDataStorage\\UserDataStorage.db";
        bool bOpen = m_olddb.Open(CCodeOperation::UnicodeToUTF_8(strOldDBPath).c_str());
        if (!bOpen) return false;

        LoadOldData_Folder();
        LoadOldData_Soft();
        LoadOldData_Case();
        LoadOldData_CaseFile();

        m_olddb.Close();
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool MergeUserData::LoadOldData_Folder()
{INTERFUN;
    // 已下载的网络装机方案软件列表
    string strSql = "Select SoftLibID,SoftLibName,Category,FolderPath,SoftNum,TotalSize from SoftLibFolder";
    CppSQLite3Table table = m_olddb.GetTable(strSql.c_str());

    // 添加方案已下载软件
    for (int i = 0; i < table.NumRows(); i++)
    {
        table.SetRow(i);
        MergeSoftLibFolder* pFolder = new MergeSoftLibFolder();
        pFolder->id         = table.GetIntField(0);
        pFolder->name       = table.GetStringField(1);
        pFolder->category   = table.GetStringField(2);
        pFolder->path       = CCodeOperation::UTF_8ToUnicode(table.GetStringField(3));
        pFolder->num        = table.GetIntField(4);
        pFolder->size       = table.GetIntField(5);

        _oldFolderList.push_back(pFolder);
    }
    return true;
}

bool MergeUserData::LoadOldData_Soft()
{INTERFUN;
    // 已下载的网络装机方案软件列表
    string strSql = "Select FileID,SoftLib_ID,Name,Path,Size,\
                    Version,Format,SupportDevice,AppName,InstallTime,\
                    LastWriteTime,IsLegalSoft,Buyer,PhoneCategory,FBundleVersion,\
                    VersionCode,SDKVersion,FID from SoftLibFile";
    CppSQLite3Table table = m_olddb.GetTable(strSql.c_str());

    // 添加方案已下载软件
    for (int i = 0; i < table.NumRows(); i++)
    {
        table.SetRow(i);
        MergeSoftInfo* pSoft = new MergeSoftInfo();
        GetSoftInfo(pSoft, &table);

        for (vector<MergeSoftLibFolder*>::iterator it = _oldFolderList.begin(); it != _oldFolderList.end(); it++)
        {
            MergeSoftLibFolder* pFolder = *it;
            if (pFolder->id == pSoft->linkID)
                pFolder->fileList.push_back(pSoft);
        }
    }    
    return true;
}

bool MergeUserData::LoadOldData_Case()
{INTERFUN;
    // 已下载的网络装机方案软件列表
    string strSql = "Select PlanID,Category,PlanName, SoftNum, TotalSize,\
                    PlanInstallTime,InstallNum,LastUseTime from InstallPlan where PlanType=0";
    CppSQLite3Table table = m_olddb.GetTable(strSql.c_str());
    // 添加方案已下载软件
    for (int i = 0; i < table.NumRows(); i++)
    {
        table.SetRow(i);
        MergeInstallPlan* pData = new MergeInstallPlan();
        pData->id           = table.GetIntField(0);
        pData->category     = table.GetStringField(1);
        pData->name         = table.GetStringField(2);
        pData->num          = table.GetIntField(3);
        pData->size         = table.GetIntField(4);

        pData->runtime      = table.GetIntField(5);
        pData->installnum   = table.GetIntField(6);
        string str = table.GetStringField(7);
        
        pData->lastuseTime  = BaseOperation::ParstTimeFormat(CCodeOperation::UTF_8ToUnicode(str).c_str());

        _oldPlanList.push_back(pData);
    }
    return true;
}

bool MergeUserData::LoadOldData_CaseFile()
{INTERFUN;
    // 已下载的网络装机方案软件列表
    string strSql = "Select FileID,Plan_ID,Name,Path,Size,\
                    Version,Format,SupportDevice,AppName,InstallTime,\
                    LastWriteTime,IsLegalSoft,Buyer,PhoneCategory,FBundleVersion,\
                    VersionCode,SDKVersion,FID from InstallPlanFile";
    CppSQLite3Table table = m_olddb.GetTable(strSql.c_str());

    // 添加方案已下载软件
    for (int i = 0; i < table.NumRows(); i++)
    {
        table.SetRow(i);
        MergeSoftInfo* pSoft = new MergeSoftInfo();
        GetSoftInfo(pSoft, &table);

        for (vector<MergeInstallPlan*>::iterator it = _oldPlanList.begin(); it != _oldPlanList.end(); it++)
        {
            MergeInstallPlan* pPlan= *it;
            if (pPlan->id == pSoft->linkID)
                pPlan->fileList.push_back(pSoft);
        }
    }    
    return true;
}

// new Data
bool MergeUserData::LoadNewData()
{INTERFUN;
    try
    {
        LoadNewData_Folder(); 
        LoadNewData_Soft();   
        LoadNewData_Case();   
        LoadNewData_CaseFile();
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool MergeUserData::LoadNewData_Folder()
{INTERFUN;
    // 已下载的网络装机方案软件列表
    CppSQLite3Table table;
    wstring strSql = L"Select SoftLibID,SoftLibName,Category,FolderPath,SoftNum,TotalSize from SoftLibFolder";
    CDBManager::DB_QUERY_TABLEDATA(strSql.c_str(),table);

    // 添加方案已下载软件
    for (int i = 0; i < table.NumRows(); i++)
    {
        table.SetRow(i);
        MergeSoftLibFolder* pFolder = new MergeSoftLibFolder();
        pFolder->id         = table.GetIntField(0);
        pFolder->name       = table.GetStringField(1);
        pFolder->category   = table.GetStringField(2);
        pFolder->path       = CCodeOperation::UTF_8ToUnicode(table.GetStringField(3));
        pFolder->num        = table.GetIntField(4);
        pFolder->size       = table.GetIntField(5);

        _nMaxLibFolderID = max(_nMaxLibFolderID, pFolder->id);
        _newFolderList.push_back(pFolder);
    }    
    return true;
}

bool MergeUserData::LoadNewData_Soft()
{INTERFUN;
    // 已下载的网络装机方案软件列表
    CppSQLite3Table table;
    wstring strSql = L"Select FileID,SoftLib_ID,Name,Path,Size,\
                    Version,Format,SupportDevice,AppName,InstallTime,\
                    LastWriteTime,IsLegalSoft,Buyer,PhoneCategory,FBundleVersion,\
                    VersionCode,SDKVersion,FID from SoftLibFile";
    CDBManager::DB_QUERY_TABLEDATA(strSql.c_str(),table);


    // 添加方案已下载软件
    for (int i = 0; i < table.NumRows(); i++)
    {
        table.SetRow(i);
        MergeSoftInfo* pSoft = new MergeSoftInfo();
        GetSoftInfo(pSoft, &table);
        _nMaxLibFileID = max(_nMaxLibFileID, pSoft->id);

        for (vector<MergeSoftLibFolder*>::iterator it = _newFolderList.begin(); it != _newFolderList.end(); it++)
        {
            MergeSoftLibFolder* pFolder = *it;
            if (pFolder->id == pSoft->linkID)
                pFolder->fileList.push_back(pSoft);
        }
    }    
    return true;
}

bool MergeUserData::LoadNewData_Case()
{INTERFUN;
    CppSQLite3Table table;
    wstring strSql = L"Select PlanID,Category,PlanName, SoftNum, TotalSize,\
                     PlanInstallTime,InstallNum,LastUseTime from InstallPlan where PlanType=3";
    CDBManager::DB_QUERY_TABLEDATA(strSql.c_str(),table);
    
    for (int i = 0; i < table.NumRows(); i++)
    {
        table.SetRow(i);
        MergeInstallPlan* pData = new MergeInstallPlan();
        pData->id           = table.GetIntField(0);
        pData->category     = table.GetStringField(1);
        pData->name         = table.GetStringField(2);
        pData->num          = table.GetIntField(3);
        pData->size         = table.GetIntField(4);

        pData->runtime      = table.GetIntField(5);
        pData->installnum   = table.GetIntField(6);
        string str = table.GetStringField(7);
        pData->lastuseTime  = table.GetIntField(7);        

        _nMaxPlanID = max(_nMaxPlanID, pData->id);
        _newPlanList.push_back(pData);
    }
    return true;
}

bool MergeUserData::LoadNewData_CaseFile()
{INTERFUN;
    CppSQLite3Table table;
    wstring strSql = L"Select FileID,Plan_ID,Name,Path,Size,\
                      Version,Format,SupportDevice,AppName,InstallTime,\
                      LastWriteTime,IsLegalSoft,Buyer,PhoneCategory,FBundleVersion,\
                      VersionCode,SDKVersion,FID from InstallPlanFile";
    CDBManager::DB_QUERY_TABLEDATA(strSql.c_str(),table);

    // 添加方案已下载软件
    for (int i = 0; i < table.NumRows(); i++)
    {
        table.SetRow(i);
        MergeSoftInfo* pSoft = new MergeSoftInfo();
        GetSoftInfo(pSoft, &table);

        _nMaxPlanFileID = max(_nMaxPlanFileID, pSoft->id);
        for (vector<MergeInstallPlan*>::iterator it = _oldPlanList.begin(); it != _oldPlanList.end(); it++)
        {
            MergeInstallPlan* pPlan= *it;
            if (pPlan->id == pSoft->linkID)
                pPlan->fileList.push_back(pSoft);
        }
    }    
    return true;
}

// Merge
bool MergeUserData::MergeData()
{INTERFUN;
    MergeData_Folder();
    MergeData_Case();
    return true;
}

bool MergeUserData::MergeData_Folder()
{INTERFUN;
    for (vector<MergeSoftLibFolder*>::iterator it = _oldFolderList.begin(); it != _oldFolderList.end(); it++)
    {
        MergeSoftLibFolder* pOld = *it;
        bool bFound = false;
        wstring strPath = pOld->path + L"\\";

        for (vector<MergeSoftLibFolder*>::iterator it2 = _newFolderList.begin(); it2 != _newFolderList.end(); it2++)
        {
            MergeSoftLibFolder* pNew = *it2;
            if (CStrOperation::CompareUnCase(pNew->path, strPath) == 0)
            {
                bFound = true;
                break;
            }
        }

        if (!bFound)
        {
            InsertLibFolder(pOld);
        }
    }

    return true;
}

bool MergeUserData::MergeData_Case()
{INTERFUN;
    for (vector<MergeInstallPlan*>::iterator it = _oldPlanList.begin(); it != _oldPlanList.end(); it++)
    {
        MergeInstallPlan* pOld = *it;
        InsertPlan(pOld);
    }
    return true;
}

void MergeUserData::GetSoftInfo( MergeSoftInfo* pSoft, CppSQLite3Table* pTable )
{INTERFUN;
    pSoft->id               = pTable->GetIntField(0);
    pSoft->linkID           = pTable->GetIntField(1);
    pSoft->name             = pTable->GetStringField(2);
    pSoft->path             = CCodeOperation::UTF_8ToUnicode(pTable->GetStringField(3));
    pSoft->size             = pTable->GetIntField(4);

    pSoft->version          = pTable->GetStringField(5);
    pSoft->fileFormat       = pTable->GetStringField(6);
    pSoft->supportDevice    = pTable->GetStringField(7);
    pSoft->appName          = pTable->GetStringField(8);
    pSoft->InstallTime      = pTable->GetIntField(9);
    
    pSoft->LastWriteTime    = BaseOperation::ParstTimeFormat(CCodeOperation::UTF_8ToUnicode(pTable->GetStringField(10)));
    pSoft->bLegalSoft       = pTable->GetIntField(11);
    pSoft->buyer            = pTable->GetStringField(12);
    pSoft->phoneCategory    = pTable->GetStringField(13);
    pSoft->FBundleVersion   = pTable->GetStringField(14);

    pSoft->versionCode      = pTable->GetIntField(15);
    pSoft->SDKVersion       = pTable->GetStringField(16);
}

bool MergeUserData::InsertLibFolder( MergeSoftLibFolder* pData )
{INTERFUN;
    _nMaxLibFolderID++;
    char sql[2048] = "";
    sprintf_s(sql,2048,("insert into SoftLibFolder(SoftLibID,SoftLibName,Category,FolderPath,SoftNum,TotalSize) \
values('%d','%s','%s','%s','%d','%d')")
                    ,_nMaxLibFolderID
                    ,FormatValue(pData->name).c_str()
                    ,pData->category.c_str()
                    ,FormatValue(CCodeOperation::UnicodeToUTF_8(pData->path)).c_str()
                    ,pData->num
                    ,pData->size
                    );
    LOG->WriteDebug(CCodeOperation::UTF_8ToUnicode(sql));
    //CDBManager::DoExecSql(sql);
    for (vector<MergeSoftInfo*>::iterator it = pData->fileList.begin(); it != pData->fileList.end(); it++)
    {
        MergeSoftInfo* pSoft = *it;
        InsertLibFile(pSoft, _nMaxLibFolderID);
    }
    return true;   
}

bool MergeUserData::InsertLibFile( MergeSoftInfo* pData, int linkID )
{INTERFUN;
    CHAR sql[2048] = "";
    sprintf_s(sql,2048,"insert into SoftLibFile(SoftLib_ID,Name,Path,Size,\
Version,Format,SupportDevice,AppName,InstallTime,\
LastWriteTime,IsLegalSoft,Buyer,PhoneCategory,FBundleVersion,\
VersionCode,SDKVersion,FID) \
values('%d','%s','%s','%d',\
'%s','%s','%s','%s','%d',\
'%lld','%d','%s','%s','%s',\
'%d','%s','%d')"
                    ,linkID
                    ,FormatValue(pData->name).c_str()
                    ,FormatValue(CCodeOperation::UnicodeToUTF_8(pData->path)).c_str()
                    ,pData->size

                    ,pData->version.c_str()
                    ,pData->fileFormat.c_str()
                    ,pData->supportDevice.c_str()
                    ,FormatValue(pData->appName).c_str()
                    ,pData->InstallTime

                    ,pData->LastWriteTime
                    ,pData->bLegalSoft
                    ,pData->buyer.c_str()
                    ,pData->phoneCategory.c_str()
                    ,pData->FBundleVersion.c_str()

                    ,pData->versionCode
                    ,pData->SDKVersion.c_str()
					,pData->fid);
    LOG->WriteDebug(CCodeOperation::UTF_8ToUnicode(sql));    
    CDBManager::DoExecSql(sql);
    CopyIcon(pData);
    return true;
}

bool MergeUserData::InsertPlan( MergeInstallPlan* pData )
{INTERFUN;
    _nMaxPlanID++;
    CHAR sql[2048] = "";
    sprintf_s(sql,2048,"insert into InstallPlan(PlanID,Category,PlanName, SoftNum, TotalSize,\
PlanInstallTime,InstallNum,LastUseTime,PlanType) \
values('%d','%s','%s','%d','%d',\
'%d','%d','%lld','%d')"
                    ,_nMaxPlanID
                    ,pData->category.c_str()
                    ,FormatValue(pData->name).c_str()
                    ,pData->num
                    ,pData->size

                    ,pData->runtime
                    ,pData->installnum
                    ,pData->lastuseTime
                    ,3
                    );
    LOG->WriteDebug(CCodeOperation::UTF_8ToUnicode(sql));
    CDBManager::DoExecSql(sql);
    for (vector<MergeSoftInfo*>::iterator it = pData->fileList.begin(); it != pData->fileList.end(); it++)
    {
        MergeSoftInfo* pSoft = *it;
        InsertPlanFile(pSoft, _nMaxPlanID);
    }
    return true;
}

bool MergeUserData::InsertPlanFile( MergeSoftInfo* pData, int linkID )
{INTERFUN;
    CHAR sql[2048] = "";
    sprintf_s(sql,2048,"insert into InstallPlanFile(Plan_ID,Name,Path,Size,\
Version,Format,SupportDevice,AppName,InstallTime,\
LastWriteTime,IsLegalSoft,Buyer,PhoneCategory,FBundleVersion,\
VersionCode,SDKVersion) \
values('%d','%s','%s','%d',\
'%s','%s','%s','%s','%d',\
'%lld','%d','%s','%s','%s',\
'%d','%s')"
                    ,linkID
                    ,FormatValue(pData->name).c_str()
                    ,FormatValue(CCodeOperation::UnicodeToUTF_8(pData->path)).c_str()
                    ,pData->size

                    ,pData->version.c_str()
                    ,pData->fileFormat.c_str()
                    ,pData->supportDevice.c_str()
                    ,FormatValue(pData->appName).c_str()
                    ,pData->InstallTime

                    ,pData->LastWriteTime
                    ,pData->bLegalSoft
                    ,pData->buyer.c_str()
                    ,pData->phoneCategory.c_str()
                    ,pData->FBundleVersion.c_str()

                    ,pData->versionCode
                    ,pData->SDKVersion.c_str()
                    );
    LOG->WriteDebug(CCodeOperation::UTF_8ToUnicode(sql));
    CDBManager::DoExecSql(sql);
    CopyIcon(pData);
    return true;
}

bool MergeUserData::CopyIcon( MergeSoftInfo* pSoft )
{INTERFUN;
    wstring iconPath = CFileOperation::GetParentDir(pSoft->path) + L"icon\\" + CCodeOperation::UTF_8ToUnicode(pSoft->appName) + L".png";
    wstring iconNewPath = _iconPath 
        + UIStringFormat::CheckDownLoadFileName(CCodeOperation::UTF_8ToUnicode(pSoft->name))
        + UIStringFormat::CheckDownLoadFileName(CCodeOperation::UTF_8ToUnicode(pSoft->version))
        + CFileOperation::GetFileExt(pSoft->path) 
        + L".png";
    //iconNewPath = CStrOperation::ReplaceW(iconNewPath, L" ", L"_");
    //iconNewPath = CStrOperation::ReplaceW(iconNewPath, L"?", L"_");
    return CFileOperation::CopyFile(iconPath, iconNewPath, true);
}

std::string MergeUserData::FormatValue( string str )
{INTERFUN;
    str = CStrOperation::Replace(str, "/", "//");
    str = CStrOperation::Replace(str, "'", "''");
    str = CStrOperation::Replace(str, "[", "/[");
    str = CStrOperation::Replace(str, "]", "/]");
    str = CStrOperation::Replace(str, "%", "/%");
    str = CStrOperation::Replace(str, "&", "/&");
    str = CStrOperation::Replace(str, "_", "/_");
    str = CStrOperation::Replace(str, "(", "/(");
    str = CStrOperation::Replace(str, ")", "/)");
    return str;
}

INSTALLCASE_END