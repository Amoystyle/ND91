#include "pch_module.h"

#ifdef MODULE_THEME

#include "Module/Theme/AndroidThemeManager.h"
#include "Module/Theme/PandaThemeActionReturn.h"
#include "Module/Theme/ThemeOperateCommand.h"
#include "Module/Theme/AndroidThemeXmlHelper.h"

#include "Module/Common/AndroidXMLParser/AndroidXMLOperation.h"
#include "Module/Application/UninstallCommand.h"
#include "Module/Application/InstallCommand.h"
#include "Module/Application/GetAppInfoCommand.h"
#include "Common/Path.h"

#define PANDAHOME	L"com.nd.android.pandahome"//�̼�2.0����
#define PANDAHOME2  L"com.nd.android.pandahome2"//�̼�2.0����
#define SMARTHOME   L"com.nd.android.smarthome"//��׿����

#define PANDAHOME_VERSION	L"1.9.2"
#define PANDAHOME2_VERSION  L"1.8"
#define SMARTHOME_VERSION	L"0.92"

#define smartHomeThemeInfoRemotePath	L"/sdcard/smarthomethemeinfo.xml" //��׿�����xml
#define smartHomeThemePath				L"/sdcard/SmartHome/Themes/"

#define All_THEME_INFO_XML  L"/sdcard/nd/pandahome/themeinfo/themeinfo.xml"         // �̼�2.0���µĻ�ȡȫ�������xml�ļ�
#define THEME_BRIEF_XML	    L"/sdcard/nd/pandahome/themeinfo/themebriefinfo.xml"    // �̼�2.0���ϻ�ȡ����id�б��xml�ļ�
#define THEME_DETAIL_ROOT   L"/mnt/sdcard/nd/pandahome/themeinfo/"                      // ������ϸ��Ϣxml�ļ���ͼƬ�洢·��

#define PANDA_THEME_XML     L"\\panda_theme.xml"                    // apt��ѹ�����������Ϣ��xml�ļ�

AndroidThemeManager::AndroidThemeManager(const DeviceData* pDevice)
:ThemeManager(pDevice)
{
    _pActionReturnData = NULL;
    //_bActionReturnData = false;
	_curActionType = ENone;
    _pCurrentTheme = NULL;
	
}

AndroidThemeManager::~AndroidThemeManager()
{

}

void ND91Assistant::AndroidThemeManager::Release()
{
    SAFE_DELETE(_pActionReturnData);
}

void ND91Assistant::AndroidThemeManager::PandaThemeAction( const PandaThemeActionReturn* pData )
{
	if(pData->_nAction == _curActionType)
	{
		SAFE_DELETE(_pActionReturnData);
		_pActionReturnData = new PandaThemeActionReturn(pData);
		//_bActionReturnData = true;
	}
}

ND91Assistant::eThemeInitCode ND91Assistant::AndroidThemeManager::InitEnvironment()
{
#ifdef MODULE_APP
    double version = _wtof(_pDevice->GetDevInfo()->_deviceProductVersion.c_str());
    if(version < 2.0)
        return eThemeInitCode_Android_NotSupport;
	eThemeInitCode initcode=eThemeInitCode_Error;

	int panda2= AppInstallStatus(PANDAHOME2,PANDAHOME2_VERSION);
	int smart=AppInstallStatus(SMARTHOME,SMARTHOME_VERSION);

	if(version<2.1)
	{
		if(panda2==1)
			return eThemeInitCode_Success;
		else
			return eThemeInitCode_Android_NotSupport_SmartHome;
	}
	if(panda2==1 && smart==1)
		return eThemeInitCode_Success;
	if(panda2==-1&&smart==-1)
		return eThemeInitCode_Android_NeedInstall_BothHome;
	if(panda2==0 && smart==0)
		return eThemeInitCode_Error;
	if(panda2!=1)
		return eThemeInitCode_Android_NeedInstall_SmartHome;
	if(smart!=1)
		return eThemeInitCode_Android_NeedInstall_PandaHome_Two;
	return eThemeInitCode_Error;
#else
    return eThemeInitCode_Success;
#endif

}

