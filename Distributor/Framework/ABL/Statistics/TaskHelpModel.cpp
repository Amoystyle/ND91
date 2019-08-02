#include "stdafx.h"
#include "TaskHelpModel.h"
#include "Base/ClientInfo.h"
#include "Common/DealTime.h"
#include "RevertData.h"
#include "Web.h"
#include "FileOperationEx.h"
#include "Thread/TaskStatisticsThread.h"
#include "Manager/ThreadManager.h"
#include "StatisticHelp.h"
#include "TaskHelpModel.h"
#include "DeviceDataHelp.h"
#include <fstream>
#define LENGTH 49

static TaskHelpModel* _pHelpModel = NULL;

TaskHelpModel* TaskHelpModel::GetInstance()
{//INTERFUN;
	if ( !_pHelpModel )
	{
		_pHelpModel = new TaskHelpModel();
	}
	return _pHelpModel;
}

TaskHelpModel::TaskHelpModel()
{//INTERFUN;
	_nDowntCount = 0;
	_nInstallCount = 0;
	//TaskHelpModel::GetInstance();
	_nowVersion = CCodeOperation::UnicodeToUTF_8(CCodeOperation::s2ws(CClientInfo::GetInstance()->GetVersion()));
}

TaskHelpModel::~TaskHelpModel()
{//INTERFUN;
	ReleaseData();
}

void TaskHelpModel::ReleaseData()
{//INTERFUN;
	_nDowntCount = 0;
	_nInstallCount = 0;

	for ( int i=0;i<(int)_verDownloads.size();i++ )
	{
		APPDOWNLOAD_RECORD *p = _verDownloads[i];
		if ( p )
		{
			SAFE_DELETE(p->pData);
			SAFE_DELETE(p);
		}
	}
	_verDownloads.clear();

	for ( int i=0;i<(int)_verInstalls.size();i++ )
	{
		DEVICE_LIST* p = (DEVICE_LIST*)_verInstalls[i]->pData;
		if ( p )
		{
			SAFE_DELETE( p->pData );
			SAFE_DELETE( p );
			SAFE_DELETE(_verInstalls[i]);
		}
	}
	_verInstalls.clear();
}

void TaskHelpModel::PostDownLoadInfo( wstring m_appid, wstring m_platform,wstring m_csFileDownUrl,wstring m_strName,wstring m_appVersion
									  ,wstring m_csFileSavePath)
{//INTERFUN;
	std::string temp;
	GetUserLoginInfoMsg userCenterMsg;
	userCenterMsg.SendAndGetReturn();
	APPOPERATE_INFO* app = new APPOPERATE_INFO;
	temp			= CCodeOperation::UnicodeToUTF_8(m_appid);
	SetValues(app->identifier, temp);
	wstring str = CStrOperation::toLowerW(m_platform);
	app->operateType = str == L"iphone" ? 1 : str == L"ipad" ? 7 : 4;
	memset(app->channel, '\0', LENGTH);
	SetValues(app->channel, userCenterMsg.channelID);
	wstring filePath = m_csFileSavePath + m_strName + m_appVersion + L"." + CFileOperation::GetFileExt(m_csFileDownUrl);
	temp = userCenterMsg.bLongin && userCenterMsg.bCpaUser ? CFileOperation::GetFileMD5(filePath) : "";
	memset(app->hashValue, '\0', LENGTH);
	SetValues(app->hashValue, temp);
	wstring softName = m_strName;
	temp			= CCodeOperation::UnicodeToUTF_8(softName);
	SetValues(app->softName, temp);

	time_t t = time(NULL);
	app->operateTime= (long)t + 28800;

	temp			= CCodeOperation::UnicodeToUTF_8(m_appVersion);
	SetValues(app->softVersion, temp);

	LOG_DEBUG(L"TaskHelpModel::PostDownLoadInfo, APPOPERATE_INFO Get");

	APPDOWNLOAD_RECORD* down = new APPDOWNLOAD_RECORD;

	SetValues(down->appVersion, _nowVersion);
	temp = CSystemInfo::GetMac();
	SetValues(down->mac, temp);
	temp = CSystemInfo::GetOs();
	SetValues(down->pcVersion, temp);


	t = time(NULL);
	down->sendTime	= (long)t + 28800;
	down->pData		= app;
	down->length	= 1;
	
	LOG_DEBUG(L"TaskHelpModel::PostDownLoadInfo, APPDOWNLOAD_RECORD Get");

	try
	{
		down->userid = userCenterMsg.bLongin ? _wtoi(userCenterMsg.userId.c_str()) : 0 ;
		const string threadid = "PostDownLoadInfo" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
		CTaskStatisticsThread* cTaskStThread = new CTaskStatisticsThread(threadid,Action_PostDownLoadInfo);
		cTaskStThread->SetModel(this);
		cTaskStThread->SetVoidParam(down);
		cTaskStThread->Run();
		/*BaseParam param;
		param.SetVoid(down);
		HANDLE nHandle = StartSimpleActionThread(Action_PostDownLoadInfo, &param);	*/
	}
	catch (...)
	{
		LOG_DEBUG(L"TaskHelpModel::PostDownLoadInfo, Error!!!");
	}

}


