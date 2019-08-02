#pragma once

#include "WebUIReceiver.h"

class WebSoftCategory;

//˵����
//1.��Ҫ������Ϣ������ SetReceive
//2.�����������Ϣ����
//    ��Ϣ���ͣ�WebCategories_Click
//    wParam��  ����ID
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
	// Method:    ���÷�������
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
