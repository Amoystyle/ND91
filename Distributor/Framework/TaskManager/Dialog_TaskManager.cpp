// D:\91助手c++\91MobileCpp\Main\Demo\Demo\TaskManager\Dialog_TaskManager.cpp : 实现文件
//

#include "stdafx.h"
#include "Dialog_TaskManager.h"
#include "TaskItem.h"
#include "TaskList.h"
#include "TaskControl.h"

// Dialog_TaskManager 对话框
enum
{
	COL_TASKID = 0,
	COL_TASKSTATE,
	COL_TASKPROGRESS,
	COL_TASKTYPE,
	COL_TASKPROINFO
}; 
IMPLEMENT_DYNAMIC(Dialog_TaskManager, CDialogBase)

Dialog_TaskManager::Dialog_TaskManager(CWnd* pParent /*=NULL*/)
	: CDialogBase(Dialog_TaskManager::IDD, pParent)
{INTERFUN;
	m_pTaskList = NULL;
	m_listTaskID.clear();
	m_bAutoClearDone = FALSE;
	m_bLock = FALSE;	
	ThreadTreeControl::InitThreadTreeControl(_T("d:\\91AssistantTaskThreadLog.log"));
}

Dialog_TaskManager::~Dialog_TaskManager()
{INTERFUN;
	m_listTaskID.clear();
	TaskControl::Destroy();
}

void Dialog_TaskManager::DoDataExchange(CDataExchange* pDX)
{INTERFUN;
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TASk, m_LCETask);
	DDX_Control(pDX,IDC_CHECK_CLEARDONE,m_checkAutoClearDone);
}


BEGIN_MESSAGE_MAP(Dialog_TaskManager, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_START, &Dialog_TaskManager::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &Dialog_TaskManager::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_RETRY, &Dialog_TaskManager::OnBnClickedButtonRetry)
	ON_BN_CLICKED(IDC_BUTTON_MODIFYTASK, &Dialog_TaskManager::OnBnClickedButtonModifytask)
	ON_BN_CLICKED(IDC_BUTTON_DELTASK, &Dialog_TaskManager::OnBnClickedButtonDeltask)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &Dialog_TaskManager::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_ADDTASK, &Dialog_TaskManager::OnBnClickedButtonAddtask)
	ON_BN_CLICKED(IDC_BUTTON_CLEARDONE, &Dialog_TaskManager::OnBnClickedButtonCleardone)
	ON_BN_CLICKED(IDC_CHECK_CLEARDONE, &Dialog_TaskManager::OnBnClickedCheckCleardone)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_ADDTASK2, &Dialog_TaskManager::OnBnClickedButtonAddtask2)
	ON_BN_CLICKED(IDC_BUTTON_ADDTASK3, &Dialog_TaskManager::OnBnClickedButtonAddtask3)
END_MESSAGE_MAP()
BOOL Dialog_TaskManager::OnInitDialog()
{INTERFUN;
	 __super::OnInitDialog();

	//CenterWindow();
	 TaskControl::Init();

	m_LCETask.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_LCETask.InsertColumn(0, _T("ID"),     LVCFMT_LEFT, 40);
	m_LCETask.InsertColumn(1, _T("状态"),   LVCFMT_LEFT, 80);
	m_LCETask.InsertColumn(2, _T("进度"),   LVCFMT_LEFT, 40);
	m_LCETask.InsertColumn(3, _T("任务类型"),   LVCFMT_LEFT, 40);
	m_LCETask.InsertColumn(4, _T("描述"),   LVCFMT_LEFT, 40);

	// 是否创建"自动清除已完成"定时器
	if (m_bAutoClearDone)
	{
		SetTimer(TIMER_CLEARDONE, 500, NULL);
		((CButton*)this->GetDlgItem(IDC_CHECK_CLEARDONE))->SetCheck(BST_CHECKED);
	}
	else
	{
		((CButton*)this->GetDlgItem(IDC_CHECK_CLEARDONE))->SetCheck(BST_UNCHECKED);
	}
	CString l_str = _T("任务管理");
	this->SetWindowText(l_str);

	ShowWindow(SW_SHOW);
	SetTimer(TIMER_UPDATESTATE, 50, NULL);
	return TRUE;
}

