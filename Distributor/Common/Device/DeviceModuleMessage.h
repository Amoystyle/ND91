#pragma once

struct NotifyRefreshModuleMsg : public BaseMessage<NotifyRefreshModuleMsg>
{
	wstring deviceID;
};
