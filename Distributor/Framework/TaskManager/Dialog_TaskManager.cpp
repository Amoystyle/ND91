// D:\91����c++\91MobileCpp\Main\Demo\Demo\TaskManager\Dialog_TaskManager.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Dialog_TaskManager.h"
#include "TaskItem.h"
#include "TaskList.h"
#include "TaskControl.h"

// Dialog_TaskManager �Ի���
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
	m_LCETask.InsertColumn(1, _T("״̬"),   LVCFMT_LEFT, 80);
	m_LCETask.InsertColumn(2, _T("����"),   LVCFMT_LEFT, 40);
	m_LCETask.InsertColumn(3, _T("��������"),   LVCFMT_LEFT, 40);
	m_LCETask.InsertColumn(4, _T("����"),   LVCFMT_LEFT, 40);

	// �Ƿ񴴽�"�Զ���������"��ʱ��
	if (m_bAutoClearDone)
	{
		SetTimer(TIMER_CLEARDONE, 500, NULL);
		((CButton*)this->GetDlgItem(IDC_CHECK_CLEARDONE))->SetCheck(BST_CHECKED);
	}
	else
	{
		((CButton*)this->GetDlgItem(IDC_CHECK_CLEARDONE))->SetCheck(BST_UNCHECKED);
	}
	CString l_str = _T("�������");
	this->SetWindowText(l_str);

	ShowWindow(SW_SHOW);
	SetTimer(TIMER_UPDATESTATE, 50, NULL);
	return TRUE;
}

// Dialog_TaskManager ��Ϣ�������

void Dialog_TaskManager::OnBnClickedButtonStart()
{INTERFUN;
	int column = m_LCETask.GetNextItem(   -1,   LVNI_ALL   |   LVNI_SELECTED);
	if(column<0)
		return;
	if(column<0)
		return;
	m_bLock = TRUE;

	// ��ȡ��ǰѡ�����
	int l_iTaskID = m_LCETask.GetItemData(column);
	// ���������
	if (m_pTaskList->ActivateItem(l_iTaskID))
	{
		UpdateRow(column);
	}

	// ����ִ���߳�
	TaskControl::g_gtiGlobalTaskInfo->StartThread();

	m_bLock = FALSE;
}

void Dialog_TaskManager::OnBnClickedButtonPause()
{INTERFUN;
	int column = m_LCETask.GetNextItem(   -1,   LVNI_ALL   |   LVNI_SELECTED);
	if(column<0)
		return;
	m_bLock = TRUE;
	// ��ȡ��ǰѡ�����
	int l_iTaskID = m_LCETask.GetItemData(column);
	TASK_STATUS l_stateTask;
	if (!m_pTaskList->GetTaskState(l_iTaskID, l_stateTask))
	{	
		m_bLock = FALSE;
		return ;
	}
	// ���ݵ�ǰ�����״̬������Ӧ�Ĳ���
	if (PROCEDING == l_stateTask)
	{
		int threadid = TaskControl::getDealTaskThreadNumByTaskID(l_iTaskID);
		if(threadid >=0 )
			TaskControl::g_gtiGlobalTaskInfo->SuspendDealingTask(threadid);
	}
	// ���������
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

	// ��ȡ��ǰѡ�����
	int l_iTaskID = m_LCETask.GetItemData(column);
	// ���������
	if (m_pTaskList->RetryItem(l_iTaskID))
	{
		UpdateRow(column);
	}

	// ����ִ���߳�
	TaskControl::g_gtiGlobalTaskInfo->StartThread();

	m_bLock = FALSE;
}