// Dialog_TaskManager 消息处理程序

void Dialog_TaskManager::OnBnClickedButtonStart()
{INTERFUN;
	int column = m_LCETask.GetNextItem(   -1,   LVNI_ALL   |   LVNI_SELECTED);
	if(column<0)
		return;
	if(column<0)
		return;
	m_bLock = TRUE;

	// 获取当前选择的项
	int l_iTaskID = m_LCETask.GetItemData(column);
	// 激活该任务
	if (m_pTaskList->ActivateItem(l_iTaskID))
	{
		UpdateRow(column);
	}

	// 开启执行线程
	TaskControl::g_gtiGlobalTaskInfo->StartThread();

	m_bLock = FALSE;
}

void Dialog_TaskManager::OnBnClickedButtonPause()
{INTERFUN;
	int column = m_LCETask.GetNextItem(   -1,   LVNI_ALL   |   LVNI_SELECTED);
	if(column<0)
		return;
	m_bLock = TRUE;
	// 获取当前选择的项
	int l_iTaskID = m_LCETask.GetItemData(column);
	TASK_STATUS l_stateTask;
	if (!m_pTaskList->GetTaskState(l_iTaskID, l_stateTask))
	{	
		m_bLock = FALSE;
		return ;
	}
	// 根据当前任务的状态，做对应的操作
	if (PROCEDING == l_stateTask)
	{
		int threadid = TaskControl::getDealTaskThreadNumByTaskID(l_iTaskID);
		if(threadid >=0 )
			TaskControl::g_gtiGlobalTaskInfo->SuspendDealingTask(threadid);
	}
	// 挂起该任务
	//else 
	//{
		m_pTaskList->SuspendItem(l_iTaskID);
	//}

	UpdateRow(column);

	m_bLock = FALSE;
}

void Dialog_TaskManager::OnBnClickedButtonRetry()
{INTERFUN;
	int column = m_LCETask.GetNextItem(   -1,   LVNI_ALL   |   LVNI_SELECTED);
	if(column<0)
		return;

	if(column<0)
		return;
	m_bLock = TRUE;

	// 获取当前选择的项
	int l_iTaskID = m_LCETask.GetItemData(column);
	// 激活该任务
	if (m_pTaskList->RetryItem(l_iTaskID))
	{
		UpdateRow(column);
	}

	// 开启执行线程
	TaskControl::g_gtiGlobalTaskInfo->StartThread();

	m_bLock = FALSE;
}

void Dialog_TaskManager::OnBnClickedButtonModifytask()
{INTERFUN;
	if(m_LCETask.GetSelectedColumn()<0)
		return;
	// 获取当前选择的项
	int l_iTaskID = m_LCETask.GetItemData(m_LCETask.GetSelectedColumn());
	// 获取选择任务的错误信息
	LPCTSTR l_szErrorInfo;
	l_szErrorInfo = m_pTaskList->GetTaskNote(l_iTaskID);
	if (!l_szErrorInfo)
	{
		return ;
	}

	CString l_strErrorInfo(l_szErrorInfo);

	AfxMessageBox(l_szErrorInfo);
}

