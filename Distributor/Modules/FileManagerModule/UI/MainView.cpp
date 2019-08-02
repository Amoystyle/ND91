#include "StdAfx.h"
#include <windows.h>
#include "MainView.h"
#include "Logic/virtualdirdata.h"
#include "Logic/virtualnamedirdata.h"
#include "UIWindow.h"
#include <string.h>
#include "common/Statistics/StatisticsMessageDefine.h"

UfslibHandle _pUfsHandle;

CTreeNodeUIEx::CTreeNodeUIEx(bool bGroup) 
: CTreeNodeUI(bGroup)
{
}

void CTreeNodeUIEx::CalcControl(RECT & rectCalc)
{
    RECT rcCalc = {0, 0, 0, 0};
    TextStyle* pStyle = GetTextStyle(_T("txt"));
    if (pStyle) pStyle->GetFontObj()->CalcText(GetWindow()->GetPaintDC(), rcCalc, GetText());
    int textwidth = rcCalc.right-rcCalc.left;

    rectCalc.left = 0;
    rectCalc.top = 0;
    rectCalc.right = textwidth+GetInset().left+40;
    rectCalc.bottom = 0;
}

MainView::MainView(wstring deviceId)
{
	_deviceId = deviceId;
    _pTree = NULL;
    _pList = NULL;
    _bCut        = false;
    _isFinish    = true;
    _lastEditPath = L"/";
	_appTreeNode = NULL;
	_check_exit = false;
	_model = new FileManagerModel(this,_deviceId);
    International::SetString(FILE_COPY_NAME, L"����_");
}

MainView::~MainView(void)
{
	Release();
}

// �б�������
bool __stdcall CompareFileNode(CListItemUI* pControlA, CListItemUI* pControlB, int nCol, bool exchange)
{

    RETURN_VALUE_IF( ! pControlA || ! pControlB, ! exchange);
    NodeData* pNodeA = (NodeData*) pControlA->GetUserData();
    NodeData* pNodeB = (NodeData*) pControlB->GetUserData();
    RETURN_VALUE_IF( ! pNodeA || ! pNodeB, ! exchange );

    if ( pControlA->GetCount() <= 2 )
        return !exchange;
    if ( pControlA->GetCount() <= 2 )
        return exchange;

    if ( pNodeA->IsDir() && ! pNodeB->IsDir() )
        return ! exchange;
    else if ( ! pNodeA->IsDir() && pNodeB->IsDir() )
        return exchange;

    switch (nCol)
    {
    case 1:     // ����
		{
			wstring tempA = CStrOperation::trimLeftW(pNodeA->_displayName, L" ");
			wstring tempB = CStrOperation::trimLeftW(pNodeB->_displayName, L" ");
			return lstrcmpi(tempA.c_str(), tempB.c_str()) < 0;
		}
    case 2:     // ��С
        return pNodeA->_size < pNodeB->_size;

    case 4:     // ʱ��
        return pNodeA->_modifyTime < pNodeB->_modifyTime;

    default:
        return !exchange;
    }
}

// ��������
bool CompareTreeNode(CTreeNodeUI* pNodeA,CTreeNodeUI* pNodeB)
{
    return true;//StrCmp(pNodeA->GetText(), pNodeB->GetText()) >= 0;
}

void MainView::OnCreate()
{
    SetVisible(false);

    _pList = (CListUI*)GetItem(_T("FM_buttom_right.FM_file_lsit.FM_list"));
    RETURN_IF(! _pList);
    _pList->SetSortCallBack(CompareFileNode);
    _pList->ModifyControlFlags(UIFLAG_ENABLE_DRAG|UIFLAG_ENABLE_DROP, 0);

	_model->CreateDirThread();	//	Ԥ����dirĿ¼����Ϊ�״δ���dirʱ���ر��

	LoadData();

}


bool MainView::OnKeyIn( TNotifyUI* pNotify )
{
	// ͳ�ƹ���
	CStaticsticsMessage msg(Statistic_ID_SearchFilter);
	msg.Send();

    CControlUI* pControl = GetItem(_T("FM_top.FM_top_right.FM_search_edit"));
    if ( pControl )
    {
        _filterKey = pControl->GetText();
		if (_filterKey.length() != 0)		//����ո���Ч
			FilteList();
		else
			OnRefresh(NULL);
        

        CControlUI* pButton = GetItem(_T("FM_top.FM_top_right.FM_clear"));
        if ( pButton ) pButton->SetVisible( ! _filterKey.empty());
    }
    return true;
}

bool MainView::OnClearSearch( TNotifyUI* pNotify )
{
    _filterKey.clear();
    CControlUI* pControl = GetItem(_T("FM_top.FM_top_right.FM_search_edit"));
    if (pControl) pControl->SetText(L"");
	CControlUI* pButton = GetItem(_T("FM_top.FM_top_right.FM_clear"));
	pButton->SetVisible(false);
	OnKeyIn(NULL);
    return true;
}

bool MainView::OnGoto( TNotifyUI* pNotify )
{
    CControlUI* pControl = GetItem(_T("FM_top.FM_top_left.FM_path"));
    if ( pControl )
    {
        Goto(pControl->GetText());
    }
    return true;
}

bool MainView::OnPathComplete( TNotifyUI* pNotify )
{
    if ( pNotify->wParam == VK_RETURN )
    {
        OnGoto(NULL);
        return true;
    }
    return false;
}

void MainView::SetPath(wstring path)
{
    CControlUI* pControl = GetItem(_T("FM_top.FM_top_left.FM_path"));
    if ( pControl )
    {
        pControl->SetText(path.c_str());
        _lastEditPath = path;
    }
}

bool MainView::OnGoPrev( TNotifyUI* pNotify )
{
    _model->GoPrev();
    return true;
}

bool MainView::OnGoNext( TNotifyUI* pNotify )
{
    _model->GoNext();
    return true;
}

bool MainView::OnGoParent( TNotifyUI* pNotify )
{
    RETURN_FALSE_IF( ! _pTree );

	DirData *pData = GetCurrentFolder();
	RETURN_FALSE_IF(!pData);
    RETURN_FALSE_IF(pData->IsRoot());

    CTreeNodeUIEx* pCurrentNode = dynamic_cast<CTreeNodeUIEx*>(_pTree->GetCurSel());
    RETURN_FALSE_IF( ! pCurrentNode );

    CTreeNodeUIEx* pParentNode = (CTreeNodeUIEx*)pCurrentNode->get_parent();
    RETURN_FALSE_IF( ! pParentNode );

    _pTree->SelectItem(pParentNode);
    _pTree->EnsureVisible(pParentNode);

    NodeData* pNode = (NodeData*)pParentNode->GetMyData();
    RETURN_FALSE_IF( ! pNode );

    _model->AddHistory(pNode->GetFullPath());
    return true;
}

CTreeNodeUIEx* MainView::CreateTreeNode(LPCTSTR lpszText)
{
	CTreeNodeUIEx* pNode = new CTreeNodeUIEx(true);
	pNode->SetText(lpszText);
	pNode->CheckVisible(false);
	pNode->SetStyle(_T("FileManagerUI_TreeNode"));
	pNode->SetUserData(false);  //  δ��ȡ�ӽڵ�
    return pNode;
}

void MainView::OnInit(DirData* pRootDir)
{

    RETURN_IF(! pRootDir);

    _pTree = (CTreeUI*)GetItem(_T("FM_buttom_left.FM_tree"));
    RETURN_IF(! _pTree);

    _pList = (CListUI*)GetItem(_T("FM_buttom_right.FM_file_lsit.FM_list"));
    RETURN_IF(! _pList);

	CTreeNodeUIEx* pRootNode = NULL;
	VirtualDirData * pQuickdir = dynamic_cast<VirtualDirData *>(pRootDir);
	if (pQuickdir)
	{
		pRootNode = CreateTreeNode(CommonUIFunc::LoadString(L"FM_Quick"));
		pRootNode->SetAttribute(_T("type.image"), _T("#FileManagerUI_quick"));

	}
	else
		pRootNode = CreateTreeNode(CommonUIFunc::LoadString(L"FM_FileSystem"));
	
	RETURN_IF(! pRootNode);

	pRootNode->SetMyData(pRootDir);

    _pTree->InsertItem(pRootNode);
	_pTree->SelectItem(1);

	//���Init�����߳��е��ã� ��������ط������������ڵ��޷�չ����bug���ǳ����ٵ��ü������¼��л��ļ��ܺ�����ģ��ʱ��
	//�޸���̫���ݲ�����
	if (pQuickdir)
		pRootNode->Expand(true);
	else if (_pTree->GetCount() == 1)
		pRootNode->Expand(true);
}

bool MainView::OnExpandTree( TNotifyUI* pNotify )
{
    CTreeNodeUIEx* pNode = (CTreeNodeUIEx*)pNotify->lParam;
    RETURN_TRUE_IF( ! pNode );
    RETURN_TRUE_IF( pNode->GetUserData() ); // �����ӽڵ������򷵻�

    DirData* pDir = GetCurrentFolder(pNode);
    RETURN_FALSE_IF( ! pDir );

	if ( ! pDir->HasSubDir() )
	{        // ����Ŀ¼����ʾ������ͼ��
		pNode->SetAttribute(_T("icon.image"), _T(""));
	}
	else
	{
		deque<NodeData*> childs = pDir->GetChildrens();
		AddChildsToTree(pNode, &childs);
	}

 
    pNode->SetUserData(true); // �Ѷ�������Ҫ��ȡ�ӽڵ�
    

    return true;
}

