#include "stdafx.h"
#include "TaskHistroy.h"
#include "Common/FileOperation.h"
#include "Common/MyFileOperation.h"
#pragma warning( disable: 4996 )
bool TaskHistory::GetProfileList(stuTaskHistroy &TaskData)
{INTERFUN;
	wstring ProfileName;
	ProfileName = MyFileOperation::GetMyDocumentPath() + _T("\\NetDragon\\91Distributor\\TaskProfile.ini");

	if(!CFileOperation::IsFileExist(ProfileName.c_str()))
		return false;
	wstring vercode = MyFileOperation::GetIniFileValue(ProfileName, _T("TaskListInfo"), _T("VerCode")).c_str();
	if(vercode.size()==0)
	{
		CFileOperation::DeleteFile(ProfileName);
		return false;
	}

	int taskNum = _wtoi(MyFileOperation::GetIniFileValue(ProfileName, _T("TaskListInfo"), _T("ItemNUM")).c_str());
	TaskData.nTaskNum = taskNum;

	TaskData.TaskName.clear();  
	TaskData.SaveFileName.clear();
	TaskData.TasksMsg.clear();  
	TaskData.TaskStatus.clear();  
	TaskData.percent.clear();  
	TaskData.Totalsize.clear();  
	TaskData.Note.clear();  
	TaskData.Kind.clear(); 
	TaskData.Version.clear();
	TaskData.Platform.clear();
	TaskData.fids.clear();
	TaskData.finishedtimes.clear();
	TaskData.fileguids.clear();
	TaskData.islegal.clear();
	for(int i=0;i<taskNum;i++)
	{
		wstring keyName = _T("TaskInfo");
		keyName+=CStrOperation::IntToWString(i);
		wstring TaskNamestr = MyFileOperation::GetIniFileValue(ProfileName, keyName, _T("TaskName"));
		wstring SaveFileNamestr = MyFileOperation::GetIniFileValue(ProfileName, keyName, _T("SaveFileName")).c_str();
		//if(!CFileOperation::IsFileExist(SaveFileNamestr.c_str()))continue;
		TaskData.TaskName.push_back( TaskNamestr );
		TaskData.SaveFileName.push_back(SaveFileNamestr);
		wstring TasksMsgstr = MyFileOperation::GetIniFileValue(ProfileName, keyName, _T("TasksMsg")).c_str();
		TaskData.TasksMsg.push_back(TasksMsgstr);
		wstring TaskStatusstr = MyFileOperation::GetIniFileValue(ProfileName, keyName, _T("TaskStatus")).c_str();
		TaskData.TaskStatus.push_back(TaskStatusstr);
		wstring percentstr = MyFileOperation::GetIniFileValue(ProfileName, keyName, _T("percent")).c_str();
		TaskData.percent.push_back(percentstr);
		wstring Totalsizestr = MyFileOperation::GetIniFileValue(ProfileName, keyName, _T("Totalsize")).c_str();
		TaskData.Totalsize.push_back(Totalsizestr);
		wstring Notestr = MyFileOperation::GetIniFileValue(ProfileName, keyName, _T("Note")).c_str();
		TaskData.Note.push_back(Notestr);
		wstring Kindstr = MyFileOperation::GetIniFileValue(ProfileName,keyName, _T("Kind")).c_str();
		TaskData.Kind.push_back(Kindstr);
		wstring appVersion = MyFileOperation::GetIniFileValue(ProfileName,keyName, _T("Version")).c_str();
		TaskData.Version.push_back(appVersion);
		wstring platform = MyFileOperation::GetIniFileValue(ProfileName,keyName, _T("Platform")).c_str();
		TaskData.Platform.push_back(platform);
		wstring fid = MyFileOperation::GetIniFileValue(ProfileName,keyName, _T("Fid")).c_str();
		TaskData.fids.push_back(fid);
		wstring finishedtime=MyFileOperation::GetIniFileValue(ProfileName,keyName, _T("finishedtime")).c_str();
		time_t modifytm=GetFileLastModifyTime(SaveFileNamestr);
		if(finishedtime.empty())
			finishedtime = CStrOperation::Int64ToWString(modifytm);
		TaskData.finishedtimes.push_back(finishedtime);

		wstring tempfileguid=MyFileOperation::GetIniFileValue(ProfileName,keyName, _T("tempfileguid")).c_str();
		if(tempfileguid.empty()&&CFileOperation::IsFileExist((SaveFileNamestr+L"_0").c_str()))
		{
			tempfileguid=CFileOperation::GetGuidW();
			CFileOperation::MoveFile(SaveFileNamestr+L"_0",SaveFileNamestr+L"_"+tempfileguid);
		}
		TaskData.fileguids.push_back(tempfileguid);
		wstring islegal = MyFileOperation::GetIniFileValue(ProfileName,keyName, _T("islegal")).c_str();
		TaskData.islegal.push_back(islegal);
	}
	TaskData.nTaskNum=TaskData.SaveFileName.size();
	return true;
}

