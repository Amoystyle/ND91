#include "StdAfx.h"
#include "WebAppListFilterControl.h"

WebAppListFilterControl::WebAppListFilterControl(HWND hWnd)
{
	_updowns[DownloadLevel]=true;
	_updowns[PriceLevel]=true;
	_updowns[VersionLevel]=true;
}

WebAppListFilterControl::~WebAppListFilterControl(void)
{
}

bool WebAppListFilterControl::OnBtnClick( TNotifyUI* pNotify )
{
	if(pNotify->pSender == NULL)
		return false;

	wstring id = pNotify->pSender->GetId();
	if(id == L"btn_downloadlevel")
	{
		this->Send(WebAppListFilter_Click, this, DownloadLevel, _updowns[DownloadLevel]);
		_updowns[DownloadLevel]=!_updowns[DownloadLevel];
	}
	else if(id == L"btn_pricelevel")
	{
		this->Send(WebAppListFilter_Click, this, PriceLevel, _updowns[PriceLevel]);
		_updowns[PriceLevel]=!_updowns[PriceLevel];
	}
	else
	{
		this->Send(WebAppListFilter_Click, this, VersionLevel, _updowns[VersionLevel]);
		_updowns[VersionLevel]=!_updowns[VersionLevel];
	}
	return true;
}

std::wstring WebAppListFilterControl::GetStyleID()
{
	return _T("WebAppFilterCtrl");
}

void WebAppListFilterControl::Init( CContainerUI* pContainer )
{
	BaseView::Init(pContainer);
}

