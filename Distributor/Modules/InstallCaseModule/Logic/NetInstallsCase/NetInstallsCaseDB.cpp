#include "stdafx.h"
#include "NetInstallsCaseDB.h"
#include "../InstallsCase/InstallCaseManagerEx.h"
#include "Common/NetOperation.h"
#include "Common/UserCenter/UserCenterMsg.h"
#pragma warning(disable:4244)

INSTALLCASE_BEGIN

int g_sPlantId = 100000;
vector<PackageData*> NetInstallsCaseDB::mNetPackageDataList;
vector<PackageData*> NetInstallsCaseDB::_vecNetCpaPackageDataList;
map<int,vector<SoftData*>> NetInstallsCaseDB::mNetPlanItems;
map<int,vector<SoftData*>> NetInstallsCaseDB::mNetCpaPlanItems;

bool lessPackageId(const InstallsCaseData* lh, const InstallsCaseData* rh)
{
	return lh->PackId<rh->PackId;
}
bool uniquePackageId(const InstallsCaseData* lh, const InstallsCaseData* rh)
{
	return lh->PackId==rh->PackId;
}
NetInstallsCaseDB::NetInstallsCaseDB()
{INTERFUN;
	wstring filePath = DataPath::GetDataDir();	
	wstring m_dbName = filePath + DB_FILE_NAME;
    CFileOperation::MakeLocalDir(CFileOperation::GetFileDir(m_dbName));
	if(!CDBManager::m_isconnect)
		CDBManager::InitDB(m_dbName.c_str());
	InitTable();
	InitData();

	InitializeCriticalSection(&_csLock);
}
NetInstallsCaseDB::~NetInstallsCaseDB()
{INTERFUN;

}

void NetInstallsCaseDB::SetPackageIcon()
{

	for(vector<InstallsCaseData*>::iterator iter=InstallCaseManagerEx::GetInstance()->NetInstallPlans.begin();
		iter!=InstallCaseManagerEx::GetInstance()->NetInstallPlans.end();++iter)
	{
		NetInstallsCaseData* tmp = (NetInstallsCaseData*)(*iter);
		if (NULL==tmp)
		{
			continue;
		}

		for(vector<PackageData*>::iterator it = mNetPackageDataList.begin();it!=mNetPackageDataList.end();it++)
		{
			if(tmp->PackId==(*it)->_nPackageId&&!(*it)->_ImgSrc.empty())
			{
				wstring SavePath = DataPath::GetDataDir();
				SavePath+=L"\\icon\\";
				SavePath+=CStrOperation::IntToWString(tmp->PackId);
				SavePath+=_T(".png");
				if(!CFileOperation::IsFileExist(SavePath.c_str()))
				{
					CNetOperation::DownloadFile((*it)->_ImgSrc,SavePath);
				}
				tmp->_wsPackegeIconpath = SavePath;
				break;
			}
		}

		//设置cpa的装机包图片
		for(vector<PackageData*>::iterator it = _vecNetCpaPackageDataList.begin();it!=_vecNetCpaPackageDataList.end();++it)
		{
			if(tmp->PackId==(*it)->_nPackageId&&!(*it)->_ImgSrc.empty())
			{
				wstring SavePath = DataPath::GetDataDir();
				SavePath+=L"\\icon\\";
				SavePath+=CStrOperation::IntToWString(tmp->PackId);
				SavePath+=_T(".png");
				if(!CFileOperation::IsFileExist(SavePath.c_str()))
				{
					CNetOperation::DownloadFile((*it)->_ImgSrc,SavePath);
				}
				tmp->_wsPackegeIconpath = SavePath;
				break;
			}
		}
	}

}


void NetInstallsCaseDB::LoadData(bool loadDataBase, int nUserId, bool bCpa)
{INTERFUN;
	SetCSLock(_csLock);
	LoadNetPlansFromWeb(nUserId, bCpa);
	if(loadDataBase)
		LoadNetInstallPlans(false);
	else
		InsertNetPlanItemToDB();

	LoadNetCpaInstallPlans();
	SetPackageIcon();
}

void NetInstallsCaseDB::InitTable()
{INTERFUN;
	if(!CDBManager::m_db.TableExists("NetInstallPlan") )
		CreateTableNetInstallPlan();
	if(!CDBManager::m_db.TableExists("NetInstallPlanSofts") )
		CreateTableNetInstallPlanSofts();
	UpdateTableInstallPlan();
}

void NetInstallsCaseDB::InitData()
{INTERFUN;
	CString strSql = "select B.*,A.packid,A.status from NetInstallPlan A,InstallPlan B where A.planid = B.planid order by A.status";
	if(!CDBManager::DB_QUERY_TABLEDATA(strSql,mDtPlans))
	{
		/*while(!mDtPlans.Eof())
		{
			NetInstallsCaseData* planItem = BuildNetInstallPlanItem(mDtPlans.GetIntField(0),
				Charset::UTF8ToUnicode(mDtPlans.GetStringField(2)),
				mDtPlans.GetFloatField(7),
				Charset::UTF8ToUnicode(mDtPlans.GetStringField(1)),
				mDtPlans.GetIntField(9),
				(_NET_INSTALL_PLAN_STATUS)(mDtPlans.GetIntField(10))
				);
			

			mDtPlans.NextRow();
		}*/
	}
}

/************************************************************************/
/* 创建修改表结构                                                       */
/************************************************************************/
void NetInstallsCaseDB::CreateTableNetInstallPlan()
{INTERFUN;
	string strsql = "CREATE TABLE NetInstallPlan(\
					PackId       INTEGER  NOT NULL ,\
					PlanID		 INTEGER  NOT NULL ,\
					Status		 INTEGER DEFAULT 0 ,\
					IsLegal      INTEGER DEFAULT 0 ,\
					PackTime      INTEGER DEFAULT 0);";

	CDBManager::m_db.ExecDML(strsql.c_str());
}
void NetInstallsCaseDB::CreateTableNetInstallPlanSofts()
{INTERFUN;
	string strsql = "CREATE TABLE NetInstallPlanSofts(\
					PlanId					INTEGER,\
					SoftId					INTEGER ,\
					Name					NVARCHAR ,\
					Size					FLOAT ,\
					Version					NVARCHAR(20) ,\
					Extension				NVARCHAR(20) ,\
					DownloadUrl				NVARCHAR,\
					Identifier				NVARCHAR(100),\
					Icon					NVARCHAR(200));";
	CDBManager::m_db.ExecDML(strsql.c_str());
}
void NetInstallsCaseDB::UpdateTableInstallPlan()
{INTERFUN;
	string strsql = "SELECT COUNT(*) FROM sqlite_master where type='table' and name='InstallPlan' and sql like '%PlanType%';";
	int ret = CDBManager::m_db.ExecScalar(strsql.c_str());
	if(ret == 0)
	{
		strsql = "ALTER TABLE InstallPlan ADD COLUMN PlanType int DEFAULT 0;";
		CDBManager::m_db.ExecDML(strsql.c_str());
	}

	strsql = "SELECT COUNT(*) FROM sqlite_master where type='table' and name='NetInstallPlan' and sql like '%IsLegal%';";
	ret = CDBManager::m_db.ExecScalar(strsql.c_str());
	if(ret == 0)
	{
		//zhengjx20121206，上个版本网络装机方案有问题，这里根据列是否存在做删除操作（后续下载即可）
		// 清空数据
		CString clearSQL = L"delete from NetInstallPlanSofts;\
				  delete from InstallPlan where planid in(select planid from NetInstallPlan) or plantype=1;\
				  delete from NetInstallPlan;\
				  delete from InstallPlanFile where plan_id not in(select planid from InstallPlan);";
		CDBManager::DoExecSql(clearSQL);

		strsql = "ALTER TABLE NetInstallPlan ADD COLUMN IsLegal INTEGER DEFAULT 0;";
		CDBManager::m_db.ExecDML(strsql.c_str());
	}

	strsql = "SELECT COUNT(*) FROM sqlite_master where type='table' and name='NetInstallPlan' and sql like '%PackTime%';";
	ret = CDBManager::m_db.ExecScalar(strsql.c_str());
	if(ret == 0)
	{
		strsql = "ALTER TABLE NetInstallPlan ADD COLUMN PackTime int DEFAULT 0;";
		CDBManager::m_db.ExecDML(strsql.c_str());
	}
}

