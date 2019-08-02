
#ifndef __TASK_ITEM_H__
#define  __TASK_ITEM_H__

#pragma once
#pragma warning(disable:4482)
///#include "stdafx.h"

#ifdef TASKMANAGER_EXPORTS
#define TASKMANAGER_API __declspec(dllexport)
#else
#define TASKMANAGER_API __declspec(dllimport)
#endif

#ifndef _AFX
#include <windows.h> 
#include "atlstr.h"
#include "atlbase.h"
#include "atltime.h"
#else
#include <afxmt.h>
#endif
#include "CBase_ACK.h"
#include "Core/DeviceData.h"
#include "Monitor/ProgressObserver.h"
#include "Module/Application/AppData.h"

//WAITNG ��ʾδ���д���SUSPENDED������������
enum TASKMANAGER_API TASK_STATUS {
	PROCEDING=0			//����ִ��
	, FAILED			//ʧ��
	, WAITING			//�ȴ�
	, SUSPENDED			//����/��ͣ
	, FINISHED			//���
	, DEALLOCK			//��������

};
enum TASKMANAGER_API TASK_KIND {URL_DOWNLAOD = 0, UNINSTALL,INSTALL, MOVEANDROIDAPP};

//������ʽ
enum TASKMANAGER_API TASK_UPGRADE_TYPE{
	NOTUPGRADE=0,	//�ޣ���ʾ������
	PIRACY2PIRACY,	//�������������棨Android������Ϊ�˷�ʽ��
	LEGAL2LEGAL,	//��������������
	PIRACY2LEGAL,	//��������������
	LEGAL2PIRACY,	//��������������
	BOTH2PIRACY,	//��������������������
	BOTH2LEGAL};	//��������������������

const UINT MaxTimeToTry = 10;//�����ִ�д���

#define TASK_FILE_STATE_WAIT	0x00
#define TASK_FILE_STATE_LOADED	0x01
#define TASK_FILE_STATE_ERROR	0x02
#define TASK_FILE_STATE_DISABLE	0x04

//�洢����ִ������ļ�����Ϣ
struct TASKMANAGER_API TaskFileInfo
{
	static int g_iTaskInfoID;
	int iID;
	wstring sItemName;			//��ʾ������
	TASK_KIND uiInfoType;		//�ڵ���Ϣ�д洢��Ϣ����
	UINT uiFileState;			//�ļ�����״̬���ȴ�ִ��|�Ѿ����|ִ�д���|��Ч״̬
	wstring sStrMes;			//�ļ�·����URL
	unsigned long TotalSize;
	int iErrorCount;			//����������
	wstring savePath;			//������ļ���
	unsigned long *lpProcess;
	TaskFileInfo(void)
	{
		iID = 0;
		uiInfoType = URL_DOWNLAOD;
		uiFileState = TASK_FILE_STATE_WAIT;
		sStrMes=L"";
		sItemName=L"";
		savePath=L"";
		iErrorCount = 0;
		TotalSize=0;
		lpProcess= NULL;
	}
	TaskFileInfo(TASK_KIND infoType,  const wchar_t * strMes = NULL,const wchar_t * strSavepath = NULL, const wchar_t * strName = NULL,long size = 0)
	{
		iID = g_iTaskInfoID;
		g_iTaskInfoID++;
		TotalSize = size;
		uiInfoType = infoType;
		uiFileState = TASK_FILE_STATE_WAIT;
		sStrMes=strMes?strMes:L"";
		sItemName=strName?strName:L"";
		savePath=strSavepath?strSavepath:L"";
		iErrorCount = 0;
		lpProcess= NULL;
	}

