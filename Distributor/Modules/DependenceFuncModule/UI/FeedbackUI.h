#pragma once
// 问题反馈窗口管理类

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
	
	// Summary  : 获取用户网页起始操作
	// Returns  : bool
	// Parameter: LPCWSTR pszURL
	bool OnBeforeNavigate(LPCWSTR pszURL);

	// Summary  : 获取用户网页操作完成动作
	// Returns  : bool
	// Parameter: LPCWSTR pszURL
	bool OnDocumentComplete(IDispatch *pDisp, LPCWSTR pszURL);

	void OnNavigateComplete(IDispatch *pDispatch, LPCWSTR pszURL);

	virtual bool Handle( const FeedbackUiMsg* pMsg );

};
