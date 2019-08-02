#pragma once
#include "UI/WebIEUI.h"
#include <string.h>
#include <map>
using namespace std;

#include "WebSoftMainUI.h"

struct NetNode;
typedef struct WebIeUIStr
{
	CWebIEUI* web;
	NetNode node;
}WebIeUIStr;


class MainView
	: public BaseView
	,public IWebObserver
	,public BaseHandler<CpaWebButtonStatusChangedMsgEx>
{
public:
	UIBEGIN_MSG_MAP
//		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Button_MessageTest", OnMessageText)
	UIEND_MSG_MAP
	MainView();
	~MainView();

	void Load(NetNode node );	//载入

	void OnCreate();

	void Init(CContainerUI* pContainer);	//初始化

	void Prve();				//向前

	void Next();				//向后

	void Refresh();				//刷新

	void Home(NetNode node);	//主页

	void Search(NetNode node);	//搜索接口

	void ShowWebResourceMain(ENUM_PLATFORM plat);	//显示正版体验

	void Show();	//可显示ie控件

	void Hide();	//隐藏IE控件

	wstring GetCpaHashValue();

	void SetButtonStatus(VARIANT *param, int cArgs);

	//正版体验
	void WebSoftNext();			//正版体验向后

	void WebSoftRefresh();		//正版体验刷新

	void WebSoftHome();			//正版体验主页

	void WebSoftSearch(wstring searchbug);		//正版体验搜索

protected:
	virtual wstring GetStyleID();

private:
	bool OnBeforeNavigate(LPCWSTR pszURL);

	bool OnDocumentComplete(IDispatch *pDisp, LPCWSTR pszURL);

	void OnNavigateComplete(IDispatch *pDispatch, LPCWSTR pszURL);

	void HideWebControl();	//隐藏所有已经生成的ie页面
	
	CWebIEUI* ShowNewIE(wstring url);	//显示新的ie页面

	void Loading(bool flag);	//显示或隐藏载入中

	virtual bool Handle( const CpaWebButtonStatusChangedMsgEx* pMsg );

	//检查是否重新下载
	bool CheckReDownload(wstring wsUrl, int nPlatForm, vector<wstring>& vec_Hash);

private:
	map<wstring,WebIeUIStr> _webIeMap;

	CWebIEUI*		_webIeUi;	//当前激活的ie控件

	WebSoftMainUI*	_webMain;		//正版体验

	bool			_isShowIe;		//是否可以显示IE控件

	int				_nDownloadFlag;		//由于页面连接会跳转2次，整合成一次处理

};