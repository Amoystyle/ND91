#pragma once
#include "logic/TaskCenterController.h"
class TaskListUI
	: public CTabItem
	, public BaseHandler<DelTaskItemMsg>

	//, public CommonUI
	//, public MessageReceiver        // 接收消息
{
public:
	struct TooltipData{
		wstring name;		//名称
		wstring version;	//版本
		wstring size;		//大小
		wstring platform;	//平台
	};
public:
	TaskListUI(wstring deviceId, TASK_LIST_TYPE isFinishedList);
	~TaskListUI(void);

	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_pause_task", OnBottomBtnPause);//暂停按钮响应事件
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_start_task", OnBottomBtnStart);//开始按钮响应事件
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_del_task", OnBottomBtnDel);//删除按钮响应事件
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_del_task_option",OnBottomBtnDelOption); //删除选项按钮响应事件
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"item_btn_start",OnItemBtnStart); //列表中开始按钮响应事件
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"item_btn_del",OnItemBtnDel); //列表中删除按钮响应事件
		EVNET_ID_HANDLER(UINOTIFY_RCLICK,L"downtasklist",OnListRightClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK,L"downtasklist",OnListClick);
		EVNET_ID_HANDLER(UINOTIFY_DBCLICK,L"downtasklist",OnListClick);
		//EVNET_ID_HANDLER(UINOTIFY_HOVER,L"downtasklist",OnListHOVER);
		EVNET_ID_HANDLER(UINOTIFY_TVN_CHECK,L"downtasklist",OnlistChecked);//
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"install_to",OnBottomInstall);//安装到
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"add_to",OnBottomAddToCase);//添加到
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"no_tasks_btn",OnNoTaskClick);

		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_Android", OnPlatClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_iPhone", OnPlatClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_iPad", OnPlatClick);

		
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK,20000,OnMenuClick);//右键菜单
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK,21000,OnMenuClick);//右键菜单
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK,22000,OnMenuClick);//右键菜单
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK,23000,OnMenuClick);//右键菜单
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK,24000,OnMenuClick);//底部安装菜单
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK,25000,OnMenuClick);//底部方案菜单
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK,8000,OnMenuAutoDelClick);//底部删除菜单
		
	UIEND_MSG_MAP


	void OnCreate();

	// Summary  :插入一条任务到列表
	// Returns	: void
	// Parameter: CTaskItem * item任务
	void InsertTask(CTaskItem* item);

	// 刷新
	void RefreshData();

	// 更新一条任务记录
	void UpdateTaskUI(CTaskItem* item,bool finished=false);

	// 移除一条任务
	void RemoveTaskItemUI(int taskid);

	// 接受消息
	//void Receive(MESSAGE_TYPE msg, const BaseParam* pParm);
	//判断是否当前list获得焦点
	bool  ListFocused();

	void UpdateTaskImage(CTaskItem* item);

	bool IsAutoDelFinishedTask(){return _IsAutoDelFinishedTask;}

	bool Handle(const DelTaskItemMsg* pMsg);

private:

	bool	OnBottomInstall(TNotifyUI* pNotify);

	bool    OnBottomAddToCase(TNotifyUI* pNotify);

	// 底部暂停按钮事件
	bool	OnBottomBtnPause(TNotifyUI* pNotify);

	// 底部开始按钮事件
	bool	OnBottomBtnStart(TNotifyUI* pNotify);

	// 底部删除按钮事件
	bool	OnBottomBtnDel(TNotifyUI* pNotify);

	// Summary  : 响应底部按钮事件
	// Returns	: bool
	// Parameter: TNotifyUI * pNotify
	bool	OnBottomBtnCk(TNotifyUI* pNotify);

	//弹出是否自动删除菜单
	bool	OnBottomBtnDelOption(TNotifyUI* pNotify);

	// 列表项中的开始暂停按钮
	bool	OnItemBtnStart(TNotifyUI* pNotify);

	// 列表项中的删除按钮
	bool	OnItemBtnDel(TNotifyUI* pNotify);

	// 列表中点击右键
	bool	OnListRightClick(TNotifyUI* pNotify);

	bool   OnlistChecked(TNotifyUI* pNotify);

	// 列表中右键菜单事件响应
	bool	OnMenuClick(TNotifyUI* pNotify);

	// 自动删除菜单响应事件
	bool	OnMenuAutoDelClick(TNotifyUI* pNotify);

	bool	OnListClick(TNotifyUI* pNotify);

	//bool    OnListHOVER(TNotifyUI* pNotify);

	bool    OnNoTaskClick(TNotifyUI* pNotify);

	bool    OnPlatClick(TNotifyUI* pNotify);

	bool MoveEventOnList(TEventUI& event);

	void ShowListItemButton(CControlUI* pControl, bool show);

	// Summary  :取得任务状态图标
	// Returns	: tstring
	// Parameter: TASK_STATUS ts 任务状态
	tstring GetTaskStatusIconId(TASK_STATUS ts);

	// Summary  :取得任务进度文本
	// Returns	: tstring
	// Parameter: CTaskItem * item任务
	tstring GetDownProgressText(CTaskItem* item);

	// Summary  :根据任务状态取得列表项中按钮对应文本
	// Returns	: tstring
	// Parameter: int taskstate任务状态
	tstring GetButtonText(int taskstate,bool Istip=false);

	// Summary  :显示当前未完成任务数
	// Returns	: void
	//void SetTaskCount();

	// 列表右键菜单操作处理
	bool TaskOperation(int eventcode, int taskid, wstring deviceid=L"", int caseId=-1);

	// 初始化右键菜单
	void InitMenu();

	// 初始化安装设备子菜单
	void InitInstallSubMenu(CMenuUI* item);

	// 初始化安装方案子菜单
	void InitInstallCaseSubMenu(CMenuUI* item);

	void DeleteTask(int taskid);

	bool IsMultiSelected();

	void AddListGroupTittle(wstring Id, wstring text);

	//设置列表tooltip
	void SetToolTipText(TooltipData pNode, CControlUI* item);
private:

	wstring  _curdeviceId;//当前选中设备Id,空代表下载任务

	//bool     _isFinishedList;//当前选中的是否为已完成列表

	TASK_LIST_TYPE   _listtype;

	int		 _curSelTaskId;//当前右键选中任务

	bool     _IsSelectedAll;// 是否选择了全部

	bool	 _IsAutoDelFinishedTask;//是否自动删除已完成事件

	int		_platform;
	int		_type;

	int     _lastUnFinishTaskId;

	wstring _keydeleteid;	//快捷键删除时候指定OnBottomBtnCk为删除操作

	wstring  _finisheddowntaskplat;

	CRITICAL_SECTION  _removelistitemlock;

	CMenuUI* _pMenu;
};
