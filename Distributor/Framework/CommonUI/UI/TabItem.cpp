#include "stdafx.h"
#include "TabItem.h"

CTabItem::CTabItem()
{
    _pParentCtn = NULL;
}

CTabItem::~CTabItem()
{

}

void CTabItem::CreateUI( CContainerUI* pParentCtn, bool bVisible /*= true*/ )
{
    if( ! pParentCtn) return ;
    Create(_xmlid.c_str(), pParentCtn);
    //MainDlg::GetInstance()->AddNotifier(this);     
    SetVisible(bVisible);
}

void CTabItem::CreateUI( tstring id, CContainerUI* pParentCtn, bool bVisible /*= true*/ )
{
    if( ! pParentCtn) return ;
    Create(id.c_str(), pParentCtn);
    //MainDlg::GetInstance()->AddNotifier(this);     
    SetVisible(bVisible); 
}

void CTabItem::CreateUI( bool bVisible /*= true*/ )
{
    return CreateUI(_pParentCtn, bVisible);
}

void CTabItem::SetItemVisible( bool bShow )
{
    SetVisible(bShow);
    if (bShow)
    {
        Resize();
        Invalidate(true);
        OnShowComponent();
    }
    else
    {
        OnHideComponent();
    }
}
