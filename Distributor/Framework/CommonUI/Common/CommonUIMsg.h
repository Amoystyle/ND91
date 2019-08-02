#pragma once

enum NotifyIcon
{
	ICON_SUCCESS,
	ICON_WARNING
};

struct  ProgressShowMsg : BaseMessage<ProgressShowMsg>
{
	int RequestID;
	wstring deviceId;		//����������wndģ̬��ʽʱ��Ҫ��д   ��Ϊ�п���ͬʱ���ڶ�����ȴ���
};

struct ProgressShowExistMsg : BaseMessage<ProgressShowExistMsg>
{
	wstring name;		//�ļ� �����ظ���
};

struct ProgressShowExistReturnMsg : BaseMessage<ProgressShowExistReturnMsg>
{
	int return_value;	//�ļ��ظ����û�ѡ�񷵻�ֵ
};

struct ProgressMsg : BaseMessage<ProgressMsg>
{
	wstring subText;		//�ӽ�����ʾ
	wstring mainText;		//��������ʾ
	int		subPos;			//�ӽ���
	int		mainPos;		//������
	bool	wndHide;		//�رս��ȴ���
};


struct ProgressBtnMsg : BaseMessage<ProgressBtnMsg>
{
	bool	continueBtn;	//������ť
	bool	cancelBtn;		//��ֹ��ť
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
	bool isWarnning;		//�Ƿ�Ϊ����Ի���
	HANDLE event_lock;
	HWND hOwnerWnd;
};

struct GetExistActionMsg : BaseMessage<GetExistActionMsg>
{
	wstring name;
	HANDLE event_lock;
	HWND hOwnerWnd;
};