#include "stdafx.h"
#include "LeftView.h"
#include "Logic/DeviceController.h"

LeftView::LeftView(DeviceController* pController)
{
	_pController = pController;
}

LeftView::~LeftView()
{

}

std::wstring LeftView::GetStyleID()
{
	return L"Device_View_Left";
}

void LeftView::AddItem( void* key )
{
	CTreeUI* pTree = dynamic_cast<CTreeUI*>(GetItem(L"Tree"));
	if(pTree == NULL)
		return;

	CTreeNodeUI* node = new CTreeNodeUI(true);
	node->SetId(CStrOperation::IntToWString((int)DeviceModuleType_ConnecGuide).c_str());
	node->SetUserData((UINT)key);
	node->SetStyle(L"Style_US_GroupTreeNode");
	node->SetAttribute(_T("DrawNode"),_T("1"));
	node->SetText(L"正在连接中...");
	pTree->InsertItem(node);
	node->Expand(true);
	
	if(!pTree->GetCurSel())
	{
		pTree->SelectItem(node);

		DeviceInnerMsg msg;
		msg.type = Device_MsgType_TreeSelectChange;
		msg.wParam = (UINT)key;
		msg.lParam = atoi(CCodeOperation::ws2s(node->GetId()).c_str());
		msg.SendToUI();
	}

	pTree->Resize();
	pTree->Invalidate();

}

void LeftView::RemoveItem( void* key )
{
	CTreeUI* pTree = (CTreeUI*)GetItem(_T("Tree"));
	if (!pTree) 
		return;

	CTreeNodeUI* pTreeNode = FindConnectTreeNode(key);

	//删除节点后， 选择第一个与删除节点不一致的节点
	CTreeNodeUI* pSelect = NULL;
	int count = pTree->GetRoot()->num_child();
	for(int i = 0;i<count;i++)
	{
		CTreeNodeUI* node=(CTreeNodeUI*)pTree->GetRoot()->get_child(i);
		if(node != NULL && pTreeNode != node)
		{
			pSelect = node;
			break;;
		}
	}

	if(pTreeNode)
	{
		pTreeNode->SetUserData(NULL);
		pTree->RemoveItem(pTreeNode);
		pTree->Resize();
		pTree->Invalidate();
	}

	if(pSelect != NULL)
	{
		pTree->SelectItem(pSelect);

		DeviceInnerMsg msg;
		msg.type = Device_MsgType_TreeSelectChange;
		msg.wParam = pSelect->GetUserData();
		msg.lParam = atoi(CCodeOperation::ws2s(pSelect->GetId()).c_str());
		msg.PostToUI();
	}
}

void LeftView::UpdateItem( void* key, wstring deviceName, bool bCreateModules )
{
	CTreeUI* pTree = (CTreeUI*)GetItem(_T("Tree"));
	if (!pTree) 
		return;

	CTreeNodeUI* pTreeNode = FindConnectTreeNode(key);
	if(pTreeNode)
	{
		pTreeNode->SetId(CStrOperation::IntToWString((int)DeviceModuleType_DeviceInfo).c_str());
		pTreeNode->SetText(deviceName.c_str());

		int count = pTreeNode->num_child();
		if(bCreateModules && count <= 0)
		{
			CTreeNodeUI* appNode = new CTreeNodeUI(false);
			appNode->SetId(CStrOperation::IntToWString((int)DeviceModuleType_Application).c_str());
			appNode->SetUserData((UINT)key);
			appNode->SetStyle(L"Style_US_SingleTreeNode");
			appNode->SetAttribute(_T("DrawNode"),_T("1"));
			appNode->SetText(L"程序管理");
			appNode->SetVisible(true);
			pTree->InsertItem(appNode, pTreeNode);

			/*CTreeNodeUI* fileNode = new CTreeNodeUI(false);
			fileNode->SetId(CStrOperation::IntToWString((int)DeviceModuleType_FileManager).c_str());
			fileNode->SetUserData((UINT)key);
			fileNode->SetStyle(L"Style_US_SingleTreeNode");
			fileNode->SetAttribute(_T("DrawNode"),_T("1"));
			fileNode->SetText(L"文件管理");
			fileNode->SetVisible(true);
			pTree->InsertItem(fileNode, pTreeNode);

			CTreeNodeUI* backRestoreNode = new CTreeNodeUI(false);
			backRestoreNode->SetId(CStrOperation::IntToWString((int)DeviceModuleType_BackupAndRestore).c_str());
			backRestoreNode->SetUserData((UINT)key);
			backRestoreNode->SetStyle(L"Style_US_SingleTreeNode");
			backRestoreNode->SetAttribute(_T("DrawNode"),_T("1"));
			backRestoreNode->SetText(L"备份还原");
			backRestoreNode->SetVisible(true);
			pTree->InsertItem(backRestoreNode, pTreeNode);*/
		}

		pTree->Resize();
		pTree->Invalidate();
	}
}

