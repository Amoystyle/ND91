#include "stdafx.h"
#include "InstallsCaseDBEx.h"
//#include "InstallCaseManagerEx.h"
#define  DB_FILE_NAME L"\\91Distributor\\91Distributor.db"

INSTALLCASE_BEGIN

#pragma warning(disable:4244)
InstallsCaseDBEx::InstallsCaseDBEx()
{INTERFUN;
	idFix = 4;
	needReAnalyzeApps = false;
	wstring filePath = DataPath::GetDataDir();
	CFileOperation::MakeLocalDir(filePath+L"\\91Distributor");

	CDBManager::GetInstance();

	wstring m_dbName = filePath + DB_FILE_NAME;
	if(!CDBManager::m_isconnect)
		CDBManager::InitDB(m_dbName.c_str());
	InitTable();

}

InstallsCaseDBEx::~InstallsCaseDBEx()
{INTERFUN;
	
}

void InstallsCaseDBEx::InitTable()
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
	this->UpdateTableInstallPlanFile();
	this->UpdateTableSoftLibFile();
	this->UpdateTableInstallPlan();
	this->UpdateTableInstallPlanFile2();
	this->UpdateTableSoftLibFile2();
	this->UpdateTableInstallPlanFile3();
	this->UpdateTableSoftLibFile3();
}

void InstallsCaseDBEx::CreateTableProfile()
{INTERFUN;
	string strsql = "CREATE TABLE Profile(SeachDefault				INTEGER);";
	CDBManager::DoExecSql(strsql.c_str());
}

void InstallsCaseDBEx::CreateTableSoftLibFolder()
{INTERFUN;
	string strsql = "CREATE TABLE SoftLibFolder(\
					SoftLibID       INTEGER       PRIMARY KEY NOT NULL ,\
					SoftLibName				VARCHAR(1024)  NOT NULL ,\
					Category				VARCHAR(1024) ,\
					FolderPath				VARCHAR(1024) ,\
					SoftNum					INTEGER ,\
					TotalSize				FLOAT);";

	CDBManager::DoExecSql(strsql.c_str());

	
	wstring MyDocment= DataPath::GetDataDir();
	wstring pathIOS = CONFIG(_strIosDownPath);
	wstring pathAndroid = CONFIG(_strAndroidDownPath);

	strsql = "insert into SoftLibFolder values(0,'";
	strsql+=Charset::UnicodeToUTF8(CommonUIFunc::LoadString(L"InstallCase_Text_SoftWareLibiOS"));
	strsql+="','iOS','";
	strsql+=Charset::UnicodeToUTF8(pathIOS.c_str());
	strsql+="',0,0);";
	CDBManager::DoExecSql(strsql.c_str());

	strsql = "insert into SoftLibFolder values(1,'";
	strsql+=Charset::UnicodeToUTF8(CommonUIFunc::LoadString(L"InstallCase_Text_SoftWareLibAndroid"));
	strsql+="','Android','";
	strsql+=Charset::UnicodeToUTF8(pathAndroid.c_str());
	strsql+="',0,0);";
	CDBManager::DoExecSql(strsql.c_str());
}

void InstallsCaseDBEx::CreateTableSoftLibFile()
{INTERFUN;
	string strsql = "Create				TABLE MAIN.[SoftLibFile](\
					[FileID]			INTEGER PRIMARY KEY AUTOINCREMENT,\
					[SoftLib_ID]		INTEGER NOT NULL,\
					[Name]				VARCHAR(1024),\
					[Path]				VARCHAR(1024),\
					[Size]				FLOAT,\
					[Version]			VARCHAR(1024),\
					[Format]			VARCHAR(1024),\
					[SupportDevice]		VARCHAR(1024),\
					[AppName]			VARCHAR(1024),\
					[InstallTime]		INTEGER,\
					[LastWriteTime]		DATETIME,\
					[IsLegalSoft]		VARCHAR,\
					[Buyer]				VARCHAR(1024),\
					[PhoneCategory]		VARCHAR(1024),\
					[FBundleVersion]	VARCHAR(1024),\
					[VersionCode]		INTEGER,\
					[SDKVersion]		VARCHAR(1024),\
					[FID]				INTEGER,\
					[ItemID]			INTEGER);";
	CDBManager::DoExecSql(strsql.c_str());
}

