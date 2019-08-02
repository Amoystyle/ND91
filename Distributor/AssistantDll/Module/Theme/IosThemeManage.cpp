#include "pch_module.h"

#ifdef MODULE_THEME

#include "Module/Theme/IosThemeManage.h"
#include "Module/Theme/CreateMomeCommand.h"

#include "Module/System/LogOffCommand.h"
#include "Common/Path.h"

IosThemeManage::IosThemeManage(const DeviceData* pDevice)
:ThemeManager(pDevice)
{
	_pFileStream = NULL;
	_nProductVer = 0;
	_eDevicePsys = eIosDevSys_Unknow;	
	_strWSBoardThemePath = Ios_WSBoard_Theme_Path;
	_strPandaHomeThemePath = Ios_PandaHome_Theme_Path;
	_curThemePlistFilePath=Ios_PandaHome_Setting_File;

	InitApplicationNameMap();
	InitNormalInfo();
	InitThemeFilter();
	InitDefaultIcon();
}

IosThemeManage::~IosThemeManage(void)
{
}

void ND91Assistant::IosThemeManage::Release()
{
    SAFE_DELETE(_pFileStream);
    _vtPandaHomeFilter.clear();
    _vtWinterBoardFilter.clear();

    FreeApplicationNameMap();

    FreeDefaultIcon();
}

void IosThemeManage::FreeDefaultIcon()
{
	map<eAppIconType, Image*>::iterator itTwo;
	for (itTwo=_mapDefaultIcon.begin(); itTwo!=_mapDefaultIcon.end(); itTwo++)
	{
		SAFE_DELETE(itTwo->second);
	}
	_mapDefaultIcon.clear();
}

void IosThemeManage::InitDefaultIcon()
{
	FreeDefaultIcon();

	_mapDefaultIcon.insert(pair<eAppIconType, Image*>(eAppIconType_Messages, NULL));
	_mapDefaultIcon.insert(pair<eAppIconType, Image*>(eAppIconType_Calendar, NULL));
	_mapDefaultIcon.insert(pair<eAppIconType, Image*>(eAppIconType_Photos, NULL));
	_mapDefaultIcon.insert(pair<eAppIconType, Image*>(eAppIconType_Camera, NULL));
	_mapDefaultIcon.insert(pair<eAppIconType, Image*>(eAppIconType_YouTube, NULL));
	_mapDefaultIcon.insert(pair<eAppIconType, Image*>(eAppIconType_Stocks, NULL));
	_mapDefaultIcon.insert(pair<eAppIconType, Image*>(eAppIconType_Maps, NULL));
	_mapDefaultIcon.insert(pair<eAppIconType, Image*>(eAppIconType_Weather, NULL));
	_mapDefaultIcon.insert(pair<eAppIconType, Image*>(eAppIconType_Clock, NULL));
	_mapDefaultIcon.insert(pair<eAppIconType, Image*>(eAppIconType_Calculator, NULL));
	_mapDefaultIcon.insert(pair<eAppIconType, Image*>(eAppIconType_Notes, NULL));
	_mapDefaultIcon.insert(pair<eAppIconType, Image*>(eAppIconType_Settings, NULL));
	_mapDefaultIcon.insert(pair<eAppIconType, Image*>(eAppIconType_Phone, NULL));
	_mapDefaultIcon.insert(pair<eAppIconType, Image*>(eAppIconType_Mail, NULL));
	_mapDefaultIcon.insert(pair<eAppIconType, Image*>(eAppIconType_Safari, NULL));
	_mapDefaultIcon.insert(pair<eAppIconType, Image*>(eAppIconType_iPod, NULL));
}

eThemeInitCode IosThemeManage::InitEnvironment()
{
	if (!_pFileStream)
		return eThemeInitCode_Error;

	if (!_pDevice->GetDevInfo()->_ios_bJailBreaked)
		return eThemeInitCode_Ios_UnJailBreaked;

	CreateMomeCommand cmd(_pDevice);
	int code = cmd.CheckPHInstalled();

	if (_nProductVer < 200)
	{
		if (!_pFileStream->Ios_Exists(Ios_SummerBoard_App_Folder))
			return eThemeInitCode_Ios_UnInstall_SummerBoard;
	}
	else if (_nProductVer < 300)
	{
		if (!_pFileStream->Ios_Exists(Ios_WinterBoard_App_Folder))
			return eThemeInitCode_Ios_UnInstall_WinterBoard;
	}
	else if (!_pFileStream->Ios_Exists(Ios_WinterBoard_App_Folder) &&
		(!_pFileStream->Ios_Exists(Ios_PandaHome_App_Folder) || !_pFileStream->Ios_Exists(Ios_PandaHome_Dylib)))
	{
		return eThemeInitCode_Ios_UnInstall_PandaHome;
	}	

	return eThemeInitCode_Success;
}

bool IosThemeManage::Load()
{
	ClearThemeList();

	LoadPandaHomeThemes();

	LoadWinterBoardThemes();

	return true;
}

bool IosThemeManage::GetThemeDetailInfo(ThemeData* pThemeData)
{
	return true;
}

bool IosThemeManage::LoadPreviewPhoto(ThemeData* pThemeData)
{
	RETURN_FALSE_IF(!pThemeData);

    CFileOperation::DeleteFile(pThemeData->_strPreviewPathOnPC);

	if (GetPreviewDirect(pThemeData))
		return true;
	else if (GetPreviewFromSource(pThemeData))
		return true;

	return false;
}

ThemeData* IosThemeManage::GetCurTheme()
{
	bool bPandaHomeEnvironment = IsPandaHomeEnvironment();
	ThemeType eType = bPandaHomeEnvironment?ThemeType_PandaHome:ThemeType_SWBoard;
	wstring strThemeName = bPandaHomeEnvironment?GetPandaHomeCurThemeName():GetSWBoardCurThemeName();

	return FindTheme(eType, strThemeName);
}

bool IosThemeManage::Rename(ThemeData* pThemeData, wstring strNewName)
{
	RETURN_FALSE_IF(!_pFileStream);
	RETURN_FALSE_IF(!pThemeData);
	RETURN_FALSE_IF(strNewName.length() <= 0);

	ThemeData* pThemeDataTemp = FindTheme(pThemeData->_id);
	RETURN_FALSE_IF(!pThemeDataTemp);

	RETURN_TRUE_IF(pThemeDataTemp->_name == strNewName);

	wstring strThemePath = pThemeDataTemp->_strThemeFolderOnDev;
	wstring strNewThemePath = CFileOperation::GetFileDir(strThemePath) + strNewName;
	if (_pFileStream->Ios_Exists(strThemePath) &&
		!_pFileStream->Ios_Exists(strNewThemePath) &&
		_pFileStream->Ios_Rename(strThemePath, strNewThemePath))
	{
		 //预览图也需要重命名
		if (pThemeDataTemp->_themeType == ThemeType_PandaHome)
		{
			wstring strPreviewImageName = Ios_PandaHome_Theme_DownLoad;
			strPreviewImageName += L"/" + pThemeDataTemp->_name + L".jpg";
			wstring strNewPreviewImageName = Ios_PandaHome_Theme_DownLoad;
			strNewPreviewImageName += L"/" +strNewName + L".jpg";

			_pFileStream->Ios_Rename(strPreviewImageName, strNewPreviewImageName);
		}

		pThemeDataTemp->_strThemeFolderOnDev = strNewThemePath;
		pThemeDataTemp->_name = strNewName;
		pThemeData = pThemeDataTemp;

		return true;
	}

	return false;
}

