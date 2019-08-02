
#include "stdafx.h"
#include "HttpDownload.h"
#include <sys/stat.h>   // ״̬��ʾͷ�ļ�.
#include "Common/FileOperation.h"
#include "Common/CodeOperation.h"
#include <iostream>
#include "TaskControl.h"
#include "Mmsystem.h"
#include "Common/Log.h"
using namespace std;

#ifdef _MANAGED
#pragma managed(pop)
#endif
#pragma warning( disable: 4996 )
#pragma comment(lib,"ws2_32.lib")
int CHttpGet::m_nCount;
DWORD CHttpGet::m_nFileLength = 0;
//unsigned long CHttpGet::m_downloaded = 0;

//unsigned long rdownloaded = 0;
void ParseURL(CString URL,CString &host,CString &path,CString &filename)
{INTERFUN;
	URL.TrimLeft();
	URL.TrimRight();
	CString str=URL;
	CString strFind=_T("http://");
	int n=str.Find(strFind);
	if(n!=-1){
		str.Delete(0, n+strFind.GetLength());
	}

	n=str.Find('/');
	host = str.Left(n);
	n=URL.ReverseFind('/');
	path = URL.Left(n+1);
	filename = URL.Right(URL.GetLength()-n-1);
	filename.Replace(_T("//"),_T("/"));
}

// ���ǵ���������һ��ʾ����
bool fnHttpDownload(wstring strUrl,
				  wstring &strWriteFileName,
				  CTaskItem *taskItem,
				  				  TaskDealParm* taskDealParm,
				  CheckReturn checkCancel,				  
				  CString strProxy,
				  int nProxyPort,
				  int nThread
				  )
{INTERFUN;
	CHttpGet b;
	CString strHostAddr;
	CString strHttpAddr;
	CString strHttpFilename;
	wstring savepath =strWriteFileName;	
	
 LOG->WriteDebug(_T("fnHttpDownload begin"));
	//���ڸ���ʵʱ��������
	// rdownloaded = downloaded;
	//b.sectinfo->download = downloaded;
	wstring tempWFilename;
	ParseURL(strUrl.c_str(),strHostAddr,strHttpAddr,strHttpFilename);
	int pos=strHttpFilename.Find(_T("?"));
	if(pos>-1)
		strHttpFilename=strHttpFilename.Left(pos);

	if(taskItem->m_strName.empty())
		taskItem->m_strName=strHttpFilename;
	if(CStrOperation::endsWithW(savepath,L"\\"))
	{
		CFileOperation::MakeLocalDir(savepath);

		wstring invaliblechar=L"/\\:\"*?<>|";
		wstring filename=taskItem->m_strName+taskItem->m_appVersion;
		filename+=L"."+CFileOperation::GetFileExt(strHttpFilename.GetBuffer());
		for (size_t i=0;i<invaliblechar.size();i++)
		{
			wstring wstr;wstr+=invaliblechar[i];
			filename=CStrOperation::ReplaceW(filename,wstr,L"");
		}
		strWriteFileName+=filename;
		/*strWriteFileName += CStrOperation::ReplaceW(taskItem->m_strName.GetBuffer(),L"?",L"").c_str();//strHttpFilename;	//�����ļ������Ǳ���GB2312
		strWriteFileName += taskItem->m_appVersion.c_str();
		strWriteFileName += L".";
		strWriteFileName += CFileOperation::GetFileExt(strHttpFilename.GetBuffer()).c_str();*/

		if(CFileOperation::IsFileExist(strWriteFileName.c_str()))
		{
			for(int fn = 0;fn<1000;fn++)
			{
				wchar_t buff[4];
				memset(buff,0,sizeof(buff));
				_itow(fn,buff,10); 
				wstring fileext = CFileOperation::GetFileExt(strWriteFileName);
				strWriteFileName = savepath + CFileOperation::GetFileNameNoExt(strWriteFileName);
				strWriteFileName += buff;
				strWriteFileName+=_T(".");
				strWriteFileName+=fileext.c_str();
				if(!CFileOperation::IsFileExist(strWriteFileName.c_str()))
					break;
			}
		}
	}
	
	
	if(taskItem->m_uStatus != PROCEDING)
	{
		taskDealParm->userpause=true;
		return false;
	}
	taskItem->m_TaskFileInfo->savePath = strWriteFileName;
	USES_CONVERSION;
	//httpĬ����UTF8���룬���԰����ı���ΪUTF8
	string remoteFileName=CCodeOperation::UnicodeToUTF_8((strHttpFilename.GetBuffer()));
	strHttpFilename = remoteFileName.c_str();
	string remoteFilePath= CCodeOperation::UnicodeToUTF_8(strHttpAddr.GetBuffer());
	strHttpAddr = remoteFilePath.c_str();

	//cout<<strHttpAddr.GetBuffer(strHttpAddr.GetLength())<<endl;
	//���ص��ļ������ڻ��߳���Ϊ0����5��
	int retrycount = 0;	
	do{
		if(taskItem->m_uStatus != PROCEDING)
		{
			taskDealParm->userpause=true;
			return false;
		}
		if(retrycount>4)
		{
			 LOG->WriteDebug(_T("retrycount =4"));
			//����5�εõ����ļ�����Ϊ0���ж�����ʧ��!
			return false;
		}
		retrycount++;
		if(strProxy != ""){
			if(!b.HttpDownLoadProxy(strProxy,nProxyPort,strHostAddr,strHttpAddr,strHttpFilename,strWriteFileName.c_str(),nThread,checkCancel,taskDealParm,taskItem))
				return false;
		}
		else{
			if(!b.HttpDownLoadNonProxy(strHostAddr,strHttpAddr,strHttpFilename,strWriteFileName.c_str(),nThread,checkCancel,taskDealParm,taskItem))
				return false;
		}
	}while( CFileOperation::GetFileSizeW(strWriteFileName) == 0);
	return true;
}




