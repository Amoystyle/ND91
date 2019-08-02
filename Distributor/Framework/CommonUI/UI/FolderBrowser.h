#pragma once

struct COMMONUI_API TFolderParam
{
	BOOL bInited;
	HWND hTitleWnd;
	TCHAR szInitPath[MAX_PATH];

	TFolderParam();

	TFolderParam(TFolderParam& p);
};

class COMMONUI_API CFolderBrowser
{
public:
	CFolderBrowser(void);
	~CFolderBrowser(void);

    static BOOL SelectFolder(HWND hWnd, wstring& Path);
};