bool MainView::OnSelectTree( TNotifyUI* pNotify )
{
    CTreeNodeUIEx* pTreeNode = (CTreeNodeUIEx*)pNotify->lParam;

	ISVirtualNode isVirtual = SelectCurTreeNode();
	if (isVirtual == Virtual_RootNode || isVirtual == Virtual_Node)
	{
		_model->SetHistoryEnable(false);
		EnableButton(false,false,true);
	}
	else
	{
		_model->SetHistoryEnable(true);
		EnableButton(true,true,true);
	}

    EnableListButton(isVirtual != Virtual_RootNode);

    DirData* pDir = GetCurrentFolder(pTreeNode);
    RETURN_TRUE_IF( ! pDir );
	
	// ͳ�ƹ���
	wstring NodeName = pTreeNode->GetText();
	if ( NodeName == CommonUIFunc::LoadString(L"FM_flash_Disk") )
	{
		CStaticsticsMessage msg(Statistic_ID_MobileUdisk);msg.Send();
	}
	else if ( NodeName == CommonUIFunc::LoadString(L"FM_SDcard") )
	{
		CStaticsticsMessage msg(Statistic_ID_MemoryCard);msg.Send();
	}
	else if ( NodeName == CommonUIFunc::LoadString(L"FM_mydocument") )
	{
		CStaticsticsMessage msg(Statistic_ID_MyDocuments);msg.Send();
	}
	else if ( NodeName == CommonUIFunc::LoadString(L"FM_photo") )
	{
		CStaticsticsMessage msg(Statistic_ID_Picture);msg.Send();
	}
	else if ( NodeName == CommonUIFunc::LoadString(L"FM_backup") )
	{
		CStaticsticsMessage msg(Statistic_ID_VoiceMemo);msg.Send();
	}
	else if ( NodeName == CommonUIFunc::LoadString(L"FM_userapp") )
	{
		CStaticsticsMessage msg(Statistic_ID_UserAppList);msg.Send();
	}
	else if ( NodeName == CommonUIFunc::LoadString(L"FM_cydiaapp") )
	{
		CStaticsticsMessage msg(Statistic_ID_CydiaAppInstall);msg.Send();
	}
	else
	{;}


    
	
	
	
	deque<NodeData*> childs = pDir->GetChildrens();
	if (childs.size() != 0 && childs.at(0)->_name == childs.at(0)->_displayName)
	{
		_model->SetRestortAppName(true);		//���ݱ����ã�����ˢ��app����
		_model->RestoreAppNodeName(pDir);
	}


	VirtualDirData * pQuickRootdir = dynamic_cast<VirtualDirData *>(pDir);
	if (pQuickRootdir)	//����������ڵ�Ϊ�հ�
		childs.clear();

	AddChildsToList(&childs, GetParentDir(pTreeNode));

    SetPath(pDir->GetFullPath());

    _model->AddHistory(pDir->GetFullPath());
    _model->RefreshSpaceInfo(pDir->GetFullPath());

    if ( ! pDir->HasSubDir() )
    {        // ����Ŀ¼����ʾ������ͼ��
        pTreeNode->SetAttribute(_T("icon.image"), _T(""));
        pTreeNode->SetUserData(true);
    }
    


	CControlUI* pControl = GetItem(_T("FM_top.FM_top_right.FM_search_edit"));
	if ( pControl )
	{
		_filterKey = pControl->GetText();
		if (_filterKey.length() != 0)		//����ո���Ч
			FilteList();

		CControlUI* pButton = GetItem(_T("FM_top.FM_top_right.FM_clear"));
		if ( pButton ) pButton->SetVisible( ! _filterKey.empty());
	}
    return true;
}

bool MainView::OnDoubleClickTree( TNotifyUI* pNotify )
{
	CTreeNodeUIEx* pNode = (CTreeNodeUIEx*)_pTree->GetCurSel();
    RETURN_TRUE_IF( ! pNode );

    pNode->Expand( ! pNode->IsExpand() );
    return true;
}

void MainView::AddChildsToTree(CTreeNodeUIEx* pCurrentNode,
                                    deque<NodeData*>* pChilds)
{
    RETURN_IF(! _pTree);
    RETURN_IF(! pCurrentNode);

	bool sort_flag = true;
    deque<NodeData*>::iterator it;
    for( it = pChilds->begin(); it != pChilds->end(); ++it )
    {
        NodeData* pNode = *it;
       
        // �����ӽڵ�
		VirtualNameDirData *data = dynamic_cast<VirtualNameDirData*>(pNode);

 		CTreeNodeUIEx* pTreeNode = NULL;
		if (data)
		{
			pTreeNode = CreateTreeNode(data->_displayName.c_str());
			pTreeNode->SetMyData(data);

			pTreeNode->SetAttribute(_T("type.image"), data->GetLogo().c_str());

			if (data->_name == _T("Applications"))
				_appTreeNode = pTreeNode;

			sort_flag = false;
		}
		else
		{
			if ( ! pNode->IsDir() )
				continue;


			pTreeNode = CreateTreeNode(pNode->_displayName.c_str());
			pTreeNode->SetMyData(pNode);

			// LinkĿ¼��ͼ�겻һ��
			if ( pNode->_type == NodeData::LINK )
				pTreeNode->SetAttribute(_T("type.image"), _T("#FileManagerUI_FolderLink"));
		}
  
       _pTree->InsertItem(pTreeNode, pCurrentNode);
    }
	NodeData* pParentNode = (NodeData*)pCurrentNode->GetMyData();
	RETURN_IF(!pParentNode);

	if (sort_flag && pParentNode->_name != _T("Applications"))
	    _pTree->Sort(CompareTreeNode, pCurrentNode);
}

void MainView::AddChildsToList(deque<NodeData*>* pChilds, DirData* pParentDir)
{
    RETURN_IF( ! _pList );
    ClearList();

	ISVirtualNode isVirtual = SelectCurTreeNode();	//���ؿ����������ʾ��һ��
	if (isVirtual != Virtual_RootNode && isVirtual != Virtual_Node &&pParentDir ) // ��ʾ����һ��Ŀ¼��
	{
		CControlUI* pLine = new CListContainerItemUI();
		pLine->SetUserData((UINT)pParentDir);
		pLine->Select(true);

		pLine->Add(CreateIconItem(pParentDir));

		CControlUI* pItem = new CLabelUI();
		pItem->SetStyle(_T("Label"));
		pItem->SetText(CommonUIFunc::LoadString(L"FM_ParentDir"));
		SetPosition(pItem);
		pLine->Add(pItem);

		_pList->Add(pLine);
	}

    deque<NodeData*>::iterator it;
    for( it = pChilds->begin(); it != pChilds->end(); ++it )
    {
        CControlUI* pLine = new CListContainerItemUI();
        pLine->SetUserData((UINT)*it);
        _pList->Add(pLine);

        pLine->Add(CreateIconItem(*it));
        pLine->Add(CreateNameItem(*it));
        pLine->Add(CreateSizeItem(*it));
        pLine->Add(CreateTypeItem(*it));
        pLine->Add(CreateTimeItem(*it));
    }

    _pList->Resize();
    if ( _pList->GetSortCol() == -1 )
        _pList->SetSortCol(1);

	//Ӧ�ó�������
	if (_pTree->GetCurSel()->GetText() != CommonUIFunc::LoadString(_T("FM_userapp")))
		_pList->Sort();

    _pList->Invalidate();
}

CControlUI* MainView::CreateIconItem(NodeData* pNode)
{
    CControlUI* pItem = new CControlUI();
    pItem->SetInitPos(3,-1,20,17,9);
    pItem->SetStyle(L"Picture");

    switch( pNode->_type )
    {
    case NodeData::DIR:
        pItem->SetAttribute(L"bk.image", _T("#FileManagerUI_Folder"));
        break;

    case NodeData::FILE:
        pItem->SetAttribute(L"bk.image", _T("#FileManagerUI_File"));
        break;

     case NodeData::LINK:
         pItem->SetAttribute(L"bk.image",
             pNode->IsDir() ? _T("#FileManagerUI_FolderLink") : _T("#FileManagerUI_Link"));

         break;

    default:
        pItem->SetAttribute(L"bk.image", _T(""));
        break;
    }

    return pItem;
}

CControlUI* MainView::CreateNameItem( NodeData* pNode)
{
    CControlUI* pItem = new CLabelUI();
    pItem->SetStyle(_T("Label"));
	pItem->SetText( pNode->_displayName.c_str() );
    SetPosition(pItem);
    return pItem;
}

CControlUI* MainView::CreateSizeItem( NodeData* pNode )
{
    CControlUI* pItem = new CLabelUI();
    pItem->SetStyle(_T("Label"));
    
    if ( ! pNode->IsDir() )
        
    {
        long size = pNode->_size;
        pItem->SetText( CStrOperation::FormatFileSize((__int64)size).c_str() );
    }

    SetPosition(pItem);
    pItem->SetAttribute(_T("txt.halign"), _T("left"));
    return pItem;
}

CControlUI* MainView::CreateTypeItem( NodeData* pNode )
{
    CControlUI* pItem = new CLabelUI();
    pItem->SetStyle(_T("Label"));
    pItem->SetText(pNode->IsDir() ? 
        CommonUIFunc::LoadString(L"FM_Dir"):
        CommonUIFunc::LoadString(L"FM_File"));
    SetPosition(pItem);
    return pItem;
}