/************************************************************************/
/* END                                             */
/************************************************************************/



// <summary>
//检查网络上的数据是否改变，改变则更新数据库（只对于数据库中已经存在网络上的装机包ID）
map<int,int> NetInstallsCaseDB::CheckNetAppDataChange()
{
	map<int,int>				mapChangeInfo;//网络上的数据发生改变的数据包id和装机方案id
	vector<int>					vecNoChgPack;
	map<int,vector<SoftData*>>  vecWriteInfo; //需要写入数据库的信息
	struct DEL_DBINFO{
		DEL_DBINFO():nPackId(0),nSoftId(0)
		{}
		int nPackId;
		int nSoftId;
	};
	vector<DEL_DBINFO>          vecDelInfo;//需要删除的信息

	// 网络装机方案软件列表
	CppSQLite3Table dtNetCaseSofts;
	CString strSql = "Select * from NetInstallPlan";
	CDBManager::DB_QUERY_TABLEDATA(strSql,dtNetCaseSofts);
	// 已下载的网络装机方案软件列表与网络资源做比较
	CppSQLite3Table dtDownPlanSofts;
	strSql = "Select * from NetInstallPlan A,InstallPlanFile B where A.planid = B.plan_id";
	CDBManager::DB_QUERY_TABLEDATA(strSql,dtDownPlanSofts);
	// 未下载的网络装机方案软件列表与网络资源做比较
	CppSQLite3Table dtPlanSofts;
	strSql = "Select * from NetInstallPlan A,NetInstallPlanSofts B where A.planid = B.planid";
	CDBManager::DB_QUERY_TABLEDATA(strSql,dtPlanSofts);

	//做优化处理，先比较数据包的时间是否和网络上的数据包时间一致
	for (int no = 0; no < dtNetCaseSofts.NumRows(); no++)
	{
		dtNetCaseSofts.SetRow(no);
		for ( vector<PackageData*>::iterator it = mNetPackageDataList.begin(); it != mNetPackageDataList.end(); ++it ) 
		{
			PackageData* PackItem = *it;
			if(dtNetCaseSofts.GetIntField("PackId")!=PackItem->_nPackageId)
				continue;
			if (dtNetCaseSofts.GetIntField("PackTime")==PackItem->_packTime)
				vecNoChgPack.push_back(dtNetCaseSofts.GetIntField("PackId"));
			else
			{
				CString strSql = L"";
				strSql.Format(L"update NetInstallPlan set PackTime='%d' where PackId=%d;",PackItem->_packTime, PackItem->_nPackageId);
				CDBManager::DoExecSql(strSql);
				mapChangeInfo[dtNetCaseSofts.GetIntField("PackId")]=dtNetCaseSofts.GetIntField("PlanID");
			}
		}
	}

	for (map<int,vector<SoftData*>>::iterator iter = mNetPlanItems.begin();iter!=mNetPlanItems.end();iter++)
	{
		bool bCmp1=false,bCmp2=true;
		for (int no = 0; no < dtNetCaseSofts.NumRows(); no++)
		{
			dtNetCaseSofts.SetRow(no);
			if(dtNetCaseSofts.GetIntField("PackId")==iter->first)
			{
				bCmp1=true;
				break;
			}
		}
		if (!bCmp1)
			continue;
		for (vector<int>::size_type i=0;i<vecNoChgPack.size();i++)
		{
			if (vecNoChgPack[i]==iter->first)
			{
				bCmp2=false;
				break;
			}
		}
		if (!bCmp2)
			continue;

		vector<SoftData*> packSoft = iter->second;
		vector<SoftData*> temp;

		
		for (vector<SoftData*>::iterator it = packSoft.begin();it != packSoft.end(); ++it ) 
		{
			SoftData* packSoftsItem = *it;
			bool bAddDB=true;
			//Begin--比较已下载的网络装机方案
			for (int no = 0; no < dtDownPlanSofts.NumRows(); no++)
			{
				dtDownPlanSofts.SetRow(no);
				if(dtDownPlanSofts.GetIntField("PackId")==iter->first
					&&dtDownPlanSofts.GetIntField("FID")==packSoftsItem->_nSoftId)
				{
					bAddDB=false;
					break;
				}
			}
			//Begin--比较未下载的网络装机方案
			for (int no = 0; no < dtPlanSofts.NumRows(); no++)
			{
				dtPlanSofts.SetRow(no);
				if(dtPlanSofts.GetIntField("PackId")==iter->first
					&&dtPlanSofts.GetIntField("SoftId")==packSoftsItem->_nSoftId)
				{
					bAddDB=false;
					break;
				}
			}
			if (bAddDB)
				temp.push_back(packSoftsItem);
		}

		for (int no = 0; no < dtDownPlanSofts.NumRows(); no++)
		{
			dtDownPlanSofts.SetRow(no);
			if(dtDownPlanSofts.GetIntField("PackId")!=iter->first)
				continue;

			bool bDelete = true;
			for (vector<SoftData*>::iterator it = packSoft.begin();it != packSoft.end(); ++it ) 
			{
				SoftData* packSoftsItem = *it;
				if (dtDownPlanSofts.GetIntField("FID")==packSoftsItem->_nSoftId)
				{
					bDelete = false;
					break;
				}
			}
			if (bDelete)
			{	
				DEL_DBINFO Info;
				Info.nPackId=dtDownPlanSofts.GetIntField("PackId");
				Info.nSoftId=dtDownPlanSofts.GetIntField("FID");
				vecDelInfo.push_back(Info);
			}
		}

		for (int no = 0; no < dtPlanSofts.NumRows(); no++)
		{
			dtPlanSofts.SetRow(no);
			if(dtPlanSofts.GetIntField("PackId")!=iter->first)
				continue;

			bool bDelete = true;
			for (vector<SoftData*>::iterator it = packSoft.begin();it != packSoft.end(); ++it ) 
			{
				SoftData* packSoftsItem = *it;
				if (dtPlanSofts.GetIntField("SoftId")==packSoftsItem->_nSoftId)
				{
					bDelete = false;
					break;
				}
			}
			if (bDelete)
			{	
				DEL_DBINFO Info;
				Info.nPackId=dtPlanSofts.GetIntField("PackId");
				Info.nSoftId=dtPlanSofts.GetIntField("SoftId");
				vecDelInfo.push_back(Info);
			}
		}
		//End--比较未下载的网络装机方案

		vecWriteInfo[iter->first]=temp;
	}



//删除数据库信息
	for (vector<int>::size_type i=0; i<vecDelInfo.size(); i++)
	{
		CString strSql = L"";
		strSql.Format(L"delete from NetInstallPlanSofts where SoftId=%d;",vecDelInfo[i].nSoftId);
		CDBManager::DoExecSql(strSql);

		strSql.Format(L"delete from InstallPlanFile where FileID=%d;",vecDelInfo[i].nSoftId);
		CDBManager::DoExecSql(strSql);
	}
		
//向数据库插入网络装机方案内编辑过的数据
	for (map<int,vector<SoftData*>>::iterator iter = vecWriteInfo.begin();iter!=vecWriteInfo.end();iter++)
	{
		vector<SoftData*> ChgpackSoft = iter->second;
		map<int,int>::iterator Chgiter=mapChangeInfo.find(iter->first);
		if(Chgiter==mapChangeInfo.end())
			continue;

		for ( vector<SoftData*>::iterator it = ChgpackSoft.begin(); it != ChgpackSoft.end(); ++it ) 
		{
			SoftData* packSoftsItem = *it;
			InstallsAppInfo* appItem = BuildUndownloadAppItem(Chgiter->second, 
				CCodeOperation::UTF_8ToUnicode(packSoftsItem->_Name), 
				CCodeOperation::StringToWstring(packSoftsItem->_DownloadUrl), 
				FormatFileSize(packSoftsItem->_Size), 
				CCodeOperation::StringToWstring(packSoftsItem->_Version), 
				CCodeOperation::StringToWstring(packSoftsItem->_Extension), 
				CCodeOperation::StringToWstring(packSoftsItem->_Identifier), 
				CCodeOperation::StringToWstring(packSoftsItem->_ImgSrc)
				);

			appItem->m_id = packSoftsItem->_nSoftId;
			appItem->m_fid=packSoftsItem->_nSoftId;

			strSql.Format(L"Insert into NetInstallPlanSofts(PlanId,SoftId,Name,Size,Version,Extension,DownloadUrl,Identifier,Icon) values('%d','%d','%s','%d','%s','%s','%s','%s','%s');", 
				Chgiter->second,
				appItem->m_id,
				FormatSQLValue(appItem->m_strName).c_str(),
				appItem->m_FileSize,
				FormatSQLValue(appItem->m_strVersion).c_str(),
				FormatSQLValue(appItem->m_strFileType).c_str(),
				FormatSQLValue(appItem->m_strSavePath).c_str(),
				FormatSQLValue(appItem->m_strPackageID).c_str(),
				FormatSQLValue(appItem->m_Iconpath).c_str());

			CDBManager::DoExecSql(strSql);
		}

		//更新InstallPlan表的相关信息
		long TotalSize = 0;
		vector<SoftData*> packSoft = mNetPlanItems[iter->first];
		for ( vector<SoftData*>::iterator it = packSoft.begin(); it != packSoft.end(); ++it ) 
		{
			SoftData* packSoftsItem = *it;
			TotalSize += FormatFileSize(packSoftsItem->_Size);
		}
		time_t lastUseTime;
		time(&lastUseTime);

		CString strCount;
		strCount.Format(L"select count(0) from InstallPlanFile A where A.Plan_ID = '%d';",Chgiter->second);
		std::wstring str = strCount.GetString();
		int nCount = CDBManager::m_db.ExecScalar(CCodeOperation::UnicodeToUTF_8(str).c_str());

		// 更新数据库
		strSql.Format(L"update InstallPlan set SoftNum='%d',TotalSize='%d',LastUseTime='%d',PlanInstallTime='%d',InstallNum='%d' where PlanID='%d';",
			packSoft.size(),
			TotalSize,
			lastUseTime,
			0,
			nCount,
			Chgiter->second);
		CDBManager::DoExecSql(strSql);
	}

	return mapChangeInfo;
}