bool IosThemeManage::DeleteTheme(ThemeData* pThemeData)
{
	ThemeData* pThemeDataTemp = FindTheme(pThemeData->_id);
	RETURN_TRUE_IF(!pThemeDataTemp);

	RETURN_FALSE_IF(!_pFileStream);

	bool bRet = true;

	if (_pFileStream->Ios_Exists(pThemeDataTemp->_strThemeFolderOnDev))
		bRet = FileHelper::Delete(_pDevice, pThemeDataTemp->_strThemeFolderOnDev);

	if (bRet)
	{
		if (pThemeData->_themeType == ThemeType_PandaHome)
		{
			//删除熊猫桌面主题对应的预览图
			wstring strPreviewImageName = Ios_PandaHome_Theme_DownLoad;
			strPreviewImageName += L"/" + pThemeDataTemp->_name;

			_pFileStream->Ios_DeleteFile(strPreviewImageName + L".png");
			_pFileStream->Ios_DeleteFile(strPreviewImageName + L"thumbnail.png");

			_pFileStream->Ios_DeleteFile(strPreviewImageName + L".jpg");
			_pFileStream->Ios_DeleteFile(strPreviewImageName + L"thumbnail.jpg");
		}	

		//从列表中移除
        RemoveTheme(pThemeData);
		pThemeData = pThemeDataTemp = NULL;
	}

	return bRet;
}

bool IosThemeManage::CheckIsThemePath(wstring strPath)
{
	strPath = CStrOperation::trimRightW(strPath, L"\\");

	RETURN_FALSE_IF(!CFileOperation::IsDirExist(strPath.c_str()));

	strPath += L"\\";
	if (CFileOperation::IsDirExist((wstring(strPath+L"Icons")).c_str()) ||
		CFileOperation::IsDirExist((wstring(strPath+L"Private")).c_str()) ||
		CFileOperation::IsDirExist((wstring(strPath+L"Bundles")).c_str()) ||
		CFileOperation::IsDirExist((wstring(strPath+L"UIImages")).c_str()) ||
		CFileOperation::IsFileExist((wstring(strPath+L"Dock.png")).c_str()) ||
		CFileOperation::IsFileExist((wstring(strPath+L"Wallpaper.png")).c_str()) ||
		CFileOperation::IsFileExist((wstring(strPath+L"Wallpaper.html")).c_str()))
		return true;

	return false;
}

ThemeData* IosThemeManage::AddLocalTheme(wstring strLocalFile, ProgressObserver* pObserver, wstring strPreviewFile)
{
	RETURN_NULL_IF(!_pFileStream);

	strLocalFile = CStrOperation::trimRightW(strLocalFile, L"\\");

	RETURN_NULL_IF(!CFileOperation::IsDirExist(strLocalFile.c_str()));

	bool bPandaHomeEnvirnment = IsPandaHomeEnvironment();
	wstring strThemePath = bPandaHomeEnvirnment?_strPandaHomeThemePath:_strWSBoardThemePath;

	ThemeData* pThemeData = new ThemeData();
	pThemeData->_id = CFileOperation::GetGuidW();
	pThemeData->_themeType = bPandaHomeEnvirnment?ThemeType_PandaHome:ThemeType_SWBoard;
	pThemeData->_strThemeFolderOnDev = strThemePath + L"/" + CFileOperation::GetFileName(strLocalFile);
	pThemeData->_strThemeFolderOnDev = IosPublicFunc::RenameDir(pThemeData->_strThemeFolderOnDev, _pFileStream);
	pThemeData->_name = CFileOperation::GetFileName(pThemeData->_strThemeFolderOnDev);
	pThemeData->_strPreviewPathOnPC = _strPreviewImageFolder + L"\\" + pThemeData->_id + L"Preview.png";

	bool bRet = false;

	wstring strUploadTempOnPc = _pDevice->GetThemePath()->GetUploadPath()
        + CFileOperation::GetGuidW();
	CFileOperation::DeleteDirectory(strUploadTempOnPc);
	RETURN_NULL_IF(!::CreateDirectory(strUploadTempOnPc.c_str(), NULL));

	if (CFileOperation::CopyDir(strLocalFile, strUploadTempOnPc))
	{
		wstring strUploadFolderOld = strUploadTempOnPc + L"\\" + CFileOperation::GetFileName(strLocalFile);
		wstring strUploadFolderNew = strUploadTempOnPc + L"\\" + pThemeData->_name;
		if (::MoveFile(strUploadFolderOld.c_str(), strUploadFolderNew.c_str()) &&
			FileHelper::Upload(_pDevice, strUploadFolderNew, strThemePath, pObserver, false))
		{
			if (bPandaHomeEnvirnment)
			{
				//设置权限	
				DirectoryOperationCommand cmd(_pDevice);
				cmd.ChangeMode(pThemeData->_strThemeFolderOnDev, "755", L"501");
			}

			if (CFileOperation::IsFileExist(strPreviewFile.c_str()))
				::MoveFile(strPreviewFile.c_str(), pThemeData->_strPreviewPathOnPC.c_str());
			else
			{
				LoadPreviewPhoto(pThemeData);
			}			

			//上传预览图
			wstring strPreviewImageOnDev = pThemeData->_strThemeFolderOnDev + L"/" + Ios_Preview_Image_Name + L"." + CFileOperation::GetFileExt(pThemeData->_strPreviewPathOnPC);
			if (bPandaHomeEnvirnment)
			{
				strPreviewImageOnDev = Ios_PandaHome_Theme_DownLoad;
				strPreviewImageOnDev += L"/" + pThemeData->_name + L".png";
			}

			if (!_pFileStream->Ios_Exists(strPreviewImageOnDev))
			{
				_pFileStream->Ios_CopyToPhone(pThemeData->_strPreviewPathOnPC, strPreviewImageOnDev);	
				DirectoryOperationCommand cmd(_pDevice);
				cmd.ChangeMode(strPreviewImageOnDev);
			}

			bRet = true;
		}
	}

	if (bRet)
		AddTheme(pThemeData->_id, pThemeData);
	else
		SAFE_DELETE(pThemeData);

	CFileOperation::DeleteDirectory(strUploadTempOnPc);

	return pThemeData;
}

