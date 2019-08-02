#include "StdAfx.h"
#include "math.h"
#include "WebAppListPagerControl.h"

WebAppListPagerControl::WebAppListPagerControl( HWND hWnd,bool isTop /*= true*/, int pageSize /*= 20*/ )
{
	_isTop = isTop;

	_pTopModeLayout = NULL;
	_pBottomModeLayout = NULL;
	_pTopPageInfo = NULL;
	_pBottomPageInfo = NULL;
	_pContainer = NULL;
	_pEdit = NULL;

	_recordCount = 0;
	_recordPageSize = pageSize;
	_recordPageCount = 0;
	_recordPageIndex = 0;

	_pageCount = 0;
	_pagePageSize = 10;
	_pagePageCount = 0;
	_pagePageIndex = 0;
}

WebAppListPagerControl::~WebAppListPagerControl(void)
{
}

void WebAppListPagerControl::OnCreate()
{
	_pTopModeLayout = (CLayoutUI*)GetItem(L"Pager_TopMode");	
	_pBottomModeLayout = (CLayoutUI*)GetItem(L"Pager_BottomMode");
	_pTopPageInfo = (CLabelUI*)GetItem(L"Pager_TopMode.PageInfo");
	_pBottomPageInfo = (CLabelUI*)GetItem(L"Pager_BottomMode.GotoLayout.PageInfo");
	_pContainer = (CLayoutUI*)GetItem(L"Pager_BottomMode.Pager.Page_Container");
	_pEdit = (CEditUI*)GetItem(L"Pager_BottomMode.GotoLayout.gotoEdit");
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
}

void WebAppListPagerControl::SetTotalCount( int count )
{
	_recordCount = count;
	double tmp = (double)_recordCount/_recordPageSize;
	_recordPageCount = (int)ceil(tmp);
	
	_pageCount = _recordPageCount;
	tmp = (double)_pageCount/_pagePageSize;
	_pagePageCount = (int)ceil(tmp);
	
	_recordPageIndex=0;
	_pagePageIndex=0;
	if(_pContainer&&_pContainer->IsVisible()&&_pContainer->GetCount()>=1)
		_pContainer->RemoveAll();
	UpdatePageInfo();
	if(_pContainer&&_pContainer->IsVisible()&&_pContainer->GetCount()>=1)
	{
		((CRadioUI*)(_pContainer->GetItem(0)))->SetChecked(true);
	}
}

void WebAppListPagerControl::UpdatePageInfo()
{
	this->CreatePager();

	//by12.12.1 00:24 修正页码没刷新问题
	if(_pTopPageInfo&&_isTop)
	{
		wstring topText = CStrOperation::IntToWString(_recordPageIndex+1);
		topText += L"/";
		topText += CStrOperation::IntToWString(_recordPageCount);
		_pTopPageInfo->SetText(topText.c_str());
		_pTopPageInfo->Resize();
		_pTopPageInfo->Invalidate();
	}

	if(_pBottomPageInfo&&!_isTop)
	{
		wstring bottomText = L"共";
		bottomText += CStrOperation::IntToWString(_recordPageCount);
		bottomText += L"页";
		this->_pBottomPageInfo->SetText(bottomText.c_str());
	}

	if(_pContainer&&!_isTop)
	{
		int index = _recordPageIndex - (_pagePageIndex * _pagePageSize);
		CRadioUI* pRadio = (CRadioUI*)_pContainer->GetItem(index);
		if(pRadio != NULL)
		{
			pRadio->SetChecked(true);
		}
		_pBottomPageInfo->Resize();
		_pBottomPageInfo->Invalidate();
	}
}

void WebAppListPagerControl::CreatePager()
{
	if(!FixPageBar() && _pContainer->GetCount() > 0)
		return;

	if(_pContainer&&_pContainer->IsVisible())
		_pContainer->RemoveAll();

	int startIndex = _pagePageIndex * _pagePageSize;
	int endIndex = (_pagePageIndex + 1) * _pagePageSize;
	if(endIndex > _pageCount)
		endIndex = _pageCount;

	int pageItemWidth = 40;
	int size = endIndex - startIndex;
	int width = (size - 1) * 5;
	if(_pContainer&&_pContainer->IsVisible())
	{
		for (int i = startIndex; i < endIndex; i++)
		{
			int w = 40;
			if(i < 10)
			{
				w = 20; 
			}
			else if(i >= 10 && i < 100)
			{
				w = 20;
			}
			else if(i >= 100 && i < 1000)
			{
				w = 30;
			}
			else if(i > 1000 && i < 10000)
			{
				w = 35;
			}
			width += w;

			CRadioUI* pButton = new CRadioUI();
			pButton->SetId(L"btn_page");
			pButton->SetStyle(L"WebResourceUI_PageButton");
			pButton->SetText(CStrOperation::IntToWString(i+1).c_str());
			pButton->SetAttribute(L"cursor",L"hand");
			pButton->SetUserData(i);
			pButton->SetInitPos(0, 0, w, 20, 0);
			pButton->SetINotifyUI(this);
			_pContainer->Add(pButton);
		}
		_pContainer->SetInitPos(40, 0, width + 40, 20,0);
	}
}