bool TaskHistory::SaveTaskToProfile(TaskList * taskList)
{INTERFUN;
	if(taskList == NULL)
		return false;

	UINT taskNum = taskList->size();
		
	wstring ProfileName = MyFileOperation::GetMyDocumentPath() + _T("\\NetDragon\\91Distributor\\TaskProfile.ini");
	if(!CFileOperation::IsFileExist(ProfileName.c_str()))
			CreateUnicodeFile(ProfileName.c_str());		

	wstring wscount = CStrOperation::IntToWString(taskNum);
	MyFileOperation::SetIniFileValue(ProfileName.c_str(), L"TaskListInfo", L"ItemNUM",wscount);
	MyFileOperation::SetIniFileValue(ProfileName.c_str(), L"TaskListInfo", L"VerCode",L"1");
	int taskpos=0;
	for(TaskList::iterator item = taskList->begin();item != taskList->end(); item++)
	{
		CTaskItem *taskItem = *item;
		wstring keyName =  L"TaskInfo";
		keyName+=CStrOperation::IntToWString(taskpos);
		taskpos++;
		MyFileOperation::SetIniFileValue(ProfileName.c_str(),keyName, L"TaskName",taskItem->m_strName);
		MyFileOperation::SetIniFileValue(ProfileName.c_str(),keyName, L"percent",CStrOperation::Int64ToWString(taskItem->m_nProcess));
		MyFileOperation::SetIniFileValue(ProfileName.c_str(), keyName, L"TasksMsg",taskItem->m_TaskFileInfo->sStrMes);
		MyFileOperation::SetIniFileValue(ProfileName.c_str(), keyName, L"TaskStatus",CStrOperation::IntToWString(taskItem->m_uStatus));
		wstring SaveFileNames;
		if(taskItem->m_TaskFileInfo->savePath.empty())
			SaveFileNames+=taskItem->m_csFileSavePath;
		else
			SaveFileNames+=taskItem->m_TaskFileInfo->savePath;
		MyFileOperation::SetIniFileValue(ProfileName.c_str(), keyName, L"SaveFileName",SaveFileNames);
		MyFileOperation::SetIniFileValue(ProfileName.c_str(), keyName, L"Totalsize",CStrOperation::Int64ToWString(taskItem->m_TaskFileInfo->TotalSize));
		MyFileOperation::SetIniFileValue(ProfileName.c_str(), keyName, L"Note",taskItem->m_strNote);
		MyFileOperation::SetIniFileValue(ProfileName.c_str(), keyName, L"Kind",CStrOperation::IntToWString(taskItem->m_uTaskType));
		MyFileOperation::SetIniFileValue(ProfileName.c_str(), keyName, L"Version",taskItem->m_appVersion);
		MyFileOperation::SetIniFileValue(ProfileName.c_str(), keyName, L"Platform",taskItem->m_platform);
		MyFileOperation::SetIniFileValue(ProfileName.c_str(),keyName,L"Fid",CStrOperation::IntToWString(taskItem->m_fid));
		MyFileOperation::SetIniFileValue(ProfileName.c_str(),keyName,L"finishedtime",CStrOperation::Int64ToWString(taskItem->m_tEnd));
		MyFileOperation::SetIniFileValue(ProfileName.c_str(),keyName,L"tempfileguid",taskItem->m_fileguid);
		wstring strisLegal = L"";
		if (taskItem->m_isLegalSoft)
		{
			strisLegal = L"TRUE";
		}
		else
			strisLegal = L"FALSE";
		MyFileOperation::SetIniFileValue(ProfileName.c_str(),keyName, L"islegal", strisLegal);

	}
 
	return true;
}

time_t TaskHistory::GetFileLastModifyTime(wstring filename)
{
	HANDLE hFile=NULL;
	//创建文件
	hFile=CreateFile(filename.c_str(),
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	FILETIME CreationTime,LastAccessTime,LastWriteTime;
	::GetFileTime(hFile,&CreationTime,&LastAccessTime,&LastWriteTime);
	CloseHandle(hFile);

	ULARGE_INTEGER            ui;  
	ui.LowPart            =  LastWriteTime.dwLowDateTime;  
	ui.HighPart            =  LastWriteTime.dwHighDateTime;  
	return  ((time_t)(ui.QuadPart  -  116444736000000000)  /  10000000); 
}

void TaskHistory::CreateUnicodeFile(CString strPath)
{INTERFUN;
	HANDLE hFile=NULL;
	//创建文件
	hFile=CreateFile(strPath,
		GENERIC_WRITE|GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(INVALID_HANDLE_VALUE==hFile)
	{
	
		return;
	}
	DWORD dwValue=0;
	DWORD dwSize=0;
	dwSize = GetFileSize (hFile, NULL) ; 
	if (dwSize == 0xFFFFFFFF) 
	{ 
		
		CloseHandle(hFile);
		return;
	} 
	if(dwSize==0)
	{
		TCHAR p=0xfeff;//UNICODE文件开头标志
		if(!WriteFile(hFile,&p,sizeof(TCHAR),&dwValue,NULL))
		{
			
			CloseHandle(hFile);
			return;
		}
	}
	CloseHandle(hFile);
}
