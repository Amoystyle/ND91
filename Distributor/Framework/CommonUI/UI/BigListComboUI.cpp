#include "stdafx.h"
#include "BigListComboUI.h"

CBigListComBoxUI::CBigListComBoxUI():CComboBoxUI()
{

}
CBigListComBoxUI::~CBigListComBoxUI()
{

}
UI_IMPLEMENT_DYNCREATE(CBigListComBoxUI);
void CBigListComBoxUI::ShowDrop()
{
	//__super::ShowDrop();
	CListUI* List = this->GetList();
	int textlen=0;
	int ncount =List->GetCount();
	for(int i=0;i<ncount;i++)
	{
		CControlUI* item = List->GetItem(i);
		wstring text = item->GetText();
		int len = CCodeOperation::ws2s(text).length()*7;
		if(textlen<len)
			textlen=len;
	}
	
	RECT rect =	this->GetRect();

	if(textlen<rect.right-rect.left)
	{
		__super::ShowDrop();
		return;
	}
	POINT xy;
	xy.x=rect.left;
	xy.y=rect.top;
	ClientToScreen(GetWindow()->GetHWND(),&xy);
	m_pToolWindow->SetWindowPos(HWND_TOPMOST,xy.x,xy.y+(rect.bottom-rect.top),textlen+10,ncount*PER_ITEM_HEIGHT + 4,SWP_SHOWWINDOW);
	
}
