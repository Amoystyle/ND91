#pragma once
#include "ChkCpaTask.h"
enum WebResourceMsgType
{
	//Summary
	//
	WebResourceMsgTypeBegin = 0,
	WebResourceMsg_UrlChanged = WebResourceMsgTypeBegin,	//改变节点，lParam=类型为NetNode,由接受者主动释放
	WebResourceMsg_DownloadResource,						//资源下载,wParam类型为AnalysisData*,由接受者主动释放,strParam=L“CPA”表示cpa包,nParam表示接口定义的平台
	WebResourceMsg_LeftLoad,								//左边树的加载 ：lParam=表示平台（类型=ENUM_PLATFORM），wParam表示分类（类型=NetEnumAppSort），如：应用，游戏等
	WebResourceMsg_LeftLoad_UI,								//通知UI添加相应节点 lParam=表示操作系统（类型=NetEnumPhoneOS），wParam表示分类（类型=NetEnumAppSort），如：应用，游戏等
	WebResourceMsg_LeftTreeLoaded,							//左边树节点的资源加载完成 lParam=表示操作系统（类型=NetEnumPhoneOS），wParam表示分类（类型=NetEnumAppSort），如：应用，游戏等
	WebResourceMsg_TopOperate,								//上部分操作：lParam=表示平台（类型=ENUM_PLATFORM）,wParam=（类型=ENUM_URL_OPREATE)
	WebResourceMsg_TopSearch,								//上部分搜索：strParam=搜索内容, lParam=表示平台（类型=ENUM_PLATFORM）
	WebResourceMsg_HideTopSearch,							//隐藏TOP操作的搜索功能， wParam = 1 隐藏，=0 显示。
	WebResourceMsg_ClickGotoDes,							//点击一次下载会触发两次下载  为了解决这个问题是用此标记发送消息，更换DUI新版后此标记就不用了
	WebResourceMsg_AddCpaTask,                              //Cpa按钮状态改变任务添加
	WebResourceMsgTypeEnd,
};

//手机类型
enum E_PHONE_TYPE
{
	E_None = 0,
	E_IPHONE,
	E_WINDOW_MOBILE,
	E_SYMBIAN_S60,
	E_ANDROID,
	E_ANDROID_OMS,
	E_M8,
	E_IPAD,
	E_WP7,
	E_ANDROID_PAD,
	E_ANDROID_TV,
};

//网络资源模块相关
struct WebResourceInnserMsg : public BaseMessage<WebResourceInnserMsg>
{
	WebResourceInnserMsg()
	{
		nParam=0;
		pCpaInfo = NULL;
	}
	WebResourceMsgType type;
	WPARAM wParam;
	LPARAM lParam;
	void* pCpaInfo;
};


struct WebSoftMsg : BaseMessage<WebSoftMsg>
{
	UINT			message;
	int				platType;
	int				f_id;
	wstring			path;
	int				id;
	bool			apptype;
};

struct DeleteLocalCpa : BaseMessage<DeleteLocalCpa>
{
	map<string,wstring>*  _map;// 谁用谁删除
};

struct CCallJSStruct
{
	 IDispatch* pDispatch;
	 LPOLESTR pMethod;
	 VARIANT pVarResult;
	 VARIANT *ps;
	 int cArgs;
	 CCallJSStruct()
	 {
		pDispatch = NULL;
		pMethod =_T("");
		ps = NULL;
		cArgs = 0;
	 }
};

struct CpaTaskStatueMsg : BaseMessage<CpaTaskStatueMsg>
{
	CChkCpaTask* task;
	TaskStatus oldStatus;
	TaskStatus newStatus;
};