/// <summary>
/// 加载网络Cpa装机方案
/// </summary>
void NetInstallsCaseDB::LoadNetCpaInstallPlans()
{
	vector<InstallsCaseData*>& vecCaseData = InstallCaseManagerEx::GetInstance()->NetInstallPlans;
	vector<PackageData*>::iterator iter = _vecNetCpaPackageDataList.begin();
	for (; iter!=_vecNetCpaPackageDataList.end(); ++iter)
	{
		if (NULL == *iter)
		{
			continue;
		}
		NetInstallsCaseData* planItem = new NetInstallsCaseData(g_sPlantId++);
		planItem->PackId = (*iter)->_nPackageId;
		planItem->CaseName(CCodeOperation::UTF_8ToUnicode((*iter)->_Name));
		planItem->IsLegalPlan((*iter)->_nLegal);
		planItem->SetbCpa(true);
		planItem->PlanType(_INSTALL_PLAN_TYPE_NET);
		planItem->CaseType((*iter)->_nPlatfrom == "Android" ? _INSTALLS_CASE_DATA_ANDROID : _INSTALLS_CASE_DATA_IOS);
		planItem->NetInstallStatus = _NET_INSTALL_PLAN_STATUS_INIT;
		planItem->SetWebCpaUrl((*iter)->_strUrl);

		vector<SoftData*> packSoft = mNetCpaPlanItems[(*iter)->_nPackageId];
		if (packSoft.size() > 0)
		{
			for ( vector<SoftData*>::iterator it = packSoft.begin(); it != packSoft.end(); ++it ) 
			{
				SoftData* soft = *it;
				if (NULL==soft)
				{
					continue;
				}
				// 去除下载地址为空的
				if (soft->_DownloadUrl == "")
					continue;

				InstallsAppInfo* appItem = BuildUndownloadAppItem(planItem->Id(), 
					CCodeOperation::UTF_8ToUnicode(soft->_Name), 
					CCodeOperation::StringToWstring(soft->_DownloadUrl), 
					FormatFileSize(soft->_Size), 
					CCodeOperation::StringToWstring(soft->_Version), 
					CCodeOperation::StringToWstring(soft->_Extension), 
					CCodeOperation::StringToWstring(soft->_Identifier), 
					CCodeOperation::StringToWstring(soft->_ImgSrc)
					);

				appItem->m_id = soft->_nSoftId;
				appItem->m_fid=soft->_nSoftId;
				planItem->AddNetAppToList(appItem);
			}
		}
		vecCaseData.push_back(planItem);
	}
	sort(vecCaseData.begin(), vecCaseData.end(), lessPackageId);
	vector<InstallsCaseData*>::iterator uniqueiter=unique(vecCaseData.begin(), vecCaseData.end(), uniquePackageId);
	vecCaseData.erase(uniqueiter, vecCaseData.end());

}