void Dialog_TaskManager::OnBnClickedButtonDeltask()
{INTERFUN;
	int column = m_LCETask.GetNextItem(   -1,   LVNI_ALL   |   LVNI_SELECTED);
	if(column<0)
		return;
	if(column<0)
		return;
	m_bLock = TRUE;

	// 获取当前选择的项
	int l_iCurSel = column;
	int l_iTaskID = m_LCETask.GetItemData(l_iCurSel);
	// 删除任务
	if (m_pTaskList->DeleteTaskByID(l_iTaskID))
	{
		// 在界面中体现变化
		m_LCETask.DeleteItem(l_iCurSel);
		// 删除任务ID列表中的对应元素
		for (std::list<int>::iterator l_itr = m_listTaskID.begin();
			l_itr != m_listTaskID.end(); l_itr ++)
		{
			if ((*l_itr) == l_iTaskID)
			{
				m_listTaskID.erase(l_itr);
				m_bLock = FALSE;
				return ;
			}
		}
	}

	m_bLock = FALSE;
}
void Dialog_TaskManager::SetDeviceData( DeviceData* pDeviceData )
{INTERFUN;
	_pDeviceData = pDeviceData;
}

void Dialog_TaskManager::OnDisconnect()
{INTERFUN;

}
void Dialog_TaskManager::OnBnClickedButtonClose()
{INTERFUN;
	return  OnClose();
}

void Dialog_TaskManager::OnBnClickedButtonAddtask()
{INTERFUN;
	CTaskItem taskItem(0, _T("test1"),_T("http://dl.91rb.com/b11c5f5789c32aef81ac2a160e4c5f37/soft/farm/91farm_v1.56.ipa"),  TASK_KIND::URL_DOWNLAOD,_pDeviceData, _T("d:\\TaskDownLoad\\"));

	if(TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.AddItem(taskItem) == -1)
	{
		AfxMessageBox(_T("任务已经存在"));
		return;
	}

	TaskControl::g_gtiGlobalTaskInfo->StartThread();
}

void Dialog_TaskManager::SetTaskList(CTaskList* pTaskList)
{INTERFUN;
	// 1、验证参数
	ASSERT(pTaskList);

	// 2、获取任务列表的任务ID列表
	pTaskList->GetTaskIDs(&m_listTaskID);

	// 3、设置任务列表
	m_pTaskList = pTaskList;
}

void Dialog_TaskManager::OnTimer(UINT_PTR nIDEvent)
{INTERFUN;
	if (m_bLock)
	{
		TRACE ("CDlgAlbTaskCtrl::OnTimer lock ... \n");
		return ;
	}

	if (TIMER_CLEARDONE == nIDEvent)
	{
		OnBnClickedButtonCleardone();
	}
	else if (TIMER_UPDATESTATE == nIDEvent)
	{
		_UpdateLCE();
	}
}
void Dialog_TaskManager::OnBnClickedButtonCleardone()
{INTERFUN;
	// 加锁
	m_bLock = TRUE;

	// 先清除线程结构中已经完成的任务项
	std::list<int> l_listFinish;
	m_pTaskList->ClearDone(&l_listFinish);

	for (std::list<int>::iterator l_itr = l_listFinish.begin(); 
		l_itr != l_listFinish.end(); l_itr ++)
	{
		std::list<int>::iterator l_itrTaskID = m_listTaskID.begin();
		while (l_itrTaskID != m_listTaskID.end())
		{
			if ((*l_itrTaskID) == (*l_itr))
			{
				// 下一迭代器
				l_itrTaskID = m_listTaskID.erase(l_itrTaskID);
			}
			else
			{
				l_itrTaskID ++;
			}
		}
	}

	// 开锁
	m_bLock = FALSE;
}

void Dialog_TaskManager::OnBnClickedCheckCleardone()
{INTERFUN;
	// 加锁
	m_bLock = TRUE;

	if (1 == m_checkAutoClearDone.GetCheck())
	{
		m_bAutoClearDone = TRUE;
		SetTimer(TIMER_CLEARDONE, 500, NULL);
	}
	else
	{
		m_bAutoClearDone = FALSE;
		KillTimer(TIMER_CLEARDONE);
	}

	// 开锁
	m_bLock = FALSE;
}
void Dialog_TaskManager::OnClose() 
{INTERFUN;
	this->ShowWindow(SW_HIDE);
}
void Dialog_TaskManager::OnShowWindow(BOOL bShow, UINT nStatus)
{INTERFUN;
	if (bShow)
	{
		// 开启更新任务列表的计时器
		SetTimer(TIMER_UPDATESTATE, 50, NULL);
	}
	else
	{
		// 关闭更新任务列表的计时器
		KillTimer(TIMER_UPDATESTATE);
	}
}

