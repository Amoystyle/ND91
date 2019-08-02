#pragma once

#include "WebUIReceiver.h"

class WebSoftCategory;

//说明：
//1.需要设置消息接收者 SetReceive
//2.接收者添加消息处理
//    消息类型：WebCategories_Click
//    wParam：  分类ID
//3.Style:WebAppCategoryCtrl
class WebCategoriesControl
	: public BaseView
	, public WebUIReceiver
{
public:
	enum event
	{
		WebCategories_Click
	};

	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Btn_WebSoftCategory", OnCateBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"WebCategory_Combo", OnCateComboChecked)
	UIEND_MSG_MAP

	WebCategoriesControl(HWND hWnd,bool isTop);
	~WebCategoriesControl(void);

public:
	//************************************
	// Method:    设置分类内容
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: vector<WebSoftCategory * > * pData
	//************************************
	void SetCategories(vector<WebSoftCategory*>* pData, int selID=0);

	void SetTopSelCategory(int selID);
	void OnRectChanged();

	wstring GetStyleID();

	void Init(CContainerUI* pContainer);
private:
	void OnCreate();
	bool OnCateBtnClick(TNotifyUI* pNotify);
	bool OnCateComboChecked(TNotifyUI* pNotify);
private:
	vector<CButtonUI*> _pBtnList;
	bool _isTop;
	bool _bISSetCategories;

	CLayoutUI* _pTopModeLayout;
	CLayoutUI* _pBottomModeLayout;

	CLayoutUI* _pContainer;
	CComboBoxUI* _pCategory_Combo;

	int _curID;
	int _visibleBtnCount;
};
