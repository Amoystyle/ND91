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
//��������
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
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Tree",OnTreeSelectChange); //�����������ؼ���Ӧ�¼�
		UIEND_MSG_MAP

		void OnCreate();

		// Summary  : ���ڵ��л��¼�
		// Returns	: bool
		// Parameter: TNotifyUI * pNotify
		bool OnTreeSelectChange(TNotifyUI* pNotify);

		virtual void OneTaskAdded(CTaskItem* item);//����һ������
		virtual void TaskUpdatedSpeedNotify(CTaskItem* item ,wstring deviceId);//ˢ�½���
		virtual void TaskFinishedNotify(CTaskItem* item, wstring deviceId);//�������
		virtual void TaskFailNotify(CTaskItem* item, wstring deviceId);//����ʧ��
		virtual void TaskCountChangeNotify();
		virtual void TaskRemoveNotify(int taskid, wstring deviceId);
		virtual void OnDeviceHasUnfinishedTasks(wstring deviceId);

		void SelectTreeNode();

		void OnDeviceConnect(wstring sn);//�豸�����¼�
		void OnDeviceDisconnect(wstring sn);//�豸�Ͽ��¼�
private:
	//����web���������������
		virtual bool Handle(const CWebResourceDownload* pMsg);

		virtual bool Handle( const DeviceConnectMsg* pMsg );

		virtual bool Handle( const DeviceLinkOutMsg* pMsg );

		virtual bool Handle( const NotifyTaskItemMsg* pMsg );

		// Summary  :��ʾ��ǰδ���������
		// Returns	: void
		void SetTaskCount(wstring deviceId);

		// ����б�����
		void AddContainer(wstring id, TASK_LIST_TYPE isfinished);

		// Summary  :�������ڵ�
		// Returns	: void
		// Parameter: wstring text��ڵ��ı���һ��Ϊ�豸��
		// Parameter: wstring image ��ڵ�ͼ��
		// Parameter: wstring id ��ڵ�id��һ��Ϊ�豸ID
		void AddTreeNode(wstring text, wstring image, wstring id);


		//�豸���ӶϿ��¼�
		virtual void OnDeviceLinkOut(wstring sn, void* pPhone);
        virtual void OnUsbConnect(wstring sn);
        virtual void OnUsbDisconnect(wstring sn);
		virtual void OnWifiConnect(wstring sn);
        virtual void OnWifiDisconnect(wstring sn);
		void OnDeviceNameChanged(wstring sn);

		// �豸����ʱ֪ͨ
		virtual void OnDeviceActived(wstring sn);

		virtual wstring GetStyleID();
private:

		wstring  _curdeviceId;//��ǰѡ���豸Id,�մ�����������

		//bool     _isFinishedList;//��ǰѡ�е��Ƿ�Ϊ������б�

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