wstring IosThemeManage::BackupTheme(ThemeData* pThemeData, wstring SavePath)
{
	RETURN_VALUE_IF(!_pFileStream, L"");
	RETURN_VALUE_IF(!pThemeData, L"");
	RETURN_VALUE_IF(SavePath.length() <= 0, L"");

	ThemeData* pThemeDataTemp = FindTheme(pThemeData->_id);
	RETURN_VALUE_IF(!pThemeDataTemp, L"");

	wstring strLocalPath = CStrOperation::trimRightW(SavePath, L"\\") + L"\\" + pThemeData->_name;
	strLocalPath = CFileOperation::RenameDir(strLocalPath);
	//RETURN_VALUE_IF(!::CreateDirectory(strLocalPath.c_str(), NULL), L"");
	CFileOperation::MakeLocalDir(strLocalPath);

	// 需要下载的节点
	vector<NodeData*> nodes;
	DirData root(_pDevice);
	DirData* pDir = dynamic_cast<DirData*>(FileHelper::GetRealNode(&root, pThemeData->_strThemeFolderOnDev));
	RETURN_VALUE_IF(!pDir, L"");
	for(size_t i = 0; i < pDir->GetChildrens().size(); i++)
	{
		nodes.push_back(pDir->GetChildrens().at(i));
	}
	// 下载到此目录	
	bool ret = (Transfer_Success == FileHelper::Download(&nodes, strLocalPath, NULL));

	if (pThemeDataTemp->_themeType == ThemeType_PandaHome)
	{
		//熊猫桌面先不要gif预览图
		wstring strPreviewImageOnDev = Ios_PandaHome_Theme_DownLoad;
		strPreviewImageOnDev += L"/" + pThemeDataTemp->_name + L".png";

		if (!_pFileStream->Ios_Exists(strPreviewImageOnDev))
		{
			//熊猫桌面自己下载的预览图是jpg结尾的
			strPreviewImageOnDev = CStrOperation::trimRightW(strPreviewImageOnDev, L"png") + L"jpg";
			if (!_pFileStream->Ios_Exists(strPreviewImageOnDev))
				strPreviewImageOnDev = L"";
		}

		if (strPreviewImageOnDev.length() > 0)
		{
			wstring strPreviewImageLocal = strLocalPath + L"\\" + Ios_Preview_Image_Name + L".png";
			_pFileStream->Ios_DownFromPhone(strPreviewImageOnDev, strPreviewImageLocal);
		}
	}

	nodes.clear();

	RETURN_VALUE_IF(!ret, L"");

	return strLocalPath;
}

bool IosThemeManage::SetCurTheme(ThemeData* pThemeData)
{
	RETURN_FALSE_IF(!_pFileStream);

	ThemeData* pThemeDataTemp = FindTheme(pThemeData->_id);
	RETURN_FALSE_IF(!pThemeDataTemp);

	if(_nProductVer < 200)//2.0之前版本SummberBoard
	{
		return SetSummerBoardTheme(pThemeDataTemp);
	}

	CreateMomeCommand cmd(_pDevice);
	if(cmd.CheckPHInstalled()==0)
	{
		if(cmd.SetPCTheme(pThemeDataTemp->_strThemeFolderOnDev)==0)
			return true;
	}


	bool bRet = false;
	if (IsPandaHomeEnvironment())//(pThemeDataTemp->_themeType == ThemeType_PandaHome)
		bRet = SetPandaHomeTheme(pThemeDataTemp->_name);
	else
	{
		if (SetWinterBoardTheme(pThemeDataTemp->_name))
		{
			bRet = true;
			ClearWeatherInfoSettingFile();
		}
	}

	RETURN_FALSE_IF(!bRet);

	//根据iPhone当前语言修改好主题图标 
	RenameIconNameByDevLang(pThemeDataTemp->_name);

	//注销手机
	LogOffCommand loCmd(_pDevice);
	loCmd.Execute();

	return true;
}


bool IosThemeManage::SetSummerBoardTheme(ThemeData* pThemeDataTemp)
{
	IosPlistHelper xmlHelper(XML_PI, XML_DOCTYPE, eXmlType_UTF8, XML_ELEMENT(L"plist", L"", L"version", L"1.0"));

	XML_NODE_INDEX index;
	xmlHelper.AddElement(index, XML_ELEMENT(L"dict"));

	index.AddChild(0);
	xmlHelper.AddElement(index, XML_ELEMENT(L"key", L"Themes"));
	xmlHelper.AddElement(index, XML_ELEMENT(L"string",pThemeDataTemp->_strThemeFolderOnDev));

	wstring sbfoldertmp=_pDevice->GetThemePath()->GetThemeTempPath() + L"SummerBoard" + CFileOperation::GetGuidW() + L".plist";
	xmlHelper.WriteToFile(sbfoldertmp);

	if(_pFileStream->Ios_CopyToPhone(sbfoldertmp,Ios_SummerBoard_Setting_File)==Transfer_Success)
	{
		DirectoryOperationCommand(_pDevice).ChangeMode(Ios_SummerBoard_Setting_File);
		LogOffCommand loCmd(_pDevice);
		loCmd.Execute();
		return true;
	}
	return false;
}

bool IosThemeManage::RevocationTheme()
{
	RETURN_FALSE_IF(_nProductVer < 200);

	bool bRet = false;
	if (IsPandaHomeEnvironment())
		bRet = RevocationPandaHomeTheme();
	else
		bRet = RevocationWinterBoardTheme();

	RETURN_FALSE_IF(!bRet);

	//注销手机
	LogOffCommand loCmd(_pDevice);
	loCmd.Execute();

	return true;
}

void IosThemeManage::LoadPandaHomeThemes()
{
	CreateMomeCommand cmd(_pDevice);
	map<wstring, wstring> themes;
	if(cmd.CheckPHInstalled()==0 && cmd.GetThemeDataPath(themes)==0)
	{
		_strPandaHomeThemePath = themes.begin()->first;
		_curThemePlistFilePath = themes.begin()->second;
	}

	if (!IsPandaHomeEnvironment())
		return;

	DirData resultDir(_pDevice);
	resultDir._name = _strPandaHomeThemePath;

	_pFileStream->Ios_GetFileInfos(&resultDir, eFileGetType_Files);

	deque<NodeData*> childs = resultDir.GetChildrens();
	for ( size_t i=0; i < childs.size(); ++i )
	{
		NodeData* pFile = childs[i];

		if (pFile->_type != NodeData::DIR)
			continue;

		wstring name = pFile->_name;
		if (!IsInPandaHomeFilter(name))
		{
			ThemeData* pThemeData = new ThemeData();
			pThemeData->_id = CFileOperation::GetGuidW();
			pThemeData->_name = name;
			pThemeData->_strThemeFolderOnDev = _strPandaHomeThemePath + L"/" + name;
			pThemeData->_strPreviewPathOnPC = _strPreviewImageFolder + L"\\" + pThemeData->_id + L"Preview.png";
			pThemeData->_themeType = ThemeType_PandaHome;

			AddTheme(pThemeData->_id, pThemeData);
		}
	}
}