/// <summary>
/// 加载网络装机方案
/// </summary>
/// <param name="clearDataBase">重新加载网络装机方案(测试用，为true时，如果FuncInst_LoginLogoutChanged先执行，则dtPlans被其先执行不为空，第一次加载不出来，正常现象。)</param>
/// <returns></returns>
void NetInstallsCaseDB::LoadNetInstallPlans(bool clearDataBase)
{INTERFUN;
	CString strSql = "";

	if (clearDataBase)
	{
		 //清空数据
		strSql = L"delete from NetInstallPlanSofts;\
				  delete from InstallPlan where planid in(select planid from NetInstallPlan) or plantype=1;\
				  delete from NetInstallPlan;\
				  delete from InstallPlanFile where plan_id not in(select planid from InstallPlan);";
		CDBManager::DoExecSql(strSql);
	}
	else
	{
		if(mDtPlans.NumRows() == 0)
		{
			InsertNetPlanItemToDB();
			return;
		}

		// 已下载的网络装机方案软件列表
		CppSQLite3Table dtDownPlanSofts;
		CString strSql = "Select B.* from NetInstallPlan A,InstallPlanFile B where A.planid = B.plan_id";
		CDBManager::DB_QUERY_TABLEDATA(strSql,dtDownPlanSofts);

		// 未下载的网络装机方案软件列表
		CppSQLite3Table dtPlanSofts;
		strSql = "Select * from NetInstallPlan A,NetInstallPlanSofts B where A.planid = B.planid";
		CDBManager::DB_QUERY_TABLEDATA(strSql,dtPlanSofts);

		for (int mo = 0; mo < mDtPlans.NumRows(); mo++)
		{
			mDtPlans.SetRow(mo);
			bool bHasInNetPkg = false;
			//如果数据库中的方案包不在当前账户的网络装机方案，则跳过
			for (vector<PackageData*>::iterator Pkgiter = mNetPackageDataList.begin(); Pkgiter != mNetPackageDataList.end(); ++Pkgiter)
			{
				if (mDtPlans.GetIntField("PackId") == (*Pkgiter)->_nPackageId)
				{
					bHasInNetPkg = true;
				}
			}
			if (!bHasInNetPkg)
			{
				continue;
			}
			NetInstallsCaseData* planItem = BuildNetInstallPlanItem(mDtPlans.GetIntField("PlanID"),
				Charset::UTF8ToUnicode(mDtPlans.GetStringField("PlanName")),
				mDtPlans.GetFloatField("LastUseTime"),
				Charset::UTF8ToUnicode(mDtPlans.GetStringField("Category")),
				mDtPlans.GetIntField("PackId"),
				(_NET_INSTALL_PLAN_STATUS)(mDtPlans.GetIntField("Status")),
				mDtPlans.GetIntField("IsLegal")
				);

			// 添加方案已下载软件
			for (int no = 0; no < dtDownPlanSofts.NumRows(); no++)
			{
				dtDownPlanSofts.SetRow(no);

				if(dtDownPlanSofts.GetIntField("Plan_ID") != mDtPlans.GetIntField("PlanID"))
					continue;

				InstallsAppInfo *appdata = new InstallsAppInfo(dtDownPlanSofts.GetIntField("FileID"));
				appdata->m_strName = Charset::UTF8ToUnicode(dtDownPlanSofts.GetStringField("Name")).c_str();
				appdata->m_strSavePath=Charset::UTF8ToUnicode(dtDownPlanSofts.GetStringField("Path")).c_str();
				appdata->m_FileSize = dtDownPlanSofts.GetFloatField("Size");
				appdata->m_strVersion=Charset::UTF8ToUnicode(dtDownPlanSofts.GetStringField("Version")).c_str();
				appdata->m_strFileType=Charset::UTF8ToUnicode(dtDownPlanSofts.GetStringField("Format")).c_str();
				appdata->m_SupportDevice = Charset::UTF8ToUnicode(dtDownPlanSofts.GetStringField("SupportDevice")).c_str();
				appdata->m_strPackageID = Charset::UTF8ToUnicode(dtDownPlanSofts.GetStringField("AppName")).c_str();
				appdata->m_tDownloadTime = dtDownPlanSofts.GetFloatField("LastWriteTime");
				appdata->m_isLegalSoft = dtDownPlanSofts.GetIntField("IsLegalSoft");
				appdata->m_Purchaser  = Charset::UTF8ToUnicode(dtDownPlanSofts.GetStringField("Buyer")).c_str();
				appdata->m_VerCode = Charset::UTF8ToUnicode(dtDownPlanSofts.GetStringField("VersionCode")).c_str();
				appdata->m_Iconpath= DataPath::GetDataDir();
				appdata->m_Iconpath+=L"\\icon\\";
				appdata->m_Iconpath+=UIStringFormat::CheckDownLoadFileName(appdata->m_strName);
				appdata->m_Iconpath+=UIStringFormat::CheckDownLoadFileName(appdata->m_strVersion);
				appdata->m_Iconpath+=CFileOperation::GetFileExt(appdata->m_strSavePath);
				appdata->m_Iconpath+=_T(".png");
				appdata->m_SDKVersion  = Charset::UTF8ToUnicode(dtDownPlanSofts.GetStringField("SDKVersion")).c_str();
				appdata->m_fid=dtDownPlanSofts.GetIntField("FID");
				appdata->m_itemID=dtDownPlanSofts.GetIntField("ItemID");
				planItem->AddAppToList(appdata);
			}


			//CppSQLite3Table dtPlanSofts;
			//strSql = "Select * from NetInstallPlan A,NetInstallPlanSofts B where A.planid = B.planid";
			// 添加方案未下载的本地表数据
			for (int no = 0; no < dtPlanSofts.NumRows(); no++)
			{
				dtPlanSofts.SetRow(no);

				if(dtPlanSofts.GetIntField("PlanID") != mDtPlans.GetIntField("PlanID"))
					continue;

				InstallsAppInfo* appItem = BuildUndownloadAppItem(dtPlanSofts.GetIntField("PlanID"),
					Charset::UTF8ToUnicode(dtPlanSofts.GetStringField("Name")), 
					Charset::UTF8ToUnicode(dtPlanSofts.GetStringField("DownloadUrl")), 
					dtPlanSofts.GetFloatField("Size"), 
					Charset::UTF8ToUnicode(dtPlanSofts.GetStringField("Version")),
					Charset::UTF8ToUnicode(dtPlanSofts.GetStringField("Extension")), 
					Charset::UTF8ToUnicode(dtPlanSofts.GetStringField("Identifier")), 
					Charset::UTF8ToUnicode(dtPlanSofts.GetStringField("Icon")));

				appItem->m_id = dtPlanSofts.GetIntField("SoftId");
				appItem->m_fid = appItem->m_id;
				appItem->m_isLegalSoft = planItem->IsLegalPlan();
				planItem->AddNetAppToList(appItem);

				//网络装机方案下载URL可能更换，故本地数据库保存URL需要与Web端获取的数据做比对，以Web端获取的数据为准
				for(vector<PackageData*>::iterator it = mNetPackageDataList.begin();it!=mNetPackageDataList.end();it++)
				{
					PackageData* pData = *it;
					if((UINT)pData->_nPackageId==planItem->PackId)
					{
						vector<SoftData*> packSoft = mNetPlanItems[pData->_nPackageId];
						for(vector<SoftData*>::iterator softdata = packSoft.begin(); softdata!=packSoft.end();softdata++)
						{
							SoftData* sData = *softdata;
							if(appItem->m_fid == sData->_nSoftId)
							{
								if( appItem->m_strSavePath != CCodeOperation::GB2312ToUnicode(sData->_DownloadUrl))
									appItem->m_strSavePath = CCodeOperation::GB2312ToUnicode(sData->_DownloadUrl);

								// 如果缩略图不存在，下载缩略图
								if( !CFileOperation::IsFileExist(appItem->m_Iconpath.c_str()))
								{
									wstring SavePath = DataPath::GetDataDir();
									SavePath+=L"\\icon\\";
									SavePath+=UIStringFormat::CheckDownLoadFileName(Charset::UTF8ToUnicode(sData->_Name.c_str()));
									SavePath+=UIStringFormat::CheckDownLoadFileName(Charset::UTF8ToUnicode(sData->_Version.c_str()));
									SavePath+=UIStringFormat::CheckDownLoadFileName(Charset::UTF8ToUnicode(sData->_Extension.c_str()));
									SavePath+=_T(".png");
									if(!CFileOperation::IsFileExist(SavePath.c_str()))
									{
										CNetOperation::DownloadFile(sData->_ImgSrc,SavePath);
									}
									appItem->m_Iconpath = SavePath;
								}
								//break;
							}
						}
						break;
					}
				}
			}

			InstallCaseManagerEx::GetInstance()->NetInstallPlans.push_back(planItem);
			mDataBasePlanList.push_back(planItem);
		}
	}

	// 网络装机方案
	InsertNetPlanItemToDB();
}
/// <summary>
/// 组合网络装机方案对象
/// </summary>
/// <param name="planId"></param>
/// <param name="PackName"></param>
/// <param name="SoftNum"></param>
/// <param name="TotalSize"></param>
/// <param name="LastUseTime"></param>
/// <param name="Category"></param>
/// <param name="PackId"></param>
/// <param name="planStatus"></param>
/// <returns></returns>
NetInstallsCaseData* NetInstallsCaseDB:: BuildNetInstallPlanItem(UINT planId, wstring PackName, time_t LastUseTime, wstring Category, UINT PackId, _NET_INSTALL_PLAN_STATUS planStatus, UINT isLegal)
{INTERFUN;
	if(planId > 0)
		planId += NET_CASE_START_INDEX;

	NetInstallsCaseData* planItem = new NetInstallsCaseData(planId);

	planItem->PlanType(_INSTALL_PLAN_TYPE_NET);
	planItem->Id(planId);
	planItem->CaseName(PackName);
	planItem->lastUseTime = LastUseTime;
	planItem->CaseType(Category == L"Android" ? _INSTALLS_CASE_DATA_ANDROID : _INSTALLS_CASE_DATA_IOS);

	planItem->PackId = PackId;
	planItem->NetInstallStatus = planStatus;
	planItem->IsLegalPlan(isLegal);
	return planItem;
}

