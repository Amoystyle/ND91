#pragma once
#include "ui/WebSoftUI/WebUIReceiver.h"
#include "UI/TabItem.h"

class WebSoftControlUI;
class WebSoftMainUI: public BaseView,public WebUIReceiver
{
public:
	enum PlatForm
	{
		Web_Android,
		Web_iPhone,
		Web_iPad
	};

	enum event
	{
		WebSoftMainUI_TabAndroid_Click,
	};

	WebSoftMainUI(HWND hWnd);
	~WebSoftMainUI(void);

	UIBEGIN_MSG_MAP
//		EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"WebSoft_PhoneType_Tab", OnWebPageSelChanged);
//		EVNET_ID_HANDLER(UINOTIFY_EN_LIMIT,L"web_search_edit", OnSearch);//�༭���������
//		EVNET_ID_HANDLER(UINOTIFY_EN_KEYDOWN,L"web_search_edit", OnSearch);//�༭���������
//		EVNET_ID_HANDLER(UINOTIFY_EN_CHANGE,L"web_search_edit", OnSearchEditChange);
//		EVNET_ID_HANDLER(UINOTIFY_CLICK,L"web_searchBox_Clear", OnSearchClear);//��ձ༭����������
// 		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_gohome",OnBtnGoHome);//�ص���ҳ��ť��Ӧ�¼�
// 		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_refresh",OnRefresh);//ˢ�°�ť��Ӧ�¼�
//		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"web_search",OnSearch);//������ť��Ӧ�¼�
//		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_goback",OnGoBack);

	UIEND_MSG_MAP

	void SetPlatform(bool IsIpad);

//	int GetSelPlatformIndex();

	wstring GetStyleID();

	void Init(CContainerUI* pContainer);

	
public:
	void OnCreate();

	bool OnWebPageSelChanged(int sel);	//ҳ���л�

	bool OnSearch(wstring searchbuf);		//����

//	bool OnSearchClear(TNotifyUI* pNotify);

//	bool OnSearchEditChange(TNotifyUI* pNotify);

	bool OnBtnGoHome();			//��ҳ

	bool OnRefresh();		//ˢ��

	bool OnGoBack();		//����


private:

	WebSoftControlUI*  _piPhoneWebSoftUI;
	WebSoftControlUI*  _piPadWebSoftUI;
	PlatForm           _curplat;

};
