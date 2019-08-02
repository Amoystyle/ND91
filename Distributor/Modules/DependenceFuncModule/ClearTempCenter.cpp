#include "stdafx.h"
#include "ClearTempCenter.h"
#include<algorithm>

ClearTempCenter::ClearTempCenter()
{
	

}

ClearTempCenter::~ClearTempCenter()
{

}

void ClearTempCenter::Init()
{
	CreateThread(0, 0, ClearLogAction, NULL, 0, NULL);
	CreateThread(0, 0, Clear_Assistant_TempThread, this, 0, NULL);
}


DWORD WINAPI ClearTempCenter::Clear_Assistant_TempThread( LPVOID lpParam )
{
	ClearTempCenter* pMainDlg = (ClearTempCenter*)lpParam;
	Sleep(15000);	//为防止启动速度慢延迟15秒对硬盘操作
	pMainDlg->Clear_Assistant();

	return 0;
}

DWORD WINAPI ClearTempCenter::ClearLogAction( PVOID lpParam )
{

	vector<wstring> files;
	CFileOperation::FindAllFileInDir(CONFIG(_strLogPath),files);
	for (size_t i=0; i<files.size();i++)
	{//删除  类似 91Distributor.log(13-08-13 18-04-21).log  这样的日志后重新获取
		vector<wstring> buf = CStrOperation::parseStringsW(files.at(i),L".");
		if (buf.size()>2)
			CFileOperation::DeleteFile(files.at(i));
	}

	files.clear();
	CFileOperation::FindAllFileInDir(CONFIG(_strLogPath),files);

	sort(files.begin(), files.end());		//升序

	if (files.size() > 10)
	{
		for (size_t i = 0;i<files.size()-10 ;i++)
		{
			CFileOperation::DeleteFile(files.at(i));
			Sleep(50);
		}
	}
	return 0;
}

bool ClearTempCenter::Clear_Assistant()
{
	try
	{
		wchar_t path[MAX_PATH] = {0};
		int strlen = GetTempPathW(MAX_PATH,path);
		vector<wstring> files;
		CFileOperation::FindAllFolderInDir(path,files);
		for (size_t i=0;i<files.size();i++)
		{
			if (files.at(i).find(_T("Assistant_")) == -1)
				continue;
			if (Compare_Time(files.at(i)))
			{
				//符合条件删除
				CFileOperation::DeleteDirectory(files.at(i));
			}
		}	
	}
	catch (...)
	{

	}
	return false;
}


bool ClearTempCenter::Compare_Time(wstring path)
{
	//返回false 不删除， 返回true删除
	WIN32_FIND_DATA wfd;

	SYSTEMTIME systime;
	::GetSystemTime(&systime);

	HANDLE hFind=::FindFirstFileW(path.c_str(),&wfd);
	if (INVALID_HANDLE_VALUE == hFind)
		return false;

	SYSTEMTIME  st;
	FileTimeToSystemTime(&wfd.ftLastWriteTime,&st);	//转换日期格式

	int temp = systime.wDay-st.wDay;
	//时间比对逻辑
	if (systime.wYear > st.wYear)
		return true;
	else if (systime.wMonth > st.wMonth)
		return true;
	else if ((systime.wDay-st.wDay) >0)
		return true;
	return false;
}