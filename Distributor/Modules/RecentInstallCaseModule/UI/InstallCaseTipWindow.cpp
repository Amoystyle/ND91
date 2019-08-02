#include "StdAfx.h"
#include "InstallCaseTipWindow.h"

InstallCaseTipWindow* InstallCaseTipWindow::_instance=NULL;

InstallCaseTipWindow::InstallCaseTipWindow(void)
{
	m_nRoundRgn = 0;
}

InstallCaseTipWindow::~InstallCaseTipWindow(void)
{
}

void InstallCaseTipWindow::Show( HWND hParent, RECT rect, wstring name, wstring size, int cnt, wstring installtimestr )
{
	if(!m_hWnd)
		Create(hParent,_T("InstallCaseTip"));
	RETURN_IF(!m_hWnd);

	SetWndStyle(GWL_EXSTYLE, WS_EX_TOOLWINDOW);

	SetLayerWindow(true);

	CControlUI* namectrl=GetItem(L"CaseTip_Layout.Name_layout.CaseName");
	if(namectrl)
		namectrl->SetText(name.c_str());

	CControlUI* sizectrl=GetItem(L"CaseTip_Layout.CaseSize_layout.CaseSize");
	if(sizectrl)
		sizectrl->SetText(size.c_str());

	CControlUI* cntctrl=GetItem(L"CaseTip_Layout.CaseAppCount_layout.CaseAppCount");
	if(cntctrl)
		cntctrl->SetText(CStrOperation::IntToWString(cnt).c_str());

	CControlUI* timectrl=GetItem(L"CaseTip_Layout.CaseInstallTime_layout.CaseInstallTime");
	if(timectrl)
		timectrl->SetText(installtimestr.c_str());

	RECT parentR;
	::GetWindowRect(hParent, &parentR);
	int width = 190;
	int height = 129;
	int nStartPosX=0;
	int nStartPosY=0;
	RECT rRect;;
	this->GetWindowRect(&rRect);
	if (parentR.left+rect.right+17+(rRect.right-rRect.left)>parentR.right)
	{
		nStartPosX=parentR.left+rect.left-17-(rRect.right-rRect.left);
	}
	else
	{
		nStartPosX=parentR.left+rect.right+17;
	}
	nStartPosY=parentR.top+rect.top;
	MoveWindow(m_hWnd,nStartPosX,nStartPosY,width,height,true);

	ShowWindow(SW_SHOW);
}

InstallCaseTipWindow* InstallCaseTipWindow::GetInstance()
{
	if(!_instance)
	{
		_instance =new InstallCaseTipWindow();
	}
	return _instance;
}

void InstallCaseTipWindow::Hide()
{
	ShowWindow(SW_HIDE);
}