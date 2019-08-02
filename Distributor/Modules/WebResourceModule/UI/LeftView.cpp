#include "stdafx.h"
#include "LeftView.h"
#include "WebResourceModule.h"
LeftView::LeftView(CContainerUI* pParent)
{
	//Create(_id, pParent);
	_pNewUI = NULL;
	_pTree = NULL;
	_pAnimation = NULL;
	_pLabel = NULL;
	_nCurTreeIndex = 0;
	_bInit = false;
}

LeftView::~LeftView()
{
}

std::wstring LeftView::GetStyleID()
{
	return L"WebResource_View_Left";
}

void LeftView::OnCreate()
{
	_pTree      = (CTreeUI*)GetItem(L"webresource_left_layout.TreeContainer");
	_pAnimation = (CAnimationUI*)GetItem(_T("webresource_left_layout.Loading"));
	_pLabel     = (CLabelUI*)GetItem(_T("webresource_left_layout.LoadText"));
	_pNewUI     = (CControlUI*)GetItem(_T("webresource_left_layout.web_funNew"));

#if _DEBUG
	assert(_pTree);
	assert(_pAnimation);
	assert(_pLabel);
#endif

	this->ClearLeftTree();
}

void LeftView::Init(CContainerUI* pContainer)
{
	BaseView::Init(pContainer);

}

void LeftView::Show()
{
	BaseView::Show();

	if (_bInit)
	{
		return;
	}
	_bInit = true;
	//开始加载数据
	WebResourceInnserMsg pMsg;
	pMsg.type     = WebResourceMsg_LeftLoad;
	pMsg.lParam   = PLATFORM_Android;//平台
	pMsg.wParam   = Etype_Application;//应用软件
	pMsg.Send();

	_ePlatform     = ePhoneOS_Android_Net;

}

bool LeftView::OnWebTreeSelChanged(TNotifyUI* pNotify)
{

	CTreeNodeUI* pNode=dynamic_cast<CTreeNodeUI*>(_pTree->GetCurSel());
	if(NULL==pNode)
	{
		return false;
	}

	_nCurTreeIndex=this->FindCurIndex(pNode);

	NetNode* pNetNode=((NetNode*)pNode->GetUserData());
	if(NULL==pNetNode)
	{
		return false;
	}
	
	WebResourceInnserMsg MsgHide;
	MsgHide.type     = WebResourceMsg_HideTopSearch;
	if (pNetNode->searchUrl.empty())
	{
		MsgHide.wParam   = 1;
	}
	else
	{
		MsgHide.wParam   = 0;	
	}
	MsgHide.SendToUI();

	NetNode* pstNetNode = new NetNode;
	*pstNetNode = *pNetNode;

	WebResourceInnserMsg MsgUrl;
	MsgUrl.type     = WebResourceMsg_UrlChanged;
	MsgUrl.lParam   = (LPARAM)pstNetNode;
	MsgUrl.SendToUI();

	return true;
}

bool LeftView::PopJailBreakSite(NetSite& stNetSite)
{
	if (stNetSite.nodes.size()<=1)
	{
		return false;
	}

	NetNode stNetNode;
	bool    bRes = false;
	vector<NetNode>::iterator iter = stNetSite.nodes.begin();
	for (; iter!=stNetSite.nodes.end(); ++iter)
	{
		if (iter->nodeName==WebResourceModule::LoadString(L"WebPhoneOS_GenuineFree"))
		{
			stNetNode = (*iter);
			stNetSite.nodes.erase(iter);
			bRes = true;
			break;
		}
	}

	if (bRes)
	{
		stNetSite.nodes.insert(stNetSite.nodes.begin(), stNetNode);
		return true;
	}
	return false;
}

void LeftView::AddNode(NetSite& netSite)
{
	_pAnimation->SetVisible(false);
	_pLabel->SetVisible(false);
	CreateGroupTreeNode(_pTree, netSite);
}

void LeftView::OnFinish()
{
	_pAnimation->SetVisible(false);
	_pLabel->SetVisible(false);
}

int LeftView::FindCurIndex(CTreeNodeUI* pCurNode)
{
	if (NULL == pCurNode)
	{
		return 0;
	}
	//默认选中第一个节点
	CTreeNodeUI* pRoot = _pTree->GetRoot();
	if (NULL==pRoot)
	{
		return 0;
	}
	if(pRoot->num_child()>0)
	{
	
		for (int i=0; i<pRoot->num_child(); i++)
		{	
			CTreeNodeUI* pGroupNode=pRoot->get_child(i);
			if (NULL==pGroupNode)
			{
				continue;
			}
			if(pCurNode == pGroupNode)
			{
				return i;
			}
		}
	}

	return 0;
}