int ND91Assistant::AndroidThemeManager::AppInstallStatus(wstring pkgname, wstring minVersion)
{
	GetAppInfoCommand  cmd(_pDevice, pkgname);
	cmd.Execute();
	const AppData* data = cmd.GetAppInfo();
	if(cmd.Success() && !data->_strVersion.empty() )
	{
		wstring versionstr = data->_strVersion;
		if (pkgname==PANDAHOME2 && data->_strVersion .empty() && data->_nVersionCode == 2190)
			versionstr = L"1.9.0";
		if(versionstr.compare(minVersion)>-1)
		return 1;
	}
	if(!cmd.ExecuteSuccess())
		return 0;

	return -1;
}

bool ND91Assistant::AndroidThemeManager::Load()
{
    // �ͷ�
    ClearThemeList();   
    double version = _wtof(_pDevice->GetDevInfo()->_deviceProductVersion.c_str());
    RETURN_FALSE_IF(version<2.0);

    ThemeOperateCommand themeCmd(_pDevice);
    themeCmd.InitUpdateThemes();

    if ( version < 2.0)
    {
        LoadAllThemes();//һ���Լ��س�ȫ����ϸ��Ϣ
    }
    else
    {
        ThemeBriefInfo BriefInfo = LoadThemeBreifInfo();// ���س�id�б�,���GetThemeDetail�����������

        for(int i = 0; i < BriefInfo._count; i++)
        {
            ThemeData* theme = new ThemeData();
            theme->_id = BriefInfo._themeIdList[i];
            AddTheme(theme->_id, theme);
            if(theme->_id == BriefInfo._currentThemeId)
            {
                _pCurrentTheme =  theme;
            }
        }
		LoadSmartThemes();
    }

    return true;
}

bool ND91Assistant::AndroidThemeManager::GetThemeDetailInfo( ThemeData* pThemeData )
{
    RETURN_FALSE_IF(!pThemeData);
    RETURN_FALSE_IF(pThemeData->_id.empty());

    bool bRec = false;

    // ���������ػ�����xml�ļ�
    ThemeOperateCommand cmd(_pDevice);
    if(!cmd.GetThemeDetailInfo(pThemeData->_id))
        return FALSE;

    wstring themeDetailRoot = THEME_DETAIL_ROOT;
    wstring themeDetailPath = themeDetailRoot+CStrOperation::ReplaceW(pThemeData->_id,L"\\",L"/")+L".xml";
    DirectoryOperationCommand dircmd(_pDevice);

    bool hasReturnFromGetDetail = false;
    int seconds = 0;
    do // �ȴ��ļ�����
    {
        hasReturnFromGetDetail = dircmd.Exist(themeDetailPath);
        seconds += 100;
        Sleep(100);
    } while (seconds<10000 && !hasReturnFromGetDetail);

    if(hasReturnFromGetDetail)
    { 
        wstring localpath = _pDevice->GetThemePath()->GetThemeTempPath();
        CFileOperation::MakeLocalDir(localpath);
        localpath += CStrOperation::ReplaceW(pThemeData->_id,L"/",L"\\")+L".xml";

        // xml�ļ�����
        CFileOperation::DeleteFile(localpath);

        // ����xml�ļ�
        if ( DownloadFileCommand(_pDevice).Download(localpath, themeDetailPath) )
        {
            // ���سɹ�

            // ɾ���ֻ������ɵ�xml�ļ�
            dircmd.DeleteFileLink(themeDetailPath);

            // ����xml�ļ��õ�������ϸ��Ϣ
            ThemeData* tmptheme = AndroidThemeXmlHelper::ResolveDetailTheme(localpath);
            pThemeData->clone(tmptheme);
            SAFE_DELETE(tmptheme);

            bRec = true;
        }
    }
    return bRec;
}

