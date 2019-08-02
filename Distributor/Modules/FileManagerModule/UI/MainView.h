#pragma once

#include "Logic/FileManagerInterface.h"
#include "Logic/FileManagerModel.h"
#include "UI/DragDropMessageReceiver.h"

enum ISVirtualNode
{
	Node_Error,			//节点指针错误
	Virtual_RootNode,	//快速入口主节点
	Virtual_Node,		//快速入口节点
	Node,				//普通节点
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
// 文件管理模块
class MainView
	: public BaseView
	, public FileManagerInterface   // 实现Model所需的方法
{
public:
    UIBEGIN_MSG_MAP

        // 浏览按钮
        EVNET_ID_HANDLER(UINOTIFY_CLICK,        L"FM_pre",    OnGoPrev);
        EVNET_ID_HANDLER(UINOTIFY_CLICK,        L"FM_next",   OnGoNext);
        EVNET_ID_HANDLER(UINOTIFY_CLICK,        L"FM_parent", OnGoParent);
        EVNET_ID_HANDLER(UINOTIFY_EN_KEYDOWN,   L"FM_path",   OnPathComplete);
        EVNET_ID_HANDLER(UINOTIFY_CLICK,        L"FM_goto",   OnGoto);

        // 过滤
        EVNET_ID_HANDLER(UINOTIFY_CLICK,        L"FM_clear",        OnClearSearch);
        EVNET_ID_HANDLER(UINOTIFY_EN_CHANGE,    L"FM_search_edit",  OnKeyIn);

        // 树
        EVNET_ID_HANDLER(UINOTIFY_TVN_ITEMEXPENDING,L"FM_tree", OnExpandTree);
        EVNET_ID_HANDLER(UINOTIFY_SELCHANGE,        L"FM_tree", OnSelectTree);
        EVNET_ID_HANDLER(UINOTIFY_DBCLICK,          L"FM_tree", OnDoubleClickTree);

        // 文件操作按钮
        EVNET_ID_HANDLER(UINOTIFY_CLICK, L"FM_addfolder",   OnAddFolder);
        EVNET_ID_HANDLER(UINOTIFY_CLICK, L"FM_uploadFile",  OnUploadFile);
        EVNET_ID_HANDLER(UINOTIFY_CLICK, L"FM_uploadDir",   OnUploadDir);
        EVNET_ID_HANDLER(UINOTIFY_CLICK, L"FM_download",    OnDownload);
        EVNET_ID_HANDLER(UINOTIFY_CLICK, L"FM_delete",      OnDelete);
        EVNET_ID_HANDLER(UINOTIFY_CLICK, L"FM_refresh",     OnRefresh);

        // 列表操作
        EVNET_ID_HANDLER(UINOTIFY_DBCLICK,      L"FM_list",     OnDoubleClickList);
        EVNET_ID_HANDLER(UINOTIFY_SELCHANGE,    L"FM_list",     OnSelectList);
		EVNET_ID_HANDLER(UINOTIFY_RCLICK,		L"FM_list",		OnShowFileListMenu);

        // 拖拉操作
        EVNET_ID_HANDLER(UINOTIFY_DROPFILES, L"FM_list", OnDropFiles)
        EVNET_ID_HANDLER(UINOTIFY_BEGIN_DRAG, L"FM_list", OnBeginDrag)
        EVNET_ID_HANDLER(UINOTIFY_DRAG_QUERYDATA, L"FM_list", OnDragQueryData)

		//右键菜单操作
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK,MENU_ID_FILEMANAGELISTMENU,OnListMenuClick);

    UIEND_MSG_MAP

    MainView(wstring deviceId);
    virtual ~MainView(void);

	void Release();

	void InitRootMsg_Handle(const InitRootMsg* pMsg);

	void RefreshNodeMsg_Handle(const RefreshNodeMsg* pMsg);

	void RefreshAppMsg_Handle(const RefreshAppMsg* pMsg);

	void DragdropFileMsg_Handle(const DragdropFileMsg* pMsg);

    // 公共刷新机制
    virtual void LoadData();