InstallsAppInfo* NetInstallsCaseDB::BuildUndownloadAppItem(UINT planId, wstring Name, wstring downUrl, long size, wstring Version, wstring Extension, wstring Identifier, wstring iconUrl)
{INTERFUN;
	InstallsAppInfo *appdata = new InstallsAppInfo(-1);	
	appdata->m_strName = Name.c_str();
	appdata->m_strSavePath=downUrl.c_str();
	appdata->m_FileSize = size;
	appdata->m_strVersion=Version.c_str();
	appdata->m_strFileType=Extension.c_str();
	appdata->m_strPackageID = Identifier.c_str();


	// 下载缩略图
	if( CStrOperation::startsWith(CStrOperation::toLowerW(iconUrl),L"http:"))
	{
		wstring SavePath = DataPath::GetDataDir();
		SavePath+=L"\\icon\\";
		SavePath+=UIStringFormat::CheckDownLoadFileName(appdata->m_strName);
		SavePath+=UIStringFormat::CheckDownLoadFileName(appdata->m_strVersion);
		SavePath+=UIStringFormat::CheckDownLoadFileName(appdata->m_strFileType);
		SavePath+=_T(".png");
		if(!CFileOperation::IsFileExist(SavePath.c_str()))
		{
			CNetOperation::DownloadFile(CCodeOperation::UnicodeToGB2312(iconUrl),SavePath);
		}
		appdata->m_Iconpath = SavePath;
	}
	// 本地缩略图
	else
	{
		appdata->m_Iconpath = iconUrl;
	}
	return appdata;
}

//@zjf 被修改过的软件信息在NetInstallPlans和mDataBasePlanList要删除原来重新载入网络上的软件信息
void NetInstallsCaseDB::UpdateNetInstallPlans(map<int,int>& mapChangePackId, PackageData* PackItem)
{
	for (map<int,int>::iterator iter = mapChangePackId.begin(); iter!=mapChangePackId.end(); ++iter)
	{
		if (PackItem->_nPackageId==iter->first)
		{
			time_t lastUseTime;
			time(&lastUseTime);

			NetInstallsCaseData* planItem = BuildNetInstallPlanItem(
				iter->second, 
				CCodeOperation::UTF_8ToUnicode(PackItem->_Name), 
				lastUseTime,
				CCodeOperation::StringToWstring(PackItem->_nPlatfrom),
				PackItem->_nPackageId,
				_NET_INSTALL_PLAN_STATUS_INIT,
				PackItem->_nLegal);

			vector<SoftData*> packSoft = mNetPlanItems[PackItem->_nPackageId];
			if (packSoft.size() > 0)
			{
				for ( vector<SoftData*>::iterator it = packSoft.begin(); it != packSoft.end(); ++it ) 
				{
					SoftData* soft = *it;

					// 去除下载地址为空的
					if (soft->_DownloadUrl == "")
						continue;

					InstallsAppInfo* appItem = BuildUndownloadAppItem(planItem->Id(), 
						CCodeOperation::UTF_8ToUnicode(soft->_Name), 
						CCodeOperation::StringToWstring(soft->_DownloadUrl), 
						FormatFileSize(soft->_Size), 
						CCodeOperation::StringToWstring(soft->_Version), 
						CCodeOperation::StringToWstring(soft->_Extension), 
						CCodeOperation::StringToWstring(soft->_Identifier), 
						CCodeOperation::StringToWstring(soft->_ImgSrc)
						);

					appItem->m_id = soft->_nSoftId;
					appItem->m_fid=soft->_nSoftId;
					planItem->AddNetAppToList(appItem);
				}
			}
			for (vector<NetInstallsCaseData*>::iterator iter=mDataBasePlanList.begin();iter!=mDataBasePlanList.end();++iter)
			{
				if ((*iter)->PackId==PackItem->_nPackageId)
				{
					mDataBasePlanList.erase(iter);
					break;
				}
			}
			InstallCaseManagerEx::GetInstance()->DeleteCaseByPackId(PackItem->_nPackageId);
			InstallCaseManagerEx::GetInstance()->NetInstallPlans.push_back(planItem);
			mDataBasePlanList.push_back(planItem);
		}
	}
}