void InstallsCaseDBEx::CreateTableInstallPlan()
{INTERFUN;
	string strsql = "CREATE TABLE InstallPlan(\
					PlanID				INTEGER       PRIMARY KEY NOT NULL ,\
					Category			VARCHAR(1024)  NOT NULL ,\
					PlanName			VARCHAR(1024) ,\
					SoftNum				INTEGER ,\
					TotalSize			FLOAT ,\
					PlanInstallTime		FLOAT ,\
					InstallNum			INTEGER ,\
					LastUseTime			DATETIME ,\
					PlanType			INTEGER ,\
					IsLegal				INTEGER DEFAULT 0);";

	CDBManager::DoExecSql(strsql.c_str());
}

void InstallsCaseDBEx::CreateTableInstallPlanFile()
{INTERFUN;
	string strsql = "Create  TABLE MAIN.[InstallPlanFile](\
					[FileID]			INTEGER PRIMARY KEY AUTOINCREMENT,\
					[Plan_ID]			INTEGER NOT NULL,\
					[Name]				VARCHAR(1024),\
					[Path]				VARCHAR(1024),\
					[Size]				FLOAT,\
					[Version]			VARCHAR(1024),\
					[Format]			VARCHAR(1024),\
					[SupportDevice]		VARCHAR(1024),\
					[AppName]			VARCHAR(1024),\
					[InstallTime]		INTEGER,\
					[LastWriteTime]		DATETIME,\
					[IsLegalSoft]		VARCHAR,\
					[Buyer]				VARCHAR(1024),\
					[PhoneCategory]		VARCHAR(1024),\
					[FBundleVersion]	VARCHAR(1024),\
					[VersionCode]		INTEGER,\
					[SDKVersion]		VARCHAR(1024),\
					[FID]				INTEGER,\
					[ItemID]			INTEGER);";
	CDBManager::DoExecSql(strsql.c_str());
}

void InstallsCaseDBEx::UpdateTableInstallPlanFile()
{INTERFUN;
	string strsql = "SELECT COUNT(*) FROM sqlite_master where type='table' and name='InstallPlanFile' and sql like '%FID%';";
	int ret = CDBManager::m_db.ExecScalar(strsql.c_str());
	if(ret == 0)
	{
		strsql = "ALTER TABLE InstallPlanFile ADD COLUMN FID INTEGER DEFAULT 0;";
		CDBManager::DoExecSql(strsql.c_str());
	}
}

void InstallsCaseDBEx::UpdateTableSoftLibFile()
{INTERFUN;
	string strsql = "SELECT COUNT(*) FROM sqlite_master where type='table' and name='SoftLibFile' and sql like '%FID%';";
	int ret = CDBManager::m_db.ExecScalar(strsql.c_str());
	if(ret == 0)
	{
		strsql = "ALTER TABLE SoftLibFile ADD COLUMN FID INTEGER DEFAULT 0;";
		CDBManager::DoExecSql(strsql.c_str());
	}
}