void IosThemeManage::LoadWinterBoardThemes()
{
	vector<wstring> vtThemeFolderName;

	DirData resultDir(_pDevice);
	resultDir._name = _strWSBoardThemePath;

	_pFileStream->Ios_GetFileInfos(&resultDir, eFileGetType_Files);

	deque<NodeData*> childs = resultDir.GetChildrens();
	for ( size_t i=0; i < childs.size(); ++i )
	{
		NodeData* pFile = childs[i];

		if (pFile->_type != NodeData::DIR)
			continue;

		wstring name = pFile->_name;

		if (!CStrOperation::endsWithW(CStrOperation::toLowerW(name), L".theme"))
		{
			vtThemeFolderName.push_back(name);			
		}
		else if (_pFileStream->Ios_Exists(_strWSBoardThemePath + L"/" + name + L"/Wallpaper.png"))
		{
			wstring newThemeFolderName = CStrOperation::trimRightW(CStrOperation::toLowerW(name), L".theme");
			if (newThemeFolderName.length() <= 0)
				newThemeFolderName = L"theme";

			newThemeFolderName = IosPublicFunc::RenameDir(newThemeFolderName, _pFileStream);
			_pFileStream->Ios_Rename(_strWSBoardThemePath + L"/" + name, _strWSBoardThemePath + L"/" + newThemeFolderName);

			vtThemeFolderName.push_back(newThemeFolderName);			
		}
	}

	vector<wstring>::iterator it;
	for (it=vtThemeFolderName.begin(); it!=vtThemeFolderName.end(); it++)
	{
		wstring strTemp = *it;
		if (strTemp.length() > 0)
		{
			ThemeData* pThemeData = new ThemeData();
			pThemeData->_id = CFileOperation::GetGuidW();
			pThemeData->_name = strTemp;
			pThemeData->_strThemeFolderOnDev = _strWSBoardThemePath + L"/" + strTemp;
			pThemeData->_strPreviewPathOnPC = _strPreviewImageFolder + L"\\" + pThemeData->_id + L"Preview.png";
			pThemeData->_themeType = ThemeType_SWBoard;

			AddTheme(pThemeData->_id, pThemeData);
		}		
	}
	vtThemeFolderName.clear();
}

bool IosThemeManage::IsPandaHomeEnvironment()
{
	RETURN_FALSE_IF(!_pFileStream);

	bool bPandaHomeEnvironment = (_nProductVer >= 300 && _pFileStream->Ios_Exists(Ios_PandaHome_Dylib));
	if (!bPandaHomeEnvironment)
	{
		//程序被删除后.会遗留主题目录.因此看看是否安装了winterboard,如果没有,则设置成熊猫桌面环境
		if (!_pFileStream->Ios_Exists(Ios_WinterBoard_App_Folder) && _pFileStream->Ios_Exists(L"/private/var/mobile/Media/PandaHome"))
			bPandaHomeEnvironment = true;
	}

	return bPandaHomeEnvironment;
}

void IosThemeManage::InitThemeFilter()
{
	_vtPandaHomeFilter.clear();
	_vtPandaHomeFilter.push_back(L"No Docked Background");
	_vtPandaHomeFilter.push_back(L"No Docked Labels");
	_vtPandaHomeFilter.push_back(L"No Undocked Labels");
	_vtPandaHomeFilter.push_back(L"No Icon Shadow");

	_vtWinterBoardFilter.clear();
	_vtWinterBoardFilter.push_back(L"White Icon Labels");
	_vtWinterBoardFilter.push_back(L"User Lock Background");
	_vtWinterBoardFilter.push_back(L"No Undocked Icon Labels");
	_vtWinterBoardFilter.push_back(L"No Docked Icon Labels");
	_vtWinterBoardFilter.push_back(L"Dim Wallpaper");
	_vtWinterBoardFilter.push_back(L"Dim Icons");
	_vtWinterBoardFilter.push_back(L"Black Navigation Bars");
	_vtWinterBoardFilter.push_back(L"Saurik");
	_vtWinterBoardFilter.push_back(L"Server Lock Example");
	_vtWinterBoardFilter.push_back(L"Solid Status Bar");
	_vtWinterBoardFilter.push_back(L"Transparent Dock");
	_vtWinterBoardFilter.push_back(L"User Wallpaper");	
}

void IosThemeManage::InitApplicationNameMap()
{
	wstring strValueEn[eAppIconType_YouTube+1] = {
		L"App Store",
		L"Maps",
		L"Contacts",
		L"Messages",
		L"iTunes",
		L"Settings",
		L"Calculator",
		L"Radio",
		L"Calendar",
		L"iPod",
		L"Videos",
		L"Mail",
		L"Notes",
		L"Phone",
		L"Safari",
		L"Camera",
		L"Photos",
		L"Clock",
		L"Keyboard",
		L"Stocks",
		L"Converter",
		L"Weather",
		L"Business",
		L"Translator",
		L"YouTube",
	};

	wstring strValueTW[eAppIconType_YouTube+1] = {
		L"App Store",
		L"地圖",
		L"聯絡資訊",
		L"訊息",
		L"iTunes",
		L"設定",
		L"計算機",
		L"無線電",
		L"行事曆",
		L"iPod",
		L"視訊",
		L"Mail",
		L"備忘錄",
		L"電話",
		L"Safari",
		L"相機",
		L"照片",
		L"時間",
		L"鍵盤",
		L"股市",
		L"換算器",
		L"天氣",
		L"商業",
		L"翻譯",
		L"YouTube",
	};

	wstring strValueCH[eAppIconType_YouTube+1] = {
		L"App Store",
		L"地图",
		L"通讯录",
		L"短信",
		L"iTunes",
		L"设置",
		L"计算器",
		L"无线电",
		L"日历",
		L"iPod",
		L"视频",
		L"Mail",
		L"备忘录",
		L"电话",
		L"Safari",
		L"相机",
		L"照片",
		L"时钟",
		L"键盘",
		L"股市",
		L"转换器",
		L"天气",
		L"商业",
		L"翻译",
		L"YouTube",
	};

	for (int eType=eAppIconType_AppStore; eType<=eAppIconType_YouTube; eType++)
	{
		APP_ICON_NAME* pIconName = new APP_ICON_NAME(strValueEn[eType], strValueCH[eType], strValueTW[eType]);
		_mapAppIconName.insert(pair<eAppIconType, APP_ICON_NAME*>((eAppIconType)eType, pIconName));
	}
}

void IosThemeManage::FreeApplicationNameMap()
{
	map<eAppIconType, APP_ICON_NAME*>::iterator it;
	for (it=_mapAppIconName.begin(); it!=_mapAppIconName.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_mapAppIconName.clear();
}

void IosThemeManage::InitNormalInfo()
{
	if (_pDevice && _pDevice->GetDevInfo())
	{
		if (_pDevice->GetDevInfo()->_connectType == USB)
			_pFileStream = new AFCFileSysemBase(_pDevice->GetDevInfo());
		else
			_pFileStream = new IosWiFiFileSystem(_pDevice->GetDevInfo()->_ios_wifi_handle);

		_nProductVer = IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion);

		if (_pDevice->GetDevInfo()->_deviceProductType.length() > 0)
		{
			if (_pDevice->GetDevInfo()->_deviceProductType.find(L"iPad") >= 0)
			{
				_eDevicePsys = eIosDevSys_Ipad;
			}
			else if (_pDevice->GetDevInfo()->_deviceProductType.compare(L"iPhone3,1") == 0 ||
				_pDevice->GetDevInfo()->_deviceProductType.compare(L"iPod4,1") == 0)
			{
				_eDevicePsys = eIosDevSys_Four;
			}
			else
				_eDevicePsys = eIosDevSys_Three;
		}

		_strWSBoardThemePath = IosPublicFunc::CheckPathByVersion(Ios_WSBoard_Theme_Path, _nProductVer, _pDevice->GetDevInfo()->_ios_bJailBreaked);
		_strPandaHomeThemePath = IosPublicFunc::CheckPathByVersion(Ios_PandaHome_Theme_Path, _nProductVer, _pDevice->GetDevInfo()->_ios_bJailBreaked);
	}	
}

