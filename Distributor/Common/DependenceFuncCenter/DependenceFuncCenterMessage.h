#pragma once

struct RequestFeedbackMsg : public BaseMessage<RequestFeedbackMsg>
{
};

struct FeedbackUiMsg : public BaseMessage<FeedbackUiMsg>
{

};

//���ͼ����Ϣ
struct IconNotifyMsg : public BaseMessage<IconNotifyMsg>
{
	WPARAM wParam;
	LPARAM lParam;
};


//���ͼ��˵���Ϣ
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

//���°汾
struct UpdateVersion_Msg : public BaseMessage<UpdateVersion_Msg>
{
};

//���°汾
struct RequestShowResourceLoadingMsg : public BaseMessage<RequestShowResourceLoadingMsg>
{
	bool bShow;
};

//�¹�����
struct WhatsNewDlgMsg : public BaseMessage<WhatsNewDlgMsg>
{
};