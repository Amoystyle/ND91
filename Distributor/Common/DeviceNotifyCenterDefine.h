#pragma once
////����ԭ���豸��Ϣת�������ĵ���Ϣ////

//msg==USER_KEY, nParam��ʾkey code
struct NotifyCenterInfoMsg : BaseMessage<NotifyCenterInfoMsg>
{
	NotifyCenterInfoMsg()
	{
		nParam = -1;
	}
	MESSAGE_TYPE msg;
	int  nParam;
};