bool IosThemeManage::IsInPandaHomeFilter(wstring strName)
{
	vector<wstring>::iterator it;
	for (it=_vtPandaHomeFilter.begin(); it!=_vtPandaHomeFilter.end(); it++)
	{
		if (strName.compare(*it) == 0)
			return true;
	}
	return false;
}

bool IosThemeManage::IsInWinterBoardFilter(wstring strName)
{
	vector<wstring>::iterator it;
	for (it=_vtWinterBoardFilter.begin(); it!=_vtWinterBoardFilter.end(); it++)
	{
		if (strName.compare(*it) == 0)
			return true;
	}
	return false;
}

bool IosThemeManage::GetPreviewDirect(ThemeData* pThemeData)
{
	RETURN_FALSE_IF(!pThemeData);
	RETURN_FALSE_IF(!_pFileStream);

	wstring strPreviewTemp = pThemeData->_strPreviewPathOnPC;
	wstring strPreviewImageOnDev = L"";
	bool bExistPreview = false;

	if (pThemeData->_themeType == ThemeType_PandaHome)
	{
		//熊猫桌面先不要gif预览图
		strPreviewImageOnDev = Ios_PandaHome_Theme_DownLoad;
		strPreviewImageOnDev += L"/" + pThemeData->_name + L".png";

		if (_pFileStream->Ios_Exists(strPreviewImageOnDev))
			bExistPreview = true;
		else
		{
			//熊猫桌面自己下载的预览图是jpg结尾的
			strPreviewImageOnDev = CStrOperation::trimRightW(strPreviewImageOnDev, L"png") + L"jpg";
			if (_pFileStream->Ios_Exists(strPreviewImageOnDev))
			{
				bExistPreview = true;
				strPreviewTemp = CStrOperation::trimRightW(strPreviewTemp, L"png") + L"jpg";
			}
		}
	}
	else
	{
		strPreviewImageOnDev = pThemeData->_strThemeFolderOnDev + L"/" + Ios_Preview_Image_Name + L".png";

		if (_pFileStream->Ios_Exists(strPreviewImageOnDev))
			bExistPreview = true;
		else
		{
			//熊猫桌面自己下载的预览图是jpg结尾的
			strPreviewImageOnDev = CStrOperation::trimRightW(strPreviewImageOnDev, L"png") + L"gif";
			if (_pFileStream->Ios_Exists(strPreviewImageOnDev))
			{
				bExistPreview = true;
				strPreviewTemp = CStrOperation::trimRightW(strPreviewTemp, L"png") + L"gif";
			}
		}
	}

	if (bExistPreview && _pFileStream->Ios_DownFromPhone(strPreviewImageOnDev, strPreviewTemp) == Transfer_Success)
	{
		pThemeData->_strPreviewPathOnPC = strPreviewTemp;
		return true;
	}

	return false;
}

bool IosThemeManage::GetPreviewFromSource(ThemeData* pThemeData)
{
	RETURN_FALSE_IF(!pThemeData);
	RETURN_FALSE_IF(!_pFileStream);

	bool bRet = false;	

	wstring strPreviewSourceFolder = _strPreviewImageFolder + L"\\" + pThemeData->_id;
	CFileOperation::DeleteDirectory(strPreviewSourceFolder);
	::CreateDirectory(strPreviewSourceFolder.c_str(), NULL);

	if (DownloadPreviewSource(strPreviewSourceFolder, pThemeData) &&
		CreatePreviewFromSource(strPreviewSourceFolder, pThemeData))
	{
		bRet = true;
	}

	CFileOperation::DeleteDirectory(strPreviewSourceFolder);

	return bRet;
}

bool IosThemeManage::DownloadPreviewSource(wstring strPreviewSourceFolder, ThemeData* pThemeData)
{	
	RETURN_FALSE_IF(!pThemeData);
	RETURN_FALSE_IF(!_pFileStream);
	RETURN_FALSE_IF(strPreviewSourceFolder.length() <= 0);

	wstring strIconPathOnPC = strPreviewSourceFolder + L"\\Icons";
	wstring strIconPathOnDev = pThemeData->_strThemeFolderOnDev + L"/Icons";
	::CreateDirectory(strIconPathOnPC.c_str(), NULL);

	// 检查是否存在Icon库,存在则下载预览需要的几个图标,减少时间
	if (CStrOperation::toLowerW(pThemeData->_name).compare(L"default")!=0 && _pFileStream->Ios_Exists(strIconPathOnDev))
	{
		map<eAppIconType, Image*>::iterator it;
		for (it=_mapDefaultIcon.begin(); it!=_mapDefaultIcon.end(); it++)
		{
			APP_ICON_NAME* pTemp = _mapAppIconName[it->first];
			wstring strIconNameOnPC = strIconPathOnPC + L"\\" + pTemp->_strEnglist + L".png";
			wstring strIconNameOnDev = strIconPathOnDev + L"/" + pTemp->_strEnglist + L".png";
			if (_pFileStream->Ios_DownFromPhone(strIconNameOnDev, strIconNameOnPC) != Transfer_Success)
			{
				//下载英文icon失败,尝试下载中文对应的icon
				strIconNameOnDev = strIconPathOnDev + L"/" + pTemp->_strChinese + L".png";
				if (_pFileStream->Ios_DownFromPhone(strIconNameOnDev, strIconNameOnPC) != Transfer_Success)
				{
					//中文下载失败,尝试下载繁体
					strIconNameOnDev = strIconPathOnDev + L"/" + pTemp->_strTaiwan + L".png";
					_pFileStream->Ios_DownFromPhone(strIconNameOnDev, strIconNameOnPC);
				}
			}
		}
	}

	wstring strWallpaperOnPC = strPreviewSourceFolder + L"\\Wallpaper.png";
	wstring strWallpaperOnDev = pThemeData->_strThemeFolderOnDev + L"/Wallpaper.png";
	if (_pFileStream->Ios_Exists(strWallpaperOnDev))
		_pFileStream->Ios_DownFromPhone(strWallpaperOnDev, strWallpaperOnPC);

	wstring strDockOnPC = strPreviewSourceFolder + L"\\Dock.png";
	wstring strDockOnDev = pThemeData->_strThemeFolderOnDev + L"/Dock.png";
	if (_pFileStream->Ios_Exists(strDockOnDev))
		_pFileStream->Ios_DownFromPhone(strDockOnDev, strDockOnPC);

	return true;
}

bool IosThemeManage::IsPixelFormatIndexed(PixelFormat pImagePixelFormat)
{
	PixelFormat indexedPixelFormats[6] = {
		PixelFormatUndefined, PixelFormatDontCare, PixelFormat16bppARGB1555, PixelFormat1bppIndexed, PixelFormat4bppIndexed, PixelFormat8bppIndexed
	};

	for (int i=0; i<6; i++)
	{
		if (pImagePixelFormat == indexedPixelFormats[i])
			return true;
	}

	return false;
}

