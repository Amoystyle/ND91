#pragma once

class LoadResourceThread
	: public BaseThread
{
public:
	LoadResourceThread(string threadId);
	~LoadResourceThread();

	virtual unsigned long Main();

private:
	void LoadResource();

};