void TaskHelpModel::PostInstallInfo( wstring m_appid, wstring m_csFileSavePath,wstring m_strName,wstring m_appVersion
									,wstring deviceId )
{//INTERFUN;
	std::string temp;
	//LoginModel* pLogin = LoginModel::GetInstance();
	GetUserLoginInfoMsg userCenterMsg;
	userCenterMsg.SendAndGetReturn();
	RETURN_IF(deviceId.empty());

	GetAblDeviceInfoMsg ablMsg;
	ablMsg.deviceID = deviceId;
	ablMsg.SendAndGetReturn();
	AblDeviceInfo* ablData = new AblDeviceInfo(&ablMsg.info);
	ablData->deviceID = deviceId;

	RETURN_IF ( !ablData );
	DeviceDataHelp help(ablData);

// 	RequestDeviceStateMsg deviceHelpMsg;
// 	deviceHelpMsg.deviceID = deviceId;
// 	deviceHelpMsg.SendAndGetReturn();
// 	RETURN_IF(!(deviceHelpMsg.bResult));

	//Web p(1);
	APPOPERATE_INFO* app = new APPOPERATE_INFO;
	temp			= CCodeOperation::UnicodeToGB2312(m_appid);
	SetValues(app->identifier, temp);
	memset(app->channel, '\0', LENGTH);
	SetValues(app->channel, userCenterMsg.channelID);
	temp = userCenterMsg.bLongin && userCenterMsg.bCpaUser? CFileOperation::GetFileMD5(m_csFileSavePath) : "" ;//CFileOperation::GetFileMD5(item->m_csFileSavePath);
	memset(app->hashValue, '\0', LENGTH);
	SetValues(app->hashValue, temp);
	wstring softName = m_strName;
	temp			= CCodeOperation::UnicodeToGB2312(softName);
	SetValues(app->softName, temp);

	time_t t = time(NULL);
	app->operateTime	= (long)t + 28800;
	app->operateType	= 2;

	temp			= CCodeOperation::UnicodeToGB2312(m_appVersion);
	SetValues(app->softVersion, temp);

	LOG_DEBUG(L"TaskHelpModel::PostInstallInfo, APPOPERATE_INFO Get");

	DEVICE_LIST* device = new DEVICE_LIST;
	temp = help.GetTrueImei();
	SetValues(device->imei, temp);
	temp = CCodeOperation::UnicodeToGB2312(ablData->baseInfo.deviceIMSI);
	SetValues(device->imsi, temp);
	temp = CCodeOperation::UnicodeToGB2312(ablData->baseInfo.deviceProductType);
	SetValues(device->mobileModel, temp);
	temp = CCodeOperation::UnicodeToGB2312(ablData->baseInfo.deviceProductVersion);
	SetValues(device->mobileVersion, temp);

	device->jailbroken		= help.GetJailbroken();
	device->platform		= help.GetDeviceType();
	device->connectMode		= ablData->connectType == WIFI_TYPE ? 10:20;
	device->pData			= app;

	LOG_DEBUG(L"TaskHelpModel::PostInstallInfo, DEVICE_LIST Get");

	APPINSTALL_RECORD* install = new APPINSTALL_RECORD;
	SetValues(install->appVersion, _nowVersion);
	temp = CSystemInfo::GetMac();
	LOG_DEBUG(L"TaskHelpModel::GetMac, Mac Get");
	SetValues(install->mac, temp);

	temp = CSystemInfo::GetOs();
	LOG_DEBUG(L"TaskHelpModel::GetOs, OS Get");
	SetValues(install->pcVersion, temp);

	t = time(NULL);
	install->sendTime	= (long)t + 28800;
	install->pData		= device;
	install->length		= 1;

	LOG_DEBUG(L"TaskHelpModel::PostInstallInfo, APPINSTALL_RECORD Get");

	try
	{

		install->userid = userCenterMsg.bLongin ? _wtoi(userCenterMsg.userId.c_str()) : 0 ;
		const string threadid = "PostInstallInfo" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
		CTaskStatisticsThread* cTaskStThread = new CTaskStatisticsThread(threadid,Action_PostInstallInfo);
		cTaskStThread->SetModel(this);
		cTaskStThread->SetVoidParam(install);
		cTaskStThread->Run();
	}
	catch (...)
	{
		LOG_DEBUG(L"TaskHelpModel::PostInstallInfo, ERROR!!!");	
	}
	
}