void InstallsCaseDBEx::UpdateTableInstallPlanFile2()
{INTERFUN;
	string strsql = "SELECT COUNT(*) FROM sqlite_master where type='table' and name='InstallPlanFile' and sql like '%CONSTRAINT pk_PlanID%';";
	int ret = CDBManager::m_db.ExecScalar(strsql.c_str());
	if(ret>0)
	{
		strsql="Begin Transaction;\
				Create  TABLE MAIN.[InstallPlanFileTemp](\
				[FileID]			INTEGER PRIMARY KEY AUTOINCREMENT,\
				[Plan_ID]			INTEGER NOT NULL,\
				[Name]				VARCHAR(1024),\
				[Path]				VARCHAR(1024),\
				[Size]				FLOAT,\
				[Version]			VARCHAR(1024),\
				[Format]			VARCHAR(1024),\
				[SupportDevice]		VARCHAR(1024),\
				[AppName]			VARCHAR(1024),\
				[InstallTime]		INTEGER,\
				[LastWriteTime]		DATETIME,\
				[IsLegalSoft]		VARCHAR,\
				[Buyer]				VARCHAR(1024),\
				[PhoneCategory]		VARCHAR(1024),\
				[FBundleVersion]	VARCHAR(1024),\
				[VersionCode]		INTEGER,\
				[SDKVersion]		VARCHAR(1024),\
				[FID]				INTEGER);\
				Insert Into MAIN.[InstallPlanFileTemp] ([Plan_ID],[Name],[Path],[Size],[Version],[Format],[SupportDevice],[AppName],\
				[InstallTime],[LastWriteTime],[IsLegalSoft],[Buyer],[PhoneCategory],[FBundleVersion],[VersionCode],[SDKVersion],[FID])\
				Select [Plan_ID],[Name],[Path],[Size],[Version],[Format],[SupportDevice],[AppName],[InstallTime],[LastWriteTime],\
				[IsLegalSoft],[Buyer],[PhoneCategory],[FBundleVersion],[VersionCode],[SDKVersion],[FID] From MAIN.[InstallPlanFile];\
				Drop Table MAIN.[InstallPlanFile];\
				Alter Table MAIN.[InstallPlanFileTemp] Rename To [InstallPlanFile];\
				Commit Transaction;";//ItemID为后加列，故这里不做数据迁移，也不更新（若更新后添加此列，后面无法判断是否需要重新解析文件以获取ItemID字段）
		CDBManager::DoExecSql(strsql.c_str());
	}
}

void InstallsCaseDBEx::UpdateTableSoftLibFile2()
{INTERFUN;
	string strsql = "SELECT COUNT(*) FROM sqlite_master where type='table' and name='SoftLibFile' and sql like '%CONSTRAINT pk_LibID%';";
	int ret = CDBManager::m_db.ExecScalar(strsql.c_str());
	if(ret>0)
	{
		strsql="Begin Transaction;\
				Create				TABLE MAIN.[SoftLibFileTemp](\
				[FileID]			INTEGER PRIMARY KEY AUTOINCREMENT,\
				[SoftLib_ID]		INTEGER NOT NULL,\
				[Name]				VARCHAR(1024),\
				[Path]				VARCHAR(1024),\
				[Size]				FLOAT,\
				[Version]			VARCHAR(1024),\
				[Format]			VARCHAR(1024),\
				[SupportDevice]		VARCHAR(1024),\
				[AppName]			VARCHAR(1024),\
				[InstallTime]		INTEGER,\
				[LastWriteTime]		DATETIME,\
				[IsLegalSoft]		VARCHAR,\
				[Buyer]				VARCHAR(1024),\
				[PhoneCategory]		VARCHAR(1024),\
				[FBundleVersion]	VARCHAR(1024),\
				[VersionCode]		INTEGER,\
				[SDKVersion]		VARCHAR(1024),\
				[FID]				INTEGER);\
				Insert Into MAIN.[SoftLibFileTemp] ([SoftLib_ID],[Name],[Path],[Size],[Version],[Format],[SupportDevice],[AppName],\
				[InstallTime],[LastWriteTime],[IsLegalSoft],[Buyer],[PhoneCategory],[FBundleVersion],[VersionCode],[SDKVersion],[FID])\
				Select [SoftLib_ID],[Name],[Path],[Size],[Version],[Format],[SupportDevice],[AppName],[InstallTime],[LastWriteTime],\
				[IsLegalSoft],[Buyer],[PhoneCategory],[FBundleVersion],[VersionCode],[SDKVersion],[FID] From MAIN.[SoftLibFile];\
				Drop Table MAIN.[SoftLibFile];\
				Alter Table MAIN.[SoftLibFileTemp] Rename To [SoftLibFile];\
				Commit Transaction;";//ItemID为后加列，故这里不做数据迁移，也不更新（若更新后添加此列，后面无法判断是否需要重新解析文件以获取ItemID字段）
		CDBManager::DoExecSql(strsql.c_str());
	}
}

