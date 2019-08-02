#pragma once
#include "Base/BaseThread.h"
#include <string>
using namespace std;


class BaseCSLock;
class CDeamonInstall;
class DeamonDownlaodThread : public BaseThread, private BaseCSLock
{
public:
	DeamonDownlaodThread(const std::string threadId, CDeamonInstall* pInstall, bool isDeamon, bool isCpa=false, bool isJailBreaked=false, bool isPad=false, int nDeviceVer=false);
	~DeamonDownlaodThread(void);

private:
	virtual unsigned long Main();

private:

	CDeamonInstall*	_pInstall;
	int				_nDeviceVer;
	bool			_isDeamon;
	bool			_isPad;
	bool			_isJailBreaked;
	bool			_isCpa;
	
	
};