void LeftView::CreateGroupTreeNode(CTreeUI* pTree,NetSite& stNetSite)
{
	bool bRes = this->PopJailBreakSite(stNetSite);

	_pNewUI->SetVisible(bRes);

	CTreeNodeUI* pSecondGroup = NULL;
	for (vector<NetNode>::iterator item = stNetSite.nodes.begin(); item !=stNetSite.nodes.end(); ++item)
	{
		pSecondGroup=CreateChildTreeNode(*item, stNetSite.platform);
		if(!(*item).nodeName.empty())
			pTree->InsertItem(pSecondGroup);
	}

	if (_ePlatform == ePhoneOS_Android_Net && stNetSite.platform == ePhoneOS_Ios_Net)
	{
		_nCurTreeIndex++;
	}
	else if (_ePlatform == ePhoneOS_Ios_Net && stNetSite.platform == ePhoneOS_Android_Net&&_nCurTreeIndex>0)
	{
		_nCurTreeIndex--;
	}
	_ePlatform = stNetSite.platform;
	if(_pTree->GetCount()<=_nCurTreeIndex)
		_nCurTreeIndex--;

	_pTree->SelectItem(_nCurTreeIndex);

	pTree->Resize();
	pTree->Invalidate();
	Invalidate();
}

CTreeNodeUI* LeftView::CreateChildTreeNode(NetNode& stNetNode, NetEnumPhoneOS platForm)
{
	if(stNetNode.nodeName.empty())
		return NULL;
	CTreeNodeUI* pSecondGroup = new CTreeNodeUI();  
	pSecondGroup->SetId((stNetNode.nodeId).c_str());
	pSecondGroup->SetStyle(_T("Style_US_SingleTreeNode"));
	pSecondGroup->SetText(stNetNode.nodeName.c_str());
	pSecondGroup->SetAttribute(_T("txt.pos"),_T("8|34,0,0,0"));
	((CTreeNodeUI*)pSecondGroup)->SetAttribute(_T("DrawNode"),_T("1"));

	//91自己的站点要加PI参数
	tstring param=L"";    
	if(stNetNode.nodeUrl.find(L"?")==tstring::npos)
	{
		if (stNetNode.nodeUrl.length() > 0 && *(stNetNode.nodeUrl.rbegin()) == '/')
			stNetNode.nodeUrl=CStrOperation::trimRightW(stNetNode.nodeUrl,L"/");
		param=L"?";
	}

	stNetNode.initUrl = stNetNode.nodeUrl;

	NetNode* pNode=new NetNode();
	pNode->nodeId=stNetNode.nodeId;
	pNode->nodeName=stNetNode.nodeName;
	pNode->nodeIcon=stNetNode.nodeIcon;	
	pNode->nodeUrl=stNetNode.nodeUrl;
	pNode->initUrl=stNetNode.initUrl;
	pNode->searchUrl=stNetNode.searchUrl;
	pNode->isselect=stNetNode.isselect;
	pNode->ieid=stNetNode.ieid;
	pNode->tagicon=stNetNode.tagicon;
	pNode->isshow=stNetNode.isshow;
	pNode->isPartner=stNetNode.isPartner;
	pNode->statid=stNetNode.statid;
	pNode->strValue = stNetNode.strValue;

	pSecondGroup->SetUserData((UINT)pNode);  
	return pSecondGroup;
}

void LeftView::ClearLeftTree()
{
	_pTree->RemoveAll();
	_pTree->SetItemHeight(39);
	_pTree->SetSelectedHeight(39);
	_pTree->SetGroupHeight(39);

	_pAnimation->SetVisible(true);
	_pLabel->SetVisible(true);
}

void LeftView::SelectSearchNode(NetNode& stNetNode)
{
	int i=0;

	CTreeNodeUI* pRoot = _pTree->GetRoot();
	if (NULL==pRoot)
	{
		return;
	}

	for (; i<pRoot->num_child(); i++)
	{	
		CTreeNodeUI* pGroupNode=pRoot->get_child(i);
		if (NULL == pGroupNode)
		{
			continue;
		}
		NetNode* pNetNode=((NetNode*)pGroupNode->GetUserData());
		if(NULL==pNetNode)
		{
			continue;
		}

		if (pNetNode->nodeName==WebResourceModule::LoadString(L"WebPhoneOS_Search"))
		{
			//pGroupNode->SetUserData((UINT)pNetNode); 
			pNetNode->searchUrl = stNetNode.searchUrl;
			break;
		}
	}

	_pTree->SelectItem(i);
}