bool IosThemeManage::CreatePreviewFromSource(wstring strPreviewSourceFolder, ThemeData* pThemeData)
{
	RETURN_FALSE_IF(!pThemeData);
	RETURN_FALSE_IF(strPreviewSourceFolder.length() <= 0);

	wstring strIconPathOnPC = strPreviewSourceFolder + L"\\Icons";
	wstring strWallpaperOnPC = strPreviewSourceFolder + L"\\Wallpaper.png";
	wstring strDockOnPC = strPreviewSourceFolder + L"\\Dock.png";

	Image* pPreviewImage = NULL;

	//存在wallpaper.png,则绘制wallpaper.png
	if (CFileOperation::IsFileExist(strWallpaperOnPC.c_str()))
	{
		pPreviewImage = Gdiplus::Image::FromFile(strWallpaperOnPC.c_str());
		if (pPreviewImage && IsPixelFormatIndexed(pPreviewImage->GetPixelFormat()))
		{
			//如果原图片是索引像素格式之类的，则需要转换
			Bitmap* pBmp = new Bitmap(pPreviewImage->GetWidth(), pPreviewImage->GetHeight(), PixelFormat32bppARGB);

			Graphics g(pBmp);
			g.SetCompositingQuality(CompositingQualityHighQuality);
			g.SetInterpolationMode(InterpolationModeHighQuality);
			g.SetSmoothingMode(SmoothingModeHighQuality);
			g.DrawImage(pPreviewImage, 0, 0);
			DUMP_PNG(pBmp, strWallpaperOnPC.c_str());

			SAFE_DELETE(pPreviewImage);
			pPreviewImage = Gdiplus::Image::FromFile(strWallpaperOnPC.c_str());

			SAFE_DELETE(pBmp);
		}
	}
	else
	{
		pPreviewImage = new Bitmap(320, 480);
		Graphics g(pPreviewImage);
		g.Clear(Color::Black);
	}

	RETURN_FALSE_IF(!pPreviewImage);

	//如果存在dock.png,则把dock.png绘制在wallpaper.png上
	if (CFileOperation::IsFileExist(strDockOnPC.c_str()))
	{
		Image* pDockImage = Gdiplus::Image::FromFile(strDockOnPC.c_str());

		Graphics g(pPreviewImage);
		g.DrawImage(pDockImage, Rect(0, 392, pPreviewImage->GetWidth(), 88), 
			0, 0, pDockImage->GetWidth(), pDockImage->GetHeight(), UnitPixel);
		SAFE_DELETE(pDockImage);
	}

	map<eAppIconType, Image*> mapDefaultIconTemp = _mapDefaultIcon;

	//读取主题包下面的icons替换默认图标(如果存在)
	if (CFileOperation::IsDirExist(strIconPathOnPC.c_str()))
	{
		map<eAppIconType, Image*>::iterator it;
		for (it=mapDefaultIconTemp.begin(); it!=mapDefaultIconTemp.end(); it++)
		{
			APP_ICON_NAME* pTemp = _mapAppIconName[it->first];
			wstring strIconNameOnPC = strIconPathOnPC + L"\\" + pTemp->_strEnglist + L".png";
			if (CFileOperation::IsFileExist(strIconNameOnPC.c_str()))
			{
				it->second = Gdiplus::Image::FromFile(strIconNameOnPC.c_str());
			}
		}
	}	

	Graphics g(pPreviewImage);
	int nIndex = 0;
	int nRow = 0;
	int nXAxis = 0;
	int nYAxis = 0;
	map<eAppIconType, Image*>::iterator it;
	for (it=mapDefaultIconTemp.begin(); it!=mapDefaultIconTemp.end(); it++)
	{
		nXAxis = nIndex % 4 * 75 + 19;

		if (nIndex != 0 && nIndex % 4 == 0)
		{
			nRow += 1;
		}
		if (nRow == 0)//第一行
		{
			nYAxis = 22;
		}
		else if (nRow == 1)
		{
			nYAxis = 127;
		}
		else if (nRow == 2)
		{
			nYAxis = 224;
		}
		else
		{
			nYAxis = 401;
		}

		Image* pIconImage = it->second;
		if (pIconImage)
			g.DrawImage(pIconImage, Rect(nXAxis, nYAxis, pIconImage->GetWidth(), pIconImage->GetHeight()), 
			0, 0, pIconImage->GetWidth(), pIconImage->GetHeight(), UnitPixel);

		if (pIconImage != _mapDefaultIcon[it->first])
			SAFE_DELETE(it->second);

		nIndex++;
	}

	mapDefaultIconTemp.clear();

	DUMP_PNG(pPreviewImage, pThemeData->_strPreviewPathOnPC.c_str());

	SAFE_DELETE(pPreviewImage);

	return true;
}

void IosThemeManage::AddDefaultIcon(eAppIconType eType, Image* pIconImage)
{
	RETURN_IF(!pIconImage);

	map<eAppIconType, Image*>::iterator find = _mapDefaultIcon.find(eType);
	if (find == _mapDefaultIcon.end())
		_mapDefaultIcon.insert(pair<eAppIconType, Image*>(eType, pIconImage->Clone()));
	else
	{
		SAFE_DELETE(find->second);
		find->second = pIconImage->Clone();
	}
}

wstring IosThemeManage::GetSWBoardCurThemeName()
{
	if (!_pDevice || !_pFileStream)
		return L"";

	wstring strThemeName = L"";
	wstring strThemePlist = _pDevice->GetThemePath()->GetThemeTempPath() + L"CurThemeInfo" + CFileOperation::GetGuidW() + L".plist";

    CFileOperation::DeleteFile(strThemePlist);
	if (_nProductVer >= 200) //winterboard
	{
		if (_pFileStream->Ios_Exists(Ios_WinterBoard_Setting_File) &&
			_pFileStream->Ios_DownFromPhone(Ios_WinterBoard_Setting_File, strThemePlist) == Transfer_Success &&
			IosPlistConverter::GetXmlFromPlist(strThemePlist, strThemePlist))
		{
			IosPlistHelper plistHelper(strThemePlist, eXmlType_UTF8);
			XML_NODE_INDEX index;
			index.AddChild(0);
			vector<rapidxml::xml_node<char>*> vtValues;
			IosPlistHelper::GetNodesFromNode(plistHelper.SearchKeyNode(index, "Themes", "array"), vtValues);
			for (vector<rapidxml::xml_node<char>*>::iterator it=vtValues.begin(); it!=vtValues.end(); it++)
			{
				rapidxml::xml_node<char>* pActiveNode = IosPlistHelper::SearchKeyNodeFromXmlNode(*it, "Active");
				if (pActiveNode && strcmp(pActiveNode->name(), "true") == 0)
				{
					strThemeName = IosPlistHelper::SearchKeyFromXmlNode(*it, "Name");
					break;
				}
			}
		}
	}

    CFileOperation::DeleteFile(strThemePlist);

	return strThemeName;
}

wstring IosThemeManage::GetPandaHomeCurThemeName()
{
	if (!_pDevice || !_pFileStream)
		return L"";

	wstring strThemeName = L"";
	wstring strThemePlist = _pDevice->GetThemePath()->GetThemeTempPath() + L"CurThemeInfo" + CFileOperation::GetGuidW() + L".plist";
    CFileOperation::DeleteFile(strThemePlist);
	if (_pFileStream->Ios_Exists(_curThemePlistFilePath ) &&
		_pFileStream->Ios_DownFromPhone(_curThemePlistFilePath, strThemePlist) == Transfer_Success &&
		IosPlistConverter::GetXmlFromPlist(strThemePlist, strThemePlist))
	{
		IosPlistHelper plistHelper(strThemePlist, eXmlType_UTF8);
		vector<wstring> vtValues;
		plistHelper.GetValuesFromNode(plistHelper.FindFirstArrayNode(), vtValues);
		if (vtValues.size() > 0)
			strThemeName = vtValues.at(0);
	}
    CFileOperation::DeleteFile(strThemePlist);

	return strThemeName;
}

