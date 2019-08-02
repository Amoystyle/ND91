#pragma once

enum ConnectGuideStep
{
	ConnectGuideStep_LinkIn,
	ConnectGuideStep_ConnectInit,
	ConnectGuideStep_ConnectWaitWindow,
	ConnectGuideStep_DriverInstall,
	ConnectGuideStep_Connecting,
	ConnectGuideStep_Error,
};

enum ConnectGuideAction
{
	ConnectGuideAction_ConnectDevice,
};

struct ConnectGuideInnerMsg : BaseMessage<ConnectGuideInnerMsg>
{
	ConnectGuideAction nAction;
	WPARAM wParam;
	LPARAM lParam;
};