bool WebAppListPagerControl::FixPageBar()
{
	int index = _recordPageIndex/_pagePageSize;
	int inner = _recordPageIndex % _pagePageSize;
	if(inner < 0)
		index--;
	else if( inner >= _pagePageSize )
		index++;

	if(index < 0)
		index = 0;
	else if(index > _pagePageCount - 1)
		index = _pagePageCount - 1;

	if(_recordPageIndex <= 0)
		index = 0;
	else if(_recordPageIndex >= _recordPageCount - 1)
		index = _pagePageCount - 1;

	bool fix = index != _pagePageIndex;
	_pagePageIndex = index;
	return fix;
}

bool WebAppListPagerControl::OnFirstBtnClick( TNotifyUI* pNotify )
{
	if(_recordPageIndex <= 0)
		return true;

	_recordPageIndex = 0;

	UpdatePageInfo();
	Send(WebAppListPager_Event, this, this->_recordPageIndex, NULL);

	return true;
}

bool WebAppListPagerControl::OnLastBtnClick( TNotifyUI* pNotify )
{
	if(_recordPageIndex >= _recordPageCount - 1)
		return true;

	_recordPageIndex = _recordPageCount - 1;

	UpdatePageInfo();
	Send(WebAppListPager_Event, this, this->_recordPageIndex, NULL);

	return true;
}

bool WebAppListPagerControl::OnPreBtnClick( TNotifyUI* pNotify )
{
	if(_recordPageIndex <= 0)
		return true;

	_recordPageIndex--;

	UpdatePageInfo();
	Send(WebAppListPager_Event, this, this->_recordPageIndex, NULL);

	return true;
}

bool WebAppListPagerControl::OnNextBtnClick( TNotifyUI* pNotify )
{
	if(_recordPageIndex >= _recordPageCount - 1)
		return true;

	_recordPageIndex++;

	UpdatePageInfo();
	Send(WebAppListPager_Event, this, this->_recordPageIndex, NULL);

	return true;
}

bool WebAppListPagerControl::OnPageBtnClick( TNotifyUI* pNotify )
{
	int index = pNotify->pSender->GetUserData();

	_recordPageIndex = index;

	UpdatePageInfo();
	Send(WebAppListPager_Event, this, this->_recordPageIndex, NULL);

	return true;
}

bool WebAppListPagerControl::OnGotoBtnClick( TNotifyUI* pNotify )
{
	Statistic_StatFunc(Statistic_ID_GoToPage);
	return GotoPage();
}

void WebAppListPagerControl::SetPageCount( int pageSize /*= 20*/ )
{
	_recordPageSize=pageSize;
}

void WebAppListPagerControl::SetPageIndex( int index/*=0*/ )
{
	if(index < 0 || index > _recordPageCount - 1)
		return;
	_recordPageIndex=index;
	UpdatePageInfo();
}

bool WebAppListPagerControl::OnKeyDown( TNotifyUI* pNotify )
{
	if(_pEdit == NULL || pNotify->wParam != VK_RETURN)
		return true;
	return GotoPage();
}

bool WebAppListPagerControl::GotoPage()
{
	int index = _wtoi(_pEdit->GetText());
	index--;
	if(index < 0 || index > _recordPageCount - 1)
	{
		_pEdit->SetText(CStrOperation::IntToWString(_recordPageIndex + 1).c_str());
	//	NoticeDialog(BaseUI::LoadStringW(L"web_toobigerpage"), ICON_WARNING, 2000);
		return true;
	}

	_recordPageIndex = index;
	if(CStrOperation::startsWith(_pEdit->GetText(), L"0"))
	{
		index++;
		_pEdit->SetText(CStrOperation::IntToWString(index).c_str());
		_pEdit->Resize();
		_pEdit->Invalidate();
	}
	UpdatePageInfo();
	Send(WebAppListPager_Event, this, this->_recordPageIndex, NULL);
	return true;
}

std::wstring WebAppListPagerControl::GetStyleID()
{
	return _T("WebAppPagerCtrl");
}

void WebAppListPagerControl::Init( CContainerUI* pContainer )
{
	BaseView::Init(pContainer);
}

