
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

//WAITNG 表示未进行处理，SUSPENDED表明可以续传
enum TASKMANAGER_API TASK_STATUS {
	PROCEDING=0			//正在执行
	, FAILED			//失败
	, WAITING			//等待
	, SUSPENDED			//挂起/暂停
	, FINISHED			//完成
	, DEALLOCK			//操作锁定

};
enum TASKMANAGER_API TASK_KIND {URL_DOWNLAOD = 0, UNINSTALL,INSTALL, MOVEANDROIDAPP};

//升级方式
enum TASKMANAGER_API TASK_UPGRADE_TYPE{
	NOTUPGRADE=0,	//无，表示非升级
	PIRACY2PIRACY,	//盗版升级到盗版（Android升级均为此方式）
	LEGAL2LEGAL,	//正版升级到正版
	PIRACY2LEGAL,	//盗版升级到正版
	LEGAL2PIRACY,	//正版升级到盗版
	BOTH2PIRACY,	//盗版和正版均升级到盗版
	BOTH2LEGAL};	//盗版和正版均升级到正版

const UINT MaxTimeToTry = 10;//最大尝试执行次数

#define TASK_FILE_STATE_WAIT	0x00
#define TASK_FILE_STATE_LOADED	0x01
#define TASK_FILE_STATE_ERROR	0x02
#define TASK_FILE_STATE_DISABLE	0x04

//存储任务执行相关文件的信息
struct TASKMANAGER_API TaskFileInfo
{
	static int g_iTaskInfoID;
	int iID;
	wstring sItemName;			//显示的名称
	TASK_KIND uiInfoType;		//节点信息中存储信息类型
	UINT uiFileState;			//文件下载状态：等待执行|已经完成|执行错误|无效状态
	wstring sStrMes;			//文件路径或URL
	unsigned long TotalSize;
	int iErrorCount;			//错误的项计数
	wstring savePath;			//保存的文件名
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
//任务子项 
class TASKMANAGER_API CTaskItem
{
	friend class CTaskList;

public:

	CTaskItem(void);
	CTaskItem(int id, LPCTSTR szName, const wchar_t* URL, TASK_KIND taskType, DeviceData* pDeviceData,const wchar_t * fileSavePath = NULL);
	CTaskItem(CTaskItem& modeobj);

	virtual ~CTaskItem(void);

	//////////////////////////////////////////////////////////////////////////
	//任务队列相关
	UINT m_uID;								//任务ID
	wstring m_csFileSavePath;				//文件下载时需要的保存路径
	wstring m_csFileDownUrl;				//文件下载时的地址
	TaskFileInfo *m_TaskFileInfo;			//任务执行的相关文件以及状态
	TASK_STATUS m_uStatus;					//当前任务状态
	TASK_KIND m_uTaskType;					//任务种类:安装|下载
	int m_uTimeTried;						//已经尝试执行的次数.
	CTime m_tAdd;							//添加时间
	CTime m_tStart;							//开始时间
	time_t m_tEnd;							//结束时间

	wstring m_strNote;						//任务结果描述
	wstring m_strName;						//任务项的名称
	wstring m_realName;						//app的真实名称
	wstring m_appVersion;					//版本号
	wstring m_installstateStr;				//安装状态描述
	wstring m_platform;						//平台
	wstring m_installerror;					//安装错误描述	
	wstring m_casename;						//装机方案id
	wstring m_appiconpath;
	wstring m_oldappfullpath;					//软件全路径（升级专用）
	bool    m_needInstall;
	wstring m_appid;
	int     m_fid;
	bool    m_isLegalSoft;
	
	unsigned long m_nProcess;			//进度
	unsigned long m_nSpeed;				//下载速度
	DeviceData* _pDeviceData;			//设备句柄
	ProgressObserver* _pObserver;		//安装进度回调句柄 
	bool InstallToSdCard;			//是否安装到sd卡
	bool bUpadateGrade;
	AppData* appData;
	bool hasProcing;//是否处理处理状态，用于判断这个任务是否处在线程中.
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
	LPVOID _m_pCustomInfo;					//更多的相关信息

public:
	//获取本任务执行的百分比
	int GetTaskPercent(void);
	//设置状态
	void SetState(TASK_STATUS status);
	
	//重试次数
	void AddTriedTime(void);
	//开始时间
	void SetStartTimer(void);
	//完成时间
	void SetEndTimer(void);
	//拷贝
	void CopyValue(CTaskItem * target);
	
	void Lock();
	void Unlock();
};

//item列表
class TaskList : public CBase_ACK<CTaskItem>
{
public:
	TaskList(){};
	~TaskList(){};
};

#endif