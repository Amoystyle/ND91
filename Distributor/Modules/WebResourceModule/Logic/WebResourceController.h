#pragma once

class MainView;
class LeftView;
class TopView;
struct WebResourceInnserMsg;
struct ResponseUserLoginMsg;
struct QuitMsg;
class BaseCSLock;
struct CpaWebButtonStatusChangedMsg;
struct CpaWebIsCancelMsg;
struct GetJSInfoMsg;
struct CpaPackageMsg;
class CChkCpaDispatcher;
struct LocalCpaInfoInitFinishedMsg;
#include <map>

class WebResourceController
	:BaseHandler<WebResourceInnserMsg>
	,BaseHandler<ResponseUserLoginMsg>
	,BaseHandler<QuitMsg>
	,BaseGetData<GetLoginUserInfoMsg>
	,BaseCSLock
	,BaseHandler<CpaPackageMsg>
	,BaseHandler<DeviceChangeMsg>	
	,BaseGetData<CpaWebIsCancelMsg>
	,BaseHandler<CpaWebButtonStatusChangedMsg>
	,BaseHandler<LocalCpaInfoInitFinishedMsg>
	,BaseGetData<LocalCpaPathMsg>	
	,BaseHandler<AddCpaButtonStatusChangedMsg>
	,BaseGetData<CreateCpaDispatherMsg>
	,BaseHandler<AnalyUrlMsg>
{
	typedef void(WebResourceController::*MessageReceiveFun)(const WebResourceInnserMsg* msg);
	typedef std::map<WebResourceMsgType,MessageReceiveFun> MessageReceiverFun_MAP;
public:
	WebResourceController();
	~WebResourceController();

	bool Init(CContainerUI* pLeft, CContainerUI* pContainer);


	void Show();

	void Hide();

private:
	LeftView*        _pLeftView;
	MainView*        _pMainView;
	TopView*         _TopOperateView;
	NetNode          _CurNetNode;
	LoginStruct      _CurLoginUserInfo;
	bool			 _bIsLeftDataLoaded;	//标记左边树的数据是否加载完成
	MessageReceiverFun_MAP _mapMsgRecFuns;

	//int				 _nDownloadFlag;		//由于页面连接会跳转2次，整合成一次处理
	bool			 _bIsShow;
	int              _iAndroidDeviceCount;
	int              _iIOSDeviceCount;

	bool             _isReloadLeftNode;              //是否需要重新加载Cpa页面
	bool             _isRefreshCpaButtonStatus; //是否需要重新刷新CPA页面按钮状态

	CChkCpaDispatcher* _pCpaDispatcher;   
	map<CPaDispatcherID, CChkCpaDispatcher*>* _pOutCpaDispathers;//外部用到的dispatcher
	wstring guid;
	void Init();

	virtual bool Handle(const WebResourceInnserMsg* pMsg);
	virtual bool Handle(const ResponseUserLoginMsg* pMsg);
	virtual bool Handle(const QuitMsg* pMsg);
	virtual bool Handle(GetLoginUserInfoMsg* pMsg);
	virtual bool Handle(const CpaPackageMsg* pMsg);
	virtual bool Handle(const DeviceChangeMsg* pMsg);
	virtual bool Handle( CpaWebIsCancelMsg* pMsg );

	virtual bool Handle( const CpaWebButtonStatusChangedMsg* pMsg );

	virtual bool Handle( const LocalCpaInfoInitFinishedMsg* pMsg );

	virtual bool Handle( LocalCpaPathMsg* pMsg );	virtual bool Handle( const AddCpaButtonStatusChangedMsg* pMsg );

	virtual bool Handle( CreateCpaDispatherMsg* pMsg );

	virtual bool Handle(const AnalyUrlMsg* pMsg);

	void TopOperate(const WebResourceInnserMsg* msg);
	void LeftTreeLoaded(const WebResourceInnserMsg* msg);
	void LoadLeftTreeData(const WebResourceInnserMsg* msg);
	void AddLeftTreeData(const WebResourceInnserMsg* msg);
	void LeftTreeNodeChanged(const WebResourceInnserMsg* msg);
	void WebSearch(const WebResourceInnserMsg* msg);
	void DownloadResource(const WebResourceInnserMsg* msg);
	void HideTopSearch(const WebResourceInnserMsg* msg);
	void AddCpaTask(const WebResourceInnserMsg* msg);
	void AddCpaTask();

	void SetGuid();
	
};