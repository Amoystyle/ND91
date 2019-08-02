#ifndef _AFX
#include <windows.h>
#include <vector>
#endif
#include "TaskList.h"
#include "TaskItem.h"
#pragma once
using namespace ND91Assistant;
using namespace std;
struct stuTaskHistroy
{
	UINT nTaskNum;
	vector<wstring> TaskName;
	vector<wstring> TasksMsg;
	vector<wstring> TaskStatus;
	vector<wstring> percent;
	vector<wstring> SaveFileName;
	vector<wstring> Totalsize;
	vector<wstring> Note;
	vector<wstring> Kind;
	vector<wstring> Version;
	vector<wstring> Platform;
	vector<wstring> fids;
	vector<wstring> finishedtimes;
	vector<wstring> fileguids;
	vector<wstring> islegal;
};

class TaskHistory
{
public:
	static bool GetProfileList(stuTaskHistroy &TaskData);
	static bool SaveTaskToProfile(TaskList * taskList);
	static void CreateUnicodeFile(CString strPath);
	static time_t GetFileLastModifyTime(wstring filename);
};
