#pragma once
#include "WebResourceManager.h"

class LeftView
	: public BaseView
{
public:
	LeftView(CContainerUI* pParent=NULL);
	~LeftView();

	// ��Ϣӳ��
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"TreeContainer", OnWebTreeSelChanged);//�����ѡ���¼�
	UIEND_MSG_MAP

	virtual wstring GetStyleID();
	void OnCreate();
	void Init(CContainerUI* pContainer);

	//s:�����ڵ�
	//p:����ڵ���Ϣ
	void AddNode(NetSite& netSite);
	void OnFinish();
	void ClearLeftTree();//��������
	//���������ѡ�������ڵ�
	void SelectSearchNode(NetNode& stNetNode);

	void Show();
private:
	bool OnWebTreeSelChanged(TNotifyUI* pNotify);

	//s:������Խ��������ŵ���ǰ��һ��
	bool PopJailBreakSite(NetSite& stNetSite);
	//s:�������ڵ�
	void CreateGroupTreeNode(CTreeUI* pTree,NetSite& stNetSite);
	//s:�����ӽڵ�
	CTreeNodeUI* CreateChildTreeNode(NetNode& stNetSite, NetEnumPhoneOS platForm);

	//s:ѡ��Ĭ�Ͻڵ�
	void SelectFaultNode();

	//s:��������ǰѡ���ڵڼ����ڵ�
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