CControlUI* MainView::CreateTimeItem( NodeData* pNode )
{
    CControlUI* pItem = new CLabelUI();
    pItem->SetStyle(_T("Label"));

    time_t when = pNode->_modifyTime;
    if ( 0 == when )
        when = pNode->_createTime;

    struct tm t;
    gmtime_s(&t, &when);

    wchar_t buffer[32];
    swprintf_s(buffer, 32, L"%04d-%02d-%02d %02d:%02d:%02d",
        t.tm_year + 1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);

    pItem->SetText( buffer );
    SetPosition(pItem);

    return pItem;
}


bool MainView::GotoChild(wstring path)
{
	DirData* pDir = GetCurrentFolder();
	RETURN_FALSE_IF(! pDir);

	NodeData* pNode = pDir->findChildrenbyName(path);
	RETURN_FALSE_IF(! pNode);


	CTreeNodeUIEx* pParentNode = (CTreeNodeUIEx*)_pTree->GetCurSel();		//���ҵ�ǰѡ�е����ڵ�
	if (pParentNode)
		pParentNode->Expand(true);    // ����OnExpandTree()������ӽڵ�


	
	CTreeNodeUIEx* pTreeNode=NULL ;//= (CTreeNodeUIEx*)_pTree->GetCurSel();

	int childCount = pParentNode->num_child();
	for(int i = 0; i < childCount; ++i )
	{
		CTreeNodeUIEx* pChild = (CTreeNodeUIEx*)pParentNode->get_child(i);
		if ( pChild )
		{
			NodeData* pChildNode = (NodeData*) pChild->GetMyData();

			if ( pChildNode->_name == pNode->_name )
				pTreeNode =  pChild;//�ҵ���Ҫ�����ڵ�
		}
	}
	if ( pTreeNode )
	{
		SetPath(path);
		_pTree->SelectItem(pTreeNode);  // ����OnSelectTree()�����list��
		_pTree->EnsureVisible(pTreeNode);

		pTreeNode->Expand(true);    // ����OnExpandTree()������ӽڵ�
		return true;

	}
	else
	{
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_NotFound"));
		return false;
	}
}

bool MainView::Goto(wstring path)
{
	MyTrim(path);
    NodeData* pNode = _model->GetNode(path);
    if ( ! pNode )
    {
        SetPath(_lastEditPath);
		return false;
	}
    bool isDir = pNode->IsDir();
	
     CTreeNodeUIEx* pTreeNode = SelectTreeNode(isDir ?
         dynamic_cast<DirData*>(pNode) : pNode->GetParent());

    if ( pTreeNode )
    {
       SetPath(path);
        _pTree->SelectItem(pTreeNode);  // ����OnSelectTree()�����list��
                                        // AddHistory �� RefreshSpaceInfo
        _pTree->EnsureVisible(pTreeNode);
        if( isDir )
        {
            pTreeNode->Expand(true);    // ����OnExpandTree()������ӽڵ�
             return true;
        }
        else
        {
            return SelectListLine(pNode);
        }
    }
    else
    {
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_NotFound"));
        return false;
    }
}

CTreeNodeUIEx* MainView::SelectTreeNode( DirData* pNode )
{
    RETURN_NULL_IF( !pNode );

    if ( pNode->_name.empty() )
    {
		ISVirtualNode isVirtual = SelectCurTreeNode();
		if ((isVirtual == Node || isVirtual == Node_Error) && (_pTree->GetRoot()->get_child(1)) == NULL)
			return (CTreeNodeUIEx*)_pTree->GetRoot()->get_child(0);
		
		else
			return (CTreeNodeUIEx*)_pTree->GetRoot()->get_child(1);

    }
    else
    {
        CTreeNodeUIEx* pParentNode = SelectTreeNode(pNode->GetParent());
        RETURN_NULL_IF( !pParentNode );
        pParentNode->Expand(true);

        int childCount = pParentNode->num_child();
        for(int i = 0; i < childCount; ++i )
        {
            CTreeNodeUIEx* pChild = (CTreeNodeUIEx*)pParentNode->get_child(i);
            if ( pChild )
            {
                NodeData* pChildNode = (NodeData*) pChild->GetMyData();

                if ( pChildNode->_name == pNode->_name )
                    return pChild;
            }
        }

        return NULL;
    }
}

bool MainView::SelectListLine( NodeData* pNode )
{
    RETURN_FALSE_IF( ! _pList );
    int lineCount = _pList->GetCount();

    for(int i = 0 ; i < lineCount; i++ )
    {
        CControlUI* pLine = _pList->GetItem(i);
        NodeData* pLineNode = (NodeData*) pLine->GetUserData();
        if ( pLineNode->_name == pNode->_name )
        {
            _pList->SelectItem(pLine);
            _pList->EnsureVisible(pLine);
			pLine->SetFocus();
            return true;
        }
    }

    return false;
}

void MainView::EnableButton( bool enablePrevButton, bool enableNextButton, bool enableParentButton )
{
    CControlUI* pControl;
    if ( pControl  = GetItem(_T("FM_top.FM_top_left.FM_pre")) )
        pControl->SetEnabled(enablePrevButton);

    if ( pControl  = GetItem(_T("FM_top.FM_top_left.FM_next")) )
        pControl->SetEnabled(enableNextButton);

    if ( pControl  = GetItem(_T("FM_top.FM_top_left.FM_parent")) )
        pControl->SetEnabled(enableParentButton);

    if ( pControl  = GetItem(_T("FM_top.FM_top_left")) )
        pControl->Invalidate();
}

bool MainView::OnAddFolder( TNotifyUI* pNotify )
{
	// ͳ�ƹ���
	CStaticsticsMessage msg(Statistic_ID_CreateDir);msg.Send();

    DirData* pDir = GetCurrentFolder();
    RETURN_FALSE_IF( ! pDir );


    wstring new_name = _model->AddFolder(pDir) ;
	if (new_name.length() == 0)
	{
		return true;
	}


	CTreeNodeUIEx * pNode = (CTreeNodeUIEx *)_pTree->GetCurSel();	
	if (pNode)
	{
		pNode->SetAttribute(_T("icon.image"), _T("#FileManagerUI_defgroupnode"));
		pNode->SetUserData(true);
	}
	Refresh();

	int lineCount = _pList->GetCount();

	for(int i = 0 ; i < lineCount; i++ )
	{
		CControlUI* pLine = _pList->GetItem(i);
		NodeData* pLineNode = (NodeData*) pLine->GetUserData();
		if ( pLineNode->_name == new_name )
		{
			_pList->SelectItem(pLine);
			_pList->EnsureVisible(pLine);
			pLine->SetFocus();
			break;
		}
	}



    
    return true;
}

bool MainView::OnUploadFile( TNotifyUI* pNotify )
{
	// ͳ�ƹ���
	CStaticsticsMessage msg(Statistic_ID_UpLoadFile);msg.Send();

    DirData* pDir = GetCurrentFolder();
    RETURN_FALSE_IF( ! pDir );

	vector<wstring> files = BaseOperation::MultiFileDialog(Singleton::GetMainWnd());
    RETURN_TRUE_IF(files.empty());
	RETURN_TRUE_IF(_check_exit);
    
    _model->UploadFile(&files, pDir);
    return true;
}

bool MainView::OnUploadDir( TNotifyUI* pNotify )
{
	// ͳ�ƹ���
	CStaticsticsMessage msg(Statistic_ID_UpLoadDir);msg.Send();

    DirData* pDir = GetCurrentFolder();
    RETURN_FALSE_IF( ! pDir );

	wstring dir = BaseOperation::DirDialog(Singleton::GetMainWnd());
    RETURN_FALSE_IF( dir.empty() );
	RETURN_TRUE_IF(_check_exit);

    _model->UploadDir(dir, pDir);
    return true;
}

bool MainView::OnDelete( TNotifyUI* pNotify )
{
	// ͳ�ƹ���
	CStaticsticsMessage Statisticsmsg(Statistic_ID_DeleteFile);Statisticsmsg.Send();

    RETURN_TRUE_IF( ! _pTree );

    // �����ﴴ������Model���߳����ͷ�
    vector<NodeData*>* pDeleteNodes = new vector<NodeData*>;
    GetSelectNodes(pDeleteNodes);
	if (pDeleteNodes->empty())
	{
		SAFE_DELETE(pDeleteNodes);
		return true;
	}

    wstring msg;
    DirData* pDir = GetCurrentFolder();
	wstring sdcard = _model->GetSDCardPath();
	DEVICE_OS_TYPE EosType = _model->GetDeviceOsType();

	const wchar_t *isSdcard = NULL;
	if (pDir&&Android==EosType&&!sdcard.empty())
		isSdcard = wcsstr(pDir->GetFullPath().c_str(),sdcard.c_str());

	if (pDir&&Ios==EosType)
		isSdcard = wcsstr(pDir->GetFullPath().c_str(),sdcard.c_str());

	if (pDir && !isSdcard)
    {
        msg.assign(CommonUIFunc::LoadString(L"FM_DeleteWarning"));
    }

    const wchar_t* pTemplate = CommonUIFunc::LoadString(L"FM_DeletConfirm");
    wchar_t buffer[MAX_PATH];
    swprintf(buffer, MAX_PATH, pTemplate, pDeleteNodes->size());
    msg.append(buffer);

	if (! CommonUIFunc::ConfirmDialog(msg.c_str(), CommonUIFunc::LoadString(L"FM_DeletePrompt")))
	{
		SAFE_DELETE(pDeleteNodes);
		return true;
	}
	RETURN_TRUE_IF(_check_exit);

	_pList->ModifyControlFlags(0,UIFLAG_ENABLE_DRAG|UIFLAG_ENABLE_DROP);		//�ر��϶�
    _model->Delete(pDeleteNodes,pDir);

	{
		CTreeNodeUIEx * pTreeNode = (CTreeNodeUIEx *)_pTree->GetCurSel();
		DirData* pNewDir = GetCurrentFolder(pTreeNode);
		RETURN_TRUE_IF( ! pNewDir );


		if ( ! pNewDir->HasSubDir() )
		{        // ����Ŀ¼����ʾ������ͼ��
			pTreeNode->SetAttribute(_T("icon.image"), _T(""));
			pTreeNode->SetUserData(true);
		}
		_pTree->Invalidate();

	}
    return true;
}

