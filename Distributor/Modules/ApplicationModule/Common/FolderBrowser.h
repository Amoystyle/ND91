//===============================================================================================
// Creator: Liao Ying
// CodeProjectID:liaoy747@163.com
// Date: 2012-4-27
// Decription Language: Simple Chinese
// Description:	Microsoft û��ֱ���ṩһ�����õ�ѡ���ļ��еĽӿ�
//				���������˼��δ���, ����������, �������һЩ�޸�
//				��Ҫ����:
//				1. ֧�ֳ�ʼ��Ŀ¼
//				2. ֧�ִ洢�ϴη���Ŀ¼
//				3. ֧��Ԥ��Ŀ¼,�½��ļ���
//				4. ʹ�÷���, ���÷�����:
//
//				#include "FolderBrowser.h"
//				...
//				CString strPath;
//				::GetCurrentDirectory(MAX_PATH, strPath.GetBuffer(MAX_PATH));
//				if (CFolderBrowser::SelectFolder(this->m_hWnd, strPath))
//					m_edit.SetWindowText(strPath);								// m_edit��CEdit����, �μ�ʾ������
//				...
//
// ��������Windows Xp Sp3 + Visual Studio 2008 Sp1 �µ���ͨ��.
//===============================================================================================
// 2012.12.19   �� �޸�CStringΪwstring; ����ֵ��bug����
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