#pragma once

#include "WebUIReceiver.h"

//˵����
//1.��Ҫ������Ϣ������ SetReceive
//2.�����������Ϣ����
//    ��Ϣ���ͣ�WebAppListFilter_Click
//    wParam��  Filter
//3.Style:WebAppFilterCtrl
class WebAppListFilterControl
	: public BaseView
	, public WebUIReceiver
{
public:
	enum event
	{
		//���˰�ťClick
		WebAppListFilter_Click,
	};
	enum Filter
	{
		//���ش���
		DownloadLevel,
		//�۸�ߵ�
		PriceLevel,
		//�汾����
		VersionLevel,
	};

	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_downloadlevel", OnBtnClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_pricelevel", OnBtnClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_versionlevel", OnBtnClick);
	UIEND_MSG_MAP

public:
	WebAppListFilterControl(HWND hWnd);
	~WebAppListFilterControl(void);

	wstring GetStyleID();

	void Init(CContainerUI* pContainer);

private:
	bool OnBtnClick(TNotifyUI* pNotify);
	map<Filter,bool> _updowns;
};
