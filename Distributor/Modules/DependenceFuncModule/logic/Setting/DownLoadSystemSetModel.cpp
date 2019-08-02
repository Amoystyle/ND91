#include "StdAfx.h"
#include "DownLoadSystemSetModel.h"
#include "Common/SettingCfg.h"
DownLoadSystemSetModel::DownLoadSystemSetModel(DownLoadSystemSetInterface *pInterface)
:_pInterface(pInterface)
{INTERFUN;

}

DownLoadSystemSetModel::~DownLoadSystemSetModel()
{INTERFUN;

}

bool DownLoadSystemSetModel::Init()
{INTERFUN;
	_pInterface->SetIosDir(CONFIG(_strIosDownPath));
	_pInterface->SetAndroidDir(CONFIG(_strAndroidDownPath));
	_pInterface->SetCheck(_T("AutoDelUpdatedApp"),CONFIG(_bAutoDelUpdatedApp));
	return true;
}

void DownLoadSystemSetModel::SaveDownLoadPathSetting()
{INTERFUN;
	wstring iosdown = _pInterface->GetIosDir();
	if (iosdown.length() != 0)
	{
		CONFIG_HANDLE->SetDownLoadPath(0,iosdown);
	}
	else
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(_T("IOSSoftDownloadDir_text")),CommonUIFunc::LoadString(_T("DependenceFuncCenter_Seting_text")));
	wstring androiddown = _pInterface->GetAndroidDir();
	if (androiddown.length() != 0)
	{
		CONFIG_HANDLE->SetDownLoadPath(1,androiddown);
	}
	else
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(_T("AndroidSoftDownloadDir_text")),CommonUIFunc::LoadString(_T("DependenceFuncCenter_Seting_text")));
}

void DownLoadSystemSetModel::SaveSetting()
{
	CONFIG(_bAutoDelUpdatedApp) = _pInterface->GetCheck(_T("AutoDelUpdatedApp"));
}