void InstallsCaseDBEx::UpdateTableInstallPlanFile3()
{INTERFUN;
	string strsql = "SELECT COUNT(*) FROM sqlite_master where type='table' and name='InstallPlanFile' and sql like '%ItemID%';";
	int ret = CDBManager::m_db.ExecScalar(strsql.c_str());
	if(ret == 0)
	{
		needReAnalyzeApps = true;
		strsql = "ALTER TABLE InstallPlanFile ADD COLUMN ItemID INTEGER DEFAULT 0;";
		CDBManager::DoExecSql(strsql.c_str());
	}
}

void InstallsCaseDBEx::UpdateTableSoftLibFile3()
{INTERFUN;
	string strsql = "SELECT COUNT(*) FROM sqlite_master where type='table' and name='SoftLibFile' and sql like '%ItemID%';";
	int ret = CDBManager::m_db.ExecScalar(strsql.c_str());
	if(ret == 0)
	{
		strsql = "ALTER TABLE SoftLibFile ADD COLUMN ItemID INTEGER DEFAULT 0;";
		CDBManager::DoExecSql(strsql.c_str());
	}
}
void InstallsCaseDBEx::UpdateTableInstallPlan()
{INTERFUN;
	string strsql = "SELECT COUNT(*) FROM sqlite_master where type='table' and name='InstallPlan' and sql like '%IsLegal%';";
	int ret = CDBManager::m_db.ExecScalar(strsql.c_str());
	if(ret == 0)
	{
		strsql = "ALTER TABLE InstallPlan ADD COLUMN IsLegal INTEGER DEFAULT 0;";
		CDBManager::DoExecSql(strsql.c_str());
	}
}

void InstallsCaseDBEx::LoadData(InstallCaseManagerEx* caseManager)
{INTERFUN;
	//加载软件库文件至系统装机方案（1、加载SoftLibFolder;2、加载SoftLibFile（方法LoadLibApps））
	CppSQLite3Query q_Ret;
	CString str_SQL;
	str_SQL.Format(L"select * from SoftLibFolder;");
	if(!CDBManager::DB_QUERY_DATA(str_SQL,q_Ret))
	{
		return;
	}	
	while(!q_Ret.Eof())
	{
		int id = q_Ret.GetIntField(0) + 2;
		wstring caseName = Charset::UTF8ToUnicode(q_Ret.GetStringField(1));
		wstring folderPath = Charset::UTF8ToUnicode(q_Ret.GetStringField(3));
		_INSTALLS_CASE_DATA_TYPE caseType;

		if(Charset::UTF8ToUnicode(q_Ret.GetStringField(2)) == L"iOS")
		{
			caseType = _INSTALLS_CASE_DATA_IOS;
		}
		else//只有2个 
		{
			caseType = _INSTALLS_CASE_DATA_ANDROID;
		}

		InstallsCaseData* caseData = new InstallsCaseData(id);
		caseData->CaseName(caseName);
		caseData->CaseType(caseType);
		caseData->PlanType(_INSTALL_PLAN_TYPE_SYS);
		caseData->SetFolderPath(folderPath);
		caseManager->InstallCaseDataList.push_back(caseData);

		LoadLibApps(caseData);

		 q_Ret.NextRow();
	}

	//加载装机方案文件至用户装机方案（1、加载InstallPlan;2、加载InstallPlanFile（方法LoadCaseApps））
	str_SQL.Format(L"select * from InstallPlan where plantype=3;");
		if(!CDBManager::DB_QUERY_DATA(str_SQL,q_Ret))
		{
			return;
		}	
		while(!q_Ret.Eof())
		{
			int id = q_Ret.GetIntField(0) + idFix;
			wstring caseName = Charset::UTF8ToUnicode(q_Ret.GetStringField(2)).c_str();
			_INSTALLS_CASE_DATA_TYPE caseType;
			
			if(Charset::UTF8ToUnicode(q_Ret.GetStringField(1)) == L"iOS")
				caseType = _INSTALLS_CASE_DATA_IOS;
			else
				caseType = _INSTALLS_CASE_DATA_ANDROID;

			wstring countStr = Charset::UTF8ToUnicode(q_Ret.GetStringField(6));
			int count = 0;
			if(!countStr.empty())
				count = _wtoi(countStr.c_str());

			wstring timeStr = Charset::UTF8ToUnicode(q_Ret.GetStringField(7));
			time_t lastTime = 0;
			if(!timeStr.empty())
				lastTime = (int)_wtoi(timeStr.c_str());

			InstallsCaseData* caseData = new InstallsCaseData(id);
			caseData->CaseName(caseName);
			caseData->CaseType(caseType);
			caseData->lastUseTime = lastTime;
			caseData->InstallCount(count);
			caseData->PlanType(_INSTALL_PLAN_TYPE_USER);
			caseManager->InstallCaseDataList.push_back(caseData);

			LoadCaseApps(caseData);
			q_Ret.NextRow();
		}	
}

