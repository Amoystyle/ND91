#pragma once
// ���ⷴ�����ڹ�����

#include "common/DependenceFuncCenter/DependenceFuncCenterMessage.h"

class CWebIEUI;
class IWebObserver;
class FeedbackDlg : public CWindowUI,public IWebObserver
	, public BaseHandler<FeedbackUiMsg>
{
public:
	FeedbackDlg(void);
	virtual ~FeedbackDlg(void);

 	virtual void OnCreate();

	static DWORD WINAPI CheckAction( PVOID lpParam );

//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual wstring GetStyleID();


private:
	
	// Summary  : ��ȡ�û���ҳ��ʼ����
	// Returns  : bool
	// Parameter: LPCWSTR pszURL
	bool OnBeforeNavigate(LPCWSTR pszURL);

	// Summary  : ��ȡ�û���ҳ������ɶ���
	// Returns  : bool
	// Parameter: LPCWSTR pszURL
	bool OnDocumentComplete(IDispatch *pDisp, LPCWSTR pszURL);

	void OnNavigateComplete(IDispatch *pDispatch, LPCWSTR pszURL);

	virtual bool Handle( const FeedbackUiMsg* pMsg );

};