bool ND91Assistant::AndroidThemeManager::LoadPreviewPhoto( ThemeData* pThemeData )
{
    RETURN_FALSE_IF(!pThemeData);

    bool bRec = false;
    wstring tempfolder = _pDevice->GetThemePath()->GetPath()
        + CStrOperation::ReplaceW(pThemeData->_id,L"/",L"\\")+L"\\";
    CFileOperation::MakeLocalDir(tempfolder);

    //���ر�ֽ
    DirectoryOperationCommand dircmd(_pDevice);
    DownloadFileCommand downFileCmd(_pDevice);
    if(!pThemeData->_wallPaperPath.empty() && dircmd.Exist(pThemeData->_wallPaperPath))
    {
        wstring wallpaperLocalPath = tempfolder + CFileOperation::GetFileName(pThemeData->_wallPaperPath);
        if(downFileCmd.Download(wallpaperLocalPath, pThemeData->_wallPaperPath))
        {
            pThemeData->_localWallPaperPath = wallpaperLocalPath;
            bRec = true;
        }
    }
    pThemeData->_strPreviewPathOnPC = pThemeData->_localWallPaperPath;
    return bRec;
}

ThemeData* ND91Assistant::AndroidThemeManager::GetCurTheme()
{
    return _pCurrentTheme;
}

bool ND91Assistant::AndroidThemeManager::Rename( ThemeData* pThemeData, wstring strNewName )
{
    return false;
}

bool ND91Assistant::AndroidThemeManager::DeleteTheme( ThemeData* pThemeData )
{
    ThemeData* theme = pThemeData;
    RETURN_FALSE_IF(!theme|| theme->_id.empty());
    // Ĭ�����ⲻ��ɾ��
    if(theme->_id == L"default_system_theme" || theme->_id == L"defaultTheme")
        return false;
    double version = _wtof(_pDevice->GetDevInfo()->_deviceProductVersion.c_str());
    bool uninstallResultfail = true;
    if(theme->_themeType == ThemeType_APK)
    {
#ifdef MODULE_APP
        uninstallResultfail = UninstallCommand::SimpleUninstallApp(_pDevice,theme->_id, APP_TYPE_APK, true);
        if(!uninstallResultfail)
        {
            GetAppInfoCommand  cmd(_pDevice, theme->_id+L".apk");
            cmd.Execute();
            if(cmd.Success())
                uninstallResultfail = false;
            else
                uninstallResultfail = true;
        }
#else
        uninstallResultfail = false;
#endif
    }
    else if(version < 2.0)//apt��ʽ
    {
#ifdef MODULE_APP
        GetAppInfoCommand  cmd(_pDevice, theme->_id+L".apk");
        cmd.Execute();
        if(cmd.Success())
        {
            uninstallResultfail = UninstallCommand::SimpleUninstallApp(_pDevice,theme->_id,APP_TYPE_APK, true);
            if(!uninstallResultfail)
            {
                GetAppInfoCommand  cmd(_pDevice, theme->_id+L".apk");
                cmd.Execute();
                if (!cmd.Success())
                    uninstallResultfail = true;
                else
                    uninstallResultfail = false;
            }
        }
#else
        uninstallResultfail = false;
#endif
    }
    ThemeOperateCommand themecmd(_pDevice);
    if(!uninstallResultfail || themecmd.DelTheme(theme->_id,theme->_themeType==ThemeType_SmartHome))
    {
		_curActionType = EActionDelTheme;
		WaitForDeamonReturn();
		if(_pActionReturnData && (_pActionReturnData->_nResult == 1 || _pActionReturnData->_nResult==20))
		{
		    //themecmd.InitUpdateThemes();
			RemoveTheme(theme);
			if (_pCurrentTheme == theme)
				_pCurrentTheme = NULL;
			return true;
		}
    }
    return false;
}

ThemeData* ND91Assistant::AndroidThemeManager::AddLocalTheme( wstring strLocalFile, ProgressObserver* pObserver, wstring strPreviewFile)
{
    RETURN_NULL_IF(strLocalFile.empty() || !CFileOperation::IsFileExist(strLocalFile.c_str()));
    wstring ext = L"";
    int  pos = strLocalFile.find_last_of(L".");
    if(pos>-1)
        ext = strLocalFile.substr(pos+1);
    if(ext == L"apt")
    {
        return AddAptTheme(strLocalFile,strPreviewFile==L"smarthome");
    }
    else if(ext == L"apk")
    {
        return AddAPKTheme(strLocalFile);
    }
    return NULL;
}

