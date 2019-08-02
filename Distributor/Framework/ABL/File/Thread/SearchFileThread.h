#pragma once
#include "Common/BaseAblThread.h"
#include "Common/UserMessage/abl/AblFileMessage.h"

class SearchFileThread : public BaseAblThread , private BaseCSLock
{
public:
    // 构造函数 threadID线程唯一标识
    SearchFileThread( string threadID, int nRequestID, wstring deviceID );
    ~SearchFileThread();

	RequestSearchFileMsg* _requestSearchFileMsg;
	void Pause();
	void Resume();
protected:
    // 执行具体的操作
    virtual bool Action(DeviceData* pDevice);
	int SearchItem(NodeData* cur_node, FileItem_ABL cur_item);
	tstring _key;
	HANDLE _waitEvent;
	HANDLE _events[3];	
};