#pragma once


class ClearTempCenter

{

public:
	ClearTempCenter();
	~ClearTempCenter();

	void Init();
private:
	static DWORD WINAPI ClearLogAction( PVOID lpParam );

	static DWORD WINAPI Clear_Assistant_TempThread(LPVOID lpParam);

	bool Clear_Assistant();

	bool Compare_Time(wstring path);
};