void InstallsCaseDBEx::LoadLibApps(InstallsCaseData* casedata)
{INTERFUN;
	CppSQLite3Query q_Ret;
	CString str_SQL;
	str_SQL.Format(L"select * from SoftLibFile where PhoneCategory='%s' order by LastWriteTime desc;"
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
		appdata->m_VerCode = CStrOperation::IntToWString(q_Ret.GetIntField(15));
		appdata->m_Iconpath= DataPath::GetDataDir();//PublicFunc::GetDocmentDataPath();
		appdata->m_Iconpath+=L"\\icon\\";
		appdata->m_Iconpath+=UIStringFormat::CheckDownLoadFileName(appdata->m_strName);
		appdata->m_Iconpath+=UIStringFormat::CheckDownLoadFileName(appdata->m_strVersion);
		appdata->m_Iconpath+=CFileOperation::GetFileExt(appdata->m_strSavePath);
		appdata->m_Iconpath+=_T(".png");
		appdata->m_SDKVersion  = Charset::UTF8ToUnicode(q_Ret.GetStringField(16));
		appdata->m_fid = q_Ret.GetIntField(17);
		appdata->m_itemID = q_Ret.GetIntField(18);
		casedata->AddAppToList(appdata);

		InstallCaseManagerEx::GetInstance()->AddFolder(appdata, casedata->CaseType());

		q_Ret.NextRow();
	}	
}

void InstallsCaseDBEx::LoadCaseApps(InstallsCaseData* casedata)
{INTERFUN;
	CppSQLite3Query q_Ret;
	CString str_SQL;
	str_SQL.Format(L"select * from InstallPlanFile where Plan_ID=%d order by LastWriteTime desc"
		,casedata->Id()-idFix);
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
			appdata->m_VerCode = CStrOperation::IntToWString(q_Ret.GetIntField(15));
			appdata->m_Iconpath= DataPath::GetDataDir();//PublicFunc::GetDocmentDataPath();
			appdata->m_Iconpath+=L"\\icon\\";
			appdata->m_Iconpath+=UIStringFormat::CheckDownLoadFileName(appdata->m_strName);
			appdata->m_Iconpath+=UIStringFormat::CheckDownLoadFileName(appdata->m_strVersion);
			appdata->m_Iconpath+=CFileOperation::GetFileExt(appdata->m_strSavePath);
			appdata->m_Iconpath+=_T(".png");
			appdata->m_SDKVersion  = Charset::UTF8ToUnicode(q_Ret.GetStringField(16)).c_str();
			appdata->m_fid = q_Ret.GetIntField(17);
			appdata->m_itemID = q_Ret.GetIntField(18);
			casedata->AddAppToList(appdata);
			q_Ret.NextRow();
		}
}

