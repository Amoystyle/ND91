#pragma once

#include "Logic/FileManagerInterface.h"
#include "Logic/FileManagerModel.h"
#include "UI/DragDropMessageReceiver.h"

enum ISVirtualNode
{
	Node_Error,			//�ڵ�ָ�����
	Virtual_RootNode,	//����������ڵ�
	Virtual_Node,		//������ڽڵ�
	Node,				//��ͨ�ڵ�
};

#define NEWFOLDERNAMELENG  80
#define FOLDERNAMEMAX  160

class CTreeNodeUIEx 
	: public CTreeNodeUI
{
public:
    CTreeNodeUIEx(bool bGroup = false);
    virtual ~CTreeNodeUIEx(){ SetUserData(NULL); };
    virtual void CalcControl(RECT & rectCalc);
};

#define MENU_ID_FILEMANAGELISTMENU 30000
#define MENU_ID_FILEMANAGEPATHPASTE 30010
// �ļ�����ģ��
class MainView
	: public BaseView
	, public FileManagerInterface   // ʵ��Model����ķ���
{
public:
    UIBEGIN_MSG_MAP

        // �����ť
        EVNET_ID_HANDLER(UINOTIFY_CLICK,        L"FM_pre",    OnGoPrev);
        EVNET_ID_HANDLER(UINOTIFY_CLICK,        L"FM_next",   OnGoNext);
        EVNET_ID_HANDLER(UINOTIFY_CLICK,        L"FM_parent", OnGoParent);
        EVNET_ID_HANDLER(UINOTIFY_EN_KEYDOWN,   L"FM_path",   OnPathComplete);
        EVNET_ID_HANDLER(UINOTIFY_CLICK,        L"FM_goto",   OnGoto);

        // ����
        EVNET_ID_HANDLER(UINOTIFY_CLICK,        L"FM_clear",        OnClearSearch);
        EVNET_ID_HANDLER(UINOTIFY_EN_CHANGE,    L"FM_search_edit",  OnKeyIn);

        // ��
        EVNET_ID_HANDLER(UINOTIFY_TVN_ITEMEXPENDING,L"FM_tree", OnExpandTree);
        EVNET_ID_HANDLER(UINOTIFY_SELCHANGE,        L"FM_tree", OnSelectTree);
        EVNET_ID_HANDLER(UINOTIFY_DBCLICK,          L"FM_tree", OnDoubleClickTree);

        // �ļ�������ť
        EVNET_ID_HANDLER(UINOTIFY_CLICK, L"FM_addfolder",   OnAddFolder);
        EVNET_ID_HANDLER(UINOTIFY_CLICK, L"FM_uploadFile",  OnUploadFile);
        EVNET_ID_HANDLER(UINOTIFY_CLICK, L"FM_uploadDir",   OnUploadDir);
        EVNET_ID_HANDLER(UINOTIFY_CLICK, L"FM_download",    OnDownload);
        EVNET_ID_HANDLER(UINOTIFY_CLICK, L"FM_delete",      OnDelete);
        EVNET_ID_HANDLER(UINOTIFY_CLICK, L"FM_refresh",     OnRefresh);

        // �б����
        EVNET_ID_HANDLER(UINOTIFY_DBCLICK,      L"FM_list",     OnDoubleClickList);
        EVNET_ID_HANDLER(UINOTIFY_SELCHANGE,    L"FM_list",     OnSelectList);
		EVNET_ID_HANDLER(UINOTIFY_RCLICK,		L"FM_list",		OnShowFileListMenu);

        // ��������
        EVNET_ID_HANDLER(UINOTIFY_DROPFILES, L"FM_list", OnDropFiles)
        EVNET_ID_HANDLER(UINOTIFY_BEGIN_DRAG, L"FM_list", OnBeginDrag)
        EVNET_ID_HANDLER(UINOTIFY_DRAG_QUERYDATA, L"FM_list", OnDragQueryData)

		//�Ҽ��˵�����
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK,MENU_ID_FILEMANAGELISTMENU,OnListMenuClick);

    UIEND_MSG_MAP

    MainView(wstring deviceId);
    virtual ~MainView(void);

	void Release();

	void InitRootMsg_Handle(const InitRootMsg* pMsg);

	void RefreshNodeMsg_Handle(const RefreshNodeMsg* pMsg);

	void RefreshAppMsg_Handle(const RefreshAppMsg* pMsg);

	void DragdropFileMsg_Handle(const DragdropFileMsg* pMsg);

    // ����ˢ�»���
    virtual void LoadData();

    virtual void RefreshData()
    {
        _model->RefreshData();
        Refresh();
    }

    // �ṩ��Model�õ��麯��
    void OnInit(DirData* pRootDir);
    bool Goto(wstring path);

    void Refresh();
    void ShowSpaceInfo( wstring info );
    void SelectListLine(vector<wstring> paths);// ����ָ���ļ����ڵ��б���

    // ������Ϣ