void Dialog_TaskManager::OnBnClickedButtonModifytask()
{INTERFUN;
	if(m_LCETask.GetSelectedColumn()<0)
		return;
	// ��ȡ��ǰѡ�����
	int l_iTaskID = m_LCETask.GetItemData(m_LCETask.GetSelectedColumn());
	// ��ȡѡ������Ĵ�����Ϣ
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

	// ��ȡ��ǰѡ�����
	int l_iCurSel = column;
	int l_iTaskID = m_LCETask.GetItemData(l_iCurSel);
	// ɾ������
	if (m_pTaskList->DeleteTaskByID(l_iTaskID))
	{
		// �ڽ��������ֱ仯
		m_LCETask.DeleteItem(l_iCurSel);
		// ɾ������ID�б��еĶ�ӦԪ��
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
		AfxMessageBox(_T("�����Ѿ�����"));
		return;
	}

	TaskControl::g_gtiGlobalTaskInfo->StartThread();
}

void Dialog_TaskManager::SetTaskList(CTaskList* pTaskList)
{INTERFUN;
	// 1����֤����
	ASSERT(pTaskList);

	// 2����ȡ�����б������ID�б�
	pTaskList->GetTaskIDs(&m_listTaskID);

	// 3�����������б�
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
	// ����
	m_bLock = TRUE;

	// ������߳̽ṹ���Ѿ���ɵ�������
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
				// ��һ������
				l_itrTaskID = m_listTaskID.erase(l_itrTaskID);
			}
			else
			{
				l_itrTaskID ++;
			}
		}
	}

	// ����
	m_bLock = FALSE;
}

void Dialog_TaskManager::OnBnClickedCheckCleardone()
{INTERFUN;
	// ����
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

	// ����
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
		// �������������б�ļ�ʱ��
		SetTimer(TIMER_UPDATESTATE, 50, NULL);
	}
	else
	{
		// �رո��������б�ļ�ʱ��
		KillTimer(TIMER_UPDATESTATE);
	}
}

