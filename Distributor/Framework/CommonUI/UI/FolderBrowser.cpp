#include "StdAfx.h"

CFolderBrowser::CFolderBrowser(void)
{
}

CFolderBrowser::~CFolderBrowser(void)
{
}

#define INIT_TITLE _T("Select a folder")

//====================================================================================
//====================================================================================
int CALLBACK BrowseProc(HWND hWnd, UINT msg, LPARAM lParam, LPARAM lpData)
{
	TCHAR szBuff[MAX_PATH];
	::ZeroMemory(szBuff, sizeof(szBuff));
	TFolderParam * pFolderParam = (TFolderParam*)lpData;
	if (NULL == pFolderParam)
		return 0;

	if (pFolderParam->bInited && BFFM_SELCHANGED == msg)
	{
		SHGetPathFromIDList((LPITEMIDLIST)lParam, szBuff);
		::SetWindowText(pFolderParam->hTitleWnd, szBuff);
	}
	else if(msg == BFFM_INITIALIZED)
	{
		pFolderParam->bInited = TRUE;
		pFolderParam->hTitleWnd = ::FindWindowEx(hWnd, NULL, NULL, INIT_TITLE);
		if(pFolderParam->szInitPath[0] && wcsncmp(pFolderParam->szInitPath, _T("\\\\"), 2)!=0)
		{
			::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (long)pFolderParam->szInitPath);
			::SetWindowText(pFolderParam->hTitleWnd, pFolderParam->szInitPath);
		}
		else
			::SetWindowText(pFolderParam->hTitleWnd, _T(""));
	}
	return 0;
}

//====================================================================================
//====================================================================================
BOOL CFolderBrowser::SelectFolder(HWND hWnd, wstring& strPath)
{
	// 如果已经初始化Com, 则无需调用.
	if ( strPath.length() >= MAX_PATH)
		strPath.clear();
	
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	TCHAR szBuff[MAX_PATH];
	::ZeroMemory(szBuff, sizeof(szBuff));

	BROWSEINFO bi;
	::ZeroMemory(&bi, sizeof(bi));

	TFolderParam Param;
	LPITEMIDLIST pidlBrowse = NULL;
	LPITEMIDLIST pidlDesktop = NULL;

	LPMALLOC pMalloc = NULL;
	if (SHGetMalloc(&pMalloc) != NOERROR)
		return FALSE;

	// Get the PIDL for the Desktop folder.
	if (SHGetSpecialFolderLocation(hWnd, CSIDL_DESKTOP, &pidlDesktop) != NOERROR)
		return FALSE;

	Param.bInited = FALSE;
	Param.hTitleWnd = NULL;
	wcscpy_s(Param.szInitPath, strPath.c_str());

	bi.hwndOwner = hWnd;
	bi.pidlRoot = pidlDesktop;
	bi.lpszTitle = INIT_TITLE;
	bi.ulFlags =  BIF_RETURNONLYFSDIRS | BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	bi.lpfn = BrowseProc;
	bi.lParam = (long)&Param;
	bi.pszDisplayName = szBuff;

	pidlBrowse = SHBrowseForFolder(&bi);
	if (pidlBrowse != NULL)
	{
		strPath = SHGetPathFromIDList(pidlBrowse, szBuff) ? szBuff : _T("");
		pMalloc->Free(pidlBrowse);
	}
    else
        strPath.clear();

	pMalloc->Free(pidlDesktop);

	if (!strPath.empty())
	{
		::SetCurrentDirectory(strPath.c_str());
		return TRUE;
	}
	return FALSE;
}

TFolderParam::TFolderParam()
{
	bInited = FALSE;
	hTitleWnd = NULL;
	::ZeroMemory(szInitPath, sizeof(szInitPath));
}

TFolderParam::TFolderParam( TFolderParam& p )
{
	bInited = p.bInited;
	hTitleWnd = p.hTitleWnd;
	wcscpy_s(szInitPath, p.szInitPath);
}
