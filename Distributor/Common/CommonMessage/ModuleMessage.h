#pragma once

enum SourceModuleType
{
	SourceModuleType_DeviceInfo,
	SourceModuleType_InstallCase,
	SourceModuleType_WebResource,
	SourceModuleType_TaskCenter,
};

struct NotifySourceModule : public BaseMessage<NotifySourceModule>
{
	SourceModuleType type;
};