void MainView::TreeDelete()
{
	// ͳ�ƹ���
	CStaticsticsMessage Statisticsmsg(Statistic_ID_DeleteFile);Statisticsmsg.Send();
	RETURN_IF( ! _pTree );
	
	CTreeNodeUIEx* pNode = (CTreeNodeUIEx*)_pTree->GetCurSel();
	RETURN_IF(! pNode);

	NodeData* pParentNode = (NodeData*)pNode->GetMyData();
	RETURN_IF(!pParentNode);
	{
// 		VirtualNameDirData * pQuickdir = dynamic_cast<VirtualNameDirData *>(pParentNode);
// 		CTreeNodeUIEx* parent = (CTreeNodeUIEx *)pNode->get_parent();
// 		CTreeNodeUIEx* root = (CTreeNodeUIEx*)_pTree->GetRoot();
// 		//������� �� ���ڵ㲻�����޸�
// 		if (pQuickdir || root == parent)
// 			return;
	}

	vector<NodeData*>* pDeleteNodes = new vector<NodeData*>;

	pDeleteNodes->push_back(pParentNode);


	wstring msg;
	wstring sdcard = _model->GetSDCardPath();
	const wchar_t *isSdcard = wcsstr(pParentNode->GetFullPath().c_str(),sdcard.c_str());

	if (pParentNode && !isSdcard)
	{
		msg.assign(CommonUIFunc::LoadString(L"FM_DeleteWarning"));
	}

	const wchar_t* pTemplate = CommonUIFunc::LoadString(L"FM_DeletConfirm");
	wchar_t buffer[MAX_PATH];
	swprintf(buffer, MAX_PATH, pTemplate, pDeleteNodes->size());
	msg.append(buffer);

	if (! CommonUIFunc::ConfirmDialog(msg.c_str(), CommonUIFunc::LoadString(L"FM_DeletePrompt")))
	{
		SAFE_DELETE(pDeleteNodes);
		return;
	}
	RETURN_IF(_check_exit);

	CControlUI* parent = pNode->get_parent();
	_pTree->SelectItem(parent);	//ѡ����һ��Ŀ¼�������޷�ˢ��

	CTreeNodeUIEx * pTreeNode = (CTreeNodeUIEx *)_pTree->GetCurSel();
	DirData* pDir = GetCurrentFolder(pTreeNode);
	_model->Delete(pDeleteNodes,pDir);

	Refresh();
}



bool MainView::OnDownload( TNotifyUI* pNotify )
{
	// ͳ�ƹ���
	CStaticsticsMessage msg(Statistic_ID_DownLoadFile);msg.Send();

    DirData* pDir = GetCurrentFolder();
    RETURN_FALSE_IF( ! pDir );

    // �����ﴴ������Model���߳����ͷ�
    vector<NodeData*>* pDownloadNodes = new vector<NodeData*>;
    GetSelectNodes(pDownloadNodes);
    RETURN_TRUE_IF( pDownloadNodes->empty() );

	wstring destPath = BaseOperation::DirDialog(Singleton::GetMainWnd());
    RETURN_TRUE_IF( destPath.empty() );
	RETURN_TRUE_IF(_check_exit);
    _model->Download(destPath, pDownloadNodes);
    return true;
}

void MainView::GetSelectNodes(vector<NodeData*>* pVector, bool bShowAlert)
{
    RETURN_IF( ! _pList );
    int lineCount = _pList->GetCount();

    for(int i = 0 ; i < lineCount; i++ )
    {
        CListContainerItemUI* pLine = (CListContainerItemUI*) _pList->GetItem(i);

        if( ! pLine->IsSelect()) continue;
        if ( pLine->GetCount() <= 2 ) continue;

        NodeData* pLineNode = (NodeData*) pLine->GetUserData();
        if ( pLineNode ) pVector->push_back(pLineNode);
    }

    if ( pVector->empty() && bShowAlert)
        CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_NoSelect"),
            CommonUIFunc::LoadString(L"PROMPT"));
}

void MainView::ClearTreeNode(CTreeNodeUIEx* pNode)
{
    RETURN_IF( ! pNode );
    int childCount = pNode->num_child();
    for(int i = childCount-1; i >= 0; --i )
    {
        CTreeNodeUIEx* pChild = (CTreeNodeUIEx*)pNode->get_child(i);
        if ( pChild )
        {
            pChild->SetMyData(NULL);
            ClearTreeNode(pChild);
            _pTree->RemoveItem(pChild);
        }
    }
}

void MainView::ClearList()
{
    RETURN_IF( ! _pList );

    int childCount = _pList->GetCount();
    for(int i = childCount-1; i >= 0; --i )
    {
        CControlUI* pLine = _pList->GetItem(i);
        if ( pLine ) pLine->SetUserData(NULL);
    }
    _pList->RemoveAll();
}


void MainView::ClearTree()
{
    ClearTreeNode((CTreeNodeUIEx*)_pTree->GetRoot());
    _pTree->RemoveAll();
}

void MainView::Refresh()
{
    DoRefresh();
}

bool MainView::QuickGoto(wstring name)
{

	CTreeNodeUIEx* pTreeNode = (CTreeNodeUIEx*)_pTree->GetRoot()->get_child(1);
	if (pTreeNode && !pTreeNode->IsExpand())
		pTreeNode->Expand(true);    // ����OnExpandTree()������ӽڵ�
	CTreeNodeUIEx* pQuickRootNode = (CTreeNodeUIEx*)_pTree->GetRoot()->get_child(0);
	if(NULL!=pQuickRootNode)
	{
		for (int i=0;i<pQuickRootNode->num_child();i++)
		{
			if (NULL!=pQuickRootNode->get_child(i))
			{
				NodeData* pParentNode = (NodeData*)pQuickRootNode->get_child(i)->GetMyData();
				if (pParentNode && pParentNode->_name == name)
				{
					_pTree->SelectItem(pQuickRootNode->get_child(i));  // ����OnSelectTree()�����list��
					pQuickRootNode->get_child(i)->Expand(true);
					break;
				}
			}
		}
	}

		
	return true;
}

bool MainView::DoRefresh()
{
	ISVirtualNode isVirtual = SelectCurTreeNode();

 	if (_model->GetTreadState() == true)
 		return true;	//�߳��������У������ýڵ�

    DirData* pDir = GetCurrentFolder();
    RETURN_FALSE_IF( ! pDir );	
	wstring path = pDir->GetFullPath();

	ClearTree();
	_model->RefreshData();

 
	if (isVirtual == Virtual_RootNode ||isVirtual == Virtual_Node)
		QuickGoto(pDir->_name);

    else if ( ! Goto(path) )
        Goto(L"/");

    _model->RefreshSpaceInfo(path);
    
    GotoFirstLineOfList();

    if ( ! _filterKey.empty() )
        FilteList();

	_pList->SetFocus();	//ѡ���б�

    return true;
}

bool MainView::OnRefresh( TNotifyUI* pNotify )
{
	// ͳ�ƹ���
	CStaticsticsMessage msg(Statistic_ID_UpdateTree);msg.Send();

    return DoRefresh();
}

void MainView::FilteList()
{
    DirData* pDir = GetCurrentFolder();
    RETURN_IF( ! pDir );

    ClearList();

    deque<NodeData*> childs = pDir->GetChildrens();
    // ����
    if ( ! _filterKey.empty() )
    {
        FilteChilds(&childs);
    }
    AddChildsToList(&childs, GetParentDir(NULL));
}

DirData* MainView::GetCurrentFolder(CTreeNodeUIEx* pTreeNode)
{
    RETURN_NULL_IF( ! _pTree );
    if ( ! pTreeNode )
        pTreeNode =  (CTreeNodeUIEx*) _pTree->GetCurSel();
    RETURN_NULL_IF( ! pTreeNode );



	NodeData* pParentNode = (NodeData*)pTreeNode->GetMyData();
	RETURN_NULL_IF(!pParentNode);

	DirData* pDir = LinkDataToDirData(pParentNode);
	return pDir;
}

void MainView::ShowSpaceInfo( wstring info )
{
    CControlUI* pLabel = GetItem(_T("FM_buttom_right.FM_status_bar.FM_space_info"));
    RETURN_IF(!pLabel);

    pLabel->SetText(info.c_str());
}