std::wstring ND91Assistant::AndroidThemeManager::BackupTheme( ThemeData* pThemeData, wstring SavePath )
{
    if(!pThemeData)
        return L"";
    wstring pkgPath=L"/sdcard/PandaHome2/Themes/";
	wstring themeId=pThemeData->_id;
	if(pThemeData->_themeType==ThemeType_SmartHome)
	{
		themeId=CCodeOperation::UTF_8ToUnicode(AndroidThemeXmlHelper::urldecode(CCodeOperation::UnicodeToUTF_8(pThemeData->_id)));
		pkgPath = L"/sdcard/SmartHome/Themes/";
	}
	else
		themeId=CStrOperation::ReplaceW(themeId,L" ",L"_");
	int pos = themeId.find(L"/");
	if(pos>-1)
		themeId = themeId.substr(0,pos);

    DirectoryOperationCommand dircmd(_pDevice);

	wstring path = SavePath;
	
	DirData* dir=new DirData(_pDevice);
	dir->_name=pkgPath;
	NodeData* node=dir->findChildrenbyName(themeId);
	vector<NodeData*> nodes;nodes.push_back(node);
	//CFileOperation::DeleteDirectory(path+themeId);
	if(node && FileHelper::Download(&nodes, path,NULL) == Transfer_Success)
    {
		path += themeId;
		SAFE_DELETE(dir);
		CFileOperation::ZipDirectory(path+L"\\",path+L".apt");
		CFileOperation::DeleteDirectory(path);
        return path+L".apt";
    }
	SAFE_DELETE(dir);
    return L"";
}

bool ND91Assistant::AndroidThemeManager::SetCurTheme( ThemeData* pThemeData )
{
    ThemeData* theme = pThemeData;
    RETURN_FALSE_IF(!theme||theme->_id.empty());

    wstring themeId = theme->_id;
    if ( themeId == L"default_system_theme" || themeId == L"defaultTheme")
    {
        themeId = L"0";
    }
    ThemeOperateCommand themecmd(_pDevice);
    if(themecmd.SetTheme(themeId,pThemeData->_themeType==ThemeType_SmartHome))
    {
		_curActionType = EActionSetTheme;
		WaitForDeamonReturn();
		if(_pActionReturnData && (_pActionReturnData->_nResult == 1 || _pActionReturnData->_nResult == 11))
		{
			_pCurrentTheme = theme;
			return true;
		}
    }
    return false;
}

bool ND91Assistant::AndroidThemeManager::RevocationTheme()
{
    return false;
}

bool ND91Assistant::AndroidThemeManager::CheckIsThemePath( wstring strPath )
{
    return false;
}

bool ND91Assistant::AndroidThemeManager::WaitForDeamonReturn( int nWaitTime /*= 30000*/ )
{
	SAFE_DELETE(_pActionReturnData);
    int nTime = 0;
    while (!_pActionReturnData && nTime < nWaitTime)
    {
		Sleep(50);
        nTime += 50;
    }
    return _pActionReturnData != NULL;
}


void ND91Assistant::AndroidThemeManager::LoadAllThemes()
{
    wstring remotePath = All_THEME_INFO_XML;
    // ɾ��xml�ļ�
    DirectoryOperationCommand dircmd(_pDevice);
    dircmd.DeleteFileLink(remotePath);

    // �ػ���������xml
    ThemeOperateCommand cmd(_pDevice);
    RETURN_IF(!cmd.GetThemeInfos(remotePath))

        bool hasReturnFromGetThemeInfo = false;
    int seconds = 0;
    do 
    {
        hasReturnFromGetThemeInfo = dircmd.Exist(remotePath);
        seconds += 100;
        Sleep(100);
    } while (seconds<1000 && !hasReturnFromGetThemeInfo);

    RETURN_IF(!hasReturnFromGetThemeInfo);

    wstring localPath = _pDevice->GetThemePath()->GetThemeTempPath();
    CFileOperation::MakeLocalDir(localPath);

    localPath += CFileOperation::GetFileName(remotePath);
    CFileOperation::DeleteFile(localPath);

    // ����xml�ļ�
    if(DownloadFileCommand(_pDevice).Download(localPath, remotePath))
    {
        // ���سɹ�ɾ���ֻ���xml�ļ�
        dircmd.DeleteFileLink(remotePath);

        // �������ص����ص�xml�ļ��õ������б�
        deque<ThemeData*> themeList;
        _pCurrentTheme = AndroidThemeXmlHelper::ResolveAllThemes(localPath, &themeList);
        for (deque<ThemeData*>::iterator it = themeList.begin(); it != themeList.end(); it++)
        {
            AddTheme((*it)->_id, *it);
        }
    }
    else
    {//����ʧ�ܣ�ɾ�������ļ�
        CFileOperation::DeleteFile(localPath);
    }
}

