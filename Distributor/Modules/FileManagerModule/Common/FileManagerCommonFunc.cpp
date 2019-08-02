#include "StdAfx.h"
#include "FileManagerCommonFunc.h"
#include "Core/DeviceManager.h"

FileManagerCommonFunc::FileManagerCommonFunc(void)
{
}

FileManagerCommonFunc::~FileManagerCommonFunc(void)
{
}

std::wstring FileManagerCommonFunc::GetPrompt( MESSAGE_CODE msgCode )
{
	wstring strMsg;
	switch (msgCode)
	{
	case MESSAGE_EXPORT:
		strMsg = CommonUIFunc::LoadString(_T("Export_text"));
		break;
	case MESSAGE_IMPORT:
		strMsg = CommonUIFunc::LoadString(_T("Import_text"));
		break;
	case MESSAGE_PREPARE:
		strMsg = CommonUIFunc::LoadString(_T("Message_Prepare_text"));
		break;
	case MESSAGE_INSTALL_PREPARE:
		strMsg = CommonUIFunc::LoadString(_T("Message_Install_prepare_text"));
		break;
	case MESSAGE_INSTALL_PXLTOIPA:
		strMsg = CommonUIFunc::LoadString(_T("Message_Install_pxltoipa_text"));
		break;
	case MESSAGE_INSTALL_UPLOAD:
		strMsg = CommonUIFunc::LoadString(_T("Message_Install_upload_text"));
		break;
	case MESSAGE_INSTALL_INSTALL:
		strMsg = CommonUIFunc::LoadString(_T("Message_Install_install_text"));
		break;
	case MESSAGE_INSTALL_UNINSTALL:
		strMsg = CommonUIFunc::LoadString(_T("Message_Install_uninstall_text"));
		break;
	case MESSAGE_INSTALL_COPYDATA:
		strMsg = CommonUIFunc::LoadString(_T("Message_Install_copydata_text"));
		break;
	case MESSAGE_INSTALL_NOTIFYSB:
		strMsg = CommonUIFunc::LoadString(_T("Message_Install_notifysb_text"));
		break;
	case MESSAGE_ARCHIVE_DOWNLOAD:
		strMsg = CommonUIFunc::LoadString(_T("Message_Archive_download_text"));
		break;
	case MESSAGE_ARCHIVE_ARRANGE:
		strMsg = CommonUIFunc::LoadString(_T("Message_Archive_arrange_text"));
		break;
	case MESSAGE_ARCHIVE_PACKAGE:
		strMsg = CommonUIFunc::LoadString(_T("Message_Archive_package_text"));
		break;
	case MESSAGE_ARCHIVE_REMOVE:
		strMsg = CommonUIFunc::LoadString(_T("Message_Archive_remove_text"));
		break;
	case MESSAGE_ARCHIVE_APP_ONPHONE:
		strMsg = CommonUIFunc::LoadString(_T("Message_Archive_app_onphone_text"));
		break;
	case MESSAGE_UPLOAD:
		strMsg = CommonUIFunc::LoadString(_T("Message_Upload_text"));
		break;
	case MESSAGE_DOWNLOAD:
		strMsg = CommonUIFunc::LoadString(_T("Message_Download_text"));
		break;
	case MESSAGE_BACKUP:
		strMsg = CommonUIFunc::LoadString(_T("Message_Backup_text"));
		break;
	case MESSAGE_RESTORE:
		strMsg = CommonUIFunc::LoadString(_T("Message_Restore_text"));
		break;
	case MESSAGE_PARSE:
		strMsg = CommonUIFunc::LoadString(_T("Message_Parse_text"));
		break;
	}
	return strMsg;
}


std::wstring FileManagerCommonFunc::GetDateTimeString()
{
	SYSTEMTIME   s; 
	GetLocalTime(&s); 
	WCHAR date[1024] = L"";
	wsprintf(date,_T("%04d-%02d-%02d  %02d:%02d:%02d"),s.wYear,s.wMonth,s.wDay,s.wHour,s.wMinute,s.wSecond);
	return wstring(date);
}

wstring FileManagerCommonFunc::DirDialog(HWND pParent, wstring strDefaultPath)
{
	if (CFolderBrowser::SelectFolder(pParent, strDefaultPath))
	return strDefaultPath;
	else
	return L"";
}

#define MaxMultiFileCount 200
vector<wstring> FileManagerCommonFunc::MultiFileDialog( HWND pParent /*= NULL*/, LPCTSTR strFileType/*=L"*.*\0*.*\0"*/, DWORD flags/*=OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_EXPLORER*/ , wstring wsTitle)
{
	vector<wstring> files;

	TCHAR szFile[MaxMultiFileCount * MAX_PATH];
	TCHAR szPath[MAX_PATH];
	TCHAR szCopy[MAX_PATH];
	memset(szFile, 0, MaxMultiFileCount * MAX_PATH);
	memset(szPath, 0, MAX_PATH);
	memset(szCopy, 0, MAX_PATH);

	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.hwndOwner = pParent;
	ofn.lStructSize=sizeof(OPENFILENAME);
	ofn.lpstrFilter= strFileType;
	ofn.lpstrFile=szFile;
	ofn.nMaxFile=MAX_PATH;
	ofn.Flags=flags;
	ofn.nMaxFile = MaxMultiFileCount * MAX_PATH;
	if (!wsTitle.empty())
	ofn.lpstrTitle=wsTitle.c_str();
	if(GetOpenFileName(&ofn))//strFile得到用户所选择文件的路径和文件名 
	{
		lstrcpyn(szPath, szFile, ofn.nFileOffset);
		szPath[ofn.nFileOffset] = TEXT('\0');

		if(szPath[_tcslen(szPath) - 1] != TEXT('\\'))     // 必须加上TEXT('\\'),因为是文件目录
		{
			_tcscat_s(szPath, MAX_PATH, TEXT("\\"));
		}
		TCHAR* pFilePath = szFile + ofn.nFileOffset;
		while( *pFilePath )
		{
			ZeroMemory(szCopy, sizeof(szCopy));
			_tcscpy_s(szCopy, szPath);                // 文件路径
			_tcscat_s(szCopy, pFilePath);             // 文件名
			files.push_back(szCopy);
			pFilePath += _tcslen(pFilePath) + 1;      // 指向下一个文件名
		}
		//return files;
	}
	wstring tmp = CFileOperation::GetCurrWorkingDir();
	SetCurrentDirectory(tmp.c_str());//防止选中后文件夹被占用
	return files;
}

DeviceData* FileManagerCommonFunc::FindDevice( wstring id )
{
	return DEVICE_MANAGER->FindDevice(id);
}