void LeftView::ClearSubItem( void* key, wstring deviceName, bool bRecoveryMode )
{
	CTreeUI* pTree = (CTreeUI*)GetItem(_T("Tree"));
	if (!pTree) 
		return;

	CTreeNodeUI* pTreeNode = FindConnectTreeNode(key);
	if(pTreeNode)
	{
		if(!bRecoveryMode)
			pTreeNode->SetId(CStrOperation::IntToWString((int)DeviceModuleType_ConnecGuide).c_str());
		else
			pTreeNode->SetId(CStrOperation::IntToWString((int)DeviceModuleType_DeviceInfo).c_str());

		pTreeNode->SetText(deviceName.c_str());

		for(int i = pTreeNode->num_child()-1; i >= 0; i--)
		{
			CTreeNodeUI* node=(CTreeNodeUI*)pTreeNode->get_child(i);
			if(node != NULL)
				pTree->RemoveItem(node);
		}
		pTree->Resize();
		pTree->Invalidate();
	}
}

CTreeNodeUI* LeftView::FindConnectTreeNode( void* key )
{
	CTreeUI* pTree = dynamic_cast<CTreeUI*>(GetItem(L"Tree"));
	if(pTree == NULL)
		return NULL;

	CTreeNodeUI* pNode=pTree->GetRoot();
	int count= pNode->num_child();
	for(int i=0;i<count;i++)
	{
		CTreeNodeUI* node=(CTreeNodeUI*)pNode->get_child(i);
		if(node&&(node->GetUserData()==(UINT)key))
			return node;
	}
	return NULL;
}

void LeftView::OnCreate()
{
	CTreeUI* pTree= (CTreeUI*)GetItem(_T("Tree"));
	if(pTree)
	{
		pTree->SetItemHeight(39);
		pTree->SetSelectedHeight(39);
		pTree->SetGroupHeight(39);
	}
}

bool LeftView::OnTreeSelChanged( TNotifyUI* pNotify )
{
	CTreeUI* pTree= (CTreeUI*)GetItem(_T("Tree"));
	if(!pTree->GetCurSel())
		return true;

	DeviceInnerMsg msg;
	msg.type = Device_MsgType_TreeSelectChange;
	msg.wParam = pTree->GetCurSel()->GetUserData();
	msg.lParam = atoi(CCodeOperation::ws2s(pTree->GetCurSel()->GetId()).c_str());
	msg.SendToUI();

	return true;
}

bool LeftView::OnRefreshClick( TNotifyUI* pNotify )
{
	CStaticsticsMessage statMsg("10004");
	statMsg.Send();

	DeviceInnerMsg msg;
	msg.type = Device_MsgType_Refresh;
	msg.PostToUI();

	return true;
}

void LeftView::UpdateErrorText( void* key)
{
	CTreeUI* pTree = (CTreeUI*)GetItem(_T("Tree"));
	if (!pTree) 
		return;

	CTreeNodeUI* pTreeNode = FindConnectTreeNode(key);
	if(pTreeNode)
	{
		pTreeNode->SetId(CStrOperation::IntToWString((int)DeviceModuleType_ConnecGuide).c_str());
		pTreeNode->SetText(CommonUIFunc::LoadString(L"Device_Text_ConnectDeviceFailed"));
		pTree->Resize();
		pTree->Invalidate();
	}
}

void LeftView::UpdateRecoveryText( void* key )
{
	CTreeUI* pTree = (CTreeUI*)GetItem(_T("Tree"));
	if (!pTree) 
		return;

	CTreeNodeUI* pTreeNode = FindConnectTreeNode(key);
	if(pTreeNode)
	{
		pTreeNode->SetId(CStrOperation::IntToWString((int)DeviceModuleType_DeviceInfo).c_str());
		pTreeNode->SetText(CommonUIFunc::LoadString(L"Device_Text_RecoverMode"));
		pTree->Resize();
		pTree->Invalidate();
	}
}
