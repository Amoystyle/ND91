#pragma once

enum NotifyIcon
{
	ICON_SUCCESS,
	ICON_WARNING
};

struct  ProgressShowMsg : BaseMessage<ProgressShowMsg>
{
	int RequestID;
	wstring deviceId;		//进度条并非wnd模态形式时需要填写   因为有可能同时存在多个进度窗口
};

struct ProgressShowExistMsg : BaseMessage<ProgressShowExistMsg>
{
	wstring name;		//文件 存在重复，
};

struct ProgressShowExistReturnMsg : BaseMessage<ProgressShowExistReturnMsg>
{
	int return_value;	//文件重复窗用户选择返回值
};

struct ProgressMsg : BaseMessage<ProgressMsg>
{
	wstring subText;		//子进度显示
	wstring mainText;		//主进度显示
	int		subPos;			//子进度
	int		mainPos;		//主进度
	bool	wndHide;		//关闭进度窗口
};


struct ProgressBtnMsg : BaseMessage<ProgressBtnMsg>
{
	bool	continueBtn;	//跳过按钮
	bool	cancelBtn;		//终止按钮
};




struct InputDialogMsg : BaseMessage<InputDialogMsg>
{
	wstring strMsg;
	wstring strTitle;
	wstring strDefault;
	int maxChar;
	HANDLE event_lock;
	HWND hOwnerWnd;
};


struct CommonUIReturnMsg : BaseMessage<CommonUIReturnMsg>
{
	HANDLE event_lock;
};

struct NoticeDialogMsg : BaseMessage<NoticeDialogMsg>
{
	wstring strMsg;
	wstring strIcon;
	int nDelayTime;
	HWND hOwnerWnd;
};

struct MessageDialogMsg : BaseMessage<MessageDialogMsg>
{
	wstring strMsg;
	wstring strTitle;
	wstring strIcon;
	wstring tip;
	wstring okText;
	HWND hOwnerWnd;
};

struct ConfirmDialogMsg : BaseMessage<ConfirmDialogMsg>
{
	wstring strMsg;
	wstring strTitle;
	wstring strIcon;
	int height;
	int flag;
	bool isWarnning;		//是否为警告对话框
	HANDLE event_lock;
	HWND hOwnerWnd;
};

struct GetExistActionMsg : BaseMessage<GetExistActionMsg>
{
	wstring name;
	HANDLE event_lock;
	HWND hOwnerWnd;
};