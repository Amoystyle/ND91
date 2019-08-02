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
	Sleep(15000);	//Ϊ��ֹ�����ٶ����ӳ�15���Ӳ�̲���
	pMainDlg->Clear_Assistant();

	return 0;
}

DWORD WINAPI ClearTempCenter::ClearLogAction( PVOID lpParam )
{

	vector<wstring> files;
	CFileOperation::FindAllFileInDir(CONFIG(_strLogPath),files);
	for (size_t i=0; i<files.size();i++)
	{//ɾ��  ���� 91Distributor.log(13-08-13 18-04-21).log  ��������־�����»�ȡ
		vector<wstring> buf = CStrOperation::parseStringsW(files.at(i),L".");
		if (buf.size()>2)
			CFileOperation::DeleteFile(files.at(i));
	}

	files.clear();
	CFileOperation::FindAllFileInDir(CONFIG(_strLogPath),files);

	sort(files.begin(), files.end());		//����

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
				//��������ɾ��
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
	//����false ��ɾ���� ����trueɾ��
	WIN32_FIND_DATA wfd;

	SYSTEMTIME systime;
	::GetSystemTime(&systime);

	HANDLE hFind=::FindFirstFileW(path.c_str(),&wfd);
	if (INVALID_HANDLE_VALUE == hFind)
		return false;

	SYSTEMTIME  st;
	FileTimeToSystemTime(&wfd.ftLastWriteTime,&st);	//ת�����ڸ�ʽ

	int temp = systime.wDay-st.wDay;
	//ʱ��ȶ��߼�
	if (systime.wYear > st.wYear)
		return true;
	else if (systime.wMonth > st.wMonth)
		return true;
	else if ((systime.wDay-st.wDay) >0)
		return true;
	return false;
}