//    void Receive(MESSAGE_TYPE msg, const BaseParam* pParm);

    void GotoFirstLineOfList();

    bool ListFocused();     // ��ǰ�Ƿ��ڲ����ļ��б�
	bool TreeFocused();		//��ǰ�Ƿ��ڲ�����



	void TreeKeyLeftOrRight(bool flag);	//tree ���Ҽ����

	// �ļ��Ѵ��ڵ�ѯ�ʶԻ���
	EXIST_ACTION GetExistAction(wstring name, HWND owner);

	void MyTrim(wstring & str);		//ȥ���ַ����ҿո�



private:

	virtual wstring GetStyleID();

    virtual void OnCreate();

    // Summary  : ȡ��ǰѡ�е���:֧�ֶ�ѡ, �Զ����ˡ���һ��Ŀ¼����
    // Parameter: pVector ������������ÿ���ж�Ӧ���ļ�ϵͳ����ָ��
    void GetSelectNodes(vector<NodeData*>* pVector, bool bShowAlert=true);
    void GetPasteNodes();

    // Summary  : ����Ŀ¼���ڵ�
    // Returns  : �ڵ�ָ��
    // Parameter: LPCTSTR lpszText �ڵ���ʾ����
    CTreeNodeUIEx* CreateTreeNode(LPCTSTR lpszText);

    // ���ָ��Ŀ¼�����ݵ�Ŀ¼��
	//flag :���������app����Ҫ������ʾ���� 1ΪӦ�ó���
	void AddChildsToTree(CTreeNodeUIEx* pCurrentNode, deque<NodeData*>* pChilds);

    // ���ָ��Ŀ¼�����ݵ��ļ��б�
	//flag :���������app����Ҫ������ʾ���� 1ΪӦ�ó���
    void AddChildsToList(deque<NodeData*>* childs, DirData* pParentDir);

    // �����������ֵ�����
    void SetPosition( CControlUI* pItem );

    // �����б�
    void FilteList();

    // UI�¼�������
    bool OnKeyIn                (TNotifyUI* pNotify);   // ������˹ؼ���
    bool OnClearSearch          (TNotifyUI* pNotify);   // ������˹ؼ���
    bool OnGoPrev               (TNotifyUI* pNotify);   // ����
    bool OnGoNext               (TNotifyUI* pNotify);   // ǰ��
    bool OnGoParent             (TNotifyUI* pNotify);   // �ϼ�Ŀ¼
    bool OnPathComplete         (TNotifyUI* pNotify);   // Ŀ¼��ת
    bool OnGoto                 (TNotifyUI* pNotify);   // Ŀ¼��ת
    bool OnAddFolder            (TNotifyUI* pNotify);   // �½��ļ���
    bool OnUploadFile           (TNotifyUI* pNotify);   // �ϴ��ļ�
    bool OnUploadDir            (TNotifyUI* pNotify);   // �ϴ�Ŀ¼
    bool OnDownload             (TNotifyUI* pNotify);   // ����
    bool OnDelete               (TNotifyUI* pNotify);   // ɾ��
    bool OnRefresh              (TNotifyUI* pNotify);   // ˢ��
    bool OnExpandTree           (TNotifyUI* pNotify);   // ���ڵ�չ��
    bool OnKeyInList            (TNotifyUI* pNotify);   // �б��ϵĿ�ݼ�����

    bool OnSelectTree           (TNotifyUI* pNotify);   // �������ڵ�
    bool OnDoubleClickTree      (TNotifyUI* pNotify);   // ˫�����ڵ�
    bool OnSelectList           (TNotifyUI* pNotify);   // �����б���
    bool OnDoubleClickList      (TNotifyUI* pNotify);   // ˫���б���
	bool OnShowFileListMenu		(TNotifyUI* pNotify);   // �Ҽ��б���
    // ��������
    bool OnDropFiles            (TNotifyUI* pNotify);   // �Ͻ��ļ�
    bool OnBeginDrag            (TNotifyUI* pNotify);   // ��ʼ�ϳ��ļ�
    bool OnDragQueryData        (TNotifyUI* pNotify);   // �ϳ��ļ�

	//�Ҽ��˵��¼�
	bool OnListMenuClick			(TNotifyUI* pNotify);   // �ļ��б��Ҽ��˵�
	bool OnPathMenuClick			(TNotifyUI* pNotify);	// ·���Ҽ��˵�

    // ����ָ��Ŀ¼���ڵ����ڵ�
    CTreeNodeUIEx* SelectTreeNode( DirData* pNode );

    // ����ָ���ļ����ڵ��б���
    bool SelectListLine( NodeData* pNode );

    // ȡ�õ�ǰĿ¼
    DirData* GetCurrentFolder(CTreeNodeUIEx* pTreeNode = NULL);

    // �����ļ��б����ݵļ�������
    CControlUI* CreateIconItem(NodeData* pNode);
    CControlUI* CreateNameItem(NodeData* pNode);
    CControlUI* CreateSizeItem(NodeData* pNode);
    CControlUI* CreateTypeItem(NodeData* pNode);
    CControlUI* CreateTimeItem(NodeData* pNode);

    // ����Ŀ¼������
    void SetPath(wstring path);

    // ����ǰ��/����/��һ�㰴ť��״̬
    void EnableButton(bool enablePrevButton, bool enableNextButton, bool enableParentButton);

    // ���ļ����������Ҫ��ɾ����NodeDataָ�루����Ŀ¼�ݹ���ã�
    // ����ɾ����ؽڵ�ʱ��������Щָ�뵼�±���
    void ClearTreeNode(CTreeNodeUIEx* pNode);

    // ɾ���б����ݣ�����ֱ�ӵ���RemoveAll
    void ClearList();
    // ɾ�������ݣ�����ֱ�ӵ���RemoveAll
    void ClearTree();

    // Summary  : ȡָ���ڵ�ĸ��ڵ��Ŀ¼��������
    // Returns  : Ŀ¼����ָ��
    // Parameter: pTreeNode ָ���ڵ㣬ΪNULL���Զ�ȡ��ǰ�ڵ�
    DirData* GetParentDir(CTreeNodeUIEx* pTreeNode);

    void FilteChilds( deque<NodeData*>* childs );    // �����ӽڵ�
    void OnRename();        // ����
    void OnCopy();          // ����
    void OnCut();           // ����
    void OnPaste();         // ճ��

	bool OnCopyDirPath();	//����·��
	bool OnParseDirPath();	//ճ��·��
    // ����Ƿ���SD��
    bool CheckSDCard();

	//���ڵ�ɾ������
	void TreeDelete();
	
	//���ڵ�������
	void TreeRename();

	bool GotoChild(wstring path);	//������ڵ��list��ʱ�����

	bool QuickRefreshList();	//�������ˢ��list  

	ISVirtualNode SelectCurTreeNode();// ���ص�ǰ�����Ŀ���

	void ListKeyDown(int key);	//list��ݼ�����

	void TreeKeyDown(int key);	//tree��ݼ�����

	void RefreshAppdata();		//ˢ��app����

	void ListLetterLocation(int key);		//�б�ʹ����ĸ���ֿ��ٶ�λ

    bool DoRefresh();

	bool ListHasSelectItem();	//�ļ��б��Ƿ���ѡ����

	
    void EnableListButton( bool enable );
    bool FocusInSearchEdit();

	// link ���治��ֱ���޸ģ�������ʵĿ¼ȥ�޸�
	DirData* LinkDataToDirData(NodeData* pParentNode);

	bool QuickGoto(wstring name);

	void ShowLoading(bool visible = true);

	virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

//	void DoInitRootMsg(const BaseParam* pParm);

	bool IsRootExist(DirData* rootData);
private:

	bool				_check_exit;		//�豸�Ƿ��˳�
    FileManagerModel    *_model;             // ҵ���߼���
    CTreeUI*            _pTree;             // �ļ���
    CListUI*            _pList;             // �ļ��б�
    wstring             _filterKey;         // ���˹ؼ���
    wstring             _strTempDragDir;    // �Ϸ�ʹ�õ���ʱ�ļ���
    bool			    _bCut;              // �Ƿ��Ǽ���
    bool                _isFinish;          // �Ƿ����һ�ο��������

    vector<wstring>     _pPasteNodes;       // ��ճ���Ľڵ�

	CTreeNodeUIEx *		_appTreeNode;		// Ӧ�ó���ڵ�
	wstring				_pSelectDirName;    // ��ǰѡ�е�Ŀ¼����
    wstring             _lastEditPath;      // �ϴ���Ч��·����ַ
	
	wstring				_deviceId;			

};