bool MainView::OnDoubleClickList( TNotifyUI* pNotify )
{
    RETURN_TRUE_IF( ! _pList);
    CControlUI* pLine = _pList->GetCurSel();
    RETURN_TRUE_IF( ! pLine);

    NodeData* pLineNode = (NodeData*) pLine->GetUserData();
    RETURN_TRUE_IF( ! pLineNode);

    switch( pLineNode->_type )
    {
    case NodeData::DIR:
		{
			ISVirtualNode isVirtual = SelectCurTreeNode();
			if (isVirtual == Virtual_RootNode || isVirtual == Virtual_Node)
				GotoChild(pLineNode->_name);
			else
				Goto(pLineNode->GetFullPath());
		}
        break;

    case NodeData::FILE:
		{
			_model->DownloadAndOpenFile(pLineNode);
			RETURN_TRUE_IF(_check_exit);
		}
        break;

    case NodeData::LINK:
        {
            LinkData* pLink = dynamic_cast<LinkData*>(pLineNode);
            if ( pLink )
            {
                NodeData* pLinkNode = pLink->GetTarget();
                if ( pLinkNode ) Goto(pLinkNode->GetFullPath());
            }
        }
        break;

    default:
        break;
    }
	CControlUI* pLine_temp = _pList->GetItem(0);
	if (pLine_temp)
	{
		_pList->SelectItem(pLine_temp);
		pLine_temp->SetFocus();
	}

    return true;
}

DirData* MainView::GetParentDir(CTreeNodeUIEx* pTreeNode)
{
    RETURN_NULL_IF( ! _pTree );
    if ( ! pTreeNode )
        pTreeNode =  (CTreeNodeUIEx*) _pTree->GetCurSel();
    RETURN_NULL_IF( ! pTreeNode );

    RETURN_NULL_IF( ! _pTree );
    pTreeNode = (CTreeNodeUIEx*) _pTree->GetCurSel();
    RETURN_NULL_IF( ! pTreeNode );

    CTreeNodeUIEx* pParentNode = (CTreeNodeUIEx*)pTreeNode->get_parent();
    RETURN_NULL_IF( ! pParentNode );
    NodeData* pParentDir = (NodeData*)pParentNode->GetMyData();

    return dynamic_cast<DirData*>(pParentDir);
}

bool MainView::OnSelectList( TNotifyUI* pNotify )
{
	return true;
}

void MainView::SetPosition( CControlUI* pItem )
{
    pItem->SetAttribute(_T("txt.halign"),_T("left"));
    pItem->SetAttribute(_T("txt.pos"),_T("8|3,0,12,0"));
    pItem->SetAttribute(_T("txt.endellipsis"),_T("1"));
}

bool wcsistr(const wchar_t* str, int strLen, const wchar_t* key, int keyLen) 
{
    bool found = false;
    for(int i = 0; i <= strLen - keyLen; ++i)
    {
        found = 0 == _wcsnicmp(str+i, key, keyLen);
        if (found)
            break;
    }

    return found;
}

void MainView::FilteChilds( deque<NodeData*>* childs )
{
    deque<NodeData*>::iterator it;
    for(it = childs->begin(); it != childs->end(); )
    {
        NodeData* pNode = *it;
        wstring name = pNode->_displayName.empty() ? pNode->_name : pNode->_displayName;

        if (! wcsistr(name.c_str(), name.size(), 
                    _filterKey.c_str(), _filterKey.size()))
        {
            it = childs->erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void MainView::ListKeyDown(int key)
{
	int flag = true ;
	switch(key)
	{
	case 'C':
		{
			if ( GetKeyState(VK_CONTROL) && 0X8000 )
				OnCopy();
		}
		break;

	case 'V':
		{
			if ( GetKeyState(VK_CONTROL) && 0X8000 )
				OnPaste();
		}
		break;

	case 'X':
		{     
			if ( GetKeyState(VK_CONTROL) && 0X8000 )
				OnCut();
		}
		break;

	case VK_DELETE:
	case VK_DECIMAL:    // С���̵�Del
		OnDelete(NULL);
		break;

	case VK_F2:
		{
			if(_pTree->GetCurSel() == _appTreeNode)
				return;		//Ӧ�ó����������
		}
		flag = false;
		OnRename();
		break;

	case VK_F5:
		OnRefresh(NULL);
		break;

	case VK_BACK:
		OnGoParent(NULL);
		GotoFirstLineOfList();
		break;

	case VK_HOME:
		GotoFirstLineOfList();
		break;

	case VK_END:
		{
			if ( _pList->GetCount() == 1 )
				break;

			{
				CControlUI* pLine = _pList->GetItem(_pList->GetCount()-1);
				_pList->SelectItem(pLine);
				_pList->EnsureVisible(pLine);
			}
		}
		break;

	default:
		flag = false;
		break;
	}
	if (flag && key != 'V'&& key != 'C'&& key != 'X')
	{
		CControlUI* pLine = _pList->GetItem(0);
		if (pLine)
		{
			_pList->SelectItem(pLine);
			pLine->SetFocus();
		}
		else
			_pList->SetFocus();
	}
}

void MainView::ListLetterLocation(int key)
{
	if (key >= 97)	//���ִ�96��ʼ
		key -= 48;

	if (key>='a' && key<='z' || 
		key>='A' && key<='Z' ||
		key>='0' && key<='9')
	{
		CControlUI* cur_pLine = _pList->GetCurSel();
		int childCount = _pList->GetCount();
		bool flag = false;	//�Ȳ��ҵ�ǰѡ���е�λ�ã��Ӵ�λ��֮��ʼ����
		bool select_flag = false;	//�Ƿ��ҵ�������������Ŀ	true�ҵ� falseδ�ҵ�
		
		for(int i = 0; i <childCount; i++ )
		{
			CControlUI* pLine = _pList->GetItem(i);	//ȡ�õ�i��
			if (pLine != cur_pLine && !flag)
				continue;	//�ȹ��˵�ѡ����֮ǰ��

			flag = true;
			if ( pLine && pLine != cur_pLine)	//��ǰѡ���г���
			{
				wstring buf = pLine->GetItem(1)->GetText();	//ȡ�õ�i�е�i��
				if (buf.at(0) == key || buf.at(0)-32 == key)	//��Сдͨɱ
				{
					_pList->SelectItem(pLine);
					pLine->SetFocus();
					select_flag = true;
					break;
				}
			}
		}
		if (!select_flag)
		{	//δ�ҵ���ͷ��ʼ����
			for(int i = 0; i <childCount; i++ )
			{
				CControlUI* pLine = _pList->GetItem(i);	//ȡ�õ�i��

				if ( pLine)	
				{
					wstring buf = pLine->GetItem(1)->GetText();	//ȡ�õ�i�е�i��
					if (buf.at(0) == key || buf.at(0)-32 == key)
					{
						_pList->SelectItem(pLine);
						pLine->SetFocus();
						break;
					}
				}
			}
		}

	}
}


void MainView::TreeKeyLeftOrRight(bool flag)
{
	CTreeNodeUI* pNode = (CTreeNodeUI*)_pTree->GetCurSel();

	if (pNode)
	{
		if (flag)
		{//left
			if (!pNode->IsExpand())
				return;
		}
		else
		{//right
			CTreeNodeUI* pNode = (CTreeNodeUI*)_pTree->GetCurSel();
			if (pNode->IsExpand())
				return;
		}
		
		OnDoubleClickTree(NULL);
	}
}

void MainView::TreeKeyDown(int key)
{
	switch(key)
	{
	case VK_BACK:
		OnGoParent(NULL);
		GotoFirstLineOfList();
		break;
	
	case VK_F5:
	    Refresh();
		break;
	case VK_LEFT:
		TreeKeyLeftOrRight(true);
		break;
	case VK_RIGHT:
		TreeKeyLeftOrRight(false);
		break;

	case VK_DELETE:
	case VK_DECIMAL:    // С���̵�Del
		TreeDelete();
		break;

	case VK_F2:
		{

			CTreeNodeUIEx *curdir = (CTreeNodeUIEx *)_pTree->GetCurSel();
			if (curdir)
			{
				curdir = (CTreeNodeUIEx *)curdir->get_parent();

				if(curdir && curdir == _appTreeNode)
					return;		//Ӧ�ó����������
			}
			
		}
		TreeRename();
		break;

	default:
		break;
	}
}

bool MainView::IsRootExist( DirData* rootData )
{
	bool find = false;
	if(rootData != NULL && _pTree != NULL && _pTree->GetRoot() != NULL)
	{
		for(int i=0;i<_pTree->GetRoot()->num_child();i++)
		{
			CTreeNodeUI* item = (CTreeNodeUI*)_pTree->GetRoot()->get_child(i);
			if(item->GetMyData() == rootData)
			{
				find = true;
				break;
			}
		}
	}
	return find;
}

void MainView::OnRename()
{
	// ͳ�ƹ���
	CStaticsticsMessage msg(Statistic_ID_FileRename);msg.Send();

    vector<NodeData*>* pNodes = new vector<NodeData*>;
    GetSelectNodes(pNodes);
	if (pNodes->empty())
	{
		SAFE_DELETE(pNodes);
		return;
	}

    wstring name = pNodes->at(0)->_name;
    SAFE_DELETE(pNodes);

    bool bCancel;
	wstring newName = CommonUIFunc::InputDialog(CommonUIFunc::LoadString(L"FM_RenamePrompt"),
                                  CommonUIFunc::LoadString(L"FM_Rename"),
                                  name,
                                  bCancel,
								  NEWFOLDERNAMELENG);
	RETURN_IF(_check_exit);

	bool flag = true;
	for (size_t i=0;i<newName.size();i++)
	{
		if (newName.at(i) != L'.')
		{
			flag = false;
			break;
		}
	}
	if (flag)
	{		//ȫ������ . ������
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("FM_RenameErro")), ICON_WARNING);
		return ;
	}

	string str = CCodeOperation::ws2s(newName);

    RETURN_IF ( bCancel );  // �û�ѡ��ȡ��
	if (newName.length() == 0 || newName == _T(".") || newName == _T(".."))
	{//����������Ϊ0���ߴ���160,����������Ϊ"."����".."
		Sleep(500);
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("FM_RenameFail")), ICON_WARNING);
		return ;
	}
	int i1 = str.length();
	if ( str.length() > FOLDERNAMEMAX)
	{
		Sleep(500);
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("FM_FileNameLeng")), ICON_WARNING);
		return ;
	}
	
	MyTrim(newName);

    DirData* pDir = GetCurrentFolder();
    RETURN_IF(!pDir);
    NodeData* pNode = pDir->findChildrenbyName(name, false);
    NodeData* pNewNode = pDir->findChildrenbyName(newName, true,false);

	if (pNewNode == pNode && name == newName)
	{	//�޸ı��ڵ�ʱ�ļ�����û�иı�ʱ����ʾ
		return;
	}
    if (pNewNode != NULL)
    {
        CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("FM_RenameErro")), ICON_WARNING);
        return ;
    }
    if (pNode->_type == NodeData::LINK)
    {
        CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("FM_NoSupportRename")), ICON_WARNING);
        return ;
    }
	
	


    if (!pNode->Rename(newName))
    {
        CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("FM_RenameFail")), ICON_WARNING);
        return ;
    }

    Refresh();

    vector<wstring> paths;
    paths.push_back(newName);
    SelectListLine(paths);
}

