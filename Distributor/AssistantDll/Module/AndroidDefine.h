///////////////////////////////////////////////////////////
//  AndroidDefin.h
//  安卓格式相关的常量定义
///////////////////////////////////////////////////////////

#pragma once

// 安卓数据包的包头种类：共三种
#define ANDROID_PACKAGE_NDPC "NDPC";
#define ANDROID_PACKAGE_SYNC "SYNC";
#define ANDROID_PACKAGE_END  "SEND";

#define ANDROID_PACKAGE_OKAY    "OKAY"

#define ANDROID_DEAMON_REQUIRE_VERSION  "2.6.261"   //守护需要的最小版本号

//#include "Core/Define.h"

namespace ND91Assistant
{
	// 守护PackageName
	#define ADB_DEMON_PACKAGE_NAME  "com.nd.assistance.ServerService"

	#define ANDROID_APP_INSTALL_DIR     L"/data/local/tmp/"         //守护文件要上传到手机中的目录
	#define ANDROID_APP_INSTALL_PATH    L"/data/local/tmp/pkg.apk"  //守护上传后的完整路径
	#define ANDROID_APP_INSTALL_DIR_DEMON    L"/sdcard/nd/installers/"	//Wifi安装的安装路径

	#define Default_BookMark_CategoryId  "99999999"

    // 安卓的数据包标识， 2个字节
    enum ANDROID_COMMAND_FLAG
    {
        Unknow					= 0,		// 未知的封包标识
        Client					= 1 << 1,	// 客户端的封包
        Server					= 1 << 2,	// 服务端的封包
        NeedPostBack			= 1 << 3,	// 需要回发的封包
        CreateLongConnection	= 1 << 4,	// 建立长连接
        CloseCurrentConnection	= 1 << 5,	// 关闭长连接
        CancelSend				= 1 << 6,	// 取消发送
    };

    // 安卓的业务类型， 4个字节
    enum ANDROID_COMMAND_CHANNEL
    {
        SYSTEM = 0,			//系统业务标识(心跳包)
        CONTACT = 1,		//表示联系人相应的业务。
        SMS = 2,			//表示短信方面的业务。
        PHOTOS = 3,			//图片管理。
        TODOLIST = 4,		//待办事项
        BOOKMARK = 5,		//书签
        CALENDAR = 6,		//行事历
        MEDIA = 7,			//多媒体。
        CALLS = 8,			//通话记录
        EMAIL = 9,			//邮件
        NOTE = 10,			//便签
        SETTING = 11,		//设置
        APPLICATION = 12,	//程序管理。
        ALARM = 13,			//闹钟
        SYSINFO = 14,		//系统信息
        THEME = 15,			//主题管理
        DEAMONMANAGER = 16,	//守护管理（获取守护版本号等）
        SHELL = 17,			//shell 命令
        FILETRANSFER = 18,	//文件传输
        PUSHFILE = 19,		//上传文件
        FILEMANAGER = 20,	//文件管理接口。
		SMARTTHEME=21,		// 安卓桌面
        PIM = 199,			//PIM
        CHECKCONFIRM=200,   //检查是否需要连接确认
    };

    // 安卓的数据包，对每种命令有一个类型定义，统一写在此处
    enum ANDROID_COMMAND_TYPE
    {
        WAKE_DEMON = 0,			    //唤醒守护(通知守护有PC端接入)
        GET_DEMON_INFO = 1,         //获取守护信息
        GET_DEMON_VERSION = 3,      //获取守护版本

        GET_CONTACT = 1,            //获取联系人
        ADD_CONTACT = 2,            //新增联系人
        UPDATE_CONTACT = 3,         //更新联系人
		GET_CONTACT_PHOTO = 4,      //获取联络人头像
        DEL_CONTACT = 5,            //删除联系人
        ADD_CONTACTGROUP = 6,       //添加联系人分组
        UPDATE_CONTACTGROUP = 7,    //更新联系人分组
        DEL_CONTACTGROUP = 8,       //删除联系人分组
        SET_FAVORITE = 9,           //设置收藏联系人属性
        ADD_TOGROUP = 10,           //添加联系人到联系人分组
        REMOVE_FROMGROUP = 11,      //从联系人分组移除联系人

        GET_SMS = 21,               //获取短消息
        ADD_SMS = 22,               //新增短消息
        UPDATE_SMS = 23,            //修改短消息
        DEL_SMS = 24,		        //删除短消息
        SEND_SMS = 26,              //发送短消息
        SETREAD_SMS = 27,           //设置短信息是否已读
        RESEND_SMS = 28,            //重发短消息

        GET_CALL = 41,				//获取通话记录
        ADD_CALL = 42,              //新增通话记录
        DEL_CALL = 43,              //删除通话记录

        GET_IMEI = 1,               //获取IMEI值
		GET_BATTERY = 14,			//获取电池信息

		GET_APPINFO = 0,			//获取App信息
        GET_APPLIST = 31,           //获取APP列表
        GET_ALLAPPLIST = 30,         //获取所有APP列表
        GET_APPICON = 1,            //获取App图标
		GET_APPPERMISSIONS = 6,		//获取程序权限
		INSTALL_APP = 3,			//安装APP
		UNINSTALL_APP = 4,			//卸载APP

        GET_SCREENSHOT = 4,         //获取屏幕截图

		TABLE_QUERY = 99,			//获取列表
		ADD_RING = 0,				//添加铃声

		Query_BaseSetting = 1,      // 获取基本设置
		Query_Profiles = 3,         // 获取情景模式
		
        GET_MACADDRESS = 26,        // 获取设备MAC地址
		GET_ISROOT = 29,			// 获取android设备是否已root	
		GET_ISWRITE = 30,			// 获取system/app是否可写
		GET_ISREMOUNT = 31,			// 修改system/app为可写
		UNINSTALL_SYSTEMAPP = 32,	// 卸载android设备的系统程序

        GET_DIRSPACEINFO = 23,      // 获取目录控件信息

		GET_RESOLUTION = 15,		// 获取android设备分辨率
        GET_FILEINFO = 16,          // 获取文件信息

        ExecShell = 2,

        QUERY_DEVICENAME = 11,      //获取指定手机的名称
        ADD_DEVICENAME = 12,        //新增一个新手机的手机名称
        UPDATE_DEVICENAME = 13,     //修改一个手机的名称

		UNINSTALL_SYSTEMAPP_BYNDSH = 21, //使用NDSH方式卸载系统软件
		GET_NDSH_STATE = 20,//获取ndsh状态
		GET_SU_AUTH = 21,//获得超级用户权限
    };

	enum DataRowState
	{
		// 摘要:
		//     该行已被创建，但不属于任何 System.Data.DataRowCollection。System.Data.DataRow 在以下情况下立即处于此状态：创建之后添加到集合中之前；或从集合中移除之后。
		DataRowState_Detached = 1,
		//
		// 摘要:
		//     该行自上次调用 System.Data.DataRow.AcceptChanges() 以来尚未更改。
		DataRowState_Unchanged = 2,
		//
		// 摘要:
		//     该行已添加到 System.Data.DataRowCollection 中，System.Data.DataRow.AcceptChanges()
		//     尚未调用。
		DataRowState_Added = 4,
		//
		// 摘要:
		//     该行已通过 System.Data.DataRow 的 System.Data.DataRow.Delete() 方法被删除。
		DataRowState_Deleted = 8,
		//
		// 摘要:
		//     该行已被修改，System.Data.DataRow.AcceptChanges() 尚未调用。
		DataRowState_Modified = 16,
	};

}
