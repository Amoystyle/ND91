#include "stdafx.h"
#include "VersionController.h"

VersionController::VersionController()
{
	_pbtn = NULL;
}

void VersionController::Init(CContainerUI* pTitleToolBar, CContainerUI* pStatusBar)
{
	CLayoutUI* pVerLayout = new CLayoutUI();
	pVerLayout->SetInitPos(10,0,300,0,7);
	pVerLayout->SetAttribute(L"mode",L"lefttoright");
	pVerLayout->SetAttribute(L"spacex",L"3");
	pStatusBar->Add(pVerLayout);

	wstring now_version =CommonUIFunc::LoadString(L"DependenceFuncCenter_Now_vesion_text");
	now_version += CCodeOperation::StringToWstring(CClientInfo::GetInstance()->GetVersion());
	CLabelUI *pLabel = new CLabelUI;
	pLabel->SetInitPos(0,-1,50,12,9);
	pLabel->SetText(now_version.c_str());
	pLabel->SetId(L"curVersion");
	pLabel->SetAttribute(_T("txt.textcolor"),_T("333333"));
	pLabel->SetAttribute(L"sizewithcontent",L"1");
	pLabel->SetVisible(false);
	pVerLayout->Add(pLabel);


	CAutoSizeButtonUI *pButton = new CAutoSizeButtonUI;
	pButton->SetINotifyUI(this);
	pButton->SetId(L"BtnNewVersion");
	pButton->SetText(CommonUIFunc::LoadString(L"DependenceFuncCenter_New_vesion_text"));
	pButton->SetStyle(L"Link");
	pButton->SetInitPos(0,-1,50,12,9);
	pButton->SetAttribute(_T("txt.textcolor"),_T("2e4d68"));
	pButton->SetAttribute(_T("cursor"),_T("hand"));
	pButton->SetAttribute(L"sizewithcontent",L"1");
	pButton->SetVisible(false);
	pVerLayout->Add(pButton);

	_pbtn = pButton;
	pStatusBar->Resize();
	pStatusBar->Invalidate();
}

VersionController::~VersionController()
{

}

bool VersionController::OnNewVersion( TNotifyUI* pNotify )
{INTERFUN;
	//判断当前的文字是否为  正在检测新版本...
	//如果是则return true;
	//如果为版本号则调用update
	string now_version = CClientInfo::GetInstance()->GetVersion();
	string new_version = CClientInfo::GetInstance()->GetIniVersion();

	if (new_version.length() == 0)
	return true;

	if (_pbtn)
	{
		tstring buf = _pbtn->GetText();
		if (buf == CommonUIFunc::LoadString(_T("DependenceFuncCenter_New_vesion_text")))
		{
			if (now_version >= new_version)
			{
				return true;
			}
			else
			{
				_pbtn->SetText((CommonUIFunc::LoadString(_T("DependenceFuncCenter_UpdateTo")) + CCodeOperation::GB2312ToUnicode(new_version) + _T(")")).c_str());
				
			}
			_pbtn->SetVisible(false);
				
		}
		else	//启动update
		{
	#ifndef _DEBUG
			BaseOperation::Execute(CFileOperation::GetCurrWorkingDir() + _T("update.exe"), _T("/p2p"));
	#endif
		}
		_pbtn->GetParent()->Resize();
		_pbtn->GetParent()->Invalidate();
	}
	//统计接口
	CStaticsticsMessage msg(Statistic_ID_CheckUpdate);
	msg.Send();

	return true;
}

void VersionController::Update_Msg()
{INTERFUN;

	string now_version = CClientInfo::GetInstance()->GetVersion();
	string new_version = CClientInfo::GetInstance()->GetIniVersion();


	if (new_version.length() == 0)
	{//版本号错误
		return ;
	}

	if (_pbtn)
	{
		if (new_version != now_version)
		{
			_pbtn->SetText((CommonUIFunc::LoadString(_T("DependenceFuncCenter_UpdateTo")) + CCodeOperation::GB2312ToUnicode(new_version) + _T(")")).c_str());
		}
		else
		{
			_pbtn->SetText(CommonUIFunc::LoadString(_T("DependenceFuncCenter_New_vesion_text")));
		}
		_pbtn->SetVisible(false);
		_pbtn->GetParent()->Resize();
		_pbtn->GetParent()->Invalidate();
	}	
}

bool VersionController::Handle(const UpdateVersion_Msg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}

	this->Update_Msg();

	return true;
}