void MainView::MyTrim(wstring & str)
{
	wstring::size_type pos = str.find_last_not_of(TEXT(' '));   
	if (pos != wstring::npos)    
	{   
		str.erase(pos + 1);   
		pos = str.find_first_not_of(TEXT(' '));   
		if(pos != wstring::npos)    
		{   
			str.erase(0, pos);   
		}   
	}   
	else    
	{   
		str.erase(str.begin(), str.end());   
	}     
}

void MainView::TreeRename()
{
	// ͳ�ƹ���
	CStaticsticsMessage msg(Statistic_ID_FileRename);msg.Send();

	CTreeNodeUIEx* pTreeNode = (CTreeNodeUIEx*)_pTree->GetCurSel();
	RETURN_IF(! pTreeNode);
	{
		NodeData* pParentNode = (NodeData*)pTreeNode->GetMyData();
		RETURN_IF(!pParentNode);

		DirData* pDir = LinkDataToDirData(pParentNode);
		RETURN_IF(!pDir);
// 		VirtualNameDirData * pQuickdir = dynamic_cast<VirtualNameDirData *>(pDir);
// 		CTreeNodeUIEx* parent = (CTreeNodeUIEx *)pTreeNode->get_parent();
// 		CTreeNodeUIEx* root = (CTreeNodeUIEx*)_pTree->GetRoot();
// 		//������� �� ���ڵ㲻�����޸�
// 		if (pQuickdir || root == parent)
// 			return;
	}

	wstring name = pTreeNode->GetText();

	bool bCancel;
	wstring newName = CommonUIFunc::InputDialog(CommonUIFunc::LoadString(L"FM_RenamePrompt"),
		CommonUIFunc::LoadString(L"FM_Rename"),
		name,
		bCancel);
	RETURN_IF ( bCancel );  // �û�ѡ��ȡ��
	RETURN_IF (_check_exit)
 
	CTreeNodeUIEx* parent = (CTreeNodeUIEx *)pTreeNode->get_parent();

	NodeData* pParentNode = (NodeData*)parent->GetMyData();
	RETURN_IF(!pParentNode);

	DirData*pDir = LinkDataToDirData(pParentNode);
	RETURN_IF(!pDir);

	NodeData* pNode = pDir->findChildrenbyName(name, false);
	NodeData* pNewNode = pDir->findChildrenbyName(newName, false);
	if (pNewNode == pNode && name == newName)
	{	//�޸ı��ڵ�ʱ�ļ�����û�иı�ʱ����ʾ
		return;
	}
	if (pNewNode != NULL)
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("FM_RenameErro")), ICON_WARNING);
		return ;
	}
	if (pNode->_type == NodeData::LINK)
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("FM_NoSupportRename")), ICON_WARNING);
		return ;
	}
	if (!pNode->Rename(newName))
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("FM_RenameFail")), ICON_WARNING);
		return ;
	}

	_pTree->SelectItem(parent);	//ѡ����һ��Ŀ¼�������޷�ˢ��
	Refresh();

}

DirData* MainView::LinkDataToDirData(NodeData* pParentNode)
{

	// link ���治��ֱ���޸ģ�������ʵĿ¼ȥ�޸�
	if ( pParentNode->_type == NodeData::LINK )
	{
		LinkData* pLink = dynamic_cast<LinkData*>(pParentNode);
		RETURN_NULL_IF(!pLink)

		pParentNode = pLink->GetTarget();
		RETURN_NULL_IF(!pLink);
	}

	DirData* pDir = dynamic_cast<DirData*>(pParentNode);
	
	return pDir;
}

void MainView::OnCopy()
{
    DirData* pDir = GetCurrentFolder();
    RETURN_IF(!pDir);

    GetPasteNodes();

    _bCut = false;
    _isFinish = false;
}

void MainView::OnCut()
{
    DirData* pDir = GetCurrentFolder();
    RETURN_IF(!pDir);

    GetPasteNodes();

    _bCut = true;
    _isFinish = false;
}

void MainView::OnPaste()
{
    RETURN_IF(!CheckSDCard());

    DirData* pDir = GetCurrentFolder();
    RETURN_IF(!pDir);

     if (!_isFinish)
         _model->Paste(&_pPasteNodes, _bCut, pDir);
    _isFinish = true;
}

bool MainView::CheckSDCard()
{
    DirData* pDir = GetCurrentFolder();
    RETURN_FALSE_IF(!pDir);
    if (pDir->GetFullPath() == _model->GetSDCardPath()
        && _model->GetSDCardState() != MOUNTED)
    {
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(L"FM_NoSDCard"),ICON_WARNING);
        return false;
    }

    return true;
}

void MainView::SelectListLine(vector<wstring> paths)
{

	DirData * pData = GetCurrentFolder();
	RETURN_IF(!pData);
    deque<NodeData*> nodes = pData->GetChildrens();
    for (size_t i = 0; i < nodes.size(); i++)
    {
        NodeData* pNode = nodes.at(i);
        for (size_t j = 0; j < paths.size(); j++)
        {
            vector<wstring> temp = CStrOperation::parseStringsW(paths.at(j), L"\\");
			if(temp.size()<=0)
				continue;
            wstring fileOrFolderName = temp.at(temp.size()-1);

            // �ļ�
            if (fileOrFolderName.find(L'.') != string::npos)
            {
                vector<wstring> temp1 = CStrOperation::parseStringsW(fileOrFolderName, L".");
				if(temp1.size()<=0)
					continue;
                fileOrFolderName = temp1.at(0);
            }
            
            if (!CStrOperation::startsWith(pNode->_name, fileOrFolderName))
                continue;

            SelectListLine(pNode);
            break;
        }
    }
}

bool  MainView::ListFocused()
{
    CControlUI* pControl = this->GetWindow()->GetFocus();
    while ( pControl )
    {
        if ( pControl == _pList )
            return true;

        pControl = pControl->GetParent();
    }
    return false;
}

bool  MainView::TreeFocused()
{
	CControlUI* pControl = this->GetWindow()->GetFocus();
	while ( pControl )
	{
		if ( pControl == _pTree )
			return true;

		pControl = pControl->GetParent();
	}
	return false;
}

void MainView::GotoFirstLineOfList()
{
    CControlUI* pLine = _pList->GetItem(0);
    _pList->SelectItem(pLine);
    _pList->EnsureVisible(pLine);
}

bool MainView::OnDropFiles( TNotifyUI* pNotify )
{

    DirData* pDir = GetCurrentFolder();
    RETURN_FALSE_IF( ! pDir );

    vector<wstring>* files = new vector<wstring>();
    HDROP hDropInfo = (HDROP)pNotify->wParam;
    UINT count;
    wchar_t filePath[512];
    count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
    wstring strTempDragDir = CStrOperation::toUpperW(_strTempDragDir);
    if(count)
    {        
        for(UINT i=0; i<count; i++)
        {
            int pathLen = DragQueryFile(hDropInfo, i, filePath, sizeof(filePath));
            wstring strFilePath = CStrOperation::toUpperW(filePath);            
            if (strTempDragDir.length() > 0 && strFilePath.find(strTempDragDir) == 0)
                return false;
            if (pathLen > 0 && (CFileOperation::IsDirExist(filePath) || CFileOperation::IsFileExist(filePath)))
                files->push_back(wstring(filePath, pathLen));
        }
    }
	DragFinish(hDropInfo);
    if (files->size() > 0)
    {
		_pList->ModifyControlFlags(0,UIFLAG_ENABLE_DRAG|UIFLAG_ENABLE_DROP);		//�ر��϶�

		DragdropFileMsg msg;
		msg.sn = _deviceId;
		msg.files = files;
		msg.pDir = pDir;
		msg.PostToUI();
    }

    return true;
}

