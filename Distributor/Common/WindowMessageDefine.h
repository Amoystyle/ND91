#pragma once

//#define LOGIC_NEEDDEAMON //用于打开安装守护的过程
//#define _OPEN_DEBUG //用于打开限制的功能，方便调试
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

#pragma region 发给主线程的消息

#define WM_WIFI_CONNECT             (UINOTIFY__USER + 101)  // WIFI连接
#define WM_WIFI_DISCONNECT          (UINOTIFY__USER + 102)  // WIFI断开连接
#define WM_USB_CONNECT              (UINOTIFY__USER + 103)  // USB连接
#define WM_USB_DISCONNECT           (UINOTIFY__USER + 104)  // USB断开连接
#define WM_DAEMON_DISABLE           (UINOTIFY__USER + 105)  // 守护失去响应
#define WM_WIFI_NOTIFY              (UINOTIFY__USER + 106)  // WIFI心跳线程的通知，lParam取值:
                                                            // 0 心跳包                         
                                                            // 1 手机端手机助手点'断开wifi连接' 
                                                            // 2 系统将进入休眠                 
                                                            // 3 系统进入休眠                 
                                                            // 4 手机锁屏                       
                                                            // 5 通过电源按钮关机     
#define WM_DEVICE_LINK_IN           (UINOTIFY__USER + 107)  // 设备插入
#define WM_DEVICE_LINK_OUT          (UINOTIFY__USER + 108)  // 设备拔出
#define WM_DEVICE_OFFLINE           (UINOTIFY__USER + 109)  // 离线设备接入
#define WM_DEVICE_CONNECTFAIL       (UINOTIFY__USER + 110)  // 离线设备接入
#define WM_DEVICE_CONNECTFINISH     (UINOTIFY__USER + 111)  // 扫描设备完成
#define WM_DEVICE_NAMECHANGED       (UINOTIFY__USER + 112)  // 设备名称变更
#define WM_DEVICE_RECOVERYCONNECT   (UINOTIFY__USER + 113)  //恢复模式连接
#define WM_DEVICE_RECOVERYDISCONNECT   (UINOTIFY__USER + 114)  //恢复模式断开连接
#define WM_DEVICE_ISLOCKSCREEN      (UINOTIFY__USER + 115)  //锁屏
#define WM_DEVICE_SPACECHANGED      (UINOTIFY__USER + 116)  // 设备空间大小变更
#define WM_DEVICE_ADBOFF            (UINOTIFY__USER + 117)  // 设备Adb未打开
#define WM_DEVICE_NEEDINSTALLDRIVER (UINOTIFY__USER + 118)  // 需要安装驱动
#define WM_DEVICE_DRIVER_CHANGED    (UINOTIFY__USER + 119)  // 驱动信息变更
#define WM_DEVICE_ACTIVED           (UINOTIFY__USER + 120)  // 设备已激活
#define WM_DEVICE_ACTIVATE_FAIL     (UINOTIFY__USER + 121)  // 设备已激活
#define WM_DEVICE_CONNECTSUCCESS    (UINOTIFY__USER + 122)  //
#define WM_DEVICE_SNCHANGED         (UINOTIFY__USER + 123)  //设备SN改变

#define WM_DEVICE_MIN               WM_WIFI_CONNECT
#define WM_DEVICE_MAX               (UINOTIFY__USER + 150)
// 以上消息的wparam是字符串指针（wchar_t*)
#define WM_DEVICE_INFO              (UINOTIFY__USER + 201)   

#define WM_TASK_FINISHED	        (UINOTIFY__USER + 210)  // 任务完成
#define WM_TASK_UPDATESPEED         (UINOTIFY__USER + 211)  // 任务更新
#define WM_TASK_FAIL		        (UINOTIFY__USER + 212)  // 任务失败
#define WM_TASK_COUNTCHANGE	        (UINOTIFY__USER + 213) 
#define WM_TASK_REMOVED		        (UINOTIFY__USER + 214) 
#define WM_TASK_ADDED		        (UINOTIFY__USER + 215)  // 新增任务

#define WM_WAIT_CURSOR              (UINOTIFY__USER + 216)  // 设置 wait 光标 wParam：1=设置 0=取消

#define WM_PROMPT_BOX               (UINOTIFY__USER + 217)  // 提示对话框
#define WM_CONFIRM_BOX              (UINOTIFY__USER + 218)  // 确认对话框
#define WM_INPUT_BOX                (UINOTIFY__USER + 219)  // 输入对话框
#define WM_NOTICE_BOX               (UINOTIFY__USER + 220)  // 自动关闭提示框
#define WM_PROGRESS_BOX             (UINOTIFY__USER + 221)  // 进度对话框
#define WM_EXIST_ACTION_BOX         (UINOTIFY__USER + 222)  // 文件已存在对话框
#define WM_Refresh_APP			    (UINOTIFY__USER + 223)  //	文件管理模块刷新应用程序数据名
#define WM_Refresh_Node			    (UINOTIFY__USER + 224)  //	文件管理模块刷新节点

