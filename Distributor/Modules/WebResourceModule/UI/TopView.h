#pragma once

using namespace std;

class TopView
	: public BaseView
{
public:
	// ��Ϣӳ��
	UIBEGIN_MSG_MAP
	EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"WebRes_PhoneType_Tab", OnWebPlatFormSelChanged);//�ұ�tabѡ���¼�
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_goback", OnBtnCk);//���˰�ť��Ӧ�¼�
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_goforword",OnBtnCk); //ǰ����ť��Ӧ�¼�
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_gohome",OnBtnCk);//�ص���ҳ��ť��Ӧ�¼�
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_refresh",OnRefresh);//ˢ�°�ť��Ӧ�¼�
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"web_search",OnSearch);//������ť��Ӧ�¼�

	EVNET_ID_HANDLER(UINOTIFY_EN_LIMIT,L"web_search_edit", OnSearch);//�༭���������
	EVNET_ID_HANDLER(UINOTIFY_EN_KEYDOWN,L"web_search_edit", OnSearch);//�༭���������
	EVNET_ID_HANDLER(UINOTIFY_EN_CHANGE,L"web_search_edit", OnSearchEditChange);
	EVNET_ID_HANDLER(UINOTIFY_CLICK,L"web_searchBox_Clear", OnSearchClear);//��ձ༭����������
	EVNET_ID_HANDLER(UINOTIFY_CLICK,L"refresh", OnRefresh);//ˢ��
	UIEND_MSG_MAP

	TopView(CContainerUI* pParent=NULL);
	~TopView(void);

	void OnCreate();

	wstring GetStyleID();

	bool OnRefresh(TNotifyUI* pNotify);
	//���������ڵ㣬��Ҫ���ڵ���Ϣ����
	void ClickLeftTree(NetNode& stNetNode);
	//�Ƿ�������������
	void HideSearch(bool bHide=true);

private:

	bool OnWebPlatFormSelChanged(TNotifyUI* pNotify);

	//�����ť�¼�
	bool OnBtnCk(TNotifyUI* pNotify);

	//����
	bool OnSearch(TNotifyUI* pNotify);

	bool OnSearchClear(TNotifyUI* pNotify);

	bool OnSearchEditChange(TNotifyUI* pNotify);
	//
	bool OnBeforeNavigate( LPCWSTR pszURL);

	bool OnDocumentComplete(IDispatch *pDisp, LPCWSTR pszURL);


private:
	ENUM_PLATFORM _ePlatform;
	NetNode       _stNetNode;
};