//插入新建方案
void InstallsCaseDBEx::InsertNewCase(InstallsCaseData* casedata)
{INTERFUN;
	CString str_SQL;
	str_SQL.Format(L"Insert into InstallPlan(PlanId,Category,PlanName,SoftNum,TotalSize,LastUseTime,PlanInstallTime,InstallNum,plantype) \
				   values('%d','%s','%s','%d','%d','%d',0,0,3);",casedata->Id()-idFix,//为兼容c#版本数据库
					casedata->CaseType()==_INSTALLS_CASE_DATA_IOS?L"iOS":L"Android",
					casedata->CaseName().c_str(),casedata->GetAppNum(),casedata->GetSumFileSize(),
					casedata->lastUseTime);

	CDBManager::DoExecSql(str_SQL);
}

//方案表增加应用
void InstallsCaseDBEx::InsertCaseFile(InstallsCaseData* casedata,InstallsAppInfo* appdata)
{INTERFUN;
	CString str_SQL;
	CDBManager::DoExecSql("begin  transaction;");	

	int caseId = casedata->Id() - idFix;

	wstring phoneType;
	if(appdata->m_strFileType==L"pxl" || appdata->m_strFileType==L"ipa")
		phoneType = L"iOS";
	else
		phoneType = L"Android";

	str_SQL.Format(L"Insert Into InstallPlanFile(\
					Plan_ID, Name, Path, Size, \
					Version, Format, SupportDevice, AppName, \
					InstallTime, LastWriteTime, IsLegalSoft, \
					Buyer, PhoneCategory, FBundleVersion, \
					VersionCode, SDKVersion, FID, ItemID) \
					Values('%d','%s','%s','%d','%s','%s','%s','%s','%d','%d','%d','%s','%s','%s','%d','%s','%d','%d')",
		caseId,
		CStrOperation::ReplaceW(appdata->m_strName.c_str(), L"'", L"''").c_str(),
		CStrOperation::ReplaceW(appdata->m_strSavePath.c_str(), L"'", L"''").c_str(),
		appdata->m_FileSize,
		appdata->m_strVersion.c_str(),
		appdata->m_strFileType.c_str(),
		appdata->m_SupportDevice.c_str(),
		appdata->m_strPackageID.c_str(),
		0,
		(long)appdata->m_tDownloadTime,
		appdata->m_isLegalSoft,
		appdata->m_Purchaser.c_str(),
		phoneType.c_str(),
		appdata->m_strVersion.c_str(),
		_wtoi(appdata->m_VerCode.c_str()),
		appdata->m_SDKVersion.c_str(),
		appdata->m_fid,
		appdata->m_itemID);

	CDBManager::DoExecSql(str_SQL.GetBuffer());
	appdata->m_id = (UINT)CDBManager::m_db.LastRowId();
	str_SQL.Format(L"update InstallPlan set SoftNum=SoftNum+1,TotalSize=TotalSize+%d \
					where PlanID='%d';",appdata->m_FileSize,casedata->Id()-idFix);

	CDBManager::DoExecSql(str_SQL.GetBuffer());
	CDBManager::DoExecSql("commit transaction;");
}

//方案表移除应用
void InstallsCaseDBEx::RemoveCaseFile(InstallsCaseData* casedata,InstallsAppInfo* appdata)
{INTERFUN;
	CString str_SQL;
	//WaitForSingleObject(CDBManager::dealMutex, INFINITE);
	CDBManager::DoExecSql("begin  transaction;");	

	str_SQL.Format(L"delete from InstallPlanFile where FileID='%d' and Plan_ID='%d';"
		,appdata->m_id,casedata->Id()-idFix);
	CDBManager::DoExecSql(str_SQL.GetBuffer());
	str_SQL.Format(L"update InstallPlan set SoftNum=SoftNum-1,TotalSize=TotalSize-'%d' \
					where PlanID='%d';",appdata->m_FileSize,casedata->Id()-idFix);
	CDBManager::DoExecSql(str_SQL.GetBuffer());
	CDBManager::DoExecSql("commit transaction;");
}

