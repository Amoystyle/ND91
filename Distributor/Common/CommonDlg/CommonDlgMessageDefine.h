#pragma once

enum CommDlgType
{
	CommDlgTypeBegin,
	// <summary>
	//  �Ի��������ַ���	
	// </summary>
	CommDlgTypeInputDialog = CommDlgTypeBegin,
	// <summary>
	//  ������ʾ�Ի����Զ���ʧ��	
	// </summary>
	CommDlgTypeNoticeDialog,
	// <summary>
	//  ��Ϣ�Ի���
	// </summary>
	CommDlgTypeMessageDialog,
	// <summary>
	//  ȷ�϶Ի���
	// </summary>
	CommDlgTypeMessageConfirmDialog,
	// <summary>
	//  ȷ�϶Ի��򣨾����ͣ�
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
	LPARAM lParam;//Ϊ��Ӧ�ĵ�DialogParam�ṹ�壬�ɽ������ͷ�
};

struct ConfireDlgMsg : public BaseMessage<ConfireDlgMsg>
{
	CommDlgType commonDlgType;
	WPARAM wParam;
	LPARAM lParam;//Ϊ��Ӧ�ĵ�DialogParam�ṹ�壬�ɽ������ͷ�
};