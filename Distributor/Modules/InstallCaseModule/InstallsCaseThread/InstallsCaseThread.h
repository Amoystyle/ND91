#pragma once
#include "Logic/InstallsCase/InstallCaseModelEx.h"

class InstallsCaseThread : public BaseThread
{
public:
	InstallsCaseThread(void);
	~InstallsCaseThread(void);

	unsigned long Main();

	void SetThreadInfo(int nAction= -1, int nParam = 0, bool bParam = true, wstring strParam = L"", void* pParam = NULL, void* pSpecParam = NULL);

	static void SetInstallCaseModelEx(InstallCaseModelEx* pAddress = NULL);
public:

	//取数据库的相关数据
	struct InstallsCaseMsg
	{
		InstallsCaseMsg()
		{
			_nAction = -1;
			_nParam = 0;
			_bParam = true;
			_strParam = L"";
			_pParam = NULL;
			_pSpecParam = NULL;
		}

		int     _nAction;
		int     _nParam;
		bool    _bParam;
		wstring _strParam;

		void*   _pParam;
		void*   _pSpecParam;
	};

	InstallsCaseMsg _InstallsCaseMsg;

	static InstallCaseModelEx* pInstallCaseModelEx;
};
