#include "stdafx.h"
#include "InstallsCaseDB.h"
#include "InstallsCaseManager.h"
#include "Common/DataPath.h"
#define  DB_FILE_NAME L"\\91Distributor\\91Distributor.db"
INSTALLCASE_BEGIN

#pragma warning(disable:4244)
InstallsCaseDB::InstallsCaseDB()
{INTERFUN;
	wstring filePath = DataPath::GetDataDir();
	CFileOperation::MakeLocalDir(filePath+L"\\91Distributor");
	wstring m_dbName = filePath + DB_FILE_NAME;
	if(!CDBManager::m_isconnect)
		CDBManager::InitDB(m_dbName.c_str());
	InitTable();
}
InstallsCaseDB::~InstallsCaseDB()
{INTERFUN;
	
}
void InstallsCaseDB::CreateTableProfile()
{INTERFUN;
	string strsql = "CREATE TABLE Profile(SeachDefault				INTEGER);";
	CDBManager::m_db.ExecDML(strsql.c_str());
}
void InstallsCaseDB::CreateTableSoftLibFolder()
{INTERFUN;
	string strsql = "CREATE TABLE SoftLibFolder(\
					SoftLibID       INTEGER       PRIMARY KEY NOT NULL ,\
					SoftLibName				VARCHAR(1024)  NOT NULL ,\
					Category				VARCHAR(1024) ,\
					FolderPath				VARCHAR(1024) ,\
					SoftNum					INTEGER ,\
					TotalSize				FLOAT);";

	CDBManager::m_db.ExecDML(strsql.c_str());

	
	wstring MyDocment= DataPath::GetDataDir();
	wstring pathIOS = CONFIG(_strIosDownPath);
	wstring pathAndroid = CONFIG(_strAndroidDownPath);

	strsql = "insert into SoftLibFolder values(0,'";
	strsql+=Charset::UnicodeToUTF8(BaseUI::LoadString(L"SoftWareLibiOS"));
	strsql+="','iOS','";
	strsql+=Charset::UnicodeToUTF8(pathIOS.c_str());
	strsql+="',0,0);";
	CDBManager::m_db.ExecDML(strsql.c_str());

	strsql = "insert into SoftLibFolder values(1,'";
	strsql+=Charset::UnicodeToUTF8(BaseUI::LoadString(L"SoftWareLibAndroid"));
	strsql+="','Android','";
	strsql+=Charset::UnicodeToUTF8(pathAndroid.c_str());
	strsql+="',0,0);";
		CDBManager::m_db.ExecDML(strsql.c_str());
}
void InstallsCaseDB::CreateTableSoftLibFile()
{INTERFUN;
	string strsql = "CREATE TABLE SoftLibFile(\
					FileID       INTEGER       NOT NULL ,\
					SoftLib_ID				INTEGER  NOT NULL ,\
					Name				VARCHAR(1024) ,\
					Path					VARCHAR(1024) ,\
					Size					FLOAT ,\
					Version					VARCHAR(1024) ,\
					Format					VARCHAR(1024) ,\
					SupportDevice				VARCHAR(1024),\
					AppName				VARCHAR(1024),\
					InstallTime				INTEGER,\
					LastWriteTime				DATETIME,\
					IsLegalSoft				VARCHAR,\
					Buyer				VARCHAR(1024),\
					PhoneCategory				VARCHAR(1024),\
					FBundleVersion				VARCHAR(1024),\
					VersionCode				INTEGER,\
					SDKVersion				VARCHAR(1024),\
					CONSTRAINT pk_LibID PRIMARY KEY (FileID,SoftLib_ID) \
					);";
	CDBManager::m_db.ExecDML(strsql.c_str());

	//strsql = "ALTER TABLE SoftLibFile;		ADD CONSTRAINT pk_LibID PRIMARY KEY (FileID,SoftLib_ID);";
	//CDBManager::m_db.ExecDML(strsql.c_str());
}
void InstallsCaseDB::CreateTableInstallPlan()
{INTERFUN;
	string strsql = "CREATE TABLE InstallPlan(\
					PlanID       INTEGER       PRIMARY KEY NOT NULL ,\
					Category				VARCHAR(1024)  NOT NULL ,\
					PlanName				VARCHAR(1024) ,\
					SoftNum					INTEGER ,\
					TotalSize					FLOAT ,\
					PlanInstallTime					FLOAT ,\
					InstallNum					INTEGER ,\
					LastUseTime				DATETIME);";
	CDBManager::m_db.ExecDML(strsql.c_str());
}
void InstallsCaseDB::CreateTableInstallPlanFile()
{INTERFUN;
	string strsql = "CREATE TABLE InstallPlanFile(\
					FileID       INTEGER       NOT NULL ,\
					Plan_ID				INTEGER  NOT NULL ,\
					Name				VARCHAR(1024) ,\
					Path					VARCHAR(1024) ,\
					Size					FLOAT ,\
					Version					VARCHAR(1024) ,\
					Format					VARCHAR(1024) ,\
					SupportDevice				VARCHAR(1024),\
					AppName				VARCHAR(1024),\
					InstallTime				INTEGER,\
					LastWriteTime				DATETIME,\
					IsLegalSoft				VARCHAR,\
					Buyer				VARCHAR(1024),\
					PhoneCategory				VARCHAR(1024),\
					FBundleVersion				VARCHAR(1024),\
					VersionCode				INTEGER,\
					SDKVersion				VARCHAR(1024), \
					CONSTRAINT pk_PlanID PRIMARY KEY (FileID,Plan_ID) \
					);";
	CDBManager::m_db.ExecDML(strsql.c_str());

//	strsql = "ALTER TABLE InstallPlanFile;		ADD CONSTRAINT pk_PlanID PRIMARY KEY (FileID,Plan_ID);";
//		CDBManager::m_db.ExecDML(strsql.c_str());
}
void InstallsCaseDB::InitTable()
{INTERFUN;
	if(!CDBManager::m_db.TableExists("Profile") )
		CreateTableProfile();
	if(!CDBManager::m_db.TableExists("SoftLibFolder") )
		CreateTableSoftLibFolder();
	if(!CDBManager::m_db.TableExists("SoftLibFile") )
		CreateTableSoftLibFile();
	if(!CDBManager::m_db.TableExists("InstallPlanFile") )
		CreateTableInstallPlanFile();
	if(!CDBManager::m_db.TableExists("InstallPlan") )
		CreateTableInstallPlan();
}

