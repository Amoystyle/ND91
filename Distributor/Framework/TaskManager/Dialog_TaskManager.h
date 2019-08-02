#pragma once

#include "../Demo.h"
#include "../DialogBase.h"
// Dialog_TaskManager 对话框
class CTaskList;
class CTaskItem;
enum
{
		TIMER_CLEARDONE = 1,
		TIMER_UPDATESTATE,
};
using namespace ND91Assistant;
class Dialog_TaskManager : public CDialogBase
{
	DECLARE_DYNAMIC(Dialog_TaskManager)

public:
	Dialog_TaskManager(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Dialog_TaskManager();

// 对话框数据
	enum { IDD = IDD_DLG_TASKCTRL };

	void SetDeviceData(DeviceData* pDeviceData);

	virtual void OnDisconnect();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnBnClickedButtonRetry();
	afx_msg void OnBnClickedButtonModifytask();
	afx_msg void OnBnClickedButtonDeltask();
	afx_msg void OnBnClickedButtonClose();
	virtual BOOL OnInitDialog();

private:
	DeviceData*  _pDeviceData;
	CTaskList*			m_pTaskList;
	std::list<int>		m_listTaskID;
	BOOL				m_bAutoClearDone;
	BOOL				m_bLock;									// 用于同步计时器的操作
public:
	 CButton				m_checkAutoClearDone;
	 CListCtrl   m_LCETask;
	 afx_msg void OnBnClickedButtonAddtask();
	 afx_msg void OnBnClickedButtonCleardone();
	 afx_msg void OnBnClickedCheckCleardone();
	 void _UpdateLCE();
	 CString GetKindText(UINT kind);
	 CString Getstatename(UINT status);
	 void _UpdateDescription(int iRow, int iColumn, int iTaskID);
	 void _UpdateProgress(int iRow, int iColumn, int iTaskID);
	 void UpdateRow(int iRow);
	 afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	 afx_msg void OnClose();
	 void SetTaskList(CTaskList* pTaskList);
	 afx_msg void OnTimer(UINT_PTR nIDEvent);
	 afx_msg void OnBnClickedButtonAddtask2();
	 afx_msg void OnBnClickedButtonAddtask3();
};