bool MainView::OnBeginDrag( TNotifyUI* pNotify )
{  
    // ��Ϊѡ��
    POINT pt = { (int)(short)LOWORD(pNotify->lParam),(int)(short)HIWORD(pNotify->lParam) }; 
    //ˢ��

    vector<NodeData*>* pNodes = new vector<NodeData*>;
    GetSelectNodes(pNodes, false);
    if (pNodes->size() == 0)
        return false;

    // �����滻�ļ�
    CDataObject* pDataObject = new CDataObject;
    vector<tstring> vtFileList;
    _strTempDragDir = CONFIG(_strTemp) + L"Drag" + CStrOperation::IntToWString(::GetTickCount()) + L"\\";
    CFileOperation::MakeLocalDir(CFileOperation::GetFileDir(_strTempDragDir));
    for (vector<NodeData*>::iterator it = pNodes->begin(); it != pNodes->end(); it++)
    {
        wstring strTempDragPath;
        NodeData* pNode = *it;
        if (pNode->IsDir())
        {
            strTempDragPath = _strTempDragDir + pNode->_name;
            CFileOperation::DeleteDir(strTempDragPath);
        }
        else
        {
            strTempDragPath = _strTempDragDir + pNode->_name;
            CFileOperation::DeleteFile(strTempDragPath);
        }
        vtFileList.push_back(strTempDragPath.c_str());
    }
    pDataObject->SetFileData(vtFileList);

    DWORD dwEffect = 0;
	CWindowUI* pWindow = GetUIEngine()->GetWindow(Singleton::GetMainWnd());
	pWindow->DoDrag(((TNotifyUI*)pNotify)->pSender, pDataObject, DROPEFFECT_COPY|DROPEFFECT_MOVE, &dwEffect);
    _strTempDragDir = L"";
    for (vector<tstring>::iterator it = vtFileList.begin(); it != vtFileList.end(); it++)
    {
        CFileOperation::DeleteDir(*it);
        CFileOperation::DeleteFile(*it);
    }
    CFileOperation::DeleteDir(_strTempDragDir);
    return true;

    return true;
}

bool MainView::OnDragQueryData( TNotifyUI* pNotify )
{
    RECT rc = _pList->GetRect();
    POINT point;
    GetCursorPos(&point); 
	ScreenToClient(Singleton::GetMainWnd(), &point);
    if (point.x > rc.left && point.x < rc.right
        && point.y > rc.top && point.y < rc.bottom)
        return false;

    DirData* pDir = GetCurrentFolder();
    RETURN_FALSE_IF( ! pDir );

    // �����ﴴ������Model���߳����ͷ�
    vector<NodeData*>* pDownloadNodes = new vector<NodeData*>;
    GetSelectNodes(pDownloadNodes);
    RETURN_TRUE_IF( pDownloadNodes->empty() );

    wstring destPath = _strTempDragDir;
    RETURN_TRUE_IF( destPath.empty() );
	
	if (!_model->Download(destPath, pDownloadNodes, false, true)) // ������������ֵ����Ч
    {
        // ʧ�ܣ�ȡ����ҷ����
        CDataObject* pDataObject = (CDataObject*)pNotify->wParam;
        if (pDataObject)
        {
            pDataObject->RemoveAll();
        }
        return false;
    }

    return true;
}

ND91Assistant::EXIST_ACTION MainView::GetExistAction( std::wstring name, HWND owner )
{
	Common_EXIST_ACTION ret = CommonUIFunc::GetExistAction(name,owner);

	return EXIST_ACTION(ret);
}
 
bool MainView::QuickRefreshList()
{
	if (_model->GetAppThreadFlag())
 		return true;
	CTreeNodeUIEx * curnode = (CTreeNodeUIEx*)_pTree->GetCurSel();

	// ���ĵ�ǰ�ڵ㣺��������ӽڵ�
	ClearTreeNode(curnode);

	// ˢ������
	DirData* pDirNode = GetCurrentFolder(curnode);
	RETURN_FALSE_IF( ! pDirNode );

	pDirNode->Refresh();

	NodeData* pParentNode = (NodeData*)curnode->GetMyData();
	RETURN_FALSE_IF(!pParentNode);

	DirData* pDir = NULL;
	if (pParentNode->_type == NodeData::LINK )
	{
		pDir = LinkDataToDirData(pParentNode);
		RETURN_FALSE_IF(!pDir);
	}
	else if (pParentNode->_type == NodeData::DIR )
	{
		pDir = dynamic_cast<DirData*>(pParentNode);
		RETURN_FALSE_IF(!pDir);
	}
	
	
	deque<NodeData*> childs = pDir->GetChildrens();
	AddChildsToTree(curnode, &childs);
	curnode->SetUserData(true);   // ����OnExpandTree���ظ�����
	curnode->Expand(true);        // �ᴥ��OnExpandTree
	_pTree->Resize();
	_pTree->Invalidate();

	_pTree->SelectItem(curnode->get_parent());//��ѡ�񸸽ڵ�
	
	_pTree->SelectItem(curnode);//��ѡ���ӽڵ�
	
	_pList->Invalidate();
  	_model->SetRestortAppName(true);		//���ݱ����ã�����ˢ��app����
  	_model->RestoreAppNodeName(pDirNode);
	return true;
}

ISVirtualNode MainView::SelectCurTreeNode()
{
	if (!_pTree)
	return Node_Error;
	CTreeNodeUIEx *curdir = (CTreeNodeUIEx *)_pTree->GetCurSel();	//��ǰѡ�е����ڵ�
	if (!curdir)
		return Node_Error;

	NodeData* pParentNode = (NodeData*)curdir->GetMyData();
	if (!pParentNode)
		return Node_Error;


	VirtualDirData * pQuickRootdir = dynamic_cast<VirtualDirData *>(pParentNode);
	if (pQuickRootdir)
		return Virtual_RootNode;	//����������ڵ�


	ISVirtualNode flag = Node;
	while (curdir)
	{
 		VirtualNameDirData * pQuickdir = dynamic_cast<VirtualNameDirData *>(pParentNode);
 		if (pQuickdir)
		{//������ڷ���1
			flag = Virtual_Node;
			break;
		}
		curdir = (CTreeNodeUIEx *)curdir->get_parent();
	}


	return flag;
}

void MainView::RefreshAppdata()
{
	DirData* pDir = GetCurrentFolder(_appTreeNode);
	RETURN_IF( ! pDir );

	deque<NodeData*> pChilds = pDir->GetChildrens();
	deque<NodeData*>::iterator it;

	if (_appTreeNode == (CTreeNodeUIEx*)_pTree->GetCurSel())
	{	//���û�����������ڵ�ʱ������������list��ˢ��
		//�����ʱ�û��ڴ˵����app�ڵ��ʱ���������ˢ��


		for( it = pChilds.begin(); it != pChilds.end(); ++it )	//ˢ��list
		{
			wstring name = (*it)->_name;
			if ((*it)->_name == (*it)->_displayName)
				continue;
			
			int childCount = _pList->GetCount();		
			for(int i = 0; i <childCount; i++ )
			{
				CControlUI* pLine = _pList->GetItem(i);	//ȡ�õ�i��
				if ( pLine )
				{
					wstring buf = pLine->GetItem(1)->GetText();	//ȡ�õ�i�е�i��
					
					if (buf == name)
					{
						pLine->GetItem(1)->SetText((*it)->_displayName.c_str());
						_pList->SelectItem(pLine);
						break;
					}
				}
			}
		}
		//��ô����ڵ���ӽڵ㣬��չ������Ч
		if (_appTreeNode->IsExpand())
		{
			int num = _appTreeNode->num_child();
			if (num == 0)
				return;

			for( it = pChilds.begin(); it != pChilds.end(); ++it )	//ˢ��tree
			{
				if ((*it)->_name == (*it)->_displayName)
					continue;
				for (int i =0;i<num;i++)
				{
					CTreeNodeUIEx* pLine = (CTreeNodeUIEx*)_appTreeNode->get_child(i);	//ȡ�õ�i���ӽڵ�
					if (pLine)
					{
						wstring buf = pLine->GetText();
						if (buf == (*it)->_name)
						{
							pLine->SetText((*it)->_displayName.c_str());
							break;
						}
					}
				}
			}
			_pTree->Invalidate();
		}
	}
	
}

bool MainView::OnShowFileListMenu( TNotifyUI* pNotify )
{
	ISVirtualNode isVirtual = SelectCurTreeNode();
	if(Virtual_RootNode == isVirtual)
		return true;
	CMenuUI* pMenu = CMenuUI::Load(MENU_ID_FILEMANAGELISTMENU);

	if(_pTree->GetCurSel() == _appTreeNode)
	{
		pMenu->GetItem(12)->EnableItem(false);
	}
	if(pNotify->lParam==0)
	{
		if(!ListHasSelectItem())
		{
			pMenu->GetItem(0)->EnableItem(false);
			pMenu->GetItem(2)->EnableItem(false);
			pMenu->GetItem(4)->EnableItem(false);
			pMenu->GetItem(11)->EnableItem(false);
			pMenu->GetItem(12)->EnableItem(false);
		}
		pMenu->Show(Singleton::GetMainWnd(), NULL, this);
		return true;
	}
	else
	{
		CControlUI *pCur = (CControlUI *)pNotify->lParam;
		if(!pCur) return true;
		_pList->SelectItem(pCur);
		if (pMenu)
		{
			pMenu->SetParam(pNotify->wParam,pNotify->lParam);
			pMenu->Show(Singleton::GetMainWnd(), NULL, this);
		}
		return true;
	}

}