ND91Assistant::ThemeBriefInfo ND91Assistant::AndroidThemeManager::LoadThemeBreifInfo()
{
    ThemeBriefInfo BriefInfo;
    ThemeOperateCommand cmd(_pDevice);
    if(!cmd.GetThemeBriefInfo())
        return BriefInfo;

    wstring themeBriefPath = THEME_BRIEF_XML;
    DirectoryOperationCommand dircmd(_pDevice);

    bool hasReturnFromLoadBreifInfo = false;
    int seconds = 0;
    do // �ȴ��ļ�����
    {
        hasReturnFromLoadBreifInfo = dircmd.Exist(themeBriefPath);
        seconds += 100;
        Sleep(100);
    } while (seconds<10000 && !hasReturnFromLoadBreifInfo);

    if(hasReturnFromLoadBreifInfo)
    { 
        wstring localPath = _pDevice->GetThemePath()->GetThemeTempPath();
        CFileOperation::MakeLocalDir(localPath);

        localPath += L"themebriefinfo.xml";
        CFileOperation::DeleteFile(localPath);

        if(DownloadFileCommand(_pDevice).Download(localPath, themeBriefPath))
        {
            dircmd.DeleteFileLink(themeBriefPath);

            BriefInfo = AndroidThemeXmlHelper::ResolveBriefTheme(localPath);
        }
    }

    return BriefInfo;   
}

ThemeData* ND91Assistant::AndroidThemeManager::AddAPKTheme( wstring localApkPath )
{
    // ��װ
    // TODO�������ȡ��װ��ID�������б���������id����һ�£����Ǹ��ǻ�װ
	wstring apkid  = AndroidXMLOperation::GetPackageNameFromApk(localApkPath);
	 
	if(FindTheme(apkid))
	{
		return FindTheme(apkid);
	}
#ifdef MODULE_APP
    bool result  = InstallCommand::SimpleInstallApp(_pDevice, localApkPath, apkid, APP_TYPE_APK);
    if (!result)
        return NULL;
#else
    return NULL;
#endif

    ThemeOperateCommand  themeCmd(_pDevice);
    themeCmd.InitUpdateThemes();

   // RETURN_NULL_IF (_wtof(_pDevice->GetDevInfo()->_deviceProductVersion.c_str()) < 2.0);

    // �ȴ��ػ�����
    //WaitForDeamonReturn();
   // if (_pActionReturnData && !_pActionReturnData->_strValue.empty())
    {
        ThemeData* theme = new ThemeData();
        theme->_id = apkid;
        AddTheme(theme->_id, theme);
        GetThemeDetailInfo(theme);
        return theme;
    }
    return NULL;
}

