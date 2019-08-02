#pragma once
#include "Base/BaseThread.h"
#include <string>
using namespace std;


class BaseCSLock;
class CDeamonInstall;
class DeamonInstallThread : public BaseThread, private BaseCSLock
{
public:
	DeamonInstallThread(const string id, CDeamonInstall* pInstall, DeviceData* pData, bool bNeedChange, bool bAppSync);
	~DeamonInstallThread();

	unsigned long Main();


private:

	CDeamonInstall*	_pInstall;

	DeviceData* _pData;

	bool		_bNeedChange;

	bool		_bAppSync;
	
};