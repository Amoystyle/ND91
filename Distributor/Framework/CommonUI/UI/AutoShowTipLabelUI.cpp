#include "stdafx.h"
#include "AutoShowTipLabelUI.h"

CAutoShowTipLabelUI::CAutoShowTipLabelUI()
{INTERFUN;

}
CAutoShowTipLabelUI::~CAutoShowTipLabelUI()
{INTERFUN;

}
UI_IMPLEMENT_DYNCREATE(CAutoShowTipLabelUI);
void  CAutoShowTipLabelUI:: SetRect(RECT& rect)//SetText(LPCTSTR lpszText)
{INTERFUN;
	
	CLabelUI::SetRect(rect);
	RECT rectClient = GetInterRect();
	RECT rc;
	TextStyle* pStyle = GetTextStyle(_T("txt"));
	if (pStyle) pStyle->GetFontObj()->CalcText( GetWindow()->GetPaintDC(), rc, GetText());

	int textfit =rc.right-rc.left;
	int width = rectClient.right -rectClient.left;
	if(width<textfit)
	{
		this->SetToolTip(GetText());
		this->SetToolTipShow(true);
		this->SetAttribute(L"enablemouse",L"1");
	}
	else
	{
		this->SetToolTipShow(false);
	}
	__super::SetRect(rect);
}