//插入新建方案
void InstallsCaseDB::InsertNewCase(InstallsCaseData* casedata)
{INTERFUN;
	CString str_SQL;
	str_SQL.Format(L"insert into InstallPlan \
		values('%d','%s','%s','%d','%d','%d','%d','%d', '%d');",casedata->Id()-10,//为兼容c#版本数据库
		casedata->CaseType()==_INSTALLS_CASE_DATA_IOS?L"iOS":L"Android",
		casedata->CaseName().c_str(),casedata->GetAppNum(),casedata->GetSumFileSize(),
		0,casedata->InstallCount(),casedata->lastUseTime, 0);
	
	CDBManager::DoExecSql(str_SQL);
}
//方案表增加应用
void InstallsCaseDB::InsertCaseFile(InstallsCaseData* casedata,InstallsAppInfo* appdata)
{INTERFUN;
	//小于2的是库
	if(casedata->Id()<2)
	{
		InsertAppToLib(casedata,appdata);
		return;
	}
	CString str_SQL;
	//WaitForSingleObject(CDBManager::dealMutex, INFINITE);
	CDBManager::m_db.ExecDML("begin  transaction;");	

	str_SQL.Format(L"insert into InstallPlanFile \
				   values('%d','%d','%s','%s','%d','%s','%s','%s','%s','%d','%d','%d','%s','%s','%s','%d','%s');",
				   appdata->m_id,casedata->Id()-10,CStrOperation::ReplaceW(appdata->m_strName.c_str(), L"'", L"''").c_str(),
				   CStrOperation::ReplaceW(appdata->m_strSavePath.c_str(), L"'", L"''").c_str(),appdata->m_FileSize,
				   appdata->m_strVersion.c_str(),appdata->m_strFileType.c_str(),
				   appdata->m_SupportDevice.c_str(),appdata->m_strPackageID.c_str(),0,(long)appdata->m_tDownloadTime,
				   appdata->m_isLegalSoft,appdata->m_Purchaser.c_str(),appdata->m_strFileType==L"apk"?L"iOS":L"Android",
				   appdata->m_strVersion.c_str(),appdata->m_VerCode.c_str(),appdata->m_SDKVersion.c_str());
	CDBManager::DoExecSql(str_SQL.GetBuffer());

	str_SQL.Format(L"update InstallPlan set SoftNum=SoftNum+1,TotalSize=TotalSize+%d \
				   where PlanID='%d';",appdata->m_FileSize,casedata->Id()-10);

	CDBManager::DoExecSql(str_SQL.GetBuffer());
	CDBManager::m_db.ExecDML("commit transaction;");
	//ReleaseMutex(CDBManager::dealMutex);
}
//方案表移除应用
void InstallsCaseDB::RemoveCaseFile(InstallsCaseData* casedata,InstallsAppInfo* appdata)
{INTERFUN;
	if(casedata->Id()<2)
	{
		RemoveAppFromLib(casedata,appdata);
		return;
	}
	CString str_SQL;
	//WaitForSingleObject(CDBManager::dealMutex, INFINITE);
	CDBManager::m_db.ExecDML("begin  transaction;");	
	
	str_SQL.Format(L"delete from InstallPlanFile where FileID='%d' and Plan_ID='%d';"
		,appdata->m_id,casedata->Id()-10);
	CDBManager::DoExecSql(str_SQL.GetBuffer());
	str_SQL.Format(L"update InstallPlan set SoftNum=SoftNum-1,TotalSize=TotalSize-'%d' \
				   where PlanID='%d';",appdata->m_FileSize,casedata->Id()-10);
	CDBManager::DoExecSql(str_SQL.GetBuffer());
	CDBManager::m_db.ExecDML("commit transaction;");
	//ReleaseMutex(CDBManager::dealMutex);
}