/// <summary>
/// 向数据库插入数据不存在的网络装机方案
/// </summary>
void NetInstallsCaseDB::InsertNetPlanItemToDB()
{INTERFUN;

	//@zjf 检查网络上的数据是否改变，返回有修改的信息
	map<int,int> mapChangePackId = this->CheckNetAppDataChange();

	CString strSql = "";

	if (mNetPackageDataList.size() > 0)
	{
		for ( vector<PackageData*>::iterator it = mNetPackageDataList.begin(); it != mNetPackageDataList.end(); ++it ) 
		{
			PackageData* PackItem = *it;
			
			bool isExist = false;
			for ( vector<NetInstallsCaseData*>::iterator caseData = mDataBasePlanList.begin(); caseData != mDataBasePlanList.end(); caseData++ ) 
			{
				NetInstallsCaseData* tempData = *caseData;
				if(tempData->PackId == PackItem->_nPackageId)
				{
					isExist = true;
					break;
				}
			}

			// 已存在，继续下一项
			if (isExist)
			{
				//@zjf 被修改过的软件信息在NetInstallPlans和mDataBasePlanList要删除原来重新载入网络上的软件信息
				this->UpdateNetInstallPlans(mapChangePackId, PackItem);
				continue;
			}

			int SoftNum = 0;
			long TotalSize = 0;

			vector<SoftData*> packSoft = mNetPlanItems[PackItem->_nPackageId];
			if (packSoft.size() > 0)
			{
				SoftNum = packSoft.size();
				for ( vector<SoftData*>::iterator it = packSoft.begin(); it != packSoft.end(); ++it ) 
				{
					SoftData* packSoftsItem = *it;
					TotalSize += FormatFileSize(packSoftsItem->_Size);
				}
			}

			time_t lastUseTime;
			time(&lastUseTime);
			NetInstallsCaseData* planItem = BuildNetInstallPlanItem(0, 
				CCodeOperation::UTF_8ToUnicode(PackItem->_Name), 
				lastUseTime,
				CCodeOperation::StringToWstring(PackItem->_nPlatfrom),
				PackItem->_nPackageId,
				_NET_INSTALL_PLAN_STATUS_INIT,
				PackItem->_nLegal);

			wstring str = L"select max(planid) from InstallPlan";
			int maxPlanId = 0;
			try
			{
				wstring strCount = L"select count(0) from InstallPlan";
				int nCount = CDBManager::m_db.ExecScalar(CCodeOperation::UnicodeToUTF_8(strCount).c_str());
				if (nCount > 0)
					maxPlanId = CDBManager::m_db.ExecScalar(CCodeOperation::UnicodeToUTF_8(str).c_str());
			}
			catch(...)
			{
				maxPlanId = 0;
			}

			maxPlanId++;

			if(maxPlanId == 0)
				continue;

			int planID = 0;
			CString strHasPackExist;
			strHasPackExist.Format(L"select PlanID from NetInstallPlan where PackId='%d'", planItem->PackId);
			CppSQLite3Query q_Ret;
			if(CDBManager::DB_QUERY_DATA(strHasPackExist,q_Ret))
			{
				while(!q_Ret.Eof())
				{
					planID=q_Ret.GetIntField("PlanID");
					q_Ret.NextRow();
				}
			}
			if(planID==0)
			{
				// 插入数据库
				strSql.Format(L"Insert into InstallPlan(PlanId,Category,PlanName,SoftNum,TotalSize,IsLegal,LastUseTime,PlanInstallTime,InstallNum,PlanType) values('%d','%s','%s','%d','%d','%d','%d',0,0,1);",
					maxPlanId,
					planItem->CaseType() == _INSTALLS_CASE_DATA_ANDROID ? L"Android" : L"iOS",
					FormatSQLValue(planItem->CaseName()).c_str(),
					SoftNum,
					TotalSize,
					planItem->IsLegalPlan(),
					lastUseTime);
				CDBManager::DoExecSql(strSql);
			}
			
			long PlanId = planID==0 ? maxPlanId:planID;

			planItem->Id(PlanId + NET_CASE_START_INDEX);
			if(planID==0)
			{
				strSql.Format(L"Insert into NetInstallPlan(packid,planid,status,islegal,PackTime) values('%d','%d','%d','%d','%d');",planItem->PackId,PlanId,(int)planItem->NetInstallStatus,planItem->IsLegalPlan(),(int)PackItem->_packTime);
				CDBManager::DoExecSql(strSql.GetBuffer());
			}

			AddNetInstallPlanNetAppItems(planItem, packSoft);

			InstallCaseManagerEx::GetInstance()->NetInstallPlans.push_back(planItem);
			mDataBasePlanList.push_back(planItem);
		}
	}
}

wstring NetInstallsCaseDB::FormatSQLValue(wstring val)
{INTERFUN;
	if (L"" != val)
	{
		return CStrOperation::ReplaceW(val,L"'",L"''") ;
	}
	return L"";
}

/// <summary>
/// 将文件大小转byte大小
/// </summary>
/// <param name="fileSize"></param>
/// <returns></returns>
long NetInstallsCaseDB::FormatFileSize(string fileSize)
{INTERFUN;
	long ret = 0;
	fileSize = CStrOperation::toLower(fileSize);

	if (fileSize != "" && fileSize != "0")
	{
		if (fileSize.find("kb") != fileSize.npos)
		{
			fileSize = CStrOperation::Replace(fileSize,"kb", "");
			ret = atoi (fileSize.c_str());
			ret = ret * 1024;
		}
		else if (fileSize.find("mb") != fileSize.npos)
		{
			fileSize = CStrOperation::Replace(fileSize,"mb", "");
			ret = atoi (fileSize.c_str());
			ret = ret * 1048576;
		}
		else if (fileSize.find("gb") != fileSize.npos)
		{
			fileSize = CStrOperation::Replace(fileSize,"gb", "");
			ret = atoi (fileSize.c_str());
			ret = ret * 1073741824;
		}
	}

	return ret;
}

