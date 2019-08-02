#include "stdafx.h"
#include "SystemSetTabUI.h"
#include "Common/SettingCfg.h"

SystemSetTabUI::SystemSetTabUI()
{INTERFUN;
	_systemSetUI = new SystemSetUI();
	_downLoadSystemSetUI = new DownLoadSystemSetUI();
}

SystemSetTabUI::~SystemSetTabUI()
{INTERFUN;

}

void SystemSetTabUI::OnCreate()
{INTERFUN;
	RegisterComponent(dynamic_cast<CContainerUI*>(GetItem(_T("SystemSetTab.BasicsLayout.BasicsContainer"))), _T("BasicsSystemSetStyle"), (CTabItem*)_systemSetUI);
	RegisterComponent(dynamic_cast<CContainerUI*>(GetItem(_T("SystemSetTab.DownLoadLayout.DownLoadContainer"))), _T("DownLoadSystemSetStyle"), (CTabItem*)_downLoadSystemSetUI);
	CCompositeTabUI::OnCreate();
	ShowComponent(_T("BasicsSystemSetStyle"));
}

bool SystemSetTabUI::OnTabChange( TNotifyUI* pNotify )
{INTERFUN;
	CTabUI* pTab = (CTabUI*)GetItem(_T("SystemSetTab"));
	if (!pTab)
		return true;


	pTab->SetTabHeaderAttribute(0,_T("txt.textcolor"),_T("2e4d68"));
	pTab->SetTabHeaderAttribute(1,_T("txt.textcolor"),_T("2e4d68"));

	pTab->SetTabHeaderAttribute(pTab->GetCurSel(),_T("txt.textcolor"),_T("ffffff"));

	switch(pTab->GetCurSel())
	{
	case 0:
		{
			ShowComponent(_T("BasicsSystemSetStyle"));	
		}
		break;
	case 1:
		{
			ShowComponent(_T("DownLoadSystemSetStyle"));
		}
		break;
	}
	return true; 
}

void SystemSetTabUI::Save_Data()
{INTERFUN;
	_systemSetUI->Save_Data();
	_downLoadSystemSetUI->Save_Data();
	SETTINGCFG_HANDLE->WriteIni();
}

void SystemSetTabUI::SetPoint( void *p )
{INTERFUN;
	_systemSetUI->SetPoint(p);
	_downLoadSystemSetUI->SetPoint(p);

}