//方案表移除应用
void InstallsCaseDB::RemoveAllCaseFile(InstallsCaseData* casedata)
{INTERFUN;
	CString str_SQL;
	//WaitForSingleObject(CDBManager::dealMutex, INFINITE);
	CDBManager::m_db.ExecDML("begin  transaction;");	

	str_SQL.Format(L"delete from InstallPlanFile where Plan_ID='%d';"
		,casedata->Id()-10);
	CDBManager::DoExecSql(str_SQL.GetBuffer());
	str_SQL.Format(L"update InstallPlan set SoftNum='0',TotalSize='0' \
					where PlanID='%d';",casedata->Id()-10);
	CDBManager::DoExecSql(str_SQL.GetBuffer());
	CDBManager::m_db.ExecDML("commit transaction;");
}

//移除方案
void InstallsCaseDB::RemoveCase(InstallsCaseData* casedata)
{INTERFUN;
	CString str_SQL;
	CDBManager::m_db.ExecDML("begin  transaction;");	
	str_SQL.Format(L"delete from InstallPlan where PlanID='%d';",casedata->Id()-10);
	CDBManager::DoExecSql(str_SQL.GetBuffer());
	str_SQL.Format(L"delete from InstallPlanFile where Plan_ID='%d';",casedata->Id()-10);
	CDBManager::DoExecSql(str_SQL.GetBuffer());
	CDBManager::m_db.ExecDML("commit transaction;");
}
//方案改名
void InstallsCaseDB::RenameCase(InstallsCaseData* casedata,wstring newName)
{INTERFUN;
	CString str_SQL;
	str_SQL.Format(L"update InstallPlan set PlanName='%s'  where PlanID='%d';",casedata->CaseName().c_str(),casedata->Id()-10);
	CDBManager::DoExecSql(str_SQL.GetBuffer());
}
//库增加文件
void InstallsCaseDB::InsertAppToLib(InstallsCaseData* casedata,InstallsAppInfo* appdata)
{INTERFUN;
	CString str_SQL;
//	WaitForSingleObject(CDBManager::dealMutex, INFINITE);
	CDBManager::m_db.ExecDML("begin  transaction;");	

	wstring type=casedata->CaseType()==_INSTALLS_CASE_DATA_IOS?L"iOS":L"Android";
	str_SQL.Format(L"insert into SoftLibFile \
					values('%d','%d','%s','%s','%d','%s','%s','%s', \
					'%s',0,'%d','%d','%s','%s','%s','%s','%s');",
					appdata->m_id,casedata->Id(),CStrOperation::ReplaceW(appdata->m_strName.c_str(), L"'", L"''").c_str(),
					CStrOperation::ReplaceW(appdata->m_strSavePath.c_str(), L"'", L"''").c_str(),appdata->m_FileSize,
					appdata->m_strVersion.c_str(),appdata->m_strFileType.c_str(),
					appdata->m_SupportDevice.c_str(),appdata->m_strPackageID.c_str()				
					,(long)appdata->m_tDownloadTime,
					appdata->m_isLegalSoft,appdata->m_Purchaser.c_str(),
					type.c_str(),
					appdata->m_strVersion.c_str(),appdata->m_VerCode.c_str(),appdata->m_SDKVersion.c_str());
	CDBManager::DoExecSql(str_SQL.GetBuffer());

	str_SQL.Format(L"update SoftLibFolder set SoftNum=SoftNum+1,TotalSize=TotalSize+%d \
					where Category='%s';",appdata->m_FileSize,casedata->CaseType()==_INSTALLS_CASE_DATA_IOS?L"iOS":L"Android");

	CDBManager::DoExecSql(str_SQL.GetBuffer());
	CDBManager::m_db.ExecDML("commit transaction;");
//	ReleaseMutex(CDBManager::dealMutex);
}
//库增移除文件
void InstallsCaseDB::RemoveAppFromLib(InstallsCaseData* casedata,InstallsAppInfo* appdata)
{INTERFUN;
	CString str_SQL;
//	WaitForSingleObject(CDBManager::dealMutex, INFINITE);
	CDBManager::m_db.ExecDML("begin  transaction;");	
	str_SQL.Format(L"delete from SoftLibFile where FileID='%d' and PhoneCategory='%s';"
		,appdata->m_id,casedata->CaseType()==_INSTALLS_CASE_DATA_IOS?L"iOS":L"Android");

	CDBManager::DoExecSql(str_SQL.GetBuffer());
	str_SQL.Format(L"update SoftLibFolder set SoftNum=SoftNum-1,TotalSize=TotalSize-%d \
					where SoftLibID='%d';",appdata->m_FileSize,casedata->Id());
	CDBManager::DoExecSql(str_SQL.GetBuffer());
	CDBManager::m_db.ExecDML("commit transaction;");
//	ReleaseMutex(CDBManager::dealMutex);
}
void InstallsCaseDB::LoadData(InstallsCaseManager* caseManager)
{INTERFUN;
	caseManager->clearlist();

	CppSQLite3Query q_Ret;
	CString str_SQL;
	str_SQL.Format(L"select * from SoftLibFolder;");
	if(!CDBManager::DB_QUERY_DATA(str_SQL,q_Ret))
	{
		return;
	}	
	while(!q_Ret.Eof())
	{
		if(Charset::UTF8ToUnicode(q_Ret.GetStringField(2)) == L"iOS")
		{
			caseManager->InstallsCaseDataIOS->CaseName(Charset::UTF8ToUnicode(q_Ret.GetStringField(1)));
			caseManager->InstallsCaseDataIOS->SetFolderPath(Charset::UTF8ToUnicode(q_Ret.GetStringField(3)));
			caseManager->InstallsCaseDataIOS->CaseType(_INSTALLS_CASE_DATA_IOS);
		}
		else//只有2个 
		{
			caseManager->InstallsCaseDataAndroid->CaseName(Charset::UTF8ToUnicode(q_Ret.GetStringField(1)));
			caseManager->InstallsCaseDataAndroid->SetFolderPath(Charset::UTF8ToUnicode(q_Ret.GetStringField(3)));
			caseManager->InstallsCaseDataAndroid->CaseType(_INSTALLS_CASE_DATA_ANDROID);
		}
		 q_Ret.NextRow();
	}
	LoadLibApps(caseManager->InstallsCaseDataIOS);
	LoadLibApps(caseManager->InstallsCaseDataAndroid);

	str_SQL.Format(L"select * from InstallPlan where plantype=0;");
		if(!CDBManager::DB_QUERY_DATA(str_SQL,q_Ret))
		{
			return;
		}	
		while(!q_Ret.Eof())
		{
			InstallsCaseData *inscasedata = new InstallsCaseData(q_Ret.GetIntField(0)+10);//+10代表是方案，这里为了与c#版本兼容所做设置
			if(Charset::UTF8ToUnicode(q_Ret.GetStringField(1)) == L"iOS")
				inscasedata->CaseType(_INSTALLS_CASE_DATA_IOS);
			else
				inscasedata->CaseType(_INSTALLS_CASE_DATA_ANDROID);
			inscasedata->CaseName(Charset::UTF8ToUnicode(q_Ret.GetStringField(2)).c_str());
			LoadCaseApps(inscasedata);
			caseManager->AddCaseData(inscasedata);
			q_Ret.NextRow();
		}	
}