/// <summary>
/// 向网络装机方案添加下载包
/// </summary>
/// <param name="planItem"></param>
/// <param name="softs"></param>
/// <returns></returns>
bool NetInstallsCaseDB::AddNetInstallPlanNetAppItems(NetInstallsCaseData* planItem, vector<SoftData*> softs)
{INTERFUN;
	CString sb;

	for ( vector<SoftData*>::iterator it = softs.begin(); it != softs.end(); ++it ) 
	{
		SoftData* soft = *it;

		// 去除下载地址为空的
		if (soft->_DownloadUrl == "")
			continue;

		InstallsAppInfo* appItem = BuildUndownloadAppItem(planItem->Id(), 
			CCodeOperation::UTF_8ToUnicode(soft->_Name), 
			CCodeOperation::StringToWstring(soft->_DownloadUrl), 
			FormatFileSize(soft->_Size), 
			CCodeOperation::StringToWstring(soft->_Version), 
			CCodeOperation::StringToWstring(soft->_Extension), 
			CCodeOperation::StringToWstring(soft->_Identifier), 
			CCodeOperation::StringToWstring(soft->_ImgSrc)
			);

		appItem->m_id = soft->_nSoftId;
		appItem->m_fid=soft->_nSoftId;
		planItem->AddNetAppToList(appItem);

		sb.Format(L"Insert into NetInstallPlanSofts(PlanId,SoftId,Name,Size,Version,Extension,DownloadUrl,Identifier,Icon) values('%d','%d','%s','%d','%s','%s','%s','%s','%s');", 
			planItem->Id() - NET_CASE_START_INDEX,
			appItem->m_id,
			FormatSQLValue(appItem->m_strName).c_str(),
			appItem->m_FileSize,
			FormatSQLValue(appItem->m_strVersion).c_str(),
			FormatSQLValue(appItem->m_strFileType).c_str(),
			FormatSQLValue(appItem->m_strSavePath).c_str(),
			FormatSQLValue(appItem->m_strPackageID).c_str(),
			FormatSQLValue(appItem->m_Iconpath).c_str());

		CDBManager::DoExecSql(sb.GetBuffer());
	}

	return false;
}

/// <summary>
/// 设置网络装机方案“下载阶段”的状态
/// </summary>
/// <param name="planItem"></param>
/// <param name="Status"></param>
/// <returns></returns>
bool NetInstallsCaseDB::UpdateNetInstallPlanItemsStatus(NetInstallsCaseData* planItem, _NET_INSTALL_PLAN_STATUS Status)
{INTERFUN;
	UINT planid = planItem->Id() - NET_CASE_START_INDEX;
	CString strSql;
	strSql.Format(L"update NetInstallPlan set status = '%d' where planid='%d';",(int)Status,planid);
	CDBManager::DoExecSql(strSql.GetBuffer());
	return true;
}

bool NetInstallsCaseDB::Handle(const UpdateNetInstallPlanItemsStatusMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}
	NetInstallsCaseData* planItem = (NetInstallsCaseData*)pMsg->planItem;
	if (NULL == planItem)
	{
		return false;
	}

	this->UpdateNetInstallPlanItemsStatus(planItem, (_NET_INSTALL_PLAN_STATUS)pMsg->Status);
	
	return true;
}