void Dialog_TaskManager::UpdateRow(int iRow)
{INTERFUN;
	CString l_strTemp;
	CTime l_timeTemp;
	TASK_STATUS l_stateTemp;
	TASK_KIND l_kindTemp;
	// 遍历列表，获取各个列的值，根据该列的值
	for (int i = 0; i < 5; i ++)
	{
		int l_iID = m_LCETask.GetItemData(iRow);
		switch (i)
		{
		case COL_TASKID:				// 任务ID
			{
				l_strTemp.Format(_T("_%d"), l_iID);
				m_LCETask.SetItemText(iRow, i, m_pTaskList->GetTaskName(l_iID) + l_strTemp);
			}
			break;
		case COL_TASKSTATE:				// 任务状态
			{
				if (m_pTaskList->GetTaskState(l_iID, l_stateTemp))
				{
					m_LCETask.SetItemText(iRow, i, Getstatename(l_stateTemp));
				}
			}
			break;
		case COL_TASKTYPE:				// 任务类型
			{
				if (m_pTaskList->GetTaskType(l_iID, l_kindTemp))
				{
					CString l_str = GetKindText(l_kindTemp);
					m_LCETask.SetItemText(iRow, i, l_str);
				}
			}
			break;
		case COL_TASKPROINFO:
			{
				_UpdateDescription(iRow, i, l_iID);
			}
			break;
		case COL_TASKPROGRESS:			// 任务进度
			{
				_UpdateProgress(iRow, i, l_iID);
			}
			break;
		default:
			break;
		}
	}
}
void Dialog_TaskManager::_UpdateProgress(int iRow, int iColumn, int iTaskID)
{INTERFUN;
	TASK_KIND l_kindTemp;
	TASK_STATUS l_stateTemp;
	m_pTaskList->GetTaskType(iTaskID, l_kindTemp);
	m_pTaskList->GetTaskState(iTaskID, l_stateTemp);
	int l_iPosNew = m_pTaskList->GetTaskPercent(iTaskID);
	CString l_stProcess;
	l_stProcess.Format(_T("%d%%"),l_iPosNew);
	m_LCETask.SetItemText(iRow, iColumn, l_stProcess);
}
void Dialog_TaskManager::_UpdateDescription(int iRow, int iColumn, int iTaskID)
{INTERFUN;
	// 获取指定任务项的状态
	TASK_STATUS l_stateTask;
	m_pTaskList->GetTaskState(iTaskID, l_stateTask);

	// 设置描述文本
	CString l_strDescription;
	switch (l_stateTask)
	{
	case FAILED:
		l_strDescription =  _T("任务异常，请查看错误信息");
			break;
	case FINISHED:
		l_strDescription = _T("完成");
		break;
	case SUSPENDED:
		l_strDescription =  _T("暂停");
		break;		
	case WAITING:
		l_strDescription = _T("等待");
		break;
	case PROCEDING:
		l_strDescription =_T("正在处理");
		break;
	}

	// 为列表设置文本
	m_LCETask.SetItemText(iRow, iColumn, l_strDescription);
}

CString Dialog_TaskManager::Getstatename(UINT status)
{INTERFUN;
	CString ret;
	switch(status)
	{
	case TASK_STATUS::DEALLOCK:
		ret = _T("锁定");
		break;
	case TASK_STATUS::FAILED:
		ret = _T("失败");
		break;
	case TASK_STATUS::FINISHED:
		ret = _T("结束");
		break;
	case TASK_STATUS::PROCEDING:
		ret = _T("正在处理");
		break;
	case TASK_STATUS::SUSPENDED:
		ret = _T("暂停");
		break;
	case TASK_STATUS::WAITING:
		ret = _T("等待");
		break;
	}
	return ret;
}

