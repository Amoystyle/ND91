#include "stdafx.h"
#include "SystemSetDlg.h"


SystemSetDlg::SystemSetDlg()
: CWindowUI()
,_model(this)
{INTERFUN;
	m_nRoundRgn = 0;

	Create(Singleton::GetMainWnd(),_T("SystemSetDlg"));
	CenterWindow(Singleton::GetMainWnd());
	SetWndStyle(GWL_EXSTYLE, WS_EX_TOOLWINDOW);
	SetEnableResize(false);
}

SystemSetDlg::~SystemSetDlg()
{INTERFUN;

}

void SystemSetDlg::OnCreate()
{INTERFUN;

	ActivationApplyBtn(false);

	_systemSetTabUI = new SystemSetTabUI();
	_systemSetTabUI->SetPoint(this);
	_systemSetTabUI->Create(_T("SystemSetTabStyle"),(CContainerUI*)GetItem(_T("MySystemSet.BasicsSystemSet")));
}

bool SystemSetDlg::OnApplyBtn( TNotifyUI* pNotify )
{INTERFUN;
	//»Ö¸´Ä¬ÈÏÉèÖÃ
	_systemSetTabUI->Save_Data();
	ActivationApplyBtn(false);
	return true;
}

bool SystemSetDlg::OnOkBtn( TNotifyUI* pNotify )
{INTERFUN;
	//Ğ´ÈëÅäÖÃÎÄ¼ş
	_systemSetTabUI->Save_Data();
	EndDialog();
	return true;
}

bool SystemSetDlg::OnCancelBtn( TNotifyUI* pNotify )
{INTERFUN;
	EndDialog();
	return true;
}

bool SystemSetDlg::ActivationApplyBtn(bool flag)
{INTERFUN;
	CButtonUI *apply = (CButtonUI *)GetItem(_T("Layout_OptionButtons.Apply"));
	if (apply)
		apply->SetEnabled(flag);

	this->Invalidate();
	return true;
}