void TaskHelpModel::SaveDownloadData(APPDOWNLOAD_RECORD* pdata)
{//INTERFUN;
	wstring iniPath = CONFIG(_strCachePath) + L"downInfo.ini";
	string path = CCodeOperation::UnicodeToGB2312(iniPath);

	char temp[16], temp1[50];
	memset(temp, '\0', 16);
	sprintf_s(temp, 16,"%d", _nDowntCount);
	WritePrivateProfileStringA("Config", "num", temp, path.c_str());

	memset(temp, '\0', 16);
	sprintf_s(temp, 16, "Downdata_%d", _nInstallCount);
	memset(temp1, '\0', LENGTH);
	sprintf_s(temp1, 50, "%d", pdata->userid);
	WritePrivateProfileStringA(temp, "userid", temp1, path.c_str());	
	WritePrivateProfileStringA(temp, "mac", pdata->mac, path.c_str());
	WritePrivateProfileStringA(temp, "pcVersion", pdata->pcVersion, path.c_str());
	WritePrivateProfileStringA(temp, "zjVersion", pdata->appVersion, path.c_str());
	memset(temp1, '\0', LENGTH);
	sprintf_s(temp1, 50,"%d", pdata->sendTime);
	WritePrivateProfileStringA(temp, "Sendtime", temp1, path.c_str());

	WritePrivateProfileStringA(temp, "appid", pdata->pData->identifier, path.c_str());	
	WritePrivateProfileStringA(temp, "appname", pdata->pData->softName, path.c_str());
	WritePrivateProfileStringA(temp, "channel", pdata->pData->channel, path.c_str());
	WritePrivateProfileStringA(temp, "appversion", pdata->pData->softVersion, path.c_str());
	memset(temp1, '\0', LENGTH);
	sprintf_s(temp1, 50, "%d", pdata->pData->operateTime);
	WritePrivateProfileStringA(temp, "downtime", temp1, path.c_str());

}

void TaskHelpModel::SaveInstallData(APPINSTALL_RECORD* pdata)
{//INTERFUN;
	wstring iniPath = CONFIG(_strCachePath) + L"installInfo.ini";
	string path = CCodeOperation::UnicodeToGB2312(iniPath);

	char temp[16], temp1[50];
	memset(temp, '\0', 16);
	sprintf_s(temp, 16, "%d", _nInstallCount);
	WritePrivateProfileStringA("Config", "num", temp, path.c_str());	

	memset(temp, '\0', 16);
	sprintf_s(temp, 16, "Installdata_%d", _nInstallCount);
	memset(temp1, '\0', LENGTH);
	sprintf_s(temp1, 50, "%d", pdata->userid);
	WritePrivateProfileStringA(temp, "userid", temp1, path.c_str());	
	WritePrivateProfileStringA(temp, "mac", pdata->mac, path.c_str());
	WritePrivateProfileStringA(temp, "pcVersion", pdata->pcVersion, path.c_str());
	WritePrivateProfileStringA(temp, "zjVersion", pdata->appVersion, path.c_str());
	memset(temp1, '\0', LENGTH);
	sprintf_s(temp1, 50, "%d", pdata->sendTime);
	WritePrivateProfileStringA(temp, "Sendtime", temp1, path.c_str());

	WritePrivateProfileStringA(temp, "imei", pdata->pData->imei, path.c_str());	
	WritePrivateProfileStringA(temp, "imsi", pdata->pData->imsi, path.c_str());
	WritePrivateProfileStringA(temp, "mobileModel", pdata->pData->mobileModel, path.c_str());
	WritePrivateProfileStringA(temp, "mobileVersion", pdata->pData->mobileVersion, path.c_str());
	memset(temp, '\0', 16);
	sprintf_s(temp, 16, "%d", pdata->pData->platform);
	WritePrivateProfileStringA(temp, "platform", temp, path.c_str());	
	memset(temp, '\0', 16);
	sprintf_s(temp, 16, "%d", pdata->pData->jailbroken);
	WritePrivateProfileStringA(temp, "jailbroken", temp, path.c_str());
	memset(temp, '\0', 16);
	sprintf_s(temp, 16, "%d", pdata->pData->connectMode);
	WritePrivateProfileStringA(temp, "connectType", temp, path.c_str());

	WritePrivateProfileStringA(temp, "appid", pdata->pData->pData->identifier, path.c_str());	
	WritePrivateProfileStringA(temp, "appname", pdata->pData->pData->softName, path.c_str());
	WritePrivateProfileStringA(temp, "channel", pdata->pData->pData->channel, path.c_str());
	WritePrivateProfileStringA(temp, "appversion", pdata->pData->pData->softVersion, path.c_str());
	memset(temp1, '\0', LENGTH);
	sprintf_s(temp1, 50, "%d", pdata->pData->pData->operateTime);
	WritePrivateProfileStringA(temp, "installtime", temp1, path.c_str());
}