	TaskFileInfo(const TaskFileInfo& info)
	{
		iID = info.iID;
		TotalSize = info.TotalSize;
		uiInfoType = info.uiInfoType;
		uiFileState = info.uiFileState;
		sStrMes= info.sStrMes;
		sItemName=info.sItemName;
		savePath=info.savePath;
		iErrorCount = info.iErrorCount;
		lpProcess= info.lpProcess;
	}
	TaskFileInfo(const TaskFileInfo * info)
	{
		CopyValue(info);
	}
	void CopyValue(const TaskFileInfo * info)
	{
		if(NULL != info)
		{
			iID = info->iID;
			TotalSize = info->TotalSize;
			uiInfoType = info->uiInfoType;
			uiFileState = info->uiFileState;
			sStrMes=info->sStrMes;
			sItemName=info->sItemName;
			savePath=info->savePath;
			iErrorCount = info->iErrorCount;
			lpProcess= info->lpProcess;
		}
		else
		{
			iID = g_iTaskInfoID;
			g_iTaskInfoID++;
			uiFileState = TASK_FILE_STATE_WAIT;
			sStrMes=L"";
			sItemName=L"";
			savePath=L"";
			TotalSize = 0;
			iErrorCount = 0;
			lpProcess= NULL;
		}
		
	}
	~TaskFileInfo()
	{
	}
};

using namespace ND91Assistant;
//�������� 
class TASKMANAGER_API CTaskItem
{
	friend class CTaskList;

public:

	CTaskItem(void);
	CTaskItem(int id, LPCTSTR szName, const wchar_t* URL, TASK_KIND taskType, DeviceData* pDeviceData,const wchar_t * fileSavePath = NULL);
	CTaskItem(CTaskItem& modeobj);

	virtual ~CTaskItem(void);

	//////////////////////////////////////////////////////////////////////////
	//����������
	UINT m_uID;								//����ID
	wstring m_csFileSavePath;				//�ļ�����ʱ��Ҫ�ı���·��
	wstring m_csFileDownUrl;				//�ļ�����ʱ�ĵ�ַ
	TaskFileInfo *m_TaskFileInfo;			//����ִ�е�����ļ��Լ�״̬
	TASK_STATUS m_uStatus;					//��ǰ����״̬
	TASK_KIND m_uTaskType;					//��������:��װ|����
	int m_uTimeTried;						//�Ѿ�����ִ�еĴ���.
	CTime m_tAdd;							//���ʱ��
	CTime m_tStart;							//��ʼʱ��
	time_t m_tEnd;							//����ʱ��

	wstring m_strNote;						//����������
	wstring m_strName;						//�����������
	wstring m_realName;						//app����ʵ����
	wstring m_appVersion;					//�汾��
	wstring m_installstateStr;				//��װ״̬����
	wstring m_platform;						//ƽ̨
	wstring m_installerror;					//��װ��������	
	wstring m_casename;						//װ������id
	wstring m_appiconpath;
	wstring m_oldappfullpath;					//���ȫ·��������ר�ã�
	bool    m_needInstall;
	wstring m_appid;
	int     m_fid;
	bool    m_isLegalSoft;
	
	unsigned long m_nProcess;			//����
	unsigned long m_nSpeed;				//�����ٶ�
	DeviceData* _pDeviceData;			//�豸���
	ProgressObserver* _pObserver;		//��װ���Ȼص���� 
	bool InstallToSdCard;			//�Ƿ�װ��sd��
	bool bUpadateGrade;
	AppData* appData;
	bool hasProcing;//�Ƿ�����״̬�������ж���������Ƿ����߳���.
	bool _retryed;
	bool   _delaydelete;
	bool   _IsAndroidMoveStarted;

	wstring  m_fileguid;
	TASK_UPGRADE_TYPE m_taskupgradetype;

	void* pCpaInfo;
	//////////////////////////////////////////////////////////////////////////
protected:
	UINT _m_uiCustimInfoType;
#ifndef _AFX
	CRITICAL_SECTION   _m_csItemsListLock;
#else
	CCriticalSection _m_csItemsListLock;
#endif
	LPVOID _m_pCustomInfo;					//����������Ϣ

public:
	//��ȡ������ִ�еİٷֱ�
	int GetTaskPercent(void);
	//����״̬
	void SetState(TASK_STATUS status);
	
	//���Դ���
	void AddTriedTime(void);
	//��ʼʱ��
	void SetStartTimer(void);
	//���ʱ��
	void SetEndTimer(void);
	//����
	void CopyValue(CTaskItem * target);
	
	void Lock();
	void Unlock();
};

//item�б�
class TaskList : public CBase_ACK<CTaskItem>
{
public:
	TaskList(){};
	~TaskList(){};
};

#endif