#pragma once

enum Device_MsgType
{
	Device_MsgType_TreeSelectChange,
	Device_MsgType_Refresh,
};

struct DeviceInnerMsg : BaseMessage<DeviceInnerMsg>
{
	Device_MsgType type;
	WPARAM wParam;
	LPARAM lParam;
};