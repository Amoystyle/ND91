#pragma once
#include "Common/DeviceBaseThread.h"

class ClearMemoryThread : public DeviceBaseThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    ClearMemoryThread( string threadID, wstring deviceID );
	~ClearMemoryThread();

private:
	virtual unsigned long Main();

	void ClearMemory();

	// Summary  : ��ȡҪ������ļ���
	// Parameter: wstring �豸ID
	// Parameter: vector<wstring> &pathList ��Ҫ������ļ���
	bool GetPathList( vector<wstring> &pathList );

	// Summary  : ִ���������
	// Parameter: wstring �豸ID
	// Parameter: vector<wstring> &pathList ��Ҫ������ļ���
	INT64 ClearFiles(vector<wstring> pathList);
};