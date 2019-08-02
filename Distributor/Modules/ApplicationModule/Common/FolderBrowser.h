//===============================================================================================
// Creator: Liao Ying
// CodeProjectID:liaoy747@163.com
// Date: 2012-4-27
// Decription Language: Simple Chinese
// Description:	Microsoft 没有直接提供一个好用的选择文件夹的接口
//				在网上找了几段代码, 都不尽人意, 因此做了一些修改
//				主要特性:
//				1. 支持初始化目录
//				2. 支持存储上次访问目录
//				3. 支持预览目录,新建文件夹
//				4. 使用方便, 调用方法如:
//
//				#include "FolderBrowser.h"
//				...
//				CString strPath;
//				::GetCurrentDirectory(MAX_PATH, strPath.GetBuffer(MAX_PATH));
//				if (CFolderBrowser::SelectFolder(this->m_hWnd, strPath))
//					m_edit.SetWindowText(strPath);								// m_edit是CEdit对象, 参见示例工程
//				...
//
// 本程序在Windows Xp Sp3 + Visual Studio 2008 Sp1 下调试通过.
//===============================================================================================
// 2012.12.19   邱畅 修改CString为wstring; 返回值的bug修正
//
#pragma once
App_CC_BEGIN
struct TFolderParam
{
	BOOL bInited;
	HWND hTitleWnd;
	TCHAR szInitPath[MAX_PATH];

	TFolderParam()
	{
		bInited = FALSE;
		hTitleWnd = NULL;
		::ZeroMemory(szInitPath, sizeof(szInitPath));
	}

	TFolderParam(TFolderParam& p)
	{
		bInited = p.bInited;
		hTitleWnd = p.hTitleWnd;
		wcscpy_s(szInitPath, p.szInitPath);
	}
};

class CFolderBrowser
{
public:
	CFolderBrowser(void);
	~CFolderBrowser(void);

    static BOOL SelectFolder(HWND hWnd, wstring& Path);
};
App_CC_END