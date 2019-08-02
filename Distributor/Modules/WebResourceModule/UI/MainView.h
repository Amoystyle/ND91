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

	void Load(NetNode node );	//����

	void OnCreate();

	void Init(CContainerUI* pContainer);	//��ʼ��

	void Prve();				//��ǰ

	void Next();				//���

	void Refresh();				//ˢ��

	void Home(NetNode node);	//��ҳ

	void Search(NetNode node);	//�����ӿ�

	void ShowWebResourceMain(ENUM_PLATFORM plat);	//��ʾ��������

	void Show();	//����ʾie�ؼ�

	void Hide();	//����IE�ؼ�

	wstring GetCpaHashValue();

	void SetButtonStatus(VARIANT *param, int cArgs);

	//��������
	void WebSoftNext();			//�����������

	void WebSoftRefresh();		//��������ˢ��

	void WebSoftHome();			//����������ҳ

	void WebSoftSearch(wstring searchbug);		//������������

protected:
	virtual wstring GetStyleID();

private:
	bool OnBeforeNavigate(LPCWSTR pszURL);

	bool OnDocumentComplete(IDispatch *pDisp, LPCWSTR pszURL);

	void OnNavigateComplete(IDispatch *pDispatch, LPCWSTR pszURL);

	void HideWebControl();	//���������Ѿ����ɵ�ieҳ��
	
	CWebIEUI* ShowNewIE(wstring url);	//��ʾ�µ�ieҳ��

	void Loading(bool flag);	//��ʾ������������

	virtual bool Handle( const CpaWebButtonStatusChangedMsgEx* pMsg );

	//����Ƿ���������
	bool CheckReDownload(wstring wsUrl, int nPlatForm, vector<wstring>& vec_Hash);

private:
	map<wstring,WebIeUIStr> _webIeMap;

	CWebIEUI*		_webIeUi;	//��ǰ�����ie�ؼ�

	WebSoftMainUI*	_webMain;		//��������

	bool			_isShowIe;		//�Ƿ������ʾIE�ؼ�

	int				_nDownloadFlag;		//����ҳ�����ӻ���ת2�Σ����ϳ�һ�δ���

};