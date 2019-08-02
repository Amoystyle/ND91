#include "stdafx.h"
#include "SystemSetUI.h"
#include "SystemSetDlg.h"

SystemSetUI::SystemSetUI()
: _model(this)
{INTERFUN;

}


SystemSetUI::~SystemSetUI()
{INTERFUN;

}

void SystemSetUI::SetCheck(wstring name, bool flag )
{INTERFUN;
	CCheckBoxUI* pCheckBox = (CCheckBoxUI*)GetItem(name.c_str());
	if (pCheckBox)
	{
		pCheckBox->SetChecked(flag);
	}
}

void SystemSetUI::SetRadio( wstring name )
{INTERFUN;
	CRadioUI *pRadio = (CRadioUI *)GetItem(name.c_str());
	if (pRadio)
	{
		pRadio->SetChecked(true);
	}
}

void SystemSetUI::OnCreate()
{INTERFUN;
	_model.Init();
	((SystemSetDlg*)_dlgpoint)->ActivationApplyBtn(false);
}

bool SystemSetUI::GetCheck(wstring name)
{INTERFUN;
	CCheckBoxUI* pCheckBox = (CCheckBoxUI*)GetItem(name.c_str());
	if (pCheckBox)
		return pCheckBox->IsChecked();
	return false;
}

bool SystemSetUI::GetRadio( wstring name )
{INTERFUN;
	CRadioUI *pRadio = (CRadioUI *)GetItem(name.c_str());
	if (pRadio)
		return pRadio->IsChecked();

	return false;
}

void SystemSetUI::Save_Data()
{INTERFUN;
	_model.SaveSetting();
}

void SystemSetUI::SetPoint( void *p )
{INTERFUN;
	_dlgpoint = p;
}

bool SystemSetUI::OnContactPersonClick( TNotifyUI* pNotify )
{INTERFUN;
	((SystemSetDlg*)_dlgpoint)->ActivationApplyBtn(true);

	return true;
}

bool SystemSetUI::OnMinimizeToTrayClick( TNotifyUI* pNotify )
{INTERFUN;
	((SystemSetDlg*)_dlgpoint)->ActivationApplyBtn(true);

	return true;
}

bool SystemSetUI::OnExitAppClick( TNotifyUI* pNotify )
{INTERFUN;
	((SystemSetDlg*)_dlgpoint)->ActivationApplyBtn(true);

	return true;
}

bool SystemSetUI::OnDownLoadMsgCilck( TNotifyUI* pNotify )
{INTERFUN;
	((SystemSetDlg*)_dlgpoint)->ActivationApplyBtn(true);

	return true;
}

bool SystemSetUI::OnSoftInstallMsg( TNotifyUI* pNotify )
{INTERFUN;
	((SystemSetDlg*)_dlgpoint)->ActivationApplyBtn(true);

	return true;
}

bool SystemSetUI::OnAutoDeleteTask( TNotifyUI* pNotify )
{INTERFUN;
	((SystemSetDlg*)_dlgpoint)->ActivationApplyBtn(true);

	return true;
}