int NetInstallsCaseDB::GetMaxAppId()
{INTERFUN;
	int appId = 0;
	wstring str = L"select max(SoftId) from NetInstallPlanSofts";
	try
	{
		wstring strCount = L"select count(0) from NetInstallPlanSofts";
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

void NetInstallsCaseDB::LoadNetPlansFromWeb(int nUserId, bool bCpa)
{INTERFUN;

	int userId = nUserId;
	wstring useridpath = CFileOperation::GetCurrWorkingDir() + L"UserIDTest.txt";
	if(userId <= 0 && CFileOperation::IsFileExist(useridpath.c_str()))
		userId = 2;

	if (userId <= 0)
		userId = 4376;

	Web ApiService(1, NULL);

	//Web获取网络装机方案，盗版方案集合
	mNetPackageDataList = ApiService.GetPackageInfosC(userId,0,0);

	//Web获取网络装机方案，正版方案集合
	vector<PackageData*> mNetLegalPackageDataList;
	mNetLegalPackageDataList = ApiService.GetPackageInfosC(userId,1,0);

	if(mNetPackageDataList.size()<=0)
		mNetPackageDataList = ApiService.GetPackageInfosC(4376,0,0);
	if(mNetLegalPackageDataList.size()<=0)
		mNetLegalPackageDataList = ApiService.GetPackageInfosC(4376,1,0);
	if(mNetLegalPackageDataList.size()>0)
	{
		mNetPackageDataList.insert(mNetPackageDataList.end(), mNetLegalPackageDataList.begin(), mNetLegalPackageDataList.end());
	}

	if (bCpa)
	{
		//begin获取网络CPA装机方案
		_vecNetCpaPackageDataList = ApiService.GetPackageInfosC(userId,1,0, true);
		vector<PackageData*> vecPackageData = ApiService.GetPackageInfosC(userId,0,0, true);
		if(_vecNetCpaPackageDataList.size()<=0)
			_vecNetCpaPackageDataList = ApiService.GetPackageInfosC(4376,0,0, true);
		if(vecPackageData.size()<=0)
			vecPackageData = ApiService.GetPackageInfosC(4376,1,0, true);
		if(vecPackageData.size()>0)
		{
			_vecNetCpaPackageDataList.insert(_vecNetCpaPackageDataList.end(), vecPackageData.begin(), vecPackageData.end());
		}
		//end获取网络CPA装机方案
	}
	else
	{
		_vecNetCpaPackageDataList.clear();
	}

	if (2 == userId || 4376 == userId)
	{
		//保存默认账户下的方案包ID
		_vecDefautUserPackage.clear();
		for (vector<PackageData*>::iterator iter = mNetPackageDataList.begin(); iter != mNetPackageDataList.end(); ++iter)
		{
			if (*iter)
			{
				_vecDefautUserPackage.push_back((*iter)->_nPackageId);
			}
		}

		for (vector<PackageData*>::iterator Cpaiter = _vecNetCpaPackageDataList.begin(); Cpaiter != _vecNetCpaPackageDataList.end(); ++Cpaiter)
		{
			if (*Cpaiter)
			{
				_vecDefautUserPackage.push_back((*Cpaiter)->_nPackageId);
			}
		}
	}

	for ( vector<PackageData*>::iterator it = mNetPackageDataList.begin(); it != mNetPackageDataList.end(); ++it ) 
	{
		PackageData* PackItem = *it;

		int SoftNum = 0;
		long TotalSize = 0;
		vector<SoftData*> tmp;
		//盗版装机方案
		if( 0 == PackItem->_nLegal )
		{
			tmp = ApiService.GetAppInfosByPackageC(PackItem->_nPackageId,0,0);
		}
		//正版装机方案
		else if( 1 == PackItem->_nLegal )
		{
			tmp = ApiService.GetAppInfosByPackageC(PackItem->_nPackageId,1,0);
		}
		vector<SoftData*>* packSoft = new vector<SoftData*> ();
		packSoft->insert(packSoft->end(), tmp.begin(), tmp.end());
		mNetPlanItems[PackItem->_nPackageId] = *packSoft;
	}

	for ( vector<PackageData*>::iterator it = _vecNetCpaPackageDataList.begin(); it != _vecNetCpaPackageDataList.end(); ++it ) 
	{
		PackageData* PackItem = *it;

		int SoftNum = 0;
		long TotalSize = 0;
		vector<SoftData*> tmp;
		//盗版装机方案
		if( 0 == PackItem->_nLegal )
		{
			tmp = ApiService.GetAppInfosByPackageC(PackItem->_nPackageId,0,0);
		}
		//正版装机方案
		else if( 1 == PackItem->_nLegal )
		{
			tmp = ApiService.GetAppInfosByPackageC(PackItem->_nPackageId,1,0);
		}
		vector<SoftData*>* packSoft = new vector<SoftData*> ();
		packSoft->insert(packSoft->end(), tmp.begin(), tmp.end());
		mNetCpaPlanItems[PackItem->_nPackageId] = *packSoft;
	}

	if(mNetPackageDataList.size() <= 0&&_vecNetCpaPackageDataList.size() <= 0)
	{
		InstallCaseManagerEx::GetInstance()->NetInstallPlans.clear();
		mDataBasePlanList.clear();
		mNetPlanItems.clear();
		mNetCpaPlanItems.clear();
	}
	else
	{
		this->DelPackageItem();
	}
}

void NetInstallsCaseDB::DelPackageItem()
{
	for (vector<InstallsCaseData*>::iterator iter = InstallCaseManagerEx::GetInstance()->NetInstallPlans.begin();\
		iter != InstallCaseManagerEx::GetInstance()->NetInstallPlans.end();)
	{
		bool bDel = true;
		for (vector<int>::iterator deliter = _vecDefautUserPackage.begin(); deliter != _vecDefautUserPackage.end(); ++deliter)
		{
			if ((*iter) && *deliter == (*iter)->PackId)
			{
				bDel = false;
				break;
			}
		}
		if (bDel)
		{
			iter = InstallCaseManagerEx::GetInstance()->NetInstallPlans.erase(iter);
		}
		else
			++iter;
	}

	for (vector<NetInstallsCaseData*>::iterator iter = mDataBasePlanList.begin();iter != mDataBasePlanList.end();)
	{
		bool bDel = true;
		for (vector<int>::iterator deliter = _vecDefautUserPackage.begin(); deliter != _vecDefautUserPackage.end(); ++deliter)
		{
			if ((*iter) && *deliter == (*iter)->PackId)
			{
				bDel = false;
				break;
			}
		}
		if (bDel)
		{
			iter = mDataBasePlanList.erase(iter);
		}
		else
			++iter;
	}
}

bool NetInstallsCaseDB::Handle(const RefreshNetPlanCaseDataMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}

	NetInstallsCaseData* pTemp = (NetInstallsCaseData*)pMsg->pPlanItem;
	if (NULL == pTemp)
	{
		return false;
	}

	CreateThread(NULL, 0, NetInstallsCaseDB::RefreshNetPlanCaseData, (LPVOID)pTemp, 0, NULL);

	return true;
}

DWORD NetInstallsCaseDB::RefreshNetPlanCaseData( LPVOID pTemp )
{
	NetInstallsCaseData* caseData = (NetInstallsCaseData*)pTemp;
	if (NULL == caseData)
	{
		return 0;
	}
	vector<SoftData*> netPlanItems = mNetPlanItems[caseData->PackId];
	if (netPlanItems.size()==0)
	{
		netPlanItems = mNetCpaPlanItems[caseData->PackId];
	}
	for ( vector<SoftData*>::iterator it = netPlanItems.begin(); it != netPlanItems.end(); ++it ) 
	{
		CString sb;
		SoftData* softdata = *it;
		bool existInDownLoaded = false;
		bool isSelected = false;
		
		for(InstallsAppList::iterator itdownload = caseData->ListApp()->begin();itdownload != caseData->ListApp()->end();itdownload++)
		{
			InstallsAppInfo *appinfo = *itdownload;
			//已下载
			if(appinfo->m_strPackageID == CCodeOperation::GB2312ToUnicode(softdata->_Identifier))
			{
				//下载后被删除
				if(!CFileOperation::IsFileExist(appinfo->m_strSavePath.c_str()))
				{
					CString str_SQL;
					str_SQL.Format(L"delete from InstallPlanFile where path='%s' and Plan_ID='%d'", FormatSQLValue(appinfo->m_strSavePath).c_str(),caseData->Id() - NET_CASE_START_INDEX);
					CDBManager::DoExecSql(str_SQL.GetBuffer());
					
					caseData->ListApp()->erase(itdownload);
					break;
				}
				else
				{
					existInDownLoaded = true;
					isSelected = appinfo->m_bSelected;
				}
			}
		}
		bool existInNotDownLoad = false;
		for(InstallsAppList::iterator itnotdownload = caseData->NetAppItems->begin();itnotdownload != caseData->NetAppItems->end();itnotdownload++)
		{
			InstallsAppInfo *appinfo = *itnotdownload;
			if(appinfo->m_strPackageID == CCodeOperation::GB2312ToUnicode(softdata->_Identifier))
			{
				existInNotDownLoad = true;
				isSelected = appinfo->m_bSelected;
				break;
			}
		}
		if( !existInDownLoaded && !existInNotDownLoad )
		{
			InstallsAppInfo* appItem = BuildUndownloadAppItem(caseData->Id(), 
				CCodeOperation::UTF_8ToUnicode(softdata->_Name), 
				CCodeOperation::StringToWstring(softdata->_DownloadUrl), 
				FormatFileSize(softdata->_Size), 
				CCodeOperation::StringToWstring(softdata->_Version), 
				CCodeOperation::StringToWstring(softdata->_Extension), 
				CCodeOperation::StringToWstring(softdata->_Identifier), 
				CCodeOperation::StringToWstring(softdata->_ImgSrc)
				);

			appItem->m_id = softdata->_nSoftId;
			appItem->m_fid = softdata->_nSoftId;
			appItem->m_bSelected = isSelected;
			caseData->AddNetAppToList(appItem);

			sb.Format(L"Insert into NetInstallPlanSofts(PlanId,SoftId,Name,Size,Version,Extension,DownloadUrl,Identifier,Icon) values('%d','%d','%s','%d','%s','%s','%s','%s','%s');", 
				caseData->Id() - NET_CASE_START_INDEX,
				appItem->m_id,
				FormatSQLValue(appItem->m_strName).c_str(),
				appItem->m_FileSize,
				FormatSQLValue(appItem->m_strVersion).c_str(),
				FormatSQLValue(appItem->m_strFileType).c_str(),
				FormatSQLValue(appItem->m_strSavePath).c_str(),
				FormatSQLValue(appItem->m_strPackageID).c_str(),
				FormatSQLValue(appItem->m_Iconpath).c_str());

			CDBManager::DoExecSql(sb.GetBuffer());
		}
	}

	RefreshNetPlanCaseFinishMsg RefreshMsg;
	RefreshMsg.Send();

	return 0;
}


INSTALLCASE_END