void InstallsCaseDB::LoadCaseApps(InstallsCaseData* casedata)
{INTERFUN;
	CppSQLite3Query q_Ret;
	CString str_SQL;
	str_SQL.Format(L"select * from InstallPlanFile where Plan_ID=%d;"
		,casedata->Id()-10);
		if(!CDBManager::DB_QUERY_DATA(str_SQL,q_Ret))
		{
			return;
		}	
		while(!q_Ret.Eof())
		{
			int mid=q_Ret.GetIntField(0);
			
			InstallsAppInfo *appdata = new InstallsAppInfo(mid);
			appdata->m_strName = Charset::UTF8ToUnicode(q_Ret.GetStringField(2)).c_str();
			appdata->m_strSavePath=Charset::UTF8ToUnicode(q_Ret.GetStringField(3)).c_str();
			appdata->m_FileSize = q_Ret.GetFloatField(4);
			appdata->m_strVersion=Charset::UTF8ToUnicode(q_Ret.GetStringField(5)).c_str();
			appdata->m_strFileType=Charset::UTF8ToUnicode(q_Ret.GetStringField(6)).c_str();
			appdata->m_SupportDevice = Charset::UTF8ToUnicode(q_Ret.GetStringField(7)).c_str();
			appdata->m_strPackageID = Charset::UTF8ToUnicode(q_Ret.GetStringField(8)).c_str();
			//InstallTime
			appdata->m_tDownloadTime = q_Ret.GetFloatField(10);
			appdata->m_isLegalSoft = q_Ret.GetIntField(11);
			appdata->m_Purchaser  = Charset::UTF8ToUnicode(q_Ret.GetStringField(12)).c_str();
			//PhoneCategory
			//FBundleVersion
			appdata->m_VerCode = q_Ret.GetIntField(15);
			appdata->m_VerCode = q_Ret.GetIntField(15);
			appdata->m_Iconpath= DataPath::GetDataDir();//PublicFunc::GetDocmentDataPath();
			appdata->m_Iconpath+=L"\\icon\\";
			appdata->m_Iconpath+=UIStringFormat::CheckDownLoadFileName(appdata->m_strName);
			appdata->m_Iconpath+=UIStringFormat::CheckDownLoadFileName(appdata->m_strVersion);
			appdata->m_Iconpath+=CFileOperation::GetFileExt(appdata->m_strSavePath);
			appdata->m_Iconpath+=_T(".png");
			appdata->m_SDKVersion  = Charset::UTF8ToUnicode(q_Ret.GetStringField(16)).c_str();
			casedata->AddAppToList(appdata);
			q_Ret.NextRow();
		}	
}
void InstallsCaseDB::LoadLibApps(InstallsCaseData* casedata)
{INTERFUN;
	CppSQLite3Query q_Ret;
	CString str_SQL;
	str_SQL.Format(L"select * from SoftLibFile where PhoneCategory='%s';"
		,casedata->CaseType()==_INSTALLS_CASE_DATA_IOS?L"iOS":L"Android");
	if(!CDBManager::DB_QUERY_DATA(str_SQL,q_Ret))
	{
		return;
	}	
	while(!q_Ret.Eof())
	{
		int mid=q_Ret.GetIntField(0);
		
		InstallsAppInfo *appdata = new InstallsAppInfo(mid);
		appdata->m_strName = Charset::UTF8ToUnicode(q_Ret.GetStringField(2));
		appdata->m_strSavePath=Charset::UTF8ToUnicode(q_Ret.GetStringField(3));
		appdata->m_FileSize = q_Ret.GetFloatField(4);
		appdata->m_strVersion=Charset::UTF8ToUnicode(q_Ret.GetStringField(5));
		appdata->m_strFileType=Charset::UTF8ToUnicode(q_Ret.GetStringField(6));
		appdata->m_SupportDevice = Charset::UTF8ToUnicode(q_Ret.GetStringField(7));
		appdata->m_strPackageID = Charset::UTF8ToUnicode(q_Ret.GetStringField(8));
		//InstallTime
		appdata->m_tDownloadTime = q_Ret.GetFloatField(10);
		appdata->m_isLegalSoft = q_Ret.GetIntField(11);
		appdata->m_Purchaser  = Charset::UTF8ToUnicode(q_Ret.GetStringField(12));
		//PhoneCategory
		//FBundleVersion
		appdata->m_VerCode = q_Ret.GetIntField(15);
		appdata->m_Iconpath= DataPath::GetDataDir();//PublicFunc::GetDocmentDataPath();
		appdata->m_Iconpath+=L"\\icon\\";
		appdata->m_Iconpath+=UIStringFormat::CheckDownLoadFileName(appdata->m_strName);
		appdata->m_Iconpath+=UIStringFormat::CheckDownLoadFileName(appdata->m_strVersion);
		appdata->m_Iconpath+=CFileOperation::GetFileExt(appdata->m_strSavePath);
		appdata->m_Iconpath+=_T(".png");
		appdata->m_SDKVersion  = Charset::UTF8ToUnicode(q_Ret.GetStringField(16));
		casedata->AddAppToList(appdata);
		q_Ret.NextRow();
	}	
}

INSTALLCASE_END