//---------------------------------------------------------------------------
CHttpGet::CHttpGet()
{INTERFUN;
	m_nFileLength=0;
	//CheckCancel = NULL;
}

//---------------------------------------------------------------------------
CHttpGet::~CHttpGet()
{INTERFUN;
}

//---------------------------------------------------------------------------
BOOL CHttpGet::HttpDownLoadProxy(
								 CString strProxyAddr,
								 int nProxyPort,
								 CString strHostAddr,
								 CString strHttpAddr,
								 CString strHttpFilename,
								 CString strWriteFileName,
								 int nSectNum,
								CheckReturn checkCancel
								 , TaskDealParm* taskDealParm
								 ,CTaskItem *taskItem)
{INTERFUN;
	SOCKET hSocket;
	hSocket=ConnectHttpProxy(strProxyAddr,nProxyPort);
	if(hSocket == INVALID_SOCKET) return 1;

	// �����ļ�ͷ�������ļ���С.
	if(!SendHttpHeader(hSocket,strHostAddr,strHttpAddr,strHttpFilename,0))
	{
		LOG->WriteDebug(_T("SendHttpHeader =fail"));
		closesocket(hSocket);
		return FALSE;
	}
	closesocket(hSocket);
	try
	{
		taskItem->m_TaskFileInfo->TotalSize = CHttpGet::m_nFileLength;
	}
	catch (...)
	{
		LOG->WriteDebug(_T("SendHttpHeader =error"));
		return FALSE;
	}	


	if(!HttpDownLoad(strProxyAddr,nProxyPort,strHostAddr,80,strHttpAddr,strHttpFilename,strWriteFileName,nSectNum,true,checkCancel,taskDealParm,taskItem))
		return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL CHttpGet::HttpDownLoadNonProxy(
									CString strHostAddr,
									CString strHttpAddr,
									CString strHttpFilename,
									CString strWriteFileName,
									int nSectNum,
									CheckReturn checkCancel
									, TaskDealParm* taskDealParm ,
									CTaskItem *taskItem)
{INTERFUN;
	int nHostPort=80;

	SOCKET hSocket;
	hSocket=ConnectHttpNonProxy(strHostAddr,nHostPort);
	if(hSocket == INVALID_SOCKET) return 1;
	// �����ļ�ͷ�������ļ���С.
	SendHttpHeader(hSocket,strHostAddr,strHttpAddr,strHttpFilename,0);

	closesocket(hSocket);
	if(taskItem->m_uStatus != PROCEDING)
	{
		LOG->WriteDebug(_T("HttpDownLoadNonProxy taskItem->m_uStatus != PROCEDING"));
		return FALSE;
	}
	
		if(taskItem->m_uID < 100000&&taskItem->m_TaskFileInfo->TotalSize<=0)
			taskItem->m_TaskFileInfo->TotalSize= CHttpGet::m_nFileLength;
	
	if(!HttpDownLoad(TEXT(""),80,strHostAddr,nHostPort,strHttpAddr,strHttpFilename,strWriteFileName,nSectNum,false,checkCancel,taskDealParm,taskItem))
		return FALSE;

	return TRUE;
}

void CHttpGet::setCheckCancel(bool (*CheckReturn)())
{INTERFUN;
//	CheckCancel = func;

}
//---------------------------------------------------------------------------
BOOL CHttpGet::HttpDownLoad(
							CString strProxyAddr,
							int nProxyPort,
							CString strHostAddr,
							int nHostPort,
							CString strHttpAddr,
							CString strHttpFilename,
							CString strWriteFileName,
							int nSectNum,
							BOOL bProxy,CheckReturn checkCancel
							, TaskDealParm* taskDealParm,
							CTaskItem *taskItem)
{INTERFUN;
	ASSERT(nSectNum>0 && nSectNum<=50);

	m_nCount=0;                                    // �Ѽ���������.
	sectinfo=new CHttpSect[nSectNum];              // ����Ϣ�ṹ�����ڴ�.
	DWORD nSize= m_nFileLength/nSectNum;           // ����ָ�εĴ�С.

	int i=0;
	// �����߳�.���ܳ���50����
//	CWinThread* pthread[50];
//	for(i=0;i<nSectNum;i++)
//	{
		sectinfo[i].szProxyAddr=strProxyAddr;      // �����������ַ.
		sectinfo[i].nProxyPort =nProxyPort;		   // Host��ַ.
		sectinfo[i].szHostAddr =strHostAddr;       // Http�ļ���ַ.
		sectinfo[i].nHostPort  =nHostPort;		   // Http�ļ���.
		sectinfo[i].szHttpAddr =strHttpAddr;       // �������˿ں�.
		sectinfo[i].szHttpFilename=strHttpFilename;// Host�˿ں�.
		sectinfo[i].bProxyMode=bProxy;		       // ����ģ̬. 
		sectinfo[i].nRetrytimes=0;
		sectinfo[i].checkfunc = checkCancel;
		sectinfo[i].taskDealParm = taskDealParm;
		sectinfo[i].taskItem = taskItem;
		// ������ʱ�ļ���.
		CString strTempFileName;
		strTempFileName.Format(_T("%s_%s"),strWriteFileName.GetBuffer(), taskItem->m_fileguid.c_str());
		sectinfo[i].szDesFilename=strTempFileName; // ���غ���ļ���.

		if(i<nSectNum-1){
			sectinfo[i].nStart=i*nSize;            // �ָ����ʼλ��.
			sectinfo[i].nEnd=(i+1)*nSize;          // �ָ����ֹλ��.
		}
		else{
			sectinfo[i].nStart=i*nSize;            // �ָ����ʼλ��.
			//sectinfo[i].nStart=1744896;            // �ָ����ʼλ��.
			sectinfo[i].nEnd=m_nFileLength;        // �ָ����ֹλ��.
		}
		if(taskItem->m_uStatus != PROCEDING)
		{
			LOG->WriteDebug(_T("HttpDownLoad taskItem->m_uStatus != PROCEDING"));
			return FALSE;
		}
		UINT ret = ThreadDownLoad(&sectinfo[i]);
		// AfxBeginThread(ThreadDownLoad,&sectinfo[i],THREAD_PRIORITY_HIGHEST);
//		pthread[i] = AfxBeginThread(ThreadDownLoad,&sectinfo[i]);

//	}

	//HANDLE hThread[50];
	//for(int ii = 0 ; ii < nSectNum ; ii++)
	//	hThread[ii] = pthread[ii]->m_hThread;

	//// �ȴ������߳̽���.
	//WaitForMultipleObjects(nSectNum,hThread,TRUE,INFINITE);

	//��ĳ���ļ���δ�����꣬���ش���Ӧ�����ϲ�������������ء�
	//if(m_nCount != nSectNum)
	//	return FALSE;

		if(ret > 0)
			return FALSE;

	FILE *fpwrite;

	USES_CONVERSION;
	
	if(taskItem->m_uStatus != PROCEDING)
		return FALSE;
	RETURN_TRUE_IF(CFileOperation::GetFileSizeW(sectinfo[i].szDesFilename.GetBuffer())==0);
	
	wstring savepath=CFileOperation::GetFileDir(strWriteFileName.GetBuffer());
	wstring finalpath=strWriteFileName.GetBuffer();
	if(CFileOperation::IsFileExist(finalpath.c_str()))
	{
		for(int fn = 0;fn<1000;fn++)
		{
			wchar_t buff[4];
			memset(buff,0,sizeof(buff));
			_itow(fn,buff,10); 
			wstring fileext = CFileOperation::GetFileExt(finalpath);
			finalpath = savepath + CFileOperation::GetFileNameNoExt(finalpath);
			finalpath += buff;
			finalpath+=_T(".");
			finalpath+=fileext.c_str();
			if(!CFileOperation::IsFileExist(finalpath.c_str()))
				break;
		}
	}
	taskItem->m_TaskFileInfo->savePath=finalpath;
	// ��д�ļ�.
	if((fpwrite=_wfopen(finalpath.c_str(),L"wb"))==NULL){
		wstring wsmsg = L"FileCombine fopen =fail ";
		wsmsg+=finalpath;
		LOG->WriteDebug(wsmsg.c_str());
		return FALSE;
	}

	for(i=0;i<nSectNum;i++){
		if(taskItem->m_uStatus != PROCEDING)
			return FALSE;
		FileCombine(&sectinfo[i],fpwrite);
	}

	fclose(fpwrite);

	delete[] sectinfo;

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL CHttpGet::FileCombine(CHttpSect *pInfo, FILE *fpwrite)
{	INTERFUN;
	FILE *fpread;

	USES_CONVERSION;
	// ���ļ�.
	if((fpread=_wfopen(pInfo->szDesFilename.GetBuffer(),L"r+b"))==NULL)
	{
		wstring wsmsg = L"FileCombine fopen =fail ";
		wsmsg+=pInfo->szDesFilename.GetBuffer();
		LOG->WriteDebug(wsmsg.c_str());
		return FALSE;
	}

	DWORD nPos=pInfo->nStart;

	// �����ļ�дָ����ʼλ��.
	fseek(fpwrite,nPos,SEEK_SET);

	int c;
	// ���ļ�����д�뵽���ļ�.		
	while((c=fgetc(fpread))!=EOF)
	{
		fputc(c,fpwrite);
		nPos++;
		if(nPos==pInfo->nEnd) break;
	}

	fclose(fpread);
	DeleteFile(pInfo->szDesFilename);

	return TRUE;
}

//---------------------------------------------------------------------------
UINT CHttpGet::ThreadDownLoad(void* pParam)
{INTERFUN;
	CHttpSect *pInfo=(CHttpSect*)pParam;
	unsigned long * rdownloaded = &pInfo->taskItem->m_nProcess;
	unsigned long * rnSpeend =  &pInfo->taskItem->m_nSpeed;
	CTaskItem* taskItem = pInfo->taskItem;
	SOCKET hSocket;
	if(pInfo->bProxyMode){	
		hSocket=ConnectHttpProxy(pInfo->szProxyAddr,pInfo->nProxyPort);
	}
	else{
		hSocket=ConnectHttpNonProxy(pInfo->szHostAddr,pInfo->nHostPort);
	}
	if(hSocket == INVALID_SOCKET) return 1;
	if(taskItem->m_uStatus != PROCEDING)
	{
		pInfo->taskDealParm->userpause=true;
		closesocket(hSocket); // �ر��׽���.
		return 1;
	}
	// ������ʱ�ļ���С��Ϊ�˶ϵ�����
	DWORD nFileSize=CFileOperation::GetFileSizeW(pInfo->szDesFilename.GetBuffer());
	LOG_DEBUG(L"ThreadDownLoad nSectSize " + taskItem->m_strName+CStrOperation::Int64ToWString(pInfo->nEnd));

	LOG_DEBUG(L"ThreadDownLoad nFileSize " + taskItem->m_strName+CStrOperation::Int64ToWString(nFileSize));
	if(*rdownloaded>nFileSize)
		*rdownloaded=nFileSize;
	DWORD nSectSize=(pInfo->nEnd)-(pInfo->nStart);


	// �˶����������.
	if(nFileSize==nSectSize){
		//TRACE(_T("�ļ����������!\n"));                                     
		CHttpGet::m_nCount++;  // ����.
		closesocket(hSocket); // �ر��׽���.
		return 0;
	}

	FILE *fpwrite=downloadfile.GetFilePointer(pInfo->szDesFilename);
	if(!fpwrite)
	{
		wstring wsmsg = L"FileCombine fopen =fail ";
		wsmsg+=pInfo->szDesFilename.GetBuffer();
		LOG->WriteDebug(wsmsg.c_str());
		return 1;
	}
	if(taskItem->m_uStatus != PROCEDING)
	{
		fclose(fpwrite);
		closesocket(hSocket); // �ر��׽���.
		return 1;
	}
	// �������ط�Χ.
	SendHttpHeader(hSocket,pInfo->szHostAddr,pInfo->szHttpAddr,
		pInfo->szHttpFilename,pInfo->nStart+nFileSize);

	// �����ļ�дָ����ʼλ�ã��ϵ�����
	fseek(fpwrite,nFileSize,SEEK_SET);

	if(taskItem->m_uStatus != PROCEDING)
	{
		fclose(fpwrite);
		closesocket(hSocket); // �ر��׽���.
		return 1;
	}
	DWORD nLen; 
	DWORD nSumLen=0; 
	char szBuffer[1024*8];

	DWORD start;   //��¼ʱ��
	DWORD end;
	DWORD time;
	DWORD temptime;
	DWORD tempLen=0;

	CString tempstring;
	time = 0;
	start = timeGetTime();
	temptime = start;
	DWORD notifityTime =0;
	while(1)
	{
		if(taskItem->m_uStatus != PROCEDING)
		{
			pInfo->taskDealParm->userpause=true;
			fclose(fpwrite);
			closesocket(hSocket); // �ر��׽���.
			return 1;
		}
		if(nSumLen>=nSectSize-nFileSize) 
			break;
		nLen=recv(hSocket,szBuffer,sizeof(szBuffer),0);

		/*if (nLen == SOCKET_ERROR){
			LOG->WriteDebug(_T("Read error!\n"));
			TRACE(_T("Read error!\n"));
			fclose(fpwrite);
			closesocket(hSocket); // �ر��׽���.
			return 1;
		}*/

		if(nLen == SOCKET_ERROR||nLen==0) 
		{
			if(pInfo->nRetrytimes<5)//����3��
			{	
				pInfo->nRetrytimes++;
				fclose(fpwrite);
				closesocket(hSocket); // �ر��׽���.
				return ThreadDownLoad(pParam);
			}
			else
			{
				LOG_DEBUG(L"ThreadDownLoad Retryed 5 times fail "+taskItem->m_strName);
				if(nSumLen>=nSectSize-nFileSize) 
					break;
				else
				{
					fclose(fpwrite);
					closesocket(hSocket); // �ر��׽���.
					return 1;
				}
			}
		}

		//ԭ�Ӳ���������ͬ����
		*rdownloaded += nLen;

		nSumLen +=nLen;
 
		// ������д���ļ�.		
		fwrite(szBuffer,nLen,1,fpwrite);
		tempLen+=nLen;

		end = timeGetTime(); 
		if(end - temptime>=1000)
		{
			time = end - temptime;
			*rnSpeend = (long)(float)((float)tempLen*1000/(float)time);
			tempLen=0;
			temptime=end;
			CreateThread(0,0,(LPTHREAD_START_ROUTINE)TaskControl::ReportProcess,(LPVOID)pInfo->taskItem,0,NULL);
		}

	}

	fclose(fpwrite);      // �ر�д�ļ�.
	closesocket(hSocket); // �ر��׽���.
	CHttpGet::m_nCount++; // ����.
	return 0;
}

//---------------------------------------------------------------------------
SOCKET CHttpGet::ConnectHttpProxy(CString strProxyAddr,int nPort)
{INTERFUN;
//	TRACE(_T("���ڽ�������\n"));

	CString sTemp;
	char cTmpBuffer[1024];
	SOCKET hSocket=dealsocket.GetConnect(strProxyAddr,nPort);

	if(hSocket == INVALID_SOCKET)
	{
	//	TRACE(_T("����http������ʧ�ܣ�\n"));
		return INVALID_SOCKET;
	}

	// ����CONNCET�������������������ںʹ��������Ӵ����������
	// ��ַ�Ͷ˿ڷ���strProxyAddr,nPort ����.
	sTemp.Format(_T("CONNECT %s:%d HTTP/1.1\r\nUser-Agent:\
				 MyApp/0.1\r\n\r\n"),strProxyAddr.GetBuffer(),nPort);

	if(!SocketSend(hSocket,sTemp))
	{
	//	TRACE(_T("���Ӵ���ʧ��\n"));
		return INVALID_SOCKET;
	}

	// ȡ�ô�����Ӧ��������Ӵ���ɹ��������������
	// ����"200 Connection established".
	int nLen=GetHttpHeader(hSocket,cTmpBuffer);
	sTemp=cTmpBuffer;
	if(sTemp.Find(_T("HTTP/1.0 200 Connection established"),0)==-1)
	{
	//	TRACE(_T("���Ӵ���ʧ��\n"));
		return INVALID_SOCKET;
	}

	TRACE(sTemp);
//	TRACE(_T("�����������\n"));
	return hSocket; 
}

//---------------------------------------------------------------------------
SOCKET CHttpGet::ConnectHttpNonProxy(CString strHostAddr,int nPort)
{INTERFUN;
	//TRACE(_T("���ڽ�������\n"));
	SOCKET hSocket=dealsocket.GetConnect(strHostAddr,nPort);
	if(hSocket == INVALID_SOCKET)
		return INVALID_SOCKET;

	return hSocket;
}

//---------------------------------------------------------------------------
// ����: strHostAddr="www.aitenshi.com",
// strHttpAddr="http://www.aitenshi.com/bbs/images/",
// strHttpFilename="pics.jpg".
BOOL CHttpGet::SendHttpHeader(SOCKET hSocket,CString strHostAddr,
							  CString strHttpAddr,CString strHttpFilename,DWORD nPos)
{INTERFUN;
	// ��������. 
	CString sTemp;
	char cTmpBuffer[1025];

	// Line1: �����·��,�汾.
	sTemp.Format(_T("GET %s%s HTTP/1.1\r\n"),strHttpAddr,strHttpFilename);
	if(!SocketSend(hSocket,sTemp))
		return FALSE;

	// Line2:����.
	sTemp.Format(_T("Host: %s\r\n"),strHostAddr);
	if(!SocketSend(hSocket,sTemp)) 
		return FALSE;

	// Line3:���յ���������.
	sTemp.Format(_T("Accept: \r\n"));
	if(!SocketSend(hSocket,sTemp)) 
		return FALSE;

	// Line4:�ο���ַ.
	sTemp.Format(_T("Referer: %s\r\n"),strHttpAddr); 
	if(!SocketSend(hSocket,sTemp)) 
		return FALSE;

	// Line5:���������.
	sTemp.Format(_T("User-Agent: Mozilla/4.0 \
				 (compatible; MSIE 5.0; Windows NT; DigExt; DTS Agent;)\r\n"));

	if(!SocketSend(hSocket,sTemp)) 
		return FALSE;

	// ����. Range ��Ҫ���ص����ݷ�Χ������������Ҫ.
	sTemp.Format(_T("Range: bytes=%d-\r\n"),nPos);
	if(!SocketSend(hSocket,sTemp)) 
		return FALSE;

	// LastLine: ����.
	sTemp.Format(_T("\r\n"));
	if(!SocketSend(hSocket,sTemp)) 
		return FALSE;

	// ȡ��httpͷ.
	int i=GetHttpHeader(hSocket,cTmpBuffer);
	if(!i)
	{
	//	TRACE(_T("��ȡHTTPͷ����!\n"));
		return 0;
	}

	// ���ȡ�õ�httpͷ����404�����������ʾ���ӳ�����.

	//CString strret= CString(cTmpBuffer);
	string strret=cTmpBuffer;
	int pos=strret.find("\r\n");
	if(pos>-1)
		strret=strret.substr(0,pos);
	if(strret.find("206")==string::npos)
	{
		m_nFileLength=0;
		return FALSE;
	}

	// �õ��������ļ��Ĵ�С.
	m_nFileLength=GetFileLength(cTmpBuffer);

	//if(m_nFileLength==0)
	{
	//	if((int)strret.Find(_T("404"))!=-1) return FALSE;
	//	if((int)strret.Find(_T("403"))!=-1) return FALSE;
	}
	// ��ΪTRACE()���������ַ�������Ϊ255.
	//TRACE(CString(cTmpBuffer).GetBuffer(200));

	return TRUE;
}

//---------------------------------------------------------------------------
DWORD CHttpGet::GetHttpHeader(SOCKET sckDest,char *str)
{INTERFUN;
	BOOL bResponsed=FALSE;
	DWORD nResponseHeaderSize;

	if(!bResponsed)
	{
		char c = 0;
		int nIndex = 0;
		BOOL bEndResponse = FALSE;
		while(!bEndResponse && nIndex < 1024)
		{
			recv(sckDest,&c,1,0);
			str[nIndex++] = c;
			if(nIndex >= 4)
			{
				if( str[nIndex - 4] == '\r' && 
					str[nIndex - 3] == '\n' && 
					str[nIndex - 2] == '\r' && 
					str[nIndex - 1] == '\n')
					bEndResponse = TRUE;
			}
		}

		str[nIndex]=0;
		nResponseHeaderSize = nIndex;
		bResponsed = TRUE;
	}

	return nResponseHeaderSize;
}

//---------------------------------------------------------------------------
DWORD CHttpGet:: GetFileLength(char *httpHeader)
{INTERFUN;
	CString strHeader;
	CString strFind=_T("Content-Length:");
	int local;
	strHeader=CString(httpHeader);
	local=strHeader.Find(strFind,0);
	local+=strFind.GetLength();
	strHeader.Delete(0,local);
	local=strHeader.Find(_T("\r\n"));

	if(local!=-1){
		strHeader=strHeader.Left(local);
	}

	USES_CONVERSION;
	return atoi(CCodeOperation::UnicodeToUTF_8(strHeader.GetBuffer()).c_str());
}

//---------------------------------------------------------------------------
BOOL CHttpGet::SocketSend(SOCKET sckDest,CString szHttp)
{INTERFUN;
	int iLen=szHttp.GetLength();
	USES_CONVERSION;
	if(send (sckDest,CCodeOperation::UnicodeToUTF_8(szHttp.GetBuffer()).c_str(),iLen,0)==SOCKET_ERROR)
	{
		closesocket(sckDest);
	//	TRACE(_T("��������ʧ��!\n"));
		return FALSE;
	}

	return TRUE;
}


CDealSocket dealsocket;

//---------------------------------------------------------------------------
CDealSocket::CDealSocket()
{INTERFUN;
	// �׽��ֳ�ʼ��.
	WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;

	// ��ʼ��WinSock.
	if (WSAStartup(wVersionRequested, &wsaData)!=0)
	{
		TRACE(_T("WSAStartup\n"));
		return;
	}

	// ��� WinSock �汾.
	if (wsaData.wVersion != wVersionRequested)
	{
		TRACE(_T("WinSock version not supported\n"));
		WSACleanup();
		return;
	}
}

//---------------------------------------------------------------------------
CDealSocket::~CDealSocket()
{INTERFUN;
	// �ͷ�WinSock.
	WSACleanup();
}

//---------------------------------------------------------------------------
CString CDealSocket::GetResponse(SOCKET hSock)
{INTERFUN;
	char szBufferA[MAX_RECV_LEN];  	// ASCII�ַ���. 
	int	iReturn;					// recv�������ص�ֵ.

	CString szError;
	CString strPlus;
	strPlus.Empty();

	while(1)
	{
		// ���׽��ֽ�������.
		iReturn = recv (hSock, szBufferA, MAX_RECV_LEN, 0);
		szBufferA[iReturn]=0;
		strPlus +=szBufferA;

	//	TRACE(szBufferA);

		if (iReturn == SOCKET_ERROR)
		{
			szError.Format(_T("No data is received, recv failed. Error: %d"),
				WSAGetLastError ());
			MessageBox (NULL, szError, TEXT("Client"), MB_OK);
			break;
		}
		else if(iReturn<MAX_RECV_LEN){
			TRACE(_T("Finished receiving data\n"));
			break;
		}
	}

	return strPlus;
}

//---------------------------------------------------------------------------
SOCKET CDealSocket::GetConnect(CString host ,int port)
{INTERFUN;
	SOCKET hSocket;
	SOCKADDR_IN saServer;          // �������׽��ֵ�ַ.
	PHOSTENT phostent = NULL;	   // ָ��HOSTENT�ṹָ��.

	// ����һ���󶨵���������TCP/IP�׽���.
	if ((hSocket = socket (AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		TRACE(_T("Allocating socket failed. Error: %d\n"),WSAGetLastError ());
		return INVALID_SOCKET;
	}

	// ʹ��TCP/IPЭ��.
	saServer.sin_family = AF_INET;

	// ��ȡ��������ص���Ϣ.
	USES_CONVERSION;
	if ((phostent = gethostbyname (CCodeOperation::UnicodeToUTF_8(host.GetBuffer()).c_str())) == NULL) 
	{
		TRACE(_T("Unable to get the host name. Error: %d\n"),WSAGetLastError ());
		closesocket (hSocket);
		return INVALID_SOCKET;
	}

	// ���׽���IP��ַ��ֵ.
	memcpy ((char *)&(saServer.sin_addr), 
		phostent->h_addr, 
		phostent->h_length);

	// �趨�׽��ֶ˿ں�.
	saServer.sin_port =htons (port); 

	// ���������������׽�������.
	if (connect (hSocket,(PSOCKADDR) &saServer, 
		sizeof (saServer)) == SOCKET_ERROR) 
	{
		TRACE(_T("Connecting to the server failed. Error: %d\n"),WSAGetLastError ());
		closesocket (hSocket);
		return INVALID_SOCKET;
	}

	int nTSize = 0;
	if(setsockopt(hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&nTSize, sizeof(int)) == SOCKET_ERROR)
	{
		closesocket (hSocket);
		return INVALID_SOCKET;
	}

	int timeout= 60000; 
	if(setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == SOCKET_ERROR)
	{
		closesocket (hSocket);
		return INVALID_SOCKET;
	}

	return hSocket;
}

//---------------------------------------------------------------------------
SOCKET CDealSocket::Listening(int port)
{INTERFUN;
	SOCKET ListenSocket = INVALID_SOCKET;	// �����׽���.
	SOCKADDR_IN local_sin;				    // �����׽��ֵ�ַ.

	// ����TCP/IP�׽���.
	if ((ListenSocket = socket (AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) 
	{
		TRACE(_T("Allocating socket failed. Error: %d\n"),WSAGetLastError ());
		return INVALID_SOCKET;
	}

	// ���׽�����Ϣ�ṹ��ֵ.
	local_sin.sin_family = AF_INET;
	local_sin.sin_port = htons (port); 
	local_sin.sin_addr.s_addr = htonl (INADDR_ANY);

	// ���б�����ַ������׽��ְ�.
	if (bind (ListenSocket, 
		(struct sockaddr *) &local_sin, 
		sizeof (local_sin)) == SOCKET_ERROR) 
	{
		TRACE(_T("Binding socket failed. Error: %d\n"),WSAGetLastError ());
		closesocket (ListenSocket);
		return INVALID_SOCKET;
	}

	// �����׽��ֶ��ⲿ���ӵļ���.
	if (listen (ListenSocket, MAX_PENDING_CONNECTS) == SOCKET_ERROR) 
	{
		TRACE(_T("Listening to the client failed. Error: %d\n"),	
			WSAGetLastError ());
		closesocket (ListenSocket);
		return INVALID_SOCKET;
	}

	return ListenSocket;
}


CDownloadFile downloadfile;

//---------------------------------------------------------------------------
CDownloadFile::CDownloadFile()
{INTERFUN;
}

//---------------------------------------------------------------------------
CDownloadFile::~CDownloadFile()
{INTERFUN;
}

//---------------------------------------------------------------------------
BOOL CDownloadFile::FileExists(LPCTSTR lpszFileName)
{INTERFUN;
	DWORD dwAttributes = GetFileAttributes(lpszFileName);
	if (dwAttributes == 0xFFFFFFFF)
		return FALSE;

	if ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		==	FILE_ATTRIBUTE_DIRECTORY)
	{
		return FALSE;
	}
	else{
		return TRUE;
	}
}

//---------------------------------------------------------------------------
FILE* CDownloadFile::GetFilePointer(LPCTSTR lpszFileName)
{INTERFUN;
	FILE *fp;
	USES_CONVERSION;
	if(FileExists(lpszFileName)){
		// �������ļ�����д����.
		fp=_wfopen(lpszFileName,L"r+b");
		//fp=CreateFile(W2A(lpszFileName),"r+b");
	}
	else{
		// �������ļ�����д����.
		//fp=fopen(CDownloadFile::CStringTostring(lpszFileName),"w+b");
		
		fp=_wfopen(lpszFileName,L"w+b");
	}

	return fp;
}

//---------------------------------------------------------------------------
DWORD CDownloadFile::GetFileSizeByName(LPCTSTR lpszFileName)
{INTERFUN;
	if(!FileExists(lpszFileName)) return 0;
	struct _stat ST; 
	// ��ȡ�ļ�����.
	USES_CONVERSION;
	if(_stat64i32(CCodeOperation::UnicodeToUTF_8(lpszFileName).c_str(), &ST)!=0)
		return 0;
	UINT nFilesize=ST.st_size;
	return nFilesize; 
}

//---------------------------------------------------------------------------
// ��ȫ���ļ�������ȡ���ļ���.
CString CDownloadFile::GetShortFileName(LPCSTR lpszFullPathName)
{INTERFUN;
	CString strFileName=CString(lpszFullPathName);
	CString strShortName;
	strFileName.TrimLeft();
	strFileName.TrimRight();
	int n=strFileName.ReverseFind(_T('/'));
	strShortName=strFileName.Right(strFileName.GetLength()-n-1);
	return strShortName;
}
const char * CDownloadFile::CStringTostring(CString str)
{INTERFUN;
	//CStringA   stra(str); 
	//str.ReleaseBuffer();
	//std::string   strs   (stra.GetBuffer(0)); 
	//stra.ReleaseBuffer(); 
	//return strs.c_str();
	USES_CONVERSION;
	return W2A(str.GetBuffer());
}

const char * CDownloadFile::UnicodeToAnsi(const WCHAR* pSrc,size_t nLength) 
{ INTERFUN;
	char defaultChar[100]; 
	BOOL bUseDefaultChar; 
	char* pBuffer = new char[nLength + 1];
	int iByte = WideCharToMultiByte(CP_ACP, 0, pSrc, -1, pBuffer, 1024, defaultChar, &bUseDefaultChar); 
	if (iByte == 0) 
	{ 
		//_stprintf(tszErrMsg, theApp.GetText(_T("%s, cann't convert the source to ansi code page, the error code is 0x%x\n")).c_str(), lpszInput, GetLastError()); 
		//AfxMessageBox(tszErrMsg); 
	}
	string ret(pBuffer,iByte);
	delete[] pBuffer;
	return ret.c_str(); 
}
