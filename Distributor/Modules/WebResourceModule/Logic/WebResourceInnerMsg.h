#pragma once
#include "ChkCpaTask.h"
enum WebResourceMsgType
{
	//Summary
	//
	WebResourceMsgTypeBegin = 0,
	WebResourceMsg_UrlChanged = WebResourceMsgTypeBegin,	//�ı�ڵ㣬lParam=����ΪNetNode,�ɽ����������ͷ�
	WebResourceMsg_DownloadResource,						//��Դ����,wParam����ΪAnalysisData*,�ɽ����������ͷ�,strParam=L��CPA����ʾcpa��,nParam��ʾ�ӿڶ����ƽ̨
	WebResourceMsg_LeftLoad,								//������ļ��� ��lParam=��ʾƽ̨������=ENUM_PLATFORM����wParam��ʾ���ࣨ����=NetEnumAppSort�����磺Ӧ�ã���Ϸ��
	WebResourceMsg_LeftLoad_UI,								//֪ͨUI�����Ӧ�ڵ� lParam=��ʾ����ϵͳ������=NetEnumPhoneOS����wParam��ʾ���ࣨ����=NetEnumAppSort�����磺Ӧ�ã���Ϸ��
	WebResourceMsg_LeftTreeLoaded,							//������ڵ����Դ������� lParam=��ʾ����ϵͳ������=NetEnumPhoneOS����wParam��ʾ���ࣨ����=NetEnumAppSort�����磺Ӧ�ã���Ϸ��
	WebResourceMsg_TopOperate,								//�ϲ��ֲ�����lParam=��ʾƽ̨������=ENUM_PLATFORM��,wParam=������=ENUM_URL_OPREATE)
	WebResourceMsg_TopSearch,								//�ϲ���������strParam=��������, lParam=��ʾƽ̨������=ENUM_PLATFORM��
	WebResourceMsg_HideTopSearch,							//����TOP�������������ܣ� wParam = 1 ���أ�=0 ��ʾ��
	WebResourceMsg_ClickGotoDes,							//���һ�����ػᴥ����������  Ϊ�˽������������ô˱�Ƿ�����Ϣ������DUI�°��˱�ǾͲ�����
	WebResourceMsg_AddCpaTask,                              //Cpa��ť״̬�ı��������
	WebResourceMsgTypeEnd,
};

//�ֻ�����
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

//������Դģ�����
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
	map<string,wstring>*  _map;// ˭��˭ɾ��
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