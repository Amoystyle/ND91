#pragma once

enum CommDlgType
{
	CommDlgTypeBegin,
	// <summary>
	//  对话框：输入字符串	
	// </summary>
	CommDlgTypeInputDialog = CommDlgTypeBegin,
	// <summary>
	//  浮出提示对话框（自动消失）	
	// </summary>
	CommDlgTypeNoticeDialog,
	// <summary>
	//  消息对话框
	// </summary>
	CommDlgTypeMessageDialog,
	// <summary>
	//  确认对话框
	// </summary>
	CommDlgTypeMessageConfirmDialog,
	// <summary>
	//  确认对话框（警告型）
	// </summary>
	CommDlgTypeMessageConfirmWarnDialog,
	CommDlgTypeEnd
};

struct DialogParam
{
	DialogParam()
	{
		strMsg = L"";
		strTitle = L"";
		strIcon = L"";
	}
	
	wstring strMsg;
	wstring strTitle;
	wstring strIcon;
};

struct InputDialogParam : public DialogParam
{
	InputDialogParam()
	{
		strDefault = L"";
		bCancel = false;
		maxChar = -1;
		strReturnValue = L"";
	}
	
	wstring strDefault;
	bool bCancel;
	int maxChar;
	wstring strReturnValue;
};

struct NoticeDialogParam: public DialogParam
{
	NoticeDialogParam()
	{
		nDelayTime = 2000;
	}
	int nDelayTime;
};

struct MessageDialogParam: public DialogParam
{
	MessageDialogParam()
	{
		tip = L"";
		okText = L"";
	}
	wstring tip;
	wstring okText;
};

struct ConfirmDialogParam: public DialogParam
{
	ConfirmDialogParam()
	{
		height = 0;
		flag = 0;
		bReturnValue = false;
	}
	int height;
	int flag;
	bool bReturnValue;
};

struct CommonDlgMsg : public BaseMessage<CommonDlgMsg>
{
	CommDlgType commonDlgType;
	WPARAM wParam;
	LPARAM lParam;//为相应的的DialogParam结构体，由接收者释放
};

struct ConfireDlgMsg : public BaseMessage<ConfireDlgMsg>
{
	CommDlgType commonDlgType;
	WPARAM wParam;
	LPARAM lParam;//为相应的的DialogParam结构体，由接收者释放
};