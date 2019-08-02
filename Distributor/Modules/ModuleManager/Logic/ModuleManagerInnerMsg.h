#pragma once

enum ModuleManager_Action
{
	ModuleManager_Action_LoadResourceCompleted,
	ModuleManager_Action_LoadModules,
	ModuleManager_Action_SelectModule,
};

struct ModuleManagerInnerMsg : BaseMessage<ModuleManagerInnerMsg>
{
	ModuleManager_Action nAction;
	WPARAM wParam;
	LPARAM lParam;
};