//方案表移除应用
void InstallsCaseDBEx::RemoveAllCaseFile(InstallsCaseData* casedata)
{INTERFUN;
	CString str_SQL;
	//WaitForSingleObject(CDBManager::dealMutex, INFINITE);
	CDBManager::DoExecSql("begin  transaction;");	

	str_SQL.Format(L"delete from InstallPlanFile where Plan_ID='%d';"
		,casedata->Id()-idFix);
	CDBManager::DoExecSql(str_SQL.GetBuffer());
	str_SQL.Format(L"update InstallPlan set SoftNum='0',TotalSize='0' \
					where PlanID='%d';",casedata->Id()-idFix);
	CDBManager::DoExecSql(str_SQL.GetBuffer());
	CDBManager::DoExecSql("commit transaction;");
}

//移除方案
void InstallsCaseDBEx::RemoveCase(InstallsCaseData* casedata)
{INTERFUN;
	CString str_SQL;
	CDBManager::DoExecSql("begin  transaction;");
	str_SQL.Format(L"delete from InstallPlan where PlanID='%d';",casedata->Id()-idFix);
	CDBManager::DoExecSql(str_SQL.GetBuffer());
	str_SQL.Format(L"delete from InstallPlanFile where Plan_ID='%d';",casedata->Id()-idFix);
	CDBManager::DoExecSql(str_SQL.GetBuffer());
	CDBManager::DoExecSql("commit transaction;");
}

//方案改名
void InstallsCaseDBEx::RenameCase(InstallsCaseData* casedata,wstring newName)
{INTERFUN;
	CString str_SQL;
	str_SQL.Format(L"update InstallPlan set PlanName='%s' where PlanID='%d';",newName.c_str(), casedata->Id()-idFix);
	CDBManager::DoExecSql(str_SQL.GetBuffer());
}

//方案改名
void InstallsCaseDBEx::UpdateCase(InstallsCaseData* casedata)
{INTERFUN;
	CString str_SQL;
	str_SQL.Format(L"update InstallPlan set InstallNum = '%d', LastUseTime='%d' where PlanID='%d';", casedata->InstallCount(), (long)casedata->lastUseTime, casedata->Id()-idFix);
	CDBManager::DoExecSql(str_SQL.GetBuffer());
}

//库增加文件
void InstallsCaseDBEx::InsertAppToLib(InstallsCaseData* casedata,InstallsAppInfo* appdata)
{INTERFUN;
	CString str_SQL;
	CDBManager::DoExecSql("begin  transaction;");	

	wstring type=casedata->CaseType()==_INSTALLS_CASE_DATA_IOS?L"iOS":L"Android";

	str_SQL.Format(L"Insert Into SoftLibFile(\
					SoftLib_ID, Name, Path, Size, \
					Version, Format, SupportDevice, AppName, \
					InstallTime, LastWriteTime, IsLegalSoft, \
					Buyer, PhoneCategory, FBundleVersion, \
					VersionCode, SDKVersion,FID,ItemID) \
					Values('%d','%s','%s','%d','%s','%s','%s','%s','%d','%d','%d','%s','%s','%s','%d','%s','%d','%d')",
					casedata->Id() - 2,
					CStrOperation::ReplaceW(appdata->m_strName.c_str(), L"'", L"''").c_str(),
					CStrOperation::ReplaceW(appdata->m_strSavePath.c_str(), L"'", L"''").c_str(),
					appdata->m_FileSize,
					appdata->m_strVersion.c_str(),
					appdata->m_strFileType.c_str(),
					appdata->m_SupportDevice.c_str(),
					appdata->m_strPackageID.c_str(),
					0,
					(long)appdata->m_tDownloadTime,
					appdata->m_isLegalSoft,
					appdata->m_Purchaser.c_str(),
					type.c_str(),
					appdata->m_strVersion.c_str(),
					_wtoi(appdata->m_VerCode.c_str()),
					appdata->m_SDKVersion.c_str(),
					appdata->m_fid,
					appdata->m_itemID);
					

	CDBManager::DoExecSql(str_SQL.GetBuffer());
	appdata->m_id = CDBManager::m_db.LastRowId();
	str_SQL.Format(L"update SoftLibFolder set SoftNum=SoftNum+1,TotalSize=TotalSize+%d \
					where Category='%s';",appdata->m_FileSize,casedata->CaseType()==_INSTALLS_CASE_DATA_IOS?L"iOS":L"Android");

	CDBManager::DoExecSql(str_SQL.GetBuffer());
	CDBManager::DoExecSql("commit transaction;");
}

