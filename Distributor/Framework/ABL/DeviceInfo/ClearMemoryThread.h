#pragma once
#include "Common/DeviceBaseThread.h"

class ClearMemoryThread : public DeviceBaseThread
{
public:
    // 构造函数 threadID线程唯一标识
    ClearMemoryThread( string threadID, wstring deviceID );
	~ClearMemoryThread();

private:
	virtual unsigned long Main();

	void ClearMemory();

	// Summary  : 获取要清理的文件夹
	// Parameter: wstring 设备ID
	// Parameter: vector<wstring> &pathList 需要清理的文件夹
	bool GetPathList( vector<wstring> &pathList );

	// Summary  : 执行清理操作
	// Parameter: wstring 设备ID
	// Parameter: vector<wstring> &pathList 需要清理的文件夹
	INT64 ClearFiles(vector<wstring> pathList);
};