    virtual void RefreshData()
    {
        _model->RefreshData();
        Refresh();
    }

    // 提供给Model用的虚函数
    void OnInit(DirData* pRootDir);
    bool Goto(wstring path);

    void Refresh();
    void ShowSpaceInfo( wstring info );
    void SelectListLine(vector<wstring> paths);// 查找指定文件所在的列表行

    // 接受消息
//    void Receive(MESSAGE_TYPE msg, const BaseParam* pParm);

    void GotoFirstLineOfList();

    bool ListFocused();     // 当前是否在操作文件列表
	bool TreeFocused();		//当前是否在操作树



	void TreeKeyLeftOrRight(bool flag);	//tree 左右键点击

	// 文件已存在的询问对话框
	EXIST_ACTION GetExistAction(wstring name, HWND owner);

	void MyTrim(wstring & str);		//去除字符串右空格



private:

	virtual wstring GetStyleID();

    virtual void OnCreate();

    // Summary  : 取当前选中的行:支持多选, 自动过滤【上一级目录】行
    // Parameter: pVector 输出参数，存放每个行对应的文件系统对象指针
    void GetSelectNodes(vector<NodeData*>* pVector, bool bShowAlert=true);
    void GetPasteNodes();

    // Summary  : 创建目录树节点
    // Returns  : 节点指针
    // Parameter: LPCTSTR lpszText 节点显示内容
    CTreeNodeUIEx* CreateTreeNode(LPCTSTR lpszText);

    // 添加指定目录的内容到目录树
	//flag :快速入口中app等需要更改显示名称 1为应用程序
	void AddChildsToTree(CTreeNodeUIEx* pCurrentNode, deque<NodeData*>* pChilds);

    // 添加指定目录的内容到文件列表
	//flag :快速入口中app等需要更改显示名称 1为应用程序
    void AddChildsToList(deque<NodeData*>* childs, DirData* pParentDir);

    // 设置行中文字的属性
    void SetPosition( CControlUI* pItem );

    // 过滤列表
    void FilteList();

    // UI事件处理函数
    bool OnKeyIn                (TNotifyUI* pNotify);   // 输入过滤关键字
    bool OnClearSearch          (TNotifyUI* pNotify);   // 清除过滤关键字
    bool OnGoPrev               (TNotifyUI* pNotify);   // 后退
    bool OnGoNext               (TNotifyUI* pNotify);   // 前进
    bool OnGoParent             (TNotifyUI* pNotify);   // 上级目录
    bool OnPathComplete         (TNotifyUI* pNotify);   // 目录跳转
    bool OnGoto                 (TNotifyUI* pNotify);   // 目录跳转
    bool OnAddFolder            (TNotifyUI* pNotify);   // 新建文件夹
    bool OnUploadFile           (TNotifyUI* pNotify);   // 上传文件
    bool OnUploadDir            (TNotifyUI* pNotify);   // 上传目录
    bool OnDownload             (TNotifyUI* pNotify);   // 下载
    bool OnDelete               (TNotifyUI* pNotify);   // 删除
    bool OnRefresh              (TNotifyUI* pNotify);   // 刷新
    bool OnExpandTree           (TNotifyUI* pNotify);   // 树节点展开
    bool OnKeyInList            (TNotifyUI* pNotify);   // 列表上的快捷键操作

    bool OnSelectTree           (TNotifyUI* pNotify);   // 单击树节点
    bool OnDoubleClickTree      (TNotifyUI* pNotify);   // 双击树节点
    bool OnSelectList           (TNotifyUI* pNotify);   // 单击列表行
    bool OnDoubleClickList      (TNotifyUI* pNotify);   // 双击列表行
	bool OnShowFileListMenu		(TNotifyUI* pNotify);   // 右键列表行
    // 拖拉操作
    bool OnDropFiles            (TNotifyUI* pNotify);   // 拖进文件
    bool OnBeginDrag            (TNotifyUI* pNotify);   // 开始拖出文件
    bool OnDragQueryData        (TNotifyUI* pNotify);   // 拖出文件

