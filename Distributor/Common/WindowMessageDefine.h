#pragma once

//#define LOGIC_NEEDDEAMON //���ڴ򿪰�װ�ػ��Ĺ���
//#define _OPEN_DEBUG //���ڴ����ƵĹ��ܣ��������
//#define _DEBUG_DRIVER
#define FOLDER_ITEM_ID 8
#define CASE_ITEM_ID 9
#define COMPANY_NAME        _T("NetDragon")
#define PRODUCT_NAME        _T("91Distributor")

#define APP_NAME                _T("91Distributor")
#define APP_EXENAME             _T("Distributor.exe")
#define  DB_FILE_NAME			_T("\\91Distributor\\91Distributor.db")
#define APP_TITLE               BaseUI::LoadString(_T("App_title"))

#define MUTEX_MAINAPP           _T("MUTEX_91Installer")
#define MUTEX_CLOSEMAINAPP      _T("MUTEX_CLOSE_91Installer")
#define WNDTITLE_CLASSNAME		_T("Distributor@hi-zj.com")

#define UpdateControl(p) {p->Resize();p->Invalidate();}	
#define ShowControl(p) {p->SetVisible(true);p->Resize();p->Invalidate();}	
//#define IDR_MENU_CASE_LIST 54013
//#define IDR_MENU_EDIT_CASE 54014
//#define IDR_MENU_DELETE_CASE 54015
//#define IDR_MENU_OPEN_CASE 54016

#pragma region �������̵߳���Ϣ

#define WM_WIFI_CONNECT             (UINOTIFY__USER + 101)  // WIFI����
#define WM_WIFI_DISCONNECT          (UINOTIFY__USER + 102)  // WIFI�Ͽ�����
#define WM_USB_CONNECT              (UINOTIFY__USER + 103)  // USB����
#define WM_USB_DISCONNECT           (UINOTIFY__USER + 104)  // USB�Ͽ�����
#define WM_DAEMON_DISABLE           (UINOTIFY__USER + 105)  // �ػ�ʧȥ��Ӧ
#define WM_WIFI_NOTIFY              (UINOTIFY__USER + 106)  // WIFI�����̵߳�֪ͨ��lParamȡֵ:
                                                            // 0 ������                         
                                                            // 1 �ֻ����ֻ����ֵ�'�Ͽ�wifi����' 
                                                            // 2 ϵͳ����������                 
                                                            // 3 ϵͳ��������                 
                                                            // 4 �ֻ�����                       
                                                            // 5 ͨ����Դ��ť�ػ�     
#define WM_DEVICE_LINK_IN           (UINOTIFY__USER + 107)  // �豸����
#define WM_DEVICE_LINK_OUT          (UINOTIFY__USER + 108)  // �豸�γ�
#define WM_DEVICE_OFFLINE           (UINOTIFY__USER + 109)  // �����豸����
#define WM_DEVICE_CONNECTFAIL       (UINOTIFY__USER + 110)  // �����豸����
#define WM_DEVICE_CONNECTFINISH     (UINOTIFY__USER + 111)  // ɨ���豸���
#define WM_DEVICE_NAMECHANGED       (UINOTIFY__USER + 112)  // �豸���Ʊ��
#define WM_DEVICE_RECOVERYCONNECT   (UINOTIFY__USER + 113)  //�ָ�ģʽ����
#define WM_DEVICE_RECOVERYDISCONNECT   (UINOTIFY__USER + 114)  //�ָ�ģʽ�Ͽ�����
#define WM_DEVICE_ISLOCKSCREEN      (UINOTIFY__USER + 115)  //����
#define WM_DEVICE_SPACECHANGED      (UINOTIFY__USER + 116)  // �豸�ռ��С���
#define WM_DEVICE_ADBOFF            (UINOTIFY__USER + 117)  // �豸Adbδ��
#define WM_DEVICE_NEEDINSTALLDRIVER (UINOTIFY__USER + 118)  // ��Ҫ��װ����
#define WM_DEVICE_DRIVER_CHANGED    (UINOTIFY__USER + 119)  // ������Ϣ���
#define WM_DEVICE_ACTIVED           (UINOTIFY__USER + 120)  // �豸�Ѽ���
#define WM_DEVICE_ACTIVATE_FAIL     (UINOTIFY__USER + 121)  // �豸�Ѽ���
#define WM_DEVICE_CONNECTSUCCESS    (UINOTIFY__USER + 122)  //
#define WM_DEVICE_SNCHANGED         (UINOTIFY__USER + 123)  //�豸SN�ı�

#define WM_DEVICE_MIN               WM_WIFI_CONNECT
#define WM_DEVICE_MAX               (UINOTIFY__USER + 150)
// ������Ϣ��wparam���ַ���ָ�루wchar_t*)
#define WM_DEVICE_INFO              (UINOTIFY__USER + 201)   

#define WM_TASK_FINISHED	        (UINOTIFY__USER + 210)  // �������
#define WM_TASK_UPDATESPEED         (UINOTIFY__USER + 211)  // �������
#define WM_TASK_FAIL		        (UINOTIFY__USER + 212)  // ����ʧ��
#define WM_TASK_COUNTCHANGE	        (UINOTIFY__USER + 213) 
#define WM_TASK_REMOVED		        (UINOTIFY__USER + 214) 
#define WM_TASK_ADDED		        (UINOTIFY__USER + 215)  // ��������

#define WM_WAIT_CURSOR              (UINOTIFY__USER + 216)  // ���� wait ��� wParam��1=���� 0=ȡ��

#define WM_PROMPT_BOX               (UINOTIFY__USER + 217)  // ��ʾ�Ի���
#define WM_CONFIRM_BOX              (UINOTIFY__USER + 218)  // ȷ�϶Ի���
#define WM_INPUT_BOX                (UINOTIFY__USER + 219)  // ����Ի���
#define WM_NOTICE_BOX               (UINOTIFY__USER + 220)  // �Զ��ر���ʾ��
#define WM_PROGRESS_BOX             (UINOTIFY__USER + 221)  // ���ȶԻ���
#define WM_EXIST_ACTION_BOX         (UINOTIFY__USER + 222)  // �ļ��Ѵ��ڶԻ���
#define WM_Refresh_APP			    (UINOTIFY__USER + 223)  //	�ļ�����ģ��ˢ��Ӧ�ó���������
#define WM_Refresh_Node			    (UINOTIFY__USER + 224)  //	�ļ�����ģ��ˢ�½ڵ�

#define WM_WEBSRC_NETCONNECTED		(UINOTIFY__USER + 225)  // �����ѻָ�����
#define WM_NETCASE_LOADFINISHED	    (UINOTIFY__USER + 226)	// ����װ�������������֪ͨ

#define WM_BackUpAndRestorePro		(UINOTIFY__USER + 227)  //	���ݻ�ԭ��������Ϣ
#define WM_BackUpMsg				(UINOTIFY__USER + 228)  //	����ȡ��������ʱ������Ϣ
#define WM_RestoreMsg				(UINOTIFY__USER + 229)  //	��ԭȡ��������ʱ������Ϣ
#define WM_BackUpAndRestoreMsg		(UINOTIFY__USER + 230)  //	���ݻ�ԭ��ʼ�����ʱ������Ϣ֪ͨ������
#define WM_BackUpAndRestoreProVisible		(UINOTIFY__USER + 231)  //	���ݻ�ԭ��������ʾ�����ؿ���
#define WM_CONFIRMWARN_BOX          (UINOTIFY__USER + 232)  // ȷ�϶Ի��򣨾����ͣ�
#define WM_AotoSelectOtherRestore	(UINOTIFY__USER + 233)  //���в����豸�б���һ̨������������������ͬʱ���ִ˱�����
#define WM_Update_DeviceUI			(UINOTIFY__USER + 234)  //�豸Խ����֮���Զ����½��������Ϣ
#define WM_Vitrue_Progress			(UINOTIFY__USER + 235)  //���������ʹ��
#define WM_Wait_Appsynk				(UINOTIFY__USER + 236)  //��װappsynk�ȴ���Ϣ
#define WM_Stop_BackUpAndRestore	(UINOTIFY__USER + 237)  //��ֹ���ݻ�ԭ

#define WM_TASKUI_DEVICE_CONNECTED  (UINOTIFY__USER + 240)  // ���������豸������Ϣ

#define WM_TASKUI_DEVICE_DISCONNECT  (UINOTIFY__USER + 241)  // ���������豸������Ϣ


#define UINOTIFY__AutoGrid_PageChanged  UINOTIFY__USER+300  //CAutoGridLayoutUIҳ���л���Ϣ
#define UINOTIFY_UI_DEVICE_CONNECTED	UINOTIFY__USER+301  
#define UINOTIFY_UI_DEVICE_DISCONNECT   UINOTIFY__USER+302  


#pragma endregion
#pragma region ���³���ͨ����

#define WM_UPDATE_MSG               (WM_USER + 340) // update���������ȡ��ͨ�Ž����֪ͨ�������޸İ汾��
#define WM_ONLY		                (WM_USER + 350) // �������������д���

#define WM_SAVE_USERINFO	        (WM_USER + 360) // �����û������Ƿ�ɹ�
#define WM_USELOGIN_INFO	        (WM_USER + 361) // �û���¼�����˳�
#define WM_MY_SHOWEMPTYTIP			(WM_USER + 362) 
#define WM_MY_WEBFEEDBACK			(WM_USER + 363) 
#define WM_MY_WEBUSEINFO			(WM_USER + 364) 

#define WM_CASE_CLEARLIST	        (WM_USER + 400) //400~499Ԥ���������������update
#define WM_CASE_ADDLIST		        (WM_USER + 401)
#define WM_CASE_REFRESHLIST	        (WM_USER + 402)
#define WM_CASE_CLEARUPDATELIST		(WM_USER + 403)
#define WM_CASE_REFRESH_DEVICELIST	(WM_USER + 404)
#define WM_CASE_UPDATE_CASENAME		(WM_USER + 405)
#define WM_CASE_UPDATE_LIB_NUM		(WM_USER + 406)
#define WM_CASE_ADDUPDATELIST		(WM_USER + 407)
#define WM_CASE_EDIT_CASE	        (WM_USER + 408)
#define WM_CASE_UPDATE_CASE	        (WM_USER + 409)
#define WM_CASE_USER_END			(WM_USER + 499) //400~499Ԥ���������������update

#define WM_APP_CLEARLIST			(WM_USER + 500) //500~599����Ӧ�ó���
#define WM_APP_ADDLIST				(WM_USER + 501)
#define WM_APP_SETLOGO				(WM_USER + 502)
#define WM_APP_REFRESHLINE			(WM_USER + 503)
#define WM_APP_UPDATEDATA			(WM_USER + 504)
#define WM_APP_REMOVE				(WM_USER + 505)
#define WM_APP_SETSIZE				(WM_USER + 506)
#define WM_APP_SETAttribute         (WM_USER + 507)
#define WM_APP_SETUPGRADEBTN		(WM_USER + 508)
#define WM_APP_SETLEGALUPGRADEBTN	(WM_USER + 509)
#define WM_APP_REFRESHUPAPPSCOUNT	(WM_USER + 510)
#define WM_APP_REFRESHAPPDATA       (WM_USER + 511)
#define WM_APP_USER_END             (WM_USER + 599)

#define WM_WebLegalSoft_24HotHasLoad			(WM_USER + 600) //600~619Ԥ�������������������
#define WM_WebLegalSoft_CategorysHasLoad		(WM_USER + 601)
#define WM_WebLegalSoft_AppDatasHasLoad			(WM_USER + 602)
#define WM_WebLegalSoft_GameDatasHasLoad		(WM_USER + 603)
#define WM_WebLegalSoft_OneAppInfoHasLoad		(WM_USER + 604)
#define WM_WebLegalSoft_SearchAppsHasLoad		(WM_USER + 605)
#define WM_WebLegalSoft_IconHasDownload			(WM_USER + 606)
#define WM_WebLegalSoft_BeginDownload			(WM_USER + 607)
#define WM_WebLegalSoft_EndDownload				(WM_USER + 608)
#define WM_WebLegalSoft_BeginInstall			(WM_USER + 609)
#define WM_WebLegalSoft_EndInstall				(WM_USER + 610)
#define WM_WebLegalSoft_GotoDes					(WM_USER + 611)
#define WM_WebLegalSoft_USER_END				(WM_USER + 619)

#define WM_DRAGDROP_BEGIN						(WM_USER + 700)
#define WM_DRAGDROP_FILEMGR						(WM_USER + 701)
#define WM_DRAGDROP_HOMECASE					(WM_USER + 702)
#define WM_DRAGDROP_CASEMGR						(WM_USER + 703)
#define WM_DRAGDROP_END							(WM_USER + 799)


#define WM_FileManager_Begin					(WM_USER + 800)//600~619Ԥ�����ļ�����
#define WM_FileManager_InitRoot					(WM_USER + 801)
#define WM_FileManager_End						(WM_USER + 899)

#define WM_GROUP_BEGIN							(WM_USER + 900)  // �û��Զ�����Ϣ������Ϣ��ʼ
#define WM_GROUP_END							(WM_USER + 901)  // �û��Զ�����Ϣ������Ϣ����
#define WM_SEND_TO_MAIN							(WM_USER + 902)  // �û��Զ�����Ϣ

#define WM_ICON_NOTIFY				(WM_USER + 1000)
#define WM_DI_PROCHANGED			(WM_USER + 2000)


#pragma endregion

#ifndef TRACE
#define TRACE 
#endif