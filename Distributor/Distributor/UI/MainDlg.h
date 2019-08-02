#pragma once

/************************************************************************/
/* �������ࣺ���������������Ϣ�����������κξ�����ʾ����
/*              ������ʾ������ MainController ��
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

    // Summary  : ��ʼ��
    void Init();

	virtual bool OnClosing(WPARAM wParam, LPARAM lParam);

	virtual void OnMinimize();
private:

	MainDlg(void);
	~MainDlg(void);

	void LoadModules();

	CRadioUI* InsertTopButton(CContainerUI* bar, tstring id, tstring text, tstring icon);

	virtual bool OnEscape(){ return false;}

	// �ô�����ʾΪֱ��
	virtual void OnSetWindowRgn() {}

	// ���ڵ�һ����ʾ
	virtual void OnInitShow();
	
	void OnClose();

    // ���߳���Ϣѭ��
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	bool Handle(const InvalidateMainMsg* pMsg);

	virtual bool Handle( const RequestShowResourceLoadingMsg* pMsg );


	
private:
    bool _bInitShow;
	SourceModule* _pSelectModule;
	static MainDlg* _pInstance;
};