	//右键菜单事件
	bool OnListMenuClick			(TNotifyUI* pNotify);   // 文件列表右键菜单
	bool OnPathMenuClick			(TNotifyUI* pNotify);	// 路径右键菜单

    // 查找指定目录所在的树节点
    CTreeNodeUIEx* SelectTreeNode( DirData* pNode );

    // 查找指定文件所在的列表行
    bool SelectListLine( NodeData* pNode );

    // 取得当前目录
    DirData* GetCurrentFolder(CTreeNodeUIEx* pTreeNode = NULL);

    // 生成文件列表内容的几个方法
    CControlUI* CreateIconItem(NodeData* pNode);
    CControlUI* CreateNameItem(NodeData* pNode);
    CControlUI* CreateSizeItem(NodeData* pNode);
    CControlUI* CreateTypeItem(NodeData* pNode);
    CControlUI* CreateTimeItem(NodeData* pNode);

    // 设置目录框内容
    void SetPath(wstring path);

    // 设置前进/后退/上一层按钮的状态
    void EnableButton(bool enablePrevButton, bool enableNextButton, bool enableParentButton);

    // 从文件树中清除将要被删除的NodeData指针（对子目录递归调用）
    // 否则删除相关节点时会析构这些指针导致崩溃
    void ClearTreeNode(CTreeNodeUIEx* pNode);

    // 删除列表内容：请勿直接调用RemoveAll
    void ClearList();
    // 删除树内容：请勿直接调用RemoveAll
    void ClearTree();

    // Summary  : 取指定节点的父节点的目录对象数据
    // Returns  : 目录对象指针
    // Parameter: pTreeNode 指定节点，为NULL会自动取当前节点
    DirData* GetParentDir(CTreeNodeUIEx* pTreeNode);

    void FilteChilds( deque<NodeData*>* childs );    // 过滤子节点
    void OnRename();        // 改名
    void OnCopy();          // 复制
    void OnCut();           // 剪切
    void OnPaste();         // 粘贴

	bool OnCopyDirPath();	//复制路径
	bool OnParseDirPath();	//粘贴路径
    // 检查是否有SD卡
    bool CheckSDCard();

	//树节点删除操作
	void TreeDelete();
	
	//树节点重命名
	void TreeRename();

	bool GotoChild(wstring path);	//快速入口点击list的时候打开树

	bool QuickRefreshList();	//快速入口刷新list  

	ISVirtualNode SelectCurTreeNode();// 返回当前是在哪棵树

	void ListKeyDown(int key);	//list快捷键操作

	void TreeKeyDown(int key);	//tree快捷键操作

	void RefreshAppdata();		//刷新app数据

	void ListLetterLocation(int key);		//列表使用字母数字快速定位

    bool DoRefresh();

	bool ListHasSelectItem();	//文件列表是否有选中行

	
    void EnableListButton( bool enable );
    bool FocusInSearchEdit();

	// link 下面不能直接修改，跳到真实目录去修改
	DirData* LinkDataToDirData(NodeData* pParentNode);

	bool QuickGoto(wstring name);

	void ShowLoading(bool visible = true);

	virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

//	void DoInitRootMsg(const BaseParam* pParm);

	bool IsRootExist(DirData* rootData);
private:

	bool				_check_exit;		//设备是否退出
    FileManagerModel    *_model;             // 业务逻辑类
    CTreeUI*            _pTree;             // 文件树
    CListUI*            _pList;             // 文件列表
    wstring             _filterKey;         // 过滤关键字
    wstring             _strTempDragDir;    // 拖放使用的临时文件夹
    bool			    _bCut;              // 是否是剪切
    bool                _isFinish;          // 是否完成一次拷贝或剪切

    vector<wstring>     _pPasteNodes;       // 待粘贴的节点

	CTreeNodeUIEx *		_appTreeNode;		// 应用程序节点
	wstring				_pSelectDirName;    // 当前选中的目录名称
    wstring             _lastEditPath;      // 上次有效的路径地址
	
	wstring				_deviceId;			

};
