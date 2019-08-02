#pragma once
#include "logic/TaskCenterController.h"
class TaskListUI
	: public CTabItem
	, public BaseHandler<DelTaskItemMsg>

	//, public CommonUI
	//, public MessageReceiver        // ������Ϣ
{
public:
	struct TooltipData{
		wstring name;		//����
		wstring version;	//�汾
		wstring size;		//��С
		wstring platform;	//ƽ̨
	};
public:
	TaskListUI(wstring deviceId, TASK_LIST_TYPE isFinishedList);
	~TaskListUI(void);

	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_pause_task", OnBottomBtnPause);//��ͣ��ť��Ӧ�¼�
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_start_task", OnBottomBtnStart);//��ʼ��ť��Ӧ�¼�
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_del_task", OnBottomBtnDel);//ɾ����ť��Ӧ�¼�
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_del_task_option",OnBottomBtnDelOption); //ɾ��ѡ�ť��Ӧ�¼�
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"item_btn_start",OnItemBtnStart); //�б��п�ʼ��ť��Ӧ�¼�
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"item_btn_del",OnItemBtnDel); //�б���ɾ����ť��Ӧ�¼�
		EVNET_ID_HANDLER(UINOTIFY_RCLICK,L"downtasklist",OnListRightClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK,L"downtasklist",OnListClick);
		EVNET_ID_HANDLER(UINOTIFY_DBCLICK,L"downtasklist",OnListClick);
		//EVNET_ID_HANDLER(UINOTIFY_HOVER,L"downtasklist",OnListHOVER);
		EVNET_ID_HANDLER(UINOTIFY_TVN_CHECK,L"downtasklist",OnlistChecked);//
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"install_to",OnBottomInstall);//��װ��
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"add_to",OnBottomAddToCase);//��ӵ�
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"no_tasks_btn",OnNoTaskClick);

		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_Android", OnPlatClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_iPhone", OnPlatClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_iPad", OnPlatClick);

		
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK,20000,OnMenuClick);//�Ҽ��˵�
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK,21000,OnMenuClick);//�Ҽ��˵�
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK,22000,OnMenuClick);//�Ҽ��˵�
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK,23000,OnMenuClick);//�Ҽ��˵�
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK,24000,OnMenuClick);//�ײ���װ�˵�
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK,25000,OnMenuClick);//�ײ������˵�
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK,8000,OnMenuAutoDelClick);//�ײ�ɾ���˵�
		
	UIEND_MSG_MAP


	void OnCreate();

	// Summary  :����һ�������б�
	// Returns	: void
	// Parameter: CTaskItem * item����
	void InsertTask(CTaskItem* item);

	// ˢ��
	void RefreshData();

	// ����һ�������¼
	void UpdateTaskUI(CTaskItem* item,bool finished=false);

	// �Ƴ�һ������
	void RemoveTaskItemUI(int taskid);

	// ������Ϣ
	//void Receive(MESSAGE_TYPE msg, const BaseParam* pParm);
	//�ж��Ƿ�ǰlist��ý���
	bool  ListFocused();

	void UpdateTaskImage(CTaskItem* item);

	bool IsAutoDelFinishedTask(){return _IsAutoDelFinishedTask;}

	bool Handle(const DelTaskItemMsg* pMsg);

private:

	bool	OnBottomInstall(TNotifyUI* pNotify);

	bool    OnBottomAddToCase(TNotifyUI* pNotify);

	// �ײ���ͣ��ť�¼�
	bool	OnBottomBtnPause(TNotifyUI* pNotify);

	// �ײ���ʼ��ť�¼�
	bool	OnBottomBtnStart(TNotifyUI* pNotify);

	// �ײ�ɾ����ť�¼�
	bool	OnBottomBtnDel(TNotifyUI* pNotify);

	// Summary  : ��Ӧ�ײ���ť�¼�
	// Returns	: bool
	// Parameter: TNotifyUI * pNotify
	bool	OnBottomBtnCk(TNotifyUI* pNotify);

	//�����Ƿ��Զ�ɾ���˵�
	bool	OnBottomBtnDelOption(TNotifyUI* pNotify);

	// �б����еĿ�ʼ��ͣ��ť
	bool	OnItemBtnStart(TNotifyUI* pNotify);

	// �б����е�ɾ����ť
	bool	OnItemBtnDel(TNotifyUI* pNotify);

	// �б��е���Ҽ�
	bool	OnListRightClick(TNotifyUI* pNotify);

	bool   OnlistChecked(TNotifyUI* pNotify);

	// �б����Ҽ��˵��¼���Ӧ
	bool	OnMenuClick(TNotifyUI* pNotify);

	// �Զ�ɾ���˵���Ӧ�¼�
	bool	OnMenuAutoDelClick(TNotifyUI* pNotify);

	bool	OnListClick(TNotifyUI* pNotify);

	//bool    OnListHOVER(TNotifyUI* pNotify);

	bool    OnNoTaskClick(TNotifyUI* pNotify);

	bool    OnPlatClick(TNotifyUI* pNotify);

	bool MoveEventOnList(TEventUI& event);

	void ShowListItemButton(CControlUI* pControl, bool show);

	// Summary  :ȡ������״̬ͼ��
	// Returns	: tstring
	// Parameter: TASK_STATUS ts ����״̬
	tstring GetTaskStatusIconId(TASK_STATUS ts);

	// Summary  :ȡ����������ı�
	// Returns	: tstring
	// Parameter: CTaskItem * item����
	tstring GetDownProgressText(CTaskItem* item);

	// Summary  :��������״̬ȡ���б����а�ť��Ӧ�ı�
	// Returns	: tstring
	// Parameter: int taskstate����״̬
	tstring GetButtonText(int taskstate,bool Istip=false);

	// Summary  :��ʾ��ǰδ���������
	// Returns	: void
	//void SetTaskCount();

	// �б��Ҽ��˵���������
	bool TaskOperation(int eventcode, int taskid, wstring deviceid=L"", int caseId=-1);

	// ��ʼ���Ҽ��˵�
	void InitMenu();

	// ��ʼ����װ�豸�Ӳ˵�
	void InitInstallSubMenu(CMenuUI* item);

	// ��ʼ����װ�����Ӳ˵�
	void InitInstallCaseSubMenu(CMenuUI* item);

	void DeleteTask(int taskid);

	bool IsMultiSelected();

	void AddListGroupTittle(wstring Id, wstring text);

	//�����б�tooltip
	void SetToolTipText(TooltipData pNode, CControlUI* item);
private:

	wstring  _curdeviceId;//��ǰѡ���豸Id,�մ�����������

	//bool     _isFinishedList;//��ǰѡ�е��Ƿ�Ϊ������б�

	TASK_LIST_TYPE   _listtype;

	int		 _curSelTaskId;//��ǰ�Ҽ�ѡ������

	bool     _IsSelectedAll;// �Ƿ�ѡ����ȫ��

	bool	 _IsAutoDelFinishedTask;//�Ƿ��Զ�ɾ��������¼�

	int		_platform;
	int		_type;

	int     _lastUnFinishTaskId;

	wstring _keydeleteid;	//��ݼ�ɾ��ʱ��ָ��OnBottomBtnCkΪɾ������

	wstring  _finisheddowntaskplat;

	CRITICAL_SECTION  _removelistitemlock;

	CMenuUI* _pMenu;
};
