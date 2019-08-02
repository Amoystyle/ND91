#pragma once

struct RequestFeedbackMsg : public BaseMessage<RequestFeedbackMsg>
{
};

struct FeedbackUiMsg : public BaseMessage<FeedbackUiMsg>
{

};

//点击图标消息
struct IconNotifyMsg : public BaseMessage<IconNotifyMsg>
{
	WPARAM wParam;
	LPARAM lParam;
};


//点击图标菜单消息
struct MenuNotifyMsg : public BaseMessage<MenuNotifyMsg>
{
	MenuNotifyMsg()
	{
		uMsgType  = 0;
		nMenutype = 0;
	}
	UINT uMsgType;
	int  nMenutype;
};

//更新版本
struct UpdateVersion_Msg : public BaseMessage<UpdateVersion_Msg>
{
};

//更新版本
struct RequestShowResourceLoadingMsg : public BaseMessage<RequestShowResourceLoadingMsg>
{
	bool bShow;
};

//新功能向导
struct WhatsNewDlgMsg : public BaseMessage<WhatsNewDlgMsg>
{
};