CString Dialog_TaskManager::GetKindText(UINT kind)
{INTERFUN;
	CString ret;
	 switch(kind)
	 {
	 case TASK_KIND::INSTALL:
		 ret = _T("安装");
		 break;
	 case TASK_KIND::UNINSTALL:
		 ret = _T("卸载");
		 break;
	 case TASK_KIND::URL_DOWNLAOD:
		 ret = _T("下载");
		 break;
	 }
	 return ret;
}

void Dialog_TaskManager::_UpdateLCE()
{INTERFUN;
	// 初始化ID列表
	m_listTaskID.clear();
	SetTaskList(&(TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList));

	// 遍历过程中以任务ID列表为准，如果任务ID列表中元素和列表控件数据一样，则该行保留，否则，删除掉
	int l_iIndex = 0;
	std::list<int>::iterator l_itr = m_listTaskID.begin();
	while (l_itr != m_listTaskID.end())
	{
		// 当操作的项数没有超过超过列表控件行数时，则列表控件中保存的任务ID与任务ID列表中的数据对比
		// 如果一致，则只设置数据，并递增任务ID列表迭代器和列表控件行索引
		// 如果不一致，则在列表控件中删除掉该行,只递增列表控件行索引
		if (l_iIndex < m_LCETask.GetItemCount())
		{
			// 一致的情况
			if ((*l_itr) == m_LCETask.GetItemData(l_iIndex))
			{
				// 更新该行的数据
				UpdateRow(l_iIndex);
				// 递增迭代器
				l_itr ++;
			}
			// 不一致的情况
			else
			{
				m_LCETask.DeleteItem(l_iIndex);
			}
		}
		// 当操作的行超过了列表控件中的总行数时，在任务ID列表中索引到一个项，便在列表控件中添加一行
		else
		{
			CString str;
			str.Format(_T("%d"),l_iIndex);
			// 向列表控件中最后插入一行
			m_LCETask.InsertItem(l_iIndex,str);
			m_LCETask.SetItemData(l_iIndex, (*l_itr));
			// 更新该行的数据
			UpdateRow(l_iIndex);
			// 递增迭代器
			l_itr ++;
		}		

		// 递增列表控件行索引
		l_iIndex ++;
	}
	// 在遍历完任务ID列表后，如果列表控件中的行索引还没有超过列表控件的行总数，则删除掉这些剩余的行
	for (; l_iIndex < m_LCETask.GetItemCount(); l_iIndex ++)
	{
		m_LCETask.DeleteItem(l_iIndex);
	}

	// 刷新界面
	//m_LCETask.Invalidate();
	//m_LCETask.UpdateWindow();
}

void Dialog_TaskManager::OnBnClickedButtonAddtask2()
{INTERFUN;
	CTaskItem taskItem(0, _T("test1"),_T("http://da.91rb.com/soft/iphone/91Tribe_v1.3.ipa"),  TASK_KIND::URL_DOWNLAOD,_pDeviceData, _T("d:\\TaskDownLoad\\"));

	if(TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.AddItem(taskItem) == -1)
	{
		AfxMessageBox(_T("任务已经存在"));
		return;
	}

	TaskControl::g_gtiGlobalTaskInfo->StartThread();
}

void Dialog_TaskManager::OnBnClickedButtonAddtask3()
{INTERFUN;
	CTaskItem taskItem(0, _T("test1"),_T("http://dl.91rb.com/cc45f38dd7140b7474bcfe1838cf11ae/soft/iphone/LYOL_IPHONE_BETA_V1.0.0.5(BYWX)_1.ipa"), TASK_KIND::URL_DOWNLAOD, _pDeviceData, _T("d:\\TaskDownLoad\\"));

	if(TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.AddItem(taskItem) == -1)
	{
		AfxMessageBox(_T("任务已经存在"));
		return;
	}

	TaskControl::g_gtiGlobalTaskInfo->StartThread();
}