//库增移除文件
void InstallsCaseDBEx::RemoveAppFromLib(InstallsCaseData* casedata,InstallsAppInfo* appdata)
{INTERFUN;
	CString str_SQL;
	//	WaitForSingleObject(CDBManager::dealMutex, INFINITE);
	CDBManager::DoExecSql("begin  transaction;");	
	str_SQL.Format(L"delete from SoftLibFile where FileID='%d' and PhoneCategory='%s';"
		,appdata->m_id,casedata->CaseType()==_INSTALLS_CASE_DATA_IOS?L"iOS":L"Android");

	CDBManager::DoExecSql(str_SQL.GetBuffer());
	str_SQL.Format(L"update SoftLibFolder set SoftNum=SoftNum-1,TotalSize=TotalSize-%d \
					where SoftLibID='%d';",appdata->m_FileSize,casedata->Id() - 2);
	CDBManager::DoExecSql(str_SQL.GetBuffer());
	CDBManager::DoExecSql("commit transaction;");
	//	ReleaseMutex(CDBManager::dealMutex);
}

int InstallsCaseDBEx::GetCaseMaxId()
{INTERFUN;
	//0:ios升级方案
	//1:android升级方案
	int caseId = 0;
	wstring str = L"select max(planid) from InstallPlan";
	try
	{
		wstring strCount = L"select count(0) from InstallPlan";
		int nCount = CDBManager::m_db.ExecScalar(CCodeOperation::UnicodeToUTF_8(strCount).c_str());
		if (nCount > 0)
			caseId = CDBManager::m_db.ExecScalar(CCodeOperation::UnicodeToUTF_8(str).c_str());
	}
	catch(...)
	{
		caseId = 0;
	}
	caseId += idFix;
	caseId++;
	return caseId;
}

int InstallsCaseDBEx::GetAppMaxId()
{INTERFUN;
	int appId = 0;
	wstring str = L"select max(FileID) from SoftLibFile";
	try
	{
		wstring strCount = L"select count(0) from SoftLibFile";
		int nCount = CDBManager::m_db.ExecScalar(CCodeOperation::UnicodeToUTF_8(strCount).c_str());
		if (nCount > 0)
			appId = CDBManager::m_db.ExecScalar(CCodeOperation::UnicodeToUTF_8(str).c_str());
	}
	catch(...)
	{
		appId = 0;
	}
	appId++;
	return appId;
}

int InstallsCaseDBEx::GetInstallPlanFileMaxId()
{INTERFUN;
	int appId = 0;
	wstring str = L"select max(FileID) from InstallPlanFile";
	try
	{
		wstring strCount = L"select count(0) from InstallPlanFile";
		int nCount = CDBManager::m_db.ExecScalar(CCodeOperation::UnicodeToUTF_8(strCount).c_str());
		if (nCount > 0)
			appId = CDBManager::m_db.ExecScalar(CCodeOperation::UnicodeToUTF_8(str).c_str());
	}
	catch(...)
	{
		appId = 0;
	}
	appId++;
	return appId;
}

void InstallsCaseDBEx::UpdateItemIDByPath( wstring tableName, wstring path, int itemID )
{
	CString str_SQL;
	str_SQL.Format(L"update '%s' set ItemID = '%d' where Path='%d';", tableName, itemID, path);
	CDBManager::DoExecSql(str_SQL.GetBuffer());
}


INSTALLCASE_END