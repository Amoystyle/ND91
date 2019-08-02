#include "stdafx.h"
#include "CCompositeTabUI.h"
#include "TabItem.h"
CCompositeTabUI::CCompositeTabUI()
{

}

CCompositeTabUI::~CCompositeTabUI()
{
    _pTabItemList.clear();
}

void CCompositeTabUI::RegisterComponent( CContainerUI* pParentCtn, LPCTSTR id, CTabItem* pTabItem, int nGroupID )
{
    //pTabItem->SetContainerID(containerID);
    pTabItem->SetParentCtn(pParentCtn);
    pTabItem->SetID(id);
    pTabItem->SetXmlID(id);
    pTabItem->SetGroupID(nGroupID);
    _pTabItemList.push_back(pTabItem);
}

void CCompositeTabUI::RegisterComponent( CContainerUI* pParentCtn, LPCTSTR id, LPCTSTR xmlid, CTabItem* pTabItem, int nGroupID/*=0*/ )
{
    //pTabItem->SetContainerID(containerID);
    pTabItem->SetParentCtn(pParentCtn);
    pTabItem->SetID(id);
    pTabItem->SetXmlID(xmlid);
    pTabItem->SetGroupID(nGroupID);
    _pTabItemList.push_back(pTabItem);
}

// bool CCompositeTabUI::CreateComponent( LPCTSTR id, CContainerUI* pParentCtn, bool bVisible/*=true*/ )
// {
//     if(!pParentCtn)
//         return false;
//     SetVisible(bVisible);
//     bool ret = Create(id, pParentCtn);
//     MainDlg::GetInstance()->AddNotifier(this);     
//     return ret;
// }

void CCompositeTabUI::OnCreate()
{
    LoadRegisterComponent();
}

void CCompositeTabUI::LoadRegisterComponent()
{
    for (vector<CTabItem*>::iterator it = _pTabItemList.begin(); it != _pTabItemList.end(); it++)
    {
        CTabItem* pItem = *it;
        pItem->CreateUI();
    }
}

CTabItem* CCompositeTabUI::GetComponent( tstring id )
{
    for (vector<CTabItem*>::iterator it = _pTabItemList.begin(); it != _pTabItemList.end(); it++)
    {
        CTabItem* pItem = *it;
        if (pItem->GetID() == id)
            return pItem;
    }
    return NULL;
}

void CCompositeTabUI::HideAllComponent( int nComponentGroup/*=0*/ ) /* -1代表操作所有的Component，下同 */
{
    for (vector<CTabItem*>::iterator it = _pTabItemList.begin(); it != _pTabItemList.end(); it++)
    {
        CTabItem* pItem = *it;
        if (pItem->GetGroupID() == nComponentGroup || nComponentGroup == -1)
            pItem->SetItemVisible(false);
    }
}

void CCompositeTabUI::RemoveComponent( tstring id )
{
    for (vector<CTabItem*>::iterator it = _pTabItemList.begin(); it != _pTabItemList.end(); it++)
    {
        CTabItem* pItem = *it;
        if (pItem->GetID() == id)
        {
            _pTabItemList.erase(it);
            break;
        }
    }
}

void CCompositeTabUI::RemoveComponent( CTabItem* pTabItem )
{
    for (vector<CTabItem*>::iterator it = _pTabItemList.begin(); it != _pTabItemList.end(); it++)
    {
        CTabItem* pItem = *it;
        if (pItem == pTabItem)
        {
            _pTabItemList.erase(it);
            break;
        }
    }
}

CTabItem* CCompositeTabUI::ShowComponent( tstring id, int nComponentGroup/*=0*/ )
{
    CTabItem* pGotItem = NULL;
    for (vector<CTabItem*>::iterator it = _pTabItemList.begin(); it != _pTabItemList.end(); it++)
    {
        CTabItem* pItem = *it;
        if (nComponentGroup >= 0 && pItem->GetGroupID() != nComponentGroup)
            continue;
        if (pItem->GetID() == id) 
        {
            pItem->SetItemVisible(true);
            pGotItem = pItem;
        }
        else
        {
            pItem->SetItemVisible(false);
        }
    }
    return pGotItem;
}

void CCompositeTabUI::ShowComponent( CTabItem* pTabItem, int nComponentGroup/*=0*/, bool bShow/*=true*/ )
{
    if (!pTabItem) return;
    if (bShow) HideAllComponent(nComponentGroup);
    pTabItem->SetItemVisible(bShow);
}

CTabItem* CCompositeTabUI::GetCurrentComponent( int nComponentGroup/*=0*/ )
{
    for (vector<CTabItem*>::iterator it = _pTabItemList.begin(); it != _pTabItemList.end(); it++)
    {
        CTabItem* pItem = *it;
        if (nComponentGroup >= 0 && pItem->GetGroupID() != nComponentGroup)
            continue;
        if (pItem->IsVisible())
            return pItem;
    }
    return NULL;
}