bool IosThemeManage::SetWinterBoardTheme(wstring strThemeName)
{
	RETURN_FALSE_IF(!_pFileStream);

	wstring strWinterBoardSettingFileTemp = _pDevice->GetThemePath()->GetThemeTempPath() + L"WinterBoard" + CFileOperation::GetGuidW() + L".plist";
    CFileOperation::DeleteFile(strWinterBoardSettingFileTemp);

	wstring strValues[6] = {
		L"Black Navigation Bars",
		L"Saurik",
		//L"Server Lock Example",
		L"Solid Status Bar",
		L"Transparent Dock",
		L"Transparent Icon Labels",
		L"User Wallpaper",
	};

	if (_pFileStream->Ios_Exists(Ios_WinterBoard_Setting_File))
	{
		_pFileStream->Ios_DownFromPhone(Ios_WinterBoard_Setting_File, strWinterBoardSettingFileTemp);
		IosPlistConverter::GetXmlFromPlist(strWinterBoardSettingFileTemp, strWinterBoardSettingFileTemp);
	}
	else
		CreateWinterBoardSettingFile(strWinterBoardSettingFileTemp, strValues, 6);

	RETURN_FALSE_IF(!CFileOperation::IsFileExist(strWinterBoardSettingFileTemp.c_str()));

	UpdateWinterBoardSettingFile(strThemeName, strWinterBoardSettingFileTemp, strValues, 6);

	bool bRet = false;
	if (_pFileStream->Ios_CopyToPhone(strWinterBoardSettingFileTemp, Ios_WinterBoard_Setting_File) == Transfer_Success)
	{
		DirectoryOperationCommand(_pDevice).ChangeMode(Ios_WinterBoard_Setting_File);

		bRet = true;
	}

    CFileOperation::DeleteFile(strWinterBoardSettingFileTemp);

	return bRet;
}

void IosThemeManage::UpdateWinterBoardSettingFile(wstring strThemeName, wstring strFileName, wstring strValues[], int nLen)
{
	wstring strValuesTemp = L"";
	for (int i=0; i<nLen; i++)
	{
		strValuesTemp += strValues[i] + L"`";
	}
	wstring strThemeNameCompare = CStrOperation::ReplaceW(strThemeName, L".theme", L"");
	bool bExist = false;

	IosPlistHelper plistHelper(strFileName, eXmlType_UTF8);
	XML_NODE_INDEX index;
	index.AddChild(0);
	vector<rapidxml::xml_node<char>*> vtValues;
	IosPlistHelper::GetNodesFromNode(plistHelper.SearchKeyNode(index, "Themes", "array"), vtValues);
	for (vector<rapidxml::xml_node<char>*>::iterator it=vtValues.begin(); it!=vtValues.end(); it++)
	{
		rapidxml::xml_node<char>* pActiveNode = IosPlistHelper::SearchKeyNodeFromXmlNode(*it, "Active");
		if (pActiveNode)
		{
			wstring strThemeNameTemp = CStrOperation::ReplaceW(IosPlistHelper::SearchKeyFromXmlNode(*it, "Name"), L".theme", L"");
			if(strThemeNameTemp == strThemeNameCompare)
			{
				pActiveNode->name("true");
				bExist = true;
			}
			else if (strValuesTemp.find(strThemeNameTemp+L"`") >= 0)
			{
				continue;
			}
			else
			{
				pActiveNode->name("false");
			}
		}
	}

	if (!bExist)
	{
		XML_NODE_INDEX index;
		index.AddChild(0);
		index.AddChild(1);
		plistHelper.AddElement(index, XML_ELEMENT(L"dict"));
		index.AddChild(0);
		index.SetChildIndex(2, vtValues.size());
		plistHelper.AddElement(index, XML_ELEMENT(L"key", L"Active"));
		plistHelper.AddElement(index, XML_ELEMENT(L"true"));
		plistHelper.AddElement(index, XML_ELEMENT(L"key", L"Name"));
		plistHelper.AddElement(index, XML_ELEMENT(L"string", strThemeName));
	}

	plistHelper.WriteToFile(strFileName);
}

void IosThemeManage::CreateWinterBoardSettingFile(wstring strFileName, wstring strValues[], int nLen)
{
	IosPlistHelper xmlHelper(XML_PI, XML_DOCTYPE, eXmlType_UTF8, XML_ELEMENT(L"plist", L"", L"version", L"1.0"));

	XML_NODE_INDEX index;
	xmlHelper.AddElement(index, XML_ELEMENT(L"dict"));

	index.AddChild(0);
	xmlHelper.AddElement(index, XML_ELEMENT(L"key", L"Themes"));
	xmlHelper.AddElement(index, XML_ELEMENT(L"array"));

	index.AddChild(1);
	for (int i=0; i<nLen; i++)
	{
		xmlHelper.AddElement(index, XML_ELEMENT(L"dict"));
	}			

	index.AddChild(0);
	for (int j=0; j<nLen; j++)
	{
		index.SetChildIndex(2, j);
		xmlHelper.AddElement(index, XML_ELEMENT(L"key", L"Active"));
		xmlHelper.AddElement(index, XML_ELEMENT(L"false"));
		xmlHelper.AddElement(index, XML_ELEMENT(L"key", L"Name"));
		xmlHelper.AddElement(index, XML_ELEMENT(L"string", strValues[j]));
	}			

	xmlHelper.WriteToFile(strFileName);
}

bool IosThemeManage::SetPandaHomeTheme(wstring strThemeName)
{
	RETURN_FALSE_IF(!_pFileStream);

	wstring strWinterBoardSettingFileTemp = _pDevice->GetThemePath()->GetThemeTempPath() + L"PandaHome" + CFileOperation::GetGuidW() + L".plist";
    CFileOperation::DeleteFile(strWinterBoardSettingFileTemp);

	CreatePandaHomeSettingFile(strWinterBoardSettingFileTemp, strThemeName);

	bool bRet = false;
	if (_pFileStream->Ios_CopyToPhone(strWinterBoardSettingFileTemp, Ios_PandaHome_Setting_File) == Transfer_Success)
	{
		/*SetOwnCommand pSetOwnCmd(_pDevice);
		pSetOwnCmd.SetParams(Ios_WinterBoard_Setting_File, "501");
		pSetOwnCmd.Execute();*/
		DirectoryOperationCommand(_pDevice).ChangeMode(Ios_WinterBoard_Setting_File);
		bRet = true;
	}

	//如果有winterboard存在,取消它设置的主题,以免冲突
	if (bRet && _pFileStream->Ios_Exists(Ios_WinterBoard_App_Folder))
	{
		_pFileStream->Ios_DeleteFile(Ios_WinterBoard_Setting_File);
	}

    CFileOperation::DeleteFile(strWinterBoardSettingFileTemp);

	return bRet;
}

