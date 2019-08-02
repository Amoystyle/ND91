#pragma once

class PcSuitFuncUIItem
	: CCompositeUI
	, public BaseHandler<ReponseDownloadReportMsg>
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"PCSuitFuncItem", OnPCSuitFuncItemClick)
	UIEND_MSG_MAP

	PcSuitFuncUIItem(CControlUI* ControlUI, FunctionItem& FuncItem, EnumPhoneOS CurPlatform);
	~PcSuitFuncUIItem(void);

	void ReportProgress(bool bDownloadSuccess, UINT unAllSize, UINT unCurDownSize);

	UINT GetTaskID() { return _unTaskID;};

	bool Handle(const ReponseDownloadReportMsg* pMsg);
private:
	void Init();

	//点击第三方功能大全
	bool OnPCSuitFuncItemClick(TNotifyUI* pNotify = NULL);

private:
	CLayoutUI* _pLayoutUI;
	FunctionItem _FuncItem;

	EnumPhoneOS _CurPlatform;
	UINT _unTaskID;

	wstring  _wsSavePath;

	CButtonUI*  _pBtnUI;

	ImageStyle* _pProgressUIBG;
	ImageStyle* _pProgressUI;
};