void Dialog_TaskManager::UpdateRow(int iRow)
{INTERFUN;
	CString l_strTemp;
	CTime l_timeTemp;
	TASK_STATUS l_stateTemp;
	TASK_KIND l_kindTemp;
	// �����б���ȡ�����е�ֵ�����ݸ��е�ֵ
	for (int i = 0; i < 5; i ++)
	{
		int l_iID = m_LCETask.GetItemData(iRow);
		switch (i)
		{
		case COL_TASKID:				// ����ID
			{
				l_strTemp.Format(_T("_%d"), l_iID);
				m_LCETask.SetItemText(iRow, i, m_pTaskList->GetTaskName(l_iID) + l_strTemp);
			}
			break;
		case COL_TASKSTATE:				// ����״̬
			{
				if (m_pTaskList->GetTaskState(l_iID, l_stateTemp))
				{
					m_LCETask.SetItemText(iRow, i, Getstatename(l_stateTemp));
				}
			}
			break;
		case COL_TASKTYPE:				// ��������
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
		case COL_TASKPROGRESS:			// �������
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
	// ��ȡָ���������״̬
	TASK_STATUS l_stateTask;
	m_pTaskList->GetTaskState(iTaskID, l_stateTask);

	// ���������ı�
	CString l_strDescription;
	switch (l_stateTask)
	{
	case FAILED:
		l_strDescription =  _T("�����쳣����鿴������Ϣ");
			break;
	case FINISHED:
		l_strDescription = _T("���");
		break;
	case SUSPENDED:
		l_strDescription =  _T("��ͣ");
		break;		
	case WAITING:
		l_strDescription = _T("�ȴ�");
		break;
	case PROCEDING:
		l_strDescription =_T("���ڴ���");
		break;
	}

	// Ϊ�б������ı�
	m_LCETask.SetItemText(iRow, iColumn, l_strDescription);
}

CString Dialog_TaskManager::Getstatename(UINT status)
{INTERFUN;
	CString ret;
	switch(status)
	{
	case TASK_STATUS::DEALLOCK:
		ret = _T("����");
		break;
	case TASK_STATUS::FAILED:
		ret = _T("ʧ��");
		break;
	case TASK_STATUS::FINISHED:
		ret = _T("����");
		break;
	case TASK_STATUS::PROCEDING:
		ret = _T("���ڴ���");
		break;
	case TASK_STATUS::SUSPENDED:
		ret = _T("��ͣ");
		break;
	case TASK_STATUS::WAITING:
		ret = _T("�ȴ�");
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
		 ret = _T("��װ");
		 break;
	 case TASK_KIND::UNINSTALL:
		 ret = _T("ж��");
		 break;
	 case TASK_KIND::URL_DOWNLAOD:
		 ret = _T("����");
		 break;
	 }
	 return ret;
}

void Dialog_TaskManager::_UpdateLCE()
{INTERFUN;
	// ��ʼ��ID�б�
	m_listTaskID.clear();
	SetTaskList(&(TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList));

	// ����������������ID�б�Ϊ׼���������ID�б���Ԫ�غ��б�ؼ�����һ��������б���������ɾ����
	int l_iIndex = 0;
	std::list<int>::iterator l_itr = m_listTaskID.begin();
	while (l_itr != m_listTaskID.end())
	{
		// ������������û�г��������б�ؼ�����ʱ�����б�ؼ��б��������ID������ID�б��е����ݶԱ�
		// ���һ�£���ֻ�������ݣ�����������ID�б���������б�ؼ�������
		// �����һ�£������б�ؼ���ɾ��������,ֻ�����б�ؼ�������
		if (l_iIndex < m_LCETask.GetItemCount())
		{
			// һ�µ����
			if ((*l_itr) == m_LCETask.GetItemData(l_iIndex))
			{
				// ���¸��е�����
				UpdateRow(l_iIndex);
				// ����������
				l_itr ++;
			}
			// ��һ�µ����
			else
			{
				m_LCETask.DeleteItem(l_iIndex);
			}
		}
		// ���������г������б�ؼ��е�������ʱ��������ID�б���������һ��������б�ؼ������һ��
		else
		{
			CString str;
			str.Format(_T("%d"),l_iIndex);
			// ���б�ؼ���������һ��
			m_LCETask.InsertItem(l_iIndex,str);
			m_LCETask.SetItemData(l_iIndex, (*l_itr));
			// ���¸��е�����
			UpdateRow(l_iIndex);
			// ����������
			l_itr ++;
		}		

		// �����б�ؼ�������
		l_iIndex ++;
	}
	// �ڱ���������ID�б������б�ؼ��е���������û�г����б�ؼ�������������ɾ������Щʣ�����
	for (; l_iIndex < m_LCETask.GetItemCount(); l_iIndex ++)
	{
		m_LCETask.DeleteItem(l_iIndex);
	}

	// ˢ�½���
	//m_LCETask.Invalidate();
	//m_LCETask.UpdateWindow();
}

void Dialog_TaskManager::OnBnClickedButtonAddtask2()
{INTERFUN;
	CTaskItem taskItem(0, _T("test1"),_T("http://da.91rb.com/soft/iphone/91Tribe_v1.3.ipa"),  TASK_KIND::URL_DOWNLAOD,_pDeviceData, _T("d:\\TaskDownLoad\\"));

	if(TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.AddItem(taskItem) == -1)
	{
		AfxMessageBox(_T("�����Ѿ�����"));
		return;
	}

	TaskControl::g_gtiGlobalTaskInfo->StartThread();
}

void Dialog_TaskManager::OnBnClickedButtonAddtask3()
{INTERFUN;
	CTaskItem taskItem(0, _T("test1"),_T("http://dl.91rb.com/cc45f38dd7140b7474bcfe1838cf11ae/soft/iphone/LYOL_IPHONE_BETA_V1.0.0.5(BYWX)_1.ipa"), TASK_KIND::URL_DOWNLAOD, _pDeviceData, _T("d:\\TaskDownLoad\\"));

	if(TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.AddItem(taskItem) == -1)
	{
		AfxMessageBox(_T("�����Ѿ�����"));
		return;
	}

	TaskControl::g_gtiGlobalTaskInfo->StartThread();
}
