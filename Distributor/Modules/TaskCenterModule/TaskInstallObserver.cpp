#include "StdAfx.h"
#include "TaskInstallObserver.h"
#include "TaskModuleInnerMsg.h"
#include "TaskControl.h"
#include "logic/TaskCenterController.h"

CTaskItem* curprocessItem=NULL;
wstring    CurdeviceId;

void CALLBACK InstallProgressTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{//INTERFUN;
	curprocessItem=(CTaskItem*)idEvent;
	if(curprocessItem==NULL||curprocessItem->m_uStatus!=PROCEDING)
	{
		::KillTimer(hwnd,idEvent);
		CurdeviceId=L"";
		return;
	}
	curprocessItem->m_nProcess++;
    if(curprocessItem->m_isLegalSoft)
	    curprocessItem->m_nProcess=curprocessItem->m_nProcess>=85?85:curprocessItem->m_nProcess;
    else
        curprocessItem->m_nProcess=curprocessItem->m_nProcess>=100?99:curprocessItem->m_nProcess;
	if(curprocessItem->_pDeviceData)
	{
		CTaskModuleInnerMsg msg;
		msg._type = TASK_UPDATESPEED;
		msg.wParam = (WPARAM)curprocessItem;
		msg.strParam = curprocessItem->_pDeviceData->GetId();
		msg.SendToUI();
	}
		//TaskCenterModel::GetInstance()->TaskUpdatedSpeedNotify(curprocessItem,curprocessItem->_pDeviceData->GetId());
}

void CALLBACK AuthorizationProgressTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
    curprocessItem=(CTaskItem*)idEvent;
    if(curprocessItem==NULL||curprocessItem->m_uStatus!=PROCEDING)
    {
        ::KillTimer(hwnd,idEvent);
        CurdeviceId=L"";
        return;
    }
    curprocessItem->m_nProcess++;
    curprocessItem->m_nProcess=curprocessItem->m_nProcess>=100?99:curprocessItem->m_nProcess;
    if(curprocessItem->_pDeviceData)
	{
		CTaskModuleInnerMsg msg;
		msg._type = TASK_UPDATESPEED;
		msg.wParam = (WPARAM)curprocessItem;
		msg.strParam = curprocessItem->_pDeviceData->GetId();
		msg.SendToUI();
	}
        //TaskCenterModel::GetInstance()->TaskUpdatedSpeedNotify(curprocessItem,curprocessItem->_pDeviceData->GetId());
}

TaskInstallObserver::TaskInstallObserver(wstring deviceId, TaskCenterController* model)
:_pTaskCentermodel(model)
,_deviceId(deviceId)
{//INTERFUN;

}
TaskInstallObserver::~TaskInstallObserver(void)
{//INTERFUN;
}

void TaskInstallObserver::GetPrompt( MESSAGE_CODE msgCode )
{//INTERFUN;
	GlobalTaskInfo * gtask= TaskControl::GetInstallTaskIndexByDevice(_deviceId);
	RETURN_IF(!gtask);
	CTaskItem* item = gtask->m_tlTaskList.GetTaskByID(_taskId);
	RETURN_IF(!item);
	wstring installstate=L"";
	switch(msgCode)
	{
		case MESSAGE_INSTALL_PREPARE:			// 准备数据	
			installstate=L"准备数据";
			break;
		case MESSAGE_INSTALL_UPLOAD:				// 上传APP文件
			{
				::KillTimer(Singleton::GetMainWnd(),(UINT_PTR)item);
				installstate=L"正在准备安装";
			}
			break;
		case MESSAGE_INSTALL_INSTALL:			// 安装程序
			{
				
				installstate=L"正在安装";
				if(item->m_uTaskType==MOVEANDROIDAPP)
				{
					item->_IsAndroidMoveStarted=true;
					installstate=L"正在转移";
				}
				//curprocessItem=item;
				CurdeviceId=_deviceId;
				wstring ext=CStrOperation::toLowerW(CFileOperation::GetFileExt(item->m_TaskFileInfo->savePath));
				int elapse=0;
				if(ext==L"ipa")
					elapse = 50;
				else if(ext==L"pxl")
					elapse = 3000;
				else 
				{
					elapse=CFileOperation::GetFileSizeW(item->m_TaskFileInfo->savePath)/1024*200/1024;
					if(item->appData)elapse=(int)(item->appData->_staticDiskUsage/1024*200/1024);
					if(elapse==0)elapse=200;
				}
				::SetTimer(Singleton::GetMainWnd(),(UINT_PTR)item,elapse,&InstallProgressTimerProc);
			}
			break;
        case MESSAGE_INSTALL_AUTHORIZOR:
            {
				::SetTimer(Singleton::GetMainWnd(),(UINT_PTR)item,500,&AuthorizationProgressTimerProc);
            }
            break;
		case MESSAGE_INSTALL_UNINSTALL:          // 卸载程序
			installstate=L"卸载程序";
			break;
		case MESSAGE_INSTALL_COPYDATA:			// 拷贝程序数据
			installstate=L"拷贝程序数据";
			break;
		case MESSAGE_INSTALL_NOTIFYSB:			// 修正IOS软件图标
			//installstate=L"修正IOS软件图标";
			break;
		case MESSAGE_INSTALL_PXLTOIPA:          // 转换
			installstate=L"pxl转ipa";
			break;
		case MESSAGE_ERROR:
			{
				installstate=L"安装错误";
				item->m_uStatus=FAILED;
				if(_pTaskCentermodel)
				{
					_pTaskCentermodel->TaskFailNotify(item,_deviceId);
				}
				
				return;
			}
			break;
		default:
			break;
	}
	item->m_installstateStr = installstate;
	if(_pTaskCentermodel)
		_pTaskCentermodel->TaskUpdatedSpeedNotify(item, _deviceId);
}

void TaskInstallObserver::GetProgress( ProgressData pData )
{//INTERFUN;
	GlobalTaskInfo * gtask= TaskControl::GetInstallTaskIndexByDevice(_deviceId);
	RETURN_IF(!gtask);
	CTaskItem* item = gtask->m_tlTaskList.GetTaskByID(_taskId);
	if(!item)
		return;
	if(pData._currentTaskSum>0)
    {
        int nPer = BaseOperation::GetPer(pData._allTaskProgresss, pData._allTaskSum); //pData._currentTaskProgress*100/(pData._currentTaskSum*pData._taskNumberSum)+pData._taskNumberCompleted*100/pData._taskNumberSum;
        if (nPer < 0 || nPer > 100)
            return;
		wstring ext=CStrOperation::toLowerW(CFileOperation::GetFileExt(item->m_TaskFileInfo->savePath));
		if(ext==L"ipa")
			item->m_nProcess = nPer * (CFileOperation::GetFileSizeW(item->m_TaskFileInfo->savePath)>(10*1024*1024)?80:50) / 100;
		else if(ext==L"pxl")
			item->m_nProcess=  nPer * 30 / 100;
		else 
			item->m_nProcess=  nPer * 50 / 100;
    }
	//LOG->WriteDebug(L"SoftID="+CStrOperation::Int64ToWString(item->m_fid)+L"UpLoad Process:"+CStrOperation::Int64ToWString(item->m_nProcess));
	if(_pTaskCentermodel)
	 _pTaskCentermodel->TaskUpdatedSpeedNotify(item, _deviceId);
}

void TaskInstallObserver::Complete()
{//INTERFUN;
	//curprocessItem=NULL;
}

ND91Assistant::EXIST_ACTION TaskInstallObserver::GetExistAction( std::wstring name )
{//INTERFUN;
	return NONE;
}
