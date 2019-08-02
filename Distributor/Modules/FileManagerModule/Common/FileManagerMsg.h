#pragma once
#include "Singleton.h"
#include "Base/BaseModule.h"
#include "Base/SourceModule.h"
#include "Common/WindowMessageDefine.h"
#include "Message/Message.h"


struct InitRootMsg : BaseMessage<InitRootMsg>
{
	wstring sn;
};

struct RefreshNodeMsg : BaseMessage<RefreshNodeMsg>
{
	wstring _str;
	int		_int;
};

struct RefreshAppMsg : BaseMessage<RefreshAppMsg>
{
	wstring sn;
};

struct DragdropFileMsg : BaseMessage<DragdropFileMsg>
{
	wstring sn;
	vector<wstring>* files;
	void* pDir;
};