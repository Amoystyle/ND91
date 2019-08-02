#include "StdAfx.h"
#include "SimpleListItem.h"

CSimpleListItemUI::CSimpleListItemUI(void)
{INTERFUN;
	SetType(_T("SimpleListItem"));
	SetStyle(_T("SimpleListItem"));
	_lastshift=false;
	_lastshiftup=false;
}

CSimpleListItemUI::~CSimpleListItemUI(void)
{INTERFUN;
}

bool CSimpleListItemUI::IsSelect()
{INTERFUN;
	CListUI* pList = dynamic_cast<CListUI*>(GetParent());
	if (pList)
	{
		return __super::IsSelect()||pList->IsCheckItem(GetIndex());
	}
	return __super::IsSelect();
}

void CSimpleListItemUI::Select( bool bSelect /*= true*/, bool bRedraw /*= true*/ )
{INTERFUN;
	CListUI* pList = dynamic_cast<CListUI*>(GetParent());
	if(!pList)
		return;

	pList->CheckItem(bSelect, GetIndex(), true);	
}

bool CSimpleListItemUI::Event( TEventUI& event )
{INTERFUN;
	switch (event.Type)
	{
	case UIEVENT_BUTTONUP:
		{
			SHORT sCtrl = ::GetKeyState(VK_CONTROL);
			SHORT sShift = ::GetKeyState(VK_SHIFT);
			CListUI* pList = dynamic_cast<CListUI*>(GetParent());
			RETURN_TRUE_IF(!pList);
			bool bMulti = pList->IsMultiSelect();
			if (bMulti && sCtrl < 0 && sShift >= 0)//点击了ctrl
			{
				//return __super::Event(event);
			}
			else if (bMulti && sShift < 0 && sCtrl>=0)//点击了shift
			{
				vector<int> csindexs;
				bool lastup=false;
				for (int i=0;i<pList->GetCount();i++)
				{
					CSimpleListItemUI* item=(CSimpleListItemUI*)pList->GetItem(i);
					if(pList->IsCheckItem(i))
						csindexs.push_back(i);
					if(item->_lastshift)
					{
						lastup=item->_lastshiftup;
						item->_lastshift=false;
						item->_lastshiftup=false;
					}
				}
				if(!csindexs.empty())
				{
					int lastindex=*(csindexs.rbegin());
					_lastshiftup=lastindex>GetIndex();
					if(lastup!=_lastshiftup)
						lastindex=_lastshiftup?*(csindexs.begin()):*(csindexs.rbegin());
					else
						lastindex=lastindex>GetIndex()?lastindex:*(csindexs.begin());
					pList->SelectItem(pList->GetItem(lastindex));
				}
				_lastshift=true;
				return __super::Event(event);
			}

			bool sel = IsSelect();
			if(sel)
			{
				pList->CheckItem(false, GetIndex(), true);
			}
			else
			{
				pList->CheckItem(true, GetIndex(), true);
			}
			this->Invalidate();
			pList->SendNotify(UINOTIFY_CLICK, (WPARAM)sel, (LPARAM)this);
			//return __super::Event(event);
		}
		break;
	case UIEVENT_RBUTTONUP:
		{
			CListUI* pList = dynamic_cast<CListUI*>(GetParent());
			pList->SendNotify(UINOTIFY_RCLICK, (WPARAM)(&event.ptMouse), (LPARAM)this);
		}
		break;
	default:
		return __super::Event(event);
	}
	return true;
}
