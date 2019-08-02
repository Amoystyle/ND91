#pragma once
#include "Base/BaseThread.h"

#include <vector>
using namespace std;

class BatchDownloadThread:public BaseThread
{
public:
	BatchDownloadThread(const string id);
	~BatchDownloadThread();

	unsigned long Main();
	
	vector<wstring> BatchDownload(wstring url);

	wstring url;

	void SetCpa(bool bIsCpa = false){_bIsCpa = bIsCpa;}
	void SetPlatForm(int nPlatForm){_nPlatForm = nPlatForm;}

public:
	bool _bIsCpa;
	int  _nPlatForm;
	void*  _pCpaInfo;
};
