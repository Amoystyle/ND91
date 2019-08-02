#ifndef TaskCenterControlUI_h__
#define TaskCenterControlUI_h__

class CTaskItem;
//class TaskUIObserver;
class TaskListUI;
class MessageWnd;
class LeftView;

#include "Logic/TaskCenterController.h"
struct DeviceConnectMsg;
struct NotifyTaskItemMsg;
//任务中心
class TaskCenterControlUI
    : public CCompositeTabUI
    , public BaseView
    , public TaskUIObserver
	, BaseHandler<CWebResourceDownload>
	,BaseHandler<DeviceConnectMsg>
	,BaseHandler<DeviceLinkOutMsg>
	,public BaseHandler<NotifyTaskItemMsg>
				 
{
	public:
		TaskCenterControlUI(LeftView* pLeftView);
		~TaskCenterControlUI();

		UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Tree",OnTreeSelectChange); //任务中心树控件响应事件
		UIEND_MSG_MAP

		void OnCreate();

		// Summary  : 树节点切换事件
		// Returns	: bool
		// Parameter: TNotifyUI * pNotify
		bool OnTreeSelectChange(TNotifyUI* pNotify);

		virtual void OneTaskAdded(CTaskItem* item);//新增一个任务
		virtual void TaskUpdatedSpeedNotify(CTaskItem* item ,wstring deviceId);//刷新进度
		virtual void TaskFinishedNotify(CTaskItem* item, wstring deviceId);//任务完成
		virtual void TaskFailNotify(CTaskItem* item, wstring deviceId);//任务失败
		virtual void TaskCountChangeNotify();
		virtual void TaskRemoveNotify(int taskid, wstring deviceId);
		virtual void OnDeviceHasUnfinishedTasks(wstring deviceId);

		void SelectTreeNode();

		void OnDeviceConnect(wstring sn);//设备连接事件
		void OnDeviceDisconnect(wstring sn);//设备断开事件
private:
	//处理web上面下载软件功能
		virtual bool Handle(const CWebResourceDownload* pMsg);

		virtual bool Handle( const DeviceConnectMsg* pMsg );

		virtual bool Handle( const DeviceLinkOutMsg* pMsg );

		virtual bool Handle( const NotifyTaskItemMsg* pMsg );

		// Summary  :显示当前未完成任务数
		// Returns	: void
		void SetTaskCount(wstring deviceId);

		// 添加列表容器
		void AddContainer(wstring id, TASK_LIST_TYPE isfinished);

		// Summary  :添加树组节点
		// Returns	: void
		// Parameter: wstring text组节点文本，一般为设备名
		// Parameter: wstring image 组节点图标
		// Parameter: wstring id 组节点id，一般为设备ID
		void AddTreeNode(wstring text, wstring image, wstring id);


		//设备连接断开事件
		virtual void OnDeviceLinkOut(wstring sn, void* pPhone);
        virtual void OnUsbConnect(wstring sn);
        virtual void OnUsbDisconnect(wstring sn);
		virtual void OnWifiConnect(wstring sn);
        virtual void OnWifiDisconnect(wstring sn);
		void OnDeviceNameChanged(wstring sn);

		// 设备激活时通知
		virtual void OnDeviceActived(wstring sn);

		virtual wstring GetStyleID();
private:

		wstring  _curdeviceId;//当前选中设备Id,空代表下载任务

		//bool     _isFinishedList;//当前选中的是否为已完成列表

		TASK_LIST_TYPE   _listtype;

		bool	 _IsAutoDelFinishedTask;


		map<wstring, TaskListUI*>  _tasklistUImap;

		
		TaskListUI*   _curTasklist;

		MessageWnd*  _wndContinueInstall;

		bool         _usercancel;

		map<wstring,GlobalTaskInfo *> _mapNeedContinuesTasks;

		LeftView* _pLeftView;


};
#endif // TaskCenterControlUI_h__