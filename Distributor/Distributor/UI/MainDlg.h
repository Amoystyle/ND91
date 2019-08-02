#pragma once

/************************************************************************/
/* 主窗口类：仅包含窗口类的消息处理，不包含任何具体显示内容
/*              具体显示内容在 MainController 类
/************************************************************************/

class MainDlg 
	: public CWindowUI
	, public INotifyUI
	, public BaseCSLock
	, public BaseHandler<InvalidateMainMsg>
	, public BaseHandler<RequestShowResourceLoadingMsg>
{
public:
	UIBEGIN_MSG_MAP
	UIEND_MSG_MAP
	static MainDlg* GetInstance();

    // Summary  : 初始化
    void Init();

	virtual bool OnClosing(WPARAM wParam, LPARAM lParam);

	virtual void OnMinimize();
private:

	MainDlg(void);
	~MainDlg(void);

	void LoadModules();

	CRadioUI* InsertTopButton(CContainerUI* bar, tstring id, tstring text, tstring icon);

	virtual bool OnEscape(){ return false;}

	// 让窗口显示为直角
	virtual void OnSetWindowRgn() {}

	// 窗口第一次显示
	virtual void OnInitShow();
	
	void OnClose();

    // 主线程消息循环
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	bool Handle(const InvalidateMainMsg* pMsg);

	virtual bool Handle( const RequestShowResourceLoadingMsg* pMsg );


	
private:
    bool _bInitShow;
	SourceModule* _pSelectModule;
	static MainDlg* _pInstance;
};