bool MainView::OnListMenuClick( TNotifyUI* pNotify )
{
	// ͳ�ƹ���
	CStaticsticsMessage msg(Statistic_ID_RightBntMenu);msg.Send();

	RETURN_TRUE_IF(pNotify->wParam == NULL);
	UIMENUINFO* pInfo = (UIMENUINFO*)pNotify->wParam;
	RETURN_TRUE_IF(!pInfo->bEnableItem);
	switch (pInfo->uID)
	{
		//��
		case 30001:
			OnDoubleClickList(pNotify);
			break;
		//����·��
		case 30002:
			OnCopyDirPath();
			break;
		//����
		case 30003:
			OnDownload(pNotify);
			break;
		//�ϴ��ļ�
		case 30004:
			OnUploadFile(pNotify);
			break;
		//�ϴ�Ŀ¼
		case 30005:
			OnUploadDir(pNotify);
			break;
		//ˢ��(F5)
		case 30006:
			OnRefresh(pNotify);
			break;
		//�½��ļ���
		case 30007:
			OnAddFolder(pNotify);
			break;
		//ɾ��(Del)
		case 30008:
			OnDelete(pNotify);
			break;
		//������(F2)
		case 30009:
			OnRename();
			break;
		default:
			OnDoubleClickList(pNotify);
			break;
	}
	return true;
}

bool MainView::OnCopyDirPath()
{
	bool copy = false;
	wstring path = L"";
	RETURN_TRUE_IF( ! _pList);
	CControlUI* pLine = _pList->GetCurSel();
	RETURN_TRUE_IF( ! pLine);

	NodeData* pLineNode = (NodeData*) pLine->GetUserData();
	RETURN_TRUE_IF( ! pLineNode);

	switch( pLineNode->_type )
	{
		case NodeData::DIR:
			{
				path = pLineNode->GetFullPath();
				copy = true;
			}
			break;
		case NodeData::LINK:
			{
				LinkData* pLink = dynamic_cast<LinkData*>(pLineNode);
				if ( pLink )
				{
					NodeData* pLinkNode = pLink->GetTarget();
					if ( pLinkNode )
					{
						path = pLinkNode->GetFullPath();
						copy = true;
					}
				}
			}
			break;
		default:
			break;
	}
	if(!copy)
	{
		CommonUIFunc::NoticeDialog(L"��ѡ���ļ���·�����и��ƣ�", ICON_WARNING);
	}
	else
	{
		if (!OpenClipboard(Singleton::GetMainWnd()))
			return FALSE;
		string spath = CCodeOperation::UnicodeToGB2312(path);
		HGLOBAL hmem=GlobalAlloc(GHND,spath.length()+1);
		char* pmem=(char*)GlobalLock(hmem);
		EmptyClipboard();
		strcpy(pmem, spath.c_str());
		GlobalUnlock(hmem);
		SetClipboardData(CF_TEXT,hmem);
		CloseClipboard();
	}
	return true;
}

bool MainView::OnParseDirPath()
{
	string dirpath;
	if (!OpenClipboard(Singleton::GetMainWnd()))
		return FALSE;
	HANDLE hData = GetClipboardData(CF_TEXT);
	char* buffer = (char*)GlobalLock(hData);
	dirpath = buffer;
	GlobalUnlock(hData);
	CloseClipboard();
	CEditUI* pEdit = (CEditUI*)GetItem(_T("FM_top.FM_top_left.FM_path"));
	pEdit->SetText(CCodeOperation::GB2312ToUnicode(buffer).c_str());
	return true;
}

void MainView::GetPasteNodes()
{
    RETURN_IF( ! _pList );
    _pPasteNodes.clear();

    int lineCount = _pList->GetCount();
    for(int i = 0 ; i < lineCount; i++ )
    {
        CListContainerItemUI* pLine = (CListContainerItemUI*) _pList->GetItem(i);

        if( ! pLine->IsSelect()) continue;
        if ( pLine->GetCount() <= 2 ) continue;

        NodeData* pLineNode = (NodeData*) pLine->GetUserData();
        if ( pLineNode ) _pPasteNodes.push_back(pLineNode->GetFullPath());
    }

    if ( _pPasteNodes.empty() )
    {
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_NoSelect"), CommonUIFunc::LoadString(L"PROMPT"));
    }
}

void MainView::EnableListButton( bool enable )
{
    CControlUI* pButton;

    pButton = GetItem(_T("FM_buttom_right.FM_toolbar.FM_toolbar.FM_addfolder"));
    if ( pButton ) pButton->SetEnabled(enable);

    pButton = GetItem(_T("FM_buttom_right.FM_toolbar.FM_toolbar.FM_uploadFile"));
    if ( pButton ) pButton->SetEnabled(enable);

    pButton = GetItem(_T("FM_buttom_right.FM_toolbar.FM_toolbar.FM_uploadDir"));
    if ( pButton ) pButton->SetEnabled(enable);

    pButton = GetItem(_T("FM_buttom_right.FM_toolbar.FM_toolbar.FM_download"));
    if ( pButton ) pButton->SetEnabled(enable);

    pButton = GetItem(_T("FM_buttom_right.FM_toolbar.FM_toolbar.FM_delete"));
    if ( pButton ) pButton->SetEnabled(enable);

    if  (enable)
        _pList->ModifyControlFlags(UIFLAG_ENABLE_DRAG|UIFLAG_ENABLE_DROP,0);		//�����϶�
    else
        _pList->ModifyControlFlags(0,UIFLAG_ENABLE_DRAG|UIFLAG_ENABLE_DROP);		//�ر��϶�
}

bool MainView::FocusInSearchEdit()
{
    CControlUI* pControl = this->GetWindow()->GetFocus();
    CControlUI* pSearchEdit = GetItem(_T("FM_top.FM_top_right.FM_search_edit"));
    return pSearchEdit && pControl == pSearchEdit;
}

void MainView::Release()
{
//	Singleton::GetMessageCenter()->Detach(this);
	_check_exit = true;
	_model->Release();
}

bool MainView::ListHasSelectItem()
{
	if(!_pList)
		return false;
	CControlUI* pCurSel = _pList->GetCurSel();
	if(!pCurSel)
		return false;
	return true;
}

void MainView::LoadData()
{
	if(_model->IsDefaultLoaded())
		return;

	_model->LoadDefaultData();
}

void MainView::ShowLoading( bool visible /*= true*/ )
{
	CLayoutUI* pLoadingLayout = (CLayoutUI*)GetItem(L"LoadingLayout");
	if(pLoadingLayout == NULL)
		return;

	pLoadingLayout->SetVisible(visible);
	pLoadingLayout->Resize();
	pLoadingLayout->Invalidate();
}

CControlUI* MainView::FindControl( FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags )
{
	CLayoutUI* pLoadingLayout = (CLayoutUI*)GetItem(L"LoadingLayout");
	if(pLoadingLayout != NULL && pLoadingLayout->IsVisible())
	{
		return pLoadingLayout->FindControl(Proc, pData, uFlags);
	}
	else
	{
		return __super::FindControl(Proc, pData, uFlags);
	}
}

std::wstring MainView::GetStyleID()
{
	return L"FileManager_View_Main";
}

void MainView::InitRootMsg_Handle( const InitRootMsg* pMsg )
{
	wstring sn = pMsg->sn;

	if(_deviceId != sn)
		return;

	if(_model->GetVirtualRoot())
	{
		if(!IsRootExist(_model->GetVirtualRoot()))
		{
			deque<NodeData*> childNodes = _model->GetVirtualRoot() ->GetChildrens();
			if (childNodes.size() != 0)
			{
				OnInit(_model->GetVirtualRoot());
			}
		}
	}
	if(_model->GetRoot())
	{
		if(!IsRootExist(_model->GetRoot()))
		{
			OnInit(_model->GetRoot());
		}
	}
}

void MainView::RefreshNodeMsg_Handle( const RefreshNodeMsg* pMsg )
{
	RETURN_IF( ! _pTree || ! _pList );
	_pList->ModifyControlFlags(UIFLAG_ENABLE_DRAG|UIFLAG_ENABLE_DROP,0);		//�����϶�

	if (pMsg->_str != _deviceId)	//�ȶ�id�Ƿ�Ϊ����
		return;

	Refresh();

	if (pMsg->_int == RefreshNode_And_SelectList)
		SelectListLine(_model->_refresh_node_ver);	//��ѡ���в���

	_pList->SetFocus();	//ѡ���б�
}

void MainView::RefreshAppMsg_Handle( const RefreshAppMsg* pMsg )
{
	if (pMsg->sn != _deviceId)
		return;

	RefreshAppdata();
}

void MainView::DragdropFileMsg_Handle( const DragdropFileMsg* pMsg )
{

	if (pMsg->sn != _deviceId)
		return;

	vector<wstring>* parFiles = pMsg->files;
	vector<wstring> files;
	if(parFiles != NULL)
	{
		files.insert(files.end(), parFiles->begin(), parFiles->end());
		SAFE_DELETE(parFiles);	

		DirData* pDir = (DirData*)pMsg->pDir;
		_model->UploadFile(&files, pDir);
	}	
}