#define WM_WEBSRC_NETCONNECTED		(UINOTIFY__USER + 225)  // 网络已恢复连接
#define WM_NETCASE_LOADFINISHED	    (UINOTIFY__USER + 226)	// 网络装机方案加载完成通知

#define WM_BackUpAndRestorePro		(UINOTIFY__USER + 227)  //	备份还原进度条消息
#define WM_BackUpMsg				(UINOTIFY__USER + 228)  //	备份取消或跳过时发送消息
#define WM_RestoreMsg				(UINOTIFY__USER + 229)  //	还原取消或跳过时发送消息
#define WM_BackUpAndRestoreMsg		(UINOTIFY__USER + 230)  //	备份还原开始或结束时发送消息通知各窗体
#define WM_BackUpAndRestoreProVisible		(UINOTIFY__USER + 231)  //	备份还原进度条显示或隐藏控制
#define WM_CONFIRMWARN_BOX          (UINOTIFY__USER + 232)  // 确认对话框（警告型）
#define WM_AotoSelectOtherRestore	(UINOTIFY__USER + 233)  //所有插入设备中备份一台机器后其他机器必须同时出现此备份项
#define WM_Update_DeviceUI			(UINOTIFY__USER + 234)  //设备越狱完之后将自动更新界面相关信息
#define WM_Vitrue_Progress			(UINOTIFY__USER + 235)  //虚拟进度条使用
#define WM_Wait_Appsynk				(UINOTIFY__USER + 236)  //安装appsynk等待消息
#define WM_Stop_BackUpAndRestore	(UINOTIFY__USER + 237)  //终止备份还原

#define WM_TASKUI_DEVICE_CONNECTED  (UINOTIFY__USER + 240)  // 任务中心设备连上消息

#define WM_TASKUI_DEVICE_DISCONNECT  (UINOTIFY__USER + 241)  // 任务中心设备连上消息


#define UINOTIFY__AutoGrid_PageChanged  UINOTIFY__USER+300  //CAutoGridLayoutUI页面切换消息
#define UINOTIFY_UI_DEVICE_CONNECTED	UINOTIFY__USER+301  
#define UINOTIFY_UI_DEVICE_DISCONNECT   UINOTIFY__USER+302  


#pragma endregion
#pragma region 更新程序通信用

#define WM_UPDATE_MSG               (WM_USER + 340) // update已与服务器取得通信结果，通知主程序修改版本号
#define WM_ONLY		                (WM_USER + 350) // 单例，激活已有窗口

#define WM_SAVE_USERINFO	        (WM_USER + 360) // 保存用户资料是否成功
#define WM_USELOGIN_INFO	        (WM_USER + 361) // 用户登录或者退出
#define WM_MY_SHOWEMPTYTIP			(WM_USER + 362) 
#define WM_MY_WEBFEEDBACK			(WM_USER + 363) 
#define WM_MY_WEBUSEINFO			(WM_USER + 364) 

#define WM_CASE_CLEARLIST	        (WM_USER + 400) //400~499预留给方案管理界面update
#define WM_CASE_ADDLIST		        (WM_USER + 401)
#define WM_CASE_REFRESHLIST	        (WM_USER + 402)
#define WM_CASE_CLEARUPDATELIST		(WM_USER + 403)
#define WM_CASE_REFRESH_DEVICELIST	(WM_USER + 404)
#define WM_CASE_UPDATE_CASENAME		(WM_USER + 405)
#define WM_CASE_UPDATE_LIB_NUM		(WM_USER + 406)
#define WM_CASE_ADDUPDATELIST		(WM_USER + 407)
#define WM_CASE_EDIT_CASE	        (WM_USER + 408)
#define WM_CASE_UPDATE_CASE	        (WM_USER + 409)
#define WM_CASE_USER_END			(WM_USER + 499) //400~499预留给方案管理界面update

#define WM_APP_CLEARLIST			(WM_USER + 500) //500~599留给应用程序
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

#define WM_WebLegalSoft_24HotHasLoad			(WM_USER + 600) //600~619预留给网络正版软件体验
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


#define WM_FileManager_Begin					(WM_USER + 800)//600~619预留给文件管理
#define WM_FileManager_InitRoot					(WM_USER + 801)
#define WM_FileManager_End						(WM_USER + 899)

#define WM_GROUP_BEGIN							(WM_USER + 900)  // 用户自定义消息：组消息开始
#define WM_GROUP_END							(WM_USER + 901)  // 用户自定义消息：组消息结束
#define WM_SEND_TO_MAIN							(WM_USER + 902)  // 用户自定义消息

#define WM_ICON_NOTIFY				(WM_USER + 1000)
#define WM_DI_PROCHANGED			(WM_USER + 2000)


#pragma endregion

#ifndef TRACE
#define TRACE 
#endif