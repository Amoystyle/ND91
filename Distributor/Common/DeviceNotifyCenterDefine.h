#pragma once
////定义原来设备消息转发类中心的消息////

//msg==USER_KEY, nParam表示key code
struct NotifyCenterInfoMsg : BaseMessage<NotifyCenterInfoMsg>
{
	NotifyCenterInfoMsg()
	{
		nParam = -1;
	}
	MESSAGE_TYPE msg;
	int  nParam;
};
