#include "StdAfx.h"
#include "Logic/WebSoftInfo.h"
#include "WebCategoriesControl.h"

WebCategoriesControl::WebCategoriesControl(HWND hWnd,bool isTop)
{
	_isTop = isTop;
	_bISSetCategories = false;
	_pContainer = NULL;
	_pTopModeLayout = NULL;
	_pBottomModeLayout = NULL;
	_pCategory_Combo = NULL;
}

WebCategoriesControl::~WebCategoriesControl(void)
{
}

void WebCategoriesControl::SetTopSelCategory(int selID)
{
	RETURN_IF(!_isTop||!_pCategory_Combo);
	int conCount = _pCategory_Combo->GetCount();
	for(int i=0;i<conCount;i++)
	{
		CListItemUI* item=(CListItemUI*)_pCategory_Combo->GetItem(i);
		if(selID == (int)item->GetUserData())
		{
			_pCategory_Combo->SelectItem(item->GetIndex());
			_bISSetCategories = false;
		}
	}
}

void WebCategoriesControl::SetCategories( vector<WebSoftCategory*>* pData, int selID )
{
	if(pData == NULL || pData->size() <= 0)
		return;
	if(!_isTop)
		_pContainer->RemoveAll();
	else
		_pCategory_Combo->RemoveAll();

	RECT rect = _pContainer->GetClientRect();
	LONG width = rect.right - rect.left;

	_visibleBtnCount = width/55;
	int index = 0;
	for (vector<WebSoftCategory*>::iterator it = pData->begin(); it != pData->end(); it++)
	{
		WebSoftCategory* pCate = *it;
		if(!_isTop)
		{
			if(_pContainer == NULL)
				break;
			CButtonUI* pButton = new CButtonUI();
			pButton->SetId(L"Btn_WebSoftCategory");
			pButton->SetStyle(L"WebResourceUI_WebSoftLink");
			pButton->SetAttribute(L"bk.image",L"");
			pButton->SetAttribute(L"cursor",L"hand");
			pButton->SetAttribute(L"txt.halign",L"center");
			pButton->SetAttribute(L"txt.textcolor", L"6E748C");
			pButton->SetText(pCate->_name.c_str());
			pButton->SetUserData(pCate->_id);
			pButton->SetInitPos(0, 0, 50, 20, 0);
			pButton->SetINotifyUI(this);

			if(index>=_visibleBtnCount)
			{
				pButton->SetVisible(false);
			}
			_pBtnList.push_back(pButton);
			_pContainer->Add(pButton);
			index++;
		}
		else
		{
			if(_pCategory_Combo == NULL)
				break;

			CListItemUI* pItem = new CListItemUI();
			pItem->SetId(CStrOperation::IntToWString(pCate->_id).c_str());
			pItem->SetText(pCate->_name.c_str());
			pItem->SetUserData(pCate->_id);
			_pCategory_Combo->Add(pItem);
			if(pCate->_id == selID)
			{
				_bISSetCategories = true;
				int index = _pCategory_Combo->GetCount();
				_pCategory_Combo->SelectItem(index-1);
				_bISSetCategories = false;
			}
		}
	}

	if(!_isTop)
	{
		_pContainer->Resize();
		_pContainer->Invalidate();
	}
}

void WebCategoriesControl::OnCreate()
{
	_pTopModeLayout = (CLayoutUI*)GetItem(L"WebAppCategoryTopMode");	
	_pBottomModeLayout = (CLayoutUI*)GetItem(L"WebAppCategoryBottomMode");
	_pContainer = (CLayoutUI*)GetItem(L"WebAppCategoryBottomMode.Category_Container");
	_pCategory_Combo = (CComboBoxUI*)GetItem(L"WebAppCategoryTopMode.WebCategory_Combo");
	if(_isTop)
	{
		_pBottomModeLayout->SetVisible(false);
		_pTopModeLayout->SetVisible(true);
	}
	else
	{
		_pTopModeLayout->SetVisible(false);
		_pBottomModeLayout->SetVisible(true);
	}
	this->Resize();
	this->Invalidate();
}


bool WebCategoriesControl::OnCateBtnClick(TNotifyUI* pNotify)
{
	if(pNotify == NULL || pNotify->pSender == NULL)
		return true;
	
	Send(WebCategories_Click, this, pNotify->pSender->GetUserData(), NULL);
	return true;
}

bool WebCategoriesControl::OnCateComboChecked( TNotifyUI* pNotify )
{
	if(_bISSetCategories)
		return true;
	CListItemUI* pListItem = (CListItemUI*)_pCategory_Combo->GetCurSel();
	int id = pListItem->GetUserData();
	if(id == _curID)
		return true;
	_curID = id;
	if(pNotify == NULL || pNotify->pSender == NULL)
		return true;

	Send(WebCategories_Click, this, _pCategory_Combo->GetCurSel()->GetUserData(), NULL);
	return true;
}

void WebCategoriesControl::OnRectChanged()
{
	if(!_isTop)
	{
		RECT rect = _pContainer->GetClientRect();
		LONG width = rect.right - rect.left;
		int count = width/55;
		if(count!=_visibleBtnCount)
		{
			_visibleBtnCount = count;
			int index = 0;
			for(vector<CButtonUI*>::iterator it=_pBtnList.begin(); it!=_pBtnList.end(); it++)
			{
				CButtonUI* pbtn = *it;
				if(index<_visibleBtnCount)
					pbtn->SetVisible(true);
				else
					pbtn->SetVisible(false);
				index++;
			}
		}
		_pContainer->Resize();
		_pContainer->Invalidate();
		
	}
}

std::wstring WebCategoriesControl::GetStyleID()
{
	return _T("WebAppCategoryCtrl");
}

void WebCategoriesControl::Init( CContainerUI* pContainer )
{
	BaseView::Init(pContainer);
}