APPDOWNLOAD_RECORD* TaskHelpModel::ReadDownloadData()
{//INTERFUN;
	wstring iniPath = CONFIG(_strCachePath) + L"downInfo.ini";
	std::string path = CCodeOperation::UnicodeToGB2312(iniPath);

	if ( CFileOperation::IsFileExist(iniPath.c_str()) )
	{
		std::string data; 
		char temp[16]="", temp1[50]="";
		int num = GetIniIntValue(path.c_str(), "Config", "num");
		APPDOWNLOAD_RECORD* pdata = new APPDOWNLOAD_RECORD[num];
		
		for ( int i=0;i<num;i++ )
		{
			memset(temp, '\0', 16);
			sprintf_s(temp, 16, "Downdata_%d", i);

			APPOPERATE_INFO* app = new APPOPERATE_INFO;

			data	= GetIniCharValue(path.c_str(), temp, "appid");
			SetValues(app->identifier, data);
			data	= GetIniCharValue(path.c_str(), temp, "appname");
			SetValues(app->softName, data);
			data	= GetIniCharValue(path.c_str(), temp, "appversion");
			SetValues(app->softVersion, data);
			data	= GetIniCharValue(path.c_str(), temp, "channel");
			SetValues(app->channel, data);
			app->operateTime = GetIniIntValue(path.c_str(), temp, "downtime");

			pdata[i].userid		= GetIniIntValue(path.c_str(), temp, "userid");
			data	= GetIniCharValue(path.c_str(), temp, "mac");
			SetValues(pdata[i].mac, data);
			data	= GetIniCharValue(path.c_str(), temp, "pcVersion");
			SetValues(pdata[i].pcVersion, data);
			data	= GetIniCharValue(path.c_str(), temp, "zjVersion");
			SetValues(pdata[i].appVersion, data);
			pdata[i].sendTime	= GetIniIntValue(path.c_str(), temp, "Sendtime");
			pdata[i].pData		= app;
			pdata[i].length		= 1;
		}
		return pdata;
	}
	return NULL;
}

APPINSTALL_RECORD* TaskHelpModel::ReadInstallData()
{//INTERFUN;
	wstring iniPath = CONFIG(_strCachePath) + L"installInfo.ini";
	std::string path = CCodeOperation::UnicodeToGB2312(iniPath);

	if ( CFileOperation::IsFileExist(iniPath.c_str()) )
	{
		std::string data; 
		char temp[16]="", temp1[50]="";
		int num = GetIniIntValue(path.c_str(), "Config", "num");
		APPINSTALL_RECORD* pdata = new APPINSTALL_RECORD[num];

		for ( int i=0;i<num;i++ )
		{
			memset(temp, '\0', 16);
			sprintf_s(temp, 16, "Downdata_%d", i);

			APPOPERATE_INFO* app = new APPOPERATE_INFO;
			data	= GetIniCharValue(path.c_str(), temp, "appid");
			SetValues(app->identifier, data);
			data	= GetIniCharValue(path.c_str(), temp, "appname");
			SetValues(app->softName, data);
			data	= GetIniCharValue(path.c_str(), temp, "appversion");
			SetValues(app->softVersion, data);
			data	= GetIniCharValue(path.c_str(), temp, "channel");
			SetValues(app->channel, data);
			app->operateTime = GetIniIntValue(path.c_str(), temp, "installtime");

			DEVICE_LIST* dev = new DEVICE_LIST;
			data	= GetIniCharValue(path.c_str(), temp, "imei");
			SetValues(dev->imei, data);
			data	= GetIniCharValue(path.c_str(), temp, "imsi");
			SetValues(dev->imsi, data);
			data	= GetIniCharValue(path.c_str(), temp, "mobileModel");
			SetValues(dev->mobileModel, data);
			data	= GetIniCharValue(path.c_str(), temp, "mobileVersion");
			SetValues(dev->mobileVersion, data);
			dev->jailbroken = GetIniIntValue(path.c_str(), temp, "jailbroken");
			dev->platform	= GetIniIntValue(path.c_str(), temp, "platform");
			dev->connectMode= GetIniIntValue(path.c_str(), temp, "connectType");	
			dev->pData		= app;
			dev->length     = 1;

			pdata[i].userid		= GetIniIntValue(path.c_str(), temp, "num");
			data	= GetIniCharValue(path.c_str(), temp, "mac");
			SetValues(pdata[i].mac, data);
			data	= GetIniCharValue(path.c_str(), temp, "pcVersion");
			SetValues(pdata[i].pcVersion, data);
			pdata[i].sendTime	= GetIniIntValue(path.c_str(), temp, "num");
			pdata[i].pData		= dev;
			pdata[i].length		= 1;
		}

		return pdata;
	}
	return NULL;
}

std::string TaskHelpModel::GetIniCharValue( std::string strIniFile, std::string strGroup, std::string strKey )
{//INTERFUN;
	char buf[50] = {0};
	GetPrivateProfileStringA(strGroup.c_str(), strKey.c_str(), "", buf, LENGTH, strIniFile.c_str());
	std::string str(buf);
	return str;
}

long TaskHelpModel::GetIniIntValue( std::string strIniFile, std::string strGroup, std::string strKey )
{//INTERFUN;
	char buf[16];
	GetPrivateProfileStringA(strGroup.c_str(), strKey.c_str(), "", buf, 16, strIniFile.c_str());
	return atol(buf);
}

void TaskHelpModel::PostDataToService()
{//INTERFUN;
	//HANDLE nHandle = StartSimpleActionThread(Action_SendDataToService);	
}

void TaskHelpModel::SendDataToService( wstring strID )
{//INTERFUN;
	//BaseParam param;
	//param.SetStr(strID);
	//HANDLE nHandle = StartSimpleActionThread(Action_PostDataToService, &param);	
}

void TaskHelpModel::PostDownloadFinish( unsigned long m_nSpeed,wstring m_platform,int  m_fid, int m_uStatus)
{//INTERFUN;
	std::string url = "http://pcfirm.sj.91.com/controller.ashx?action=dostat&restype=";

	char buf[128] = {0};
	std::string append = "&speed=" + CStrOperation::Int2string(m_nSpeed);
	sprintf_s(buf, 128, "%s%s&platform=%s&resid=%d&statType=%s", 
		url.c_str(), 
		"Soft", 
		CCodeOperation::UnicodeToUTF_8(CStrOperation::toLowerW(m_platform)).c_str(), 
		m_fid, 
		m_uStatus == 4 ? "DownloadSuccess" : "DownloadFail" );//4表示FINISHED

	std::string postUrl = buf + append;


	const string threadid = "PostDownloadFinish" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	CTaskStatisticsThread* cTaskStThread = new CTaskStatisticsThread(threadid,Action_SendDownloadInfo);
	cTaskStThread->SetModel(this);
	cTaskStThread->SetStrParam(CCodeOperation::UTF_8ToUnicode(postUrl));
	cTaskStThread->Run();

	//BaseParam param;
	//param.SetStr(CCodeOperation::UTF_8ToUnicode(postUrl));
	//HANDLE nHandle = StartSimpleActionThread(Action_SendDownloadInfo, &param);	

}

void TaskHelpModel::PostInstallResultInfo( wstring m_platform,int m_fid,wstring m_appid, bool bSuccess)
{//INTERFUN;

	std::string strResult = bSuccess ? "SetupSuccess" : "SetupFail";

	std::string url = "http://pcfirm.sj.91.com/controller.ashx?action=dostat&restype=";

	char buf[256] = {0};
	sprintf_s(buf, 256, "%s%s&platform=%s&resid=%d&indentifier=%s&statType=%s", 
		url.c_str(), 
		"Soft", 
		CCodeOperation::UnicodeToUTF_8(CStrOperation::toLowerW(m_platform)).c_str(),
		m_fid < 0 ? 0 : m_fid, 
		CCodeOperation::UnicodeToUTF_8(m_appid).c_str(),
		strResult.c_str());

	std::string postUrl = buf;

	//BaseParam param;
	//param.SetStr(CCodeOperation::UTF_8ToUnicode(postUrl));
	//HANDLE nHandle = StartSimpleActionThread(Action_SendInstallInfo, &param);	

	const string threadid = "PostInstallResultInfo" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	CTaskStatisticsThread* cTaskStThread = new CTaskStatisticsThread(threadid,Action_SendInstallInfo);
	cTaskStThread->SetModel(this);
	cTaskStThread->SetStrParam(CCodeOperation::UTF_8ToUnicode(postUrl));
	cTaskStThread->Run();

}

