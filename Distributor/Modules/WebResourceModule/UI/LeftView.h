#pragma once
#include "WebResourceManager.h"

class LeftView
	: public BaseView
{
public:
	LeftView(CContainerUI* pParent=NULL);
	~LeftView();

	// 消息映射
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"TreeContainer", OnWebTreeSelChanged);//左边树选中事件
	UIEND_MSG_MAP

	virtual wstring GetStyleID();
	void OnCreate();
	void Init(CContainerUI* pContainer);

	//s:创建节点
	//p:网络节点信息
	void AddNode(NetSite& netSite);
	void OnFinish();
	void ClearLeftTree();//清除左边树
	//点击搜索，选择搜索节点
	void SelectSearchNode(NetNode& stNetNode);

	void Show();
private:
	bool OnWebTreeSelChanged(TNotifyUI* pNotify);

	//s:将“免越狱软件”放到最前面一个
	bool PopJailBreakSite(NetSite& stNetSite);
	//s:创建父节点
	void CreateGroupTreeNode(CTreeUI* pTree,NetSite& stNetSite);
	//s:创建子节点
	CTreeNodeUI* CreateChildTreeNode(NetNode& stNetSite, NetEnumPhoneOS platForm);

	//s:选中默认节点
	void SelectFaultNode();

	//s:查找树当前选中在第几个节点
	int  FindCurIndex(CTreeNodeUI* pCurNode);
private:
	CTreeUI*       _pTree;
	CAnimationUI*  _pAnimation;
	CLabelUI*      _pLabel;
	CControlUI*    _pNewUI;
	int            _nCurTreeIndex;
	NetEnumPhoneOS _ePlatform;
	bool           _bInit;
};