void IosThemeManage::CreatePandaHomeSettingFile(wstring strFileName, wstring strThemeName)
{
	IosPlistHelper xmlHelper(XML_PI, XML_DOCTYPE, eXmlType_UTF8, XML_ELEMENT(L"plist", L"", L"version", L"1.0"));

	XML_NODE_INDEX index;
	xmlHelper.AddElement(index, XML_ELEMENT(L"array"));

	index.AddChild(0);
	xmlHelper.AddElement(index, XML_ELEMENT(L"string", strThemeName));

	xmlHelper.WriteToFile(strFileName);
}

void IosThemeManage::ClearWeatherInfoSettingFile()
{
	RETURN_IF(!_pFileStream);
	RETURN_IF(!_pFileStream->Ios_Exists(Ios_Weather_Info_File));

	wstring strLocalFile =_pDevice->GetThemePath()->GetThemeTempPath() + L"WeatherInfo" + CFileOperation::GetGuidW() + L".plist";
    CFileOperation::DeleteFile(strLocalFile);

	if (_pFileStream->Ios_DownFromPhone(Ios_Weather_Info_File, strLocalFile) == Transfer_Success &&
		IosPlistConverter::GetXmlFromPlist(strLocalFile, strLocalFile))
	{
		IosPlistHelper xmlHelper(strLocalFile, eXmlType_UTF8);
		XML_NODE_INDEX index;
		index.AddChild(0);
		rapidxml::xml_node<char>* dictNode = xmlHelper.GetNodeFromIndex(&index);
		if (xmlHelper.IsKeyExist(dictNode, "SBIconClass"))
		{
			xmlHelper.DeleteNode(dictNode, "SBIconClass");
		}
		xmlHelper.WriteToFile(strLocalFile);

		_pFileStream->Ios_CopyToPhone(strLocalFile, Ios_Weather_Info_File);
	}

    CFileOperation::DeleteFile(strLocalFile);
}

void IosThemeManage::RenameIconNameByDevLang(wstring strThemeName)
{
	RETURN_IF(!_pFileStream);

	eLocaleType eLangType = IosPublicFunc::GetCurrentLang(_pFileStream, _pDevice->GetPath()->GetWorkFolder());
	wstring strIconFolder = Ios_WSBoard_Theme_Path;
	strIconFolder += L"/" + strThemeName + L"/Icons/";

	for (map<eAppIconType, APP_ICON_NAME*>::iterator it=_mapAppIconName.begin(); it!=_mapAppIconName.end(); it++) //
	{
		APP_ICON_NAME* pTemp = it->second;
		if (!pTemp)
			continue;

		wstring strNewIconName = pTemp->_strEnglist;
		wstring strOldIconNameOne = pTemp->_strChinese;
		wstring strOldIconNameTwo = pTemp->_strTaiwan;
		switch (eLangType)
		{
		case eLocaleType_ZH:
			{
				strNewIconName = pTemp->_strChinese;
				strOldIconNameOne = pTemp->_strEnglist;
			}			
			break;
		case eLocaleType_TW:
			{
				strNewIconName = pTemp->_strTaiwan;
				strOldIconNameTwo = pTemp->_strEnglist;
			}			
			break;
		}
		strNewIconName += L".png";
		strOldIconNameOne += L".png";
		strOldIconNameTwo += L".png";

		if (!_pFileStream->Ios_Exists(strIconFolder+strNewIconName))
		{
			if (_pFileStream->Ios_Exists(strIconFolder+strOldIconNameOne))
			{
				_pFileStream->Ios_Rename(strIconFolder+strOldIconNameOne, strIconFolder+strNewIconName);
			}
			else if (_pFileStream->Ios_Exists(strIconFolder+strOldIconNameTwo))
			{
				_pFileStream->Ios_Rename(strIconFolder+strOldIconNameTwo, strIconFolder+strNewIconName);
			}
		}
	}
}

bool IosThemeManage::RevocationWinterBoardTheme()
{
	RETURN_FALSE_IF(!_pFileStream);

	wstring strWinterBoardSettingFileTemp = _pDevice->GetThemePath()->GetThemeTempPath() + L"WinterBoard" + CFileOperation::GetGuidW() + L".plist";
    CFileOperation::DeleteFile(strWinterBoardSettingFileTemp);

	bool bRet = false;
	if (_pFileStream->Ios_Exists(Ios_WinterBoard_Setting_File) &&
		_pFileStream->Ios_DownFromPhone(Ios_WinterBoard_Setting_File, strWinterBoardSettingFileTemp) == Transfer_Success &&
		IosPlistConverter::GetXmlFromPlist(strWinterBoardSettingFileTemp, strWinterBoardSettingFileTemp))
	{
		IosPlistHelper plistHelper(strWinterBoardSettingFileTemp, eXmlType_UTF8);
		XML_NODE_INDEX index;
		index.AddChild(0);
		vector<rapidxml::xml_node<char>*> vtValues;
		IosPlistHelper::GetNodesFromNode(plistHelper.SearchKeyNode(index, "Themes", "array"), vtValues);
		for (vector<rapidxml::xml_node<char>*>::iterator it=vtValues.begin(); it!=vtValues.end(); it++)
		{
			rapidxml::xml_node<char>* pActiveNode = IosPlistHelper::SearchKeyNodeFromXmlNode(*it, "Active");
			if (pActiveNode)
				pActiveNode->name("false");
		}
		plistHelper.WriteToFile(strWinterBoardSettingFileTemp);

		if (_pFileStream->Ios_CopyToPhone(strWinterBoardSettingFileTemp, Ios_WinterBoard_Setting_File) == Transfer_Success)
		{
			/*SetOwnCommand pSetOwnCmd(_pDevice);
			pSetOwnCmd.SetParams(Ios_WinterBoard_Setting_File, "501");
			pSetOwnCmd.Execute();*/
			DirectoryOperationCommand(_pDevice).ChangeMode(Ios_WinterBoard_Setting_File);

			bRet = true;
		}
	}

    CFileOperation::DeleteFile(strWinterBoardSettingFileTemp);

	return bRet;
}

bool IosThemeManage::RevocationPandaHomeTheme()
{
	RETURN_FALSE_IF(!_pFileStream);

	wstring strWinterBoardSettingFileTemp = _pDevice->GetThemePath()->GetThemeTempPath() + L"PandaHome" + CFileOperation::GetGuidW() + L".plist";
    CFileOperation::DeleteFile(strWinterBoardSettingFileTemp);

	CreatePandaHomeSettingFile(strWinterBoardSettingFileTemp, L"");

	bool bRet = false;
	if (_pFileStream->Ios_CopyToPhone(strWinterBoardSettingFileTemp, Ios_PandaHome_Setting_File) == Transfer_Success)
	{
		/*SetOwnCommand pSetOwnCmd(_pDevice);
		pSetOwnCmd.SetParams(Ios_PandaHome_Setting_File, "501");
		pSetOwnCmd.Execute();*/
		DirectoryOperationCommand(_pDevice).ChangeMode(Ios_PandaHome_Setting_File);
		bRet = true;
	}

    CFileOperation::DeleteFile(strWinterBoardSettingFileTemp);

	return bRet;
}



#endif