ThemeData* ND91Assistant::AndroidThemeManager::AddAptTheme( wstring localAptPath,bool IsSmart)
{
	double version = _wtof(_pDevice->GetDevInfo()->_deviceProductVersion.c_str());
	RETURN_NULL_IF(version<2.0);

    // �ϴ������w�ļ�
    UploadFileCommand  upfileCmd(_pDevice);
	wstring remotepath=_pDevice->GetDevInfo()->_sdcardpath+L"nd/InstallTheme/";
// 	if(!upfileCmd.Upload(localAptPath, remotepath+CFileOperation::GetFileName(localAptPath)))
//         return NULL;
    upfileCmd.Upload(localAptPath, remotepath+CFileOperation::GetFileName(localAptPath));

    //��ѹ��
    wstring unzippath = _pDevice->GetThemePath()->GetThemeTempPath();
	unzippath += CFileOperation::GetFileNameNoExt(localAptPath);
	CFileOperation::MakeLocalDir(unzippath);
    CFileOperation::UnZipFile(localAptPath, unzippath);
	

    // �������������ʱĿ¼
    wstring addAptLocalTempPathRoot = _pDevice->GetThemePath()->GetAdpPath();
    CFileOperation::MakeLocalDir(addAptLocalTempPathRoot);

    addAptLocalTempPathRoot += CFileOperation::GetFileNameNoExt(localAptPath)+L".apt";
    CFileOperation::CopyFile(localAptPath, addAptLocalTempPathRoot, false);

    // ��������xml�ļ�
    wstring themexmlfile = unzippath+ PANDA_THEME_XML;
    ThemeData* newtheme = NULL;
    if(CFileOperation::IsFileExist(themexmlfile.c_str()))
    { 
        newtheme = AndroidThemeXmlHelper::ResolveTotalTheme(themexmlfile);
        if (newtheme)
        {
			if((newtheme->_themeType==ThemeType_SmartHome && (version<2.1||AppInstallStatus(SMARTHOME,SMARTHOME_VERSION)!=1))
				||(newtheme->_themeType==ThemeType_PandaHome && AppInstallStatus(PANDAHOME2,PANDAHOME2_VERSION)!=1))
			{
				SAFE_DELETE(newtheme);
				return NULL;
			}
            newtheme->_addAptLocalPath = addAptLocalTempPathRoot;
        }
    }
    // �������
    ThemeOperateCommand themeCmd(_pDevice);
    bool  result = themeCmd.AddTheme(remotepath+CFileOperation::GetFileName(localAptPath),newtheme->_themeType==ThemeType_SmartHome);
    if(result && newtheme)
    {
       // if(_wtof(_pDevice->GetDevInfo()->_deviceProductVersion.c_str()) < 2.0)
        {
			_curActionType = EActionAddTheme;
            WaitForDeamonReturn(10000);
            //if (_pActionReturnData && !_pActionReturnData->_strValue.empty())
            {
                //newtheme->_id = CCodeOperation::UTF_8ToUnicode(AndroidThemeXmlHelper::urldecode(CCodeOperation::UnicodeToUTF_8(_pActionReturnData->_strValue)));
				AddTheme(newtheme);
				//DirectoryOperationCommand(_pDevice).DeleteFileLink(remotepath+CFileOperation::GetFileName(localAptPath));
				return newtheme;
            }
        }
    }
    SAFE_DELETE(newtheme);
    return NULL;
}

void ND91Assistant::AndroidThemeManager::LoadSmartThemes()
{
	ThemeOperateCommand cmd(_pDevice);
	if(!cmd.GetSmartThemeXml())
		return;

	DirectoryOperationCommand dircmd(_pDevice);

	bool hasReturnFromLoadBreifInfo = false;
	int seconds = 0;
	do // �ȴ��ļ�����
	{
		hasReturnFromLoadBreifInfo = dircmd.Exist(smartHomeThemeInfoRemotePath);
		seconds += 100;
		Sleep(100);
	} while (seconds<10000 && !hasReturnFromLoadBreifInfo);

	if(hasReturnFromLoadBreifInfo)
	{ 
		wstring localPath = _pDevice->GetThemePath()->GetThemeTempPath();
		CFileOperation::MakeLocalDir(localPath);
		localPath += L"smarthomethemeinfo.xml";
		CFileOperation::DeleteFile(localPath);
		if(DownloadFileCommand(_pDevice).Download(localPath, smartHomeThemeInfoRemotePath))
		{
			dircmd.DeleteFileLink(smartHomeThemeInfoRemotePath);
			deque<ThemeData*> smartthems;
			ThemeData* curtheme=AndroidThemeXmlHelper::ResolveSmartThemes(localPath,&smartthems);
			for(size_t i = 0; i < smartthems.size();i++)
			{
				AddTheme(smartthems[i]->_id,smartthems[i]);
			}
		}
	}
}

#endif