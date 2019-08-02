#pragma once

class UnzipAndExecuteThread : public BaseThread
{
public:
	UnzipAndExecuteThread(void);
	~UnzipAndExecuteThread(void);

	void SetDownPath(wstring wsPath, wstring wsExeBinPath, string strPlat, string strFunName);

	unsigned long Main();

private:
	BOOL IsSoftRunning();

private:
	wstring _wsPath;
	wstring _wsExeBinPath;
	string _strPlat;
	string _strFunName;
};