void TaskHelpModel::PostInstallFailInfo( wstring filePath, int m_fid, wstring m_appid, wstring m_appVersion,
										 unsigned long TotalSize,wstring m_strName,wstring m_installerror,
										 wstring deviceId )
{//INTERFUN;
	RETURN_IF(deviceId.empty());
	/*DeviceData* pDevice = DEVICE_MANAGER->FindDevice(deviceId);
	RETURN_IF(!pDevice);*/
	RequestDeviceStateMsg deviceHelpMsg;
	deviceHelpMsg.deviceID = deviceId;
	deviceHelpMsg.SendAndGetReturn();

	RETURN_IF(!(deviceHelpMsg.bResult));
	LOG_DEBUG(L"TaskHelpModel::PostInstallFailInfo, 1");

	int mt = 0;
	wstring version = deviceHelpMsg.deviceProductVersion;//pDevice->GetDevInfo()->_deviceProductVersion;
	wstring url = L"http://bbxdata.sj.91.com/user.ashx?act=7";
	
	if(deviceHelpMsg.nOsType == ePhoneOS_Ios)
	{
		mt = 1;
		if ( deviceHelpMsg.bIsPad )
			version = L"ipad" + version;
	}
	else
		mt = 4;

	wstring pcVersion = L"pc" + CCodeOperation::s2ws(CClientInfo::GetInstance()->GetVersion());

	wchar_t urlBuf[256]={0}, dataBuf[1024]={0};
	swprintf_s( urlBuf, 256, L"%s&mt=%d&osv=%s&sv=%s", url.c_str(), mt, version.c_str(), pcVersion.c_str() );
	wstring pUrl = urlBuf;

	CDealTime gettime;
	std::string time = gettime.TimeFormat();
	std::string strhash = ""; 
	//wstring filePath = item->m_TaskFileInfo != NULL ? item->m_TaskFileInfo->savePath : item->m_csFileSavePath;
	swprintf_s( dataBuf, 1024, L"resId=%d&indentifier=%s&versionName=%s&size=%ld&clientStoreName=%s&installErrorType=%s&errorDesc=%s&downloadUrl=%s&fileMd5=%s&uuid=%s&downloadTime=%s", 
		m_fid, 
		m_appid.c_str(), 
		m_appVersion.c_str(),
		TotalSize, 
		m_strName.c_str(), 	
		CFileOperation::GetFileExt(filePath).c_str(),
		m_installerror.c_str(),
		"", 
		"", 
		CCodeOperation::UTF_8ToUnicode(deviceHelpMsg.strTrueImei).c_str(), 
		CCodeOperation::UTF_8ToUnicode(time).c_str() );

	LOG_DEBUG(L"TaskHelpModel::PostInstallFailInfo, 2");

	wstring data = dataBuf;

	std::string temp = CCodeOperation::UnicodeToUTF_8(pUrl);
	char* buf = new char[256];
	memset(buf, '\0', 256);
	memcpy_s(buf, 256, temp.c_str(), temp.length() > 256 ? 256 : temp.length() );

	/*BaseParam param;
	param.SetVoid(buf);
	param.SetStr(data);
	HANDLE nHandle = StartSimpleActionThread(Action_SendFailInfo, &param);	*/

	const string threadid = "PostInstallFailInfo" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	CTaskStatisticsThread* cTaskStThread = new CTaskStatisticsThread(threadid,Action_SendFailInfo);
	cTaskStThread->SetModel(this);
	cTaskStThread->SetStrParam(data);
	cTaskStThread->SetVoidParam(buf);
	cTaskStThread->Run();	

}

bool TaskHelpModel::Action( int nAction, StaticsticsInnerMsg* pParam )
{//INTERFUN;
	switch (nAction)
	{
	case Action_PostDownLoadInfo:
		SendDownLoadInfo(pParam);
		break;
	case Action_PostInstallInfo:
		SendInstallInfo(pParam);
		break;
	case Action_SendDataToService:
		//SendInfo(pParam);
		break;
	case Action_PostDataToService:
		SendData(pParam);
		break;
	case Action_SendDownloadInfo:
		SendDownloadSms(pParam);
		break;
	case Action_SendInstallInfo:
		SendSuccessInfo(pParam);
		break;
	case Action_SendFailInfo:
		SendFailInfo(pParam);
		break;
	}
	delete pParam;
	pParam = NULL;
	return true;
}

bool TaskHelpModel::SendDownLoadInfo( StaticsticsInnerMsg* pParam )
{//INTERFUN;
	APPDOWNLOAD_RECORD* down = (APPDOWNLOAD_RECORD*)pParam->voidParam;
	RETURN_FALSE_IF( !down );

	LOG_DEBUG(L"TaskHelpModel::PostDownLoadInfo, PostAppDownloadRecord Begin");

	Web p(1);
	int er = p.PostAppDownloadRecord(down);

	LOG_DEBUG(L"TaskHelpModel::PostDownLoadInfo, PostAppDownloadRecord End");

	if ( er )
	{
		_nDowntCount++;
		LOG_DEBUG(L"TaskHelpModel::PostDownLoadInfo, SaveDownloadData Begin");
		SaveDownloadData(down);
		LOG_DEBUG(L"TaskHelpModel::PostDownLoadInfo, SaveDownloadData End");
	}

	{
		wstring logPath = CONFIG(_strCachePath) + L"Download.log";
		
		if ( CFileOperation::IsFileExist( logPath.c_str() ) )
		{
			fstream fs;
			fs.open(logPath.c_str(),ios::out|ios::binary|ios::app);
			if (!fs) return false;
			
			std::string strTemp = down->pData->softName;
			strTemp = "软件名称:" + strTemp + "\r\n"; 
			fs.write( strTemp.c_str(), strTemp.length()+1 );

			strTemp = down->pData->identifier;
			strTemp = "软件标识:" + strTemp + "\r\n"; 
			fs.write( strTemp.c_str(), strTemp.length()+1 );

			strTemp = "是否上报下载数据成功:";
			std::string str = er == 0 ? "yes" : "no";
			strTemp += str + "\r\n";
			fs.write( strTemp.c_str(), strTemp.length()+1 );
			fs.close();
		}
	}

	return true;
}

bool TaskHelpModel::SendInstallInfo( StaticsticsInnerMsg* pParam )
{//INTERFUN;
	APPINSTALL_RECORD* install = (APPINSTALL_RECORD*)pParam->voidParam;
	RETURN_FALSE_IF( !install );

	LOG_DEBUG(L"TaskHelpModel::PostInstallInfo, PostAppInstallRecord Begin");
	Web p(1);
	int er = p.PostAppInstallRecord(install);
	LOG_DEBUG(L"TaskHelpModel::PostInstallInfo, PostAppInstallRecord End");
	if ( er )
	{
		LOG_DEBUG(L"TaskHelpModel::PostInstallInfo, SaveInstallData Begin");
		_nInstallCount++;
		SaveInstallData(install);	
		LOG_DEBUG(L"TaskHelpModel::PostInstallInfo, SaveInstallData End");	
	}

	{

		wstring logPath = CONFIG(_strCachePath) + L"install.log";
		
		if ( CFileOperation::IsFileExist( logPath.c_str() ) )
		{
			fstream fs;
			fs.open(logPath.c_str(),ios::out|ios::binary|ios::app);
			if (!fs) return false;

			std::string strTemp = install->pData->pData->softName;
			strTemp = "软件名称:" + strTemp + "\r\n"; 
			fs.write( strTemp.c_str(), strTemp.length()+1 );

			strTemp = install->pData->pData->identifier;
			strTemp = "软件标识:" + strTemp + "\r\n"; 
			fs.write( strTemp.c_str(), strTemp.length()+1 );

			strTemp = "是否上报安装数据成功:";
			std::string str = er == 0 ? "yes" : "no";
			strTemp += str + "\r\n";
			fs.write( strTemp.c_str(), strTemp.length()+1 );
			fs.close();
		}
	}

	return true;
}

bool TaskHelpModel::SendInfo( StaticsticsInnerMsg* pParam )
{//INTERFUN;
	wstring dPath = CONFIG(_strCachePath) + L"downInfo.ini";
	wstring iPath = CONFIG(_strCachePath) + L"installInfo.ini";

	APPDOWNLOAD_RECORD* pDodata = ReadDownloadData();
	APPINSTALL_RECORD* pIndata = ReadInstallData();

	Web p(1);
	if ( pDodata )
		p.PostAppDownloadRecord(pDodata);

	if ( pIndata )
		p.PostAppInstallRecord(pIndata);

	ReleaseData();
	DeleteFile(dPath.c_str());
	DeleteFile(iPath.c_str());

	return true;
}

bool TaskHelpModel::SendData( StaticsticsInnerMsg* pParam )
{//INTERFUN;
	wstring strID = pParam->strParam;
	RETURN_FALSE_IF (strID.empty());

	Web p(1);
	int num = _verDownloads.size();
	APPDOWNLOAD_RECORD* pDodata = new APPDOWNLOAD_RECORD[num];

	for ( int i=0;i<num;i++ )
	{
		APPDOWNLOAD_RECORD* p = _verDownloads[i];
		p->userid = _wtoi(strID.c_str());
		pDodata[i] = *p;	
	}
	if ( pDodata && num > 0 )
		p.PostAppDownloadRecord(pDodata);


	num = _verInstalls.size();
	APPINSTALL_RECORD* pIndata = new APPINSTALL_RECORD[num];
	for ( int i=0;i<(int)_verInstalls.size();i++ )
	{
		APPINSTALL_RECORD* p = _verInstalls[i];
		p->userid = _wtoi(strID.c_str());
		pIndata[i] = *p;	
	}
	if ( pIndata && num > 0 )
		p.PostAppInstallRecord(pIndata);

	ReleaseData();
	
	return true;
}

bool TaskHelpModel::SendDownloadSms( StaticsticsInnerMsg* pParam )
{//INTERFUN;
	wstring postUrl = pParam->strParam;
	RETURN_FALSE_IF ( postUrl.empty() );
	std::string retData = CNetOperation::Get( CCodeOperation::UnicodeToUTF_8(postUrl) );

	return true;
}

bool TaskHelpModel::SendSuccessInfo( StaticsticsInnerMsg* pParam )
{//INTERFUN;
	wstring postUrl = pParam->strParam;
	RETURN_FALSE_IF ( postUrl.empty() );
	std::string retData = CNetOperation::Get( CCodeOperation::UnicodeToUTF_8(postUrl) );

	return true;
}

bool TaskHelpModel::SendFailInfo( StaticsticsInnerMsg* pParam )
{//INTERFUN;

	char* url = (char*)pParam->voidParam;
	RETURN_FALSE_IF ( !url );
	
	std::string postUrl = url;
	wstring	data = pParam->strParam;
	if ( url ) SAFE_DELETE(url);
	
	RETURN_FALSE_IF ( postUrl.empty() );

	std::string ret = CNetOperation::Post( postUrl, CCodeOperation::UnicodeToUTF_8(data) );
	return true;

}

void TaskHelpModel::SetValues( char* szBuf, std::string str )
{
	memset(szBuf, '\0', LENGTH+1);
	memcpy_s(szBuf, LENGTH, str.c_str(), str.length() > LENGTH ? LENGTH : str.length() );
}

//bool TaskHelpModel::Handle( const StaticsticsInnerMsg* pMsg )
//{
//	Action(pMsg->_action,const_cast<StaticsticsInnerMsg*>(pMsg));
//	return true;
//}

bool TaskHelpModel::Handle( const PostDownloadInfoMsg* pMsg )
{
	PostDownLoadInfo(pMsg->m_appid,pMsg->m_platform,pMsg->m_csFileDownUrl,pMsg->m_strName,pMsg->m_appVersion,pMsg->m_csFileSavePath);
	return true;
}

bool TaskHelpModel::Handle( const PostDownloadFinishMsg* pMsg )
{
	PostDownloadFinish(pMsg->m_nSpeed,pMsg->m_platform,pMsg->m_fid,pMsg->m_uStatus);
	return true;
}

bool TaskHelpModel::Handle( const PostInstallInfoMsg* pMsg )
{
	PostInstallInfo(pMsg->m_appid,pMsg->m_csFileSavePath,pMsg->m_strName,pMsg->m_appVersion,pMsg->deviceId);
	return true;
}

bool TaskHelpModel::Handle( const PostInstallResultInfoMsg* pMsg )
{
	PostInstallResultInfo(pMsg->m_platform,pMsg->m_fid,pMsg->m_appid,pMsg->bSuccess);
	return true;
}

bool TaskHelpModel::Handle( const PostInstallFailInfoMsg* pMsg )
{
	PostInstallFailInfo(pMsg->filePath,pMsg->m_fid,pMsg->m_appid,pMsg->m_appVersion,pMsg->TotalSize,pMsg->m_strName,pMsg->m_installerror,
		pMsg->deviceId);
	return true;
}
