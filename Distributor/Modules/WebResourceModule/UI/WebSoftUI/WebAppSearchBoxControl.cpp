#include "StdAfx.h"
#include "WebAppSearchBoxControl.h"

WebAppSearchBoxControl::WebAppSearchBoxControl(void)
{
	_pEdit = NULL;
	_pINotifyUI = NULL;
}

WebAppSearchBoxControl::~WebAppSearchBoxControl(void)
{
}

void WebAppSearchBoxControl::OnCreate()
{
	_pEdit = (CEditUI*)this->GetItem(L"SearchLayout.Edit");
}

bool WebAppSearchBoxControl::OnKeyDown( TNotifyUI* pNotify )
{
	if(_pEdit == NULL || pNotify->wParam != VK_RETURN)
		return true;

	Send(WebAppSearchBox_Search, this, (WPARAM)BaseOperation::AllocateBuffer(_pEdit->GetText()), NULL);
	return true;
}

bool WebAppSearchBoxControl::OnKillFocus( TNotifyUI* pNotify )
{
// 	if(_pEdit == NULL )
// 		return true;
// 	if(CStrOperation::trimLeftW(_pEdit->GetText(), L"") == L"")
// 		_pEdit->SetText(BaseUI::LoadString(L"entersoftname"));
// 	else
// 		Send(WebAppSearchBox_Search, this, (WPARAM)BaseOperation::AllocateBuffer(_pEdit->GetText()), NULL);
	return true;
}

bool WebAppSearchBoxControl::OnSetFocuns( TNotifyUI* pNotify )
{
// 	if(_pEdit == NULL )
// 		return true;
// 	wstring edittext = _pEdit->GetText();
// 	wstring text = BaseUI::LoadString(L"entersoftname");
// 	if( edittext == text)
// 	{
// 		_pEdit->SetText(L"");
// 		_pEdit->Resize();
// 		_pEdit->Invalidate();
// 	}
	return true;
}

bool WebAppSearchBoxControl::OnSearchBtn( TNotifyUI* pNotify )
{
	Send(WebAppSearchBox_Search, this, (WPARAM)BaseOperation::AllocateBuffer(_pEdit->GetText()), NULL);
	return true;
}

std::wstring WebAppSearchBoxControl::GetStyleID()
{
	return _T("HomeSearchBox");
}

void WebAppSearchBoxControl::Init( CContainerUI* pContainer )
{
	BaseView::Init(pContainer);
}
