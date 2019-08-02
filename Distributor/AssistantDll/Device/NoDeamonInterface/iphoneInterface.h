/*
 *  iphoneInterface.h
 *
 *  Created by lindaojiang on 2012-6-8
 *  Copyright 2012 NetDragon Websoft Inc. All rights reserved.
 *
 */

#ifndef _IPHONE_INTERFACE_H_
#define _IPHONE_INTERFACE_H_

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>

#pragma pack(push, 1)

//===================================================================================================================

#define SUCCESS_IPHONEINTERFACE								0			//执行成功
#define ERR_IPHONEINTERFACE_RUN_ERROR						-1			//执行失败
#define ERR_IPHONEINTERFACE_PARAMETER_NULL					-70100		//参数错误
#define ERR_IPHONEINTERFACE_SERVER_CONFIRM					-70102		//服务端确认失败
#define ERR_IPHONEINTERFACE_CREATE_SEND_CFDATA				-70103		//本地创建CFData失败
#define ERR_IPHONEINTERFACE_SEND_DATA_ENCRYPT				-70104		//本地数据加密失败
#define ERR_IPHONEINTERFACE_SEND_DATA_SOCKET				-70105		//数据发送失败
#define ERR_IPHONEINTERFACE_WAIT_SERVER_RESULT				-70106		//接收执行结果超时
#define ERR_IPHONEINTERFACE_SERVER_CLOSE					-70107		//守护关闭,执行时系统错误或者无效指令
#define ERR_IPHONEINTERFACE_RECV_ERROR						-70108		//接收结果错误
#define ERR_IPHONEINTERFACE_RECV_INVALID_SEEION				-70109		//接收到未知数据
#define ERR_IPHONEINTERFACE_LOCAL_MALLOC_FAIL				-70110		//本地内存分配失败
#define ERR_IPHONEINTERFACE_RECV_DATA_DECRYPT				-70111		//本地解密失败
#define ERR_IPHONEINTERFACE_CREATE_RECV_CFDATA				-70112		//本地创建CFData失败
#define ERR_IPHONEINTERFACE_CREATE_CFDICTIONARY				-70113		//本地创建CFDictionary失败
#define ERR_IPHONEINTERFACE_CLIENT_RECV_END					-70114		//接收结束内部使用
#define ERR_IPHONEINTERFACE_CREATE_CFSTRINGREF				-70115		//本地创建CFString失败
#define ERR_IPHONEINTERFACE_CHECK_FUNCTION					-70116		//函数入口加密检查失败
#define ERR_IPHONEINTERFACE_LOCAL_CREATE_SOCKET				-70117		//本地创建套接口失败

#define ERR_IPHONEINTERFACE_LOCAL_CREATE_DIR				-70200		//本地创建目录失败
#define ERR_IPHONEINTERFACE_DEVICE_NULL						-70201		//设备参数为空
#define ERR_IPHONEINTERFACE_DEVICE_COPY_NULL				-70202		//设备参数复制失败
#define ERR_IPHONEINTERFACE_DEVICE_CONNECT					-70203		//AMDeviceConnect失败
#define ERR_IPHONEINTERFACE_DEVICE_PAIRING					-70204		//AMDeviceValidatePairing失败
#define ERR_IPHONEINTERFACE_START_SESSION					-70205		//AMDeviceStartSession失败
#define ERR_IPHONEINTERFACE_START_SERVICE_AFC2				-70206		//AMDeviceStartService afc2服务启动失败
#define ERR_IPHONEINTERFACE_AFC_CONNECTION_OPEN				-70207		//AFCConnectionOpen失败
#define ERR_IPHONEINTERFACE_AFC_CREATE_DIR					-70208		//AFC创建目录失败
#define ERR_IPHONEINTERFACE_AFC_PUT_FILE					-70209		//AFC上传文件失败
#define ERR_IPHONEINTERFACE_AFC_GET_FILE					-70210		//AFC下载文件失败
#define ERR_IPHONEINTERFACE_AFC_BACK_SERVICE_FILE			-70211		//备份服务文件失败
#define ERR_IPHONEINTERFACE_AFC_PUT_DAEMON_FILE				-70212		//上传守护程序失败
#define ERR_IPHONEINTERFACE_AFC_RESTORE_DAEMON_FILE			-70213		//恢复替换的服务文件失败
#define ERR_IPHONEINTERFACE_AFC_OPEN_DIRECTORY				-70214		//打开远程文件目录失败
#define ERR_IPHONEINTERFACE_START_SERVICE_DAEMON			-70215		//启动守护程序失败
#define ERR_IPHONEINTERFACE_LOCAL_DAEMON_EXEC				-70216		//本地守护文件不存在
#define ERR_IPHONEINTERFACE_LOCAL_DAEMON_LOADER				-70217		//本地守护文件不存在
#define ERR_IPHONEINTERFACE_LOCAL_DAEMON_BAK				-70218		//替换守护的手机程序备份文件不存在
#define ERR_IPHONEINTERFACE_USBMUXCONNECTBYPORT				-70219		//启动守护失败
#define ERR_IPHONEINTERFACE_POST_NOTIFICATION				-70220		//发送消息失败
#define ERR_IPHONEINTERFACE_GET_ERROR_STRING				-70221		//本地取错误原因字符串失败
#define ERR_IPHONEINTERFACE_BACKUP_DIR_NOT_EXIST			-70222		//本地备份目录不存在
#define ERR_IPHONEINTERFACE_BACKUP_DIR_PERMISSION			-70223		//本地备份目录无写权限
#define ERR_IPHONEINTERFACE_CONNECT_SERVICE_SOCKET			-70224		//启动守护程序成功,连接服务端口失败

//手机端执行错误 ERR_IPHONERUN
#define ERR_IPHONERUN_PATH_NOT_EXIST						-70300		//设备上不存在该路径
#define ERR_IPHONERUN_PARAM_ERROR							-70301		//接收到的参数错误
#define ERR_IPHONERUN_RUN_ERROR								-70302		//执行错误

//pxl安装卸载
#define ERR_IPHONERUN_PXLERROR_MKDIR						-70401		//创建目录
#define ERR_IPHONERUN_PXLERROR_CHMOD						-70402		//修改文件权限
#define ERR_IPHONERUN_PXLERROR_CHDIR						-70403		//修改工作目录
#define	ERR_IPHONERUN_PXLERROR_FILENOTEXIST					-70404		//源文件不存在
#define ERR_IPHONERUN_PXLERROR_OPENPACKAGE					-70405		//无法打开压缩包
#define ERR_IPHONERUN_PXLERROR_EXTRACT						-70406		//解压失败
#define ERR_IPHONERUN_PXLERROR_PARSE						-70407		//解析PxlPkg.plist失败
#define ERR_IPHONERUN_PXLERROR_PARSEFILE					-70408		//从PxlPkg.plist解析出的文件列表有错
#define ERR_IPHONERUN_PXLERROR_WRITEPLIST					-70409		//写安装列表失败
#define ERR_IPHONERUN_PXLERROR_MEMORY						-70410		//分配内存失败
#define ERR_IPHONERUN_PXLERROR_COPYFILE						-70411		//复制文件失败
#define ERR_IPHONERUN_PXLERROR_DELETEFILE					-70412		//删除文件失败

//安装主题
#define ERR_IPHONERUN_INSTALL_THEME_DOWNLOAD				-70501		//需要下载安装熊猫桌面
//修改铃声
#define ERR_IPHONERUN_SMS_RINGTONE_SOURCE_NOT_EXIST			-70511		//短信铃声源文件不存在
#define ERR_IPHONERUN_SMS_RINGTONE_COPY_ERROR				-70512		//复制短信铃声文件失败
#define ERR_IPHONERUN_SMS_RINGTONE_OPEN_ERROR				-70513		//打开短信铃声配置文件失败
	
//文件传输
#define ERR_IPHONERUN_FILE_TRANSFER_PATH_NOT_REGULAR		-70601		//不是普通文件,是目录或链接
#define ERR_IPHONERUN_FILE_TRANSFER_SERVER_START_SOCKET		-70602		//服务端启动套接字失败
#define ERR_IPHONERUN_FILE_TRANSFER_SERVER_OPEN_FILE		-70603		//服务端创建或打开文件失败
#define ERR_IPHONERUN_FILE_TRANSFER_LOCAL_OPEN_FILE			-70604		//本地创建或打开文件失败
#define ERR_IPHONERUN_FILE_TRANSFER_LOCAL_RECV				-70605		//本地接收时错误
#define ERR_IPHONERUN_FILE_TRANSFER_LOCAL_SEND				-70606		//本地接收时错误
#define ERR_IPHONERUN_FILE_TRANSFER_LOCAL_FWRITE			-70607		//本地写文件失败
#define ERR_IPHONERUN_FILE_TRANSFER_LOCAL_FREAD				-70608		//本地读文件失败
#define ERR_IPHONERUN_FILE_TRANSFER_FILE_SIZE_UNEQUAL		-70609		//传输后文件大小不一致

//WiFi连接
#define ERR_IPHONERUN_WIFI_CONNECT_DISABLE					-70701		//无线连接关闭
#define ERR_IPHONERUN_WIFI_PASSWORD_UNEQUAL					-70702		//无线连接密码不对
#define ERR_IPHONERUN_WIFI_USER_REFUSE_CONNECT				-70703		//无线连接用户拒绝连接

//安装ipa手机版手机助手插件(守护程序)
#define ERR_IPHONERUN_EXEC_FILE_NOT_EXIST					-71101		//可执行程序不存在
#define ERR_IPHONERUN_EXEC_FILE_COPY_ERROR					-71102		//可执行程序复制失败
#define ERR_IPHONERUN_LAUNCHD_FILE_CREATE_ERROR				-71103		//启动配置文件创建失败
#define ERR_IPHONERUN_LAUNCHD_FILE_CHMOD_ERROR				-71104		//启动配置文件修改权限失败
#define ERR_IPHONERUN_LAUNCHD_FILE_LOAD_ERROR				-71105		//启动配置文件加载失败,需提示用户重启手机

//===================================================================================================================

typedef struct IphoneInterfaceType * pIphoneInterfaceType;		//是个结构体指针就对了

#ifdef __cplusplus
extern "C" {
#endif

	//************************************
	// Method   : iphoneinterface_initialize
	// Returns  : int 返回0成功,加载dll失败返回-1
	// Parameter: PTCHAR itunesMobileDeviceDllPath  iTunesMobileDevice.dll所在路径,末尾不带'\\',(C:\Program Files\Common Files\Apple\Mobile Device Support)
	// Parameter: PTCHAR corefoundationDllPath CoreFoundation.dll以及相关dll所在路径,末尾不带'\\',(C:\Program Files\Common Files\Apple\Apple Application Support)
	// Parameter: PTCHAR logFile 日志文件存放绝对路径,包括文件名(如D:\helperdaemon.log)
	// Note     : iTunes10.5.0.142+使用,Apple Application Support版本2.1.5, Apple Mobile Device Support版本4.0.0.96, 必须能正常安装Apple Mobile Device Support(windows server 2003不能正常安装)
	//************************************
	int iphoneinterface_initialize(const PTCHAR itunesMobileDeviceDllPath, const PTCHAR corefoundationDllPath, const PTCHAR logFile);

	//************************************
	// Method   : iphoneinterface_uninitialize
	// Note     : 退出时调用一次
	//************************************
	void iphoneinterface_uninitialize(void);

	//************************************
	// Method   : setIphoneInterfaceTypeUsb
	// Returns  : pIphoneInterfaceType 10秒内使用有效(重新生成校验序列)
	// Parameter: unsigned char * security 加密二进制串,函数入口校验
	// Parameter: struct am_device * device
	// Parameter: PTCHAR localDaemonDir 本地守护文件存放目录,末尾不带'\\'
	// Parameter: PTCHAR localFileBackDir 备份文件存放路径,必须是已存在的目录,末尾不带'\\',最好是一个手机一个路径
	// Note     : usb连接
	//************************************
	pIphoneInterfaceType setIphoneInterfaceTypeUsb(const unsigned char *security, struct am_device *device, const PTCHAR localDaemonDir, const PTCHAR localFileBackDir); //加密后10秒内使用有效
	//************************************
	// Method   : setIphoneInterfaceTypeWifi
	// Returns  : pIphoneInterfaceType 10秒内使用有效(重新生成校验序列)
	// Parameter: unsigned char * security 加密二进制串,函数入口校验
	// Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001
	// Parameter: PTCHAR passwd wifi连接密码,NULL空密码_T(""),下边大部分操作每次连接都验证密码
	// Note     : wifi连接,通过iphoneinterface_install91HelperService安装手机版手机助手插件才能使用后续接口
	//************************************
	pIphoneInterfaceType setIphoneInterfaceTypeWifi(const unsigned char *security, DWORD iosIp, const PTCHAR passwd = NULL); 
	//************************************
	// Method   : freeIphoneInterfaceType
	// Note     : 释放setIphoneInterfaceTypeUsb和setIphoneInterfaceTypeWifi分配的内存
	//************************************
	void freeIphoneInterfaceType(pIphoneInterfaceType type);

	//************************************
	// Method   : iphoneinterface_install91HelperService
	// Returns  : int 返回ERR_IPHONERUN_LAUNCHD_FILE_LOAD_ERROR需要提示用户重启手机
	// Parameter: pIphoneInterfaceType iType
	// Note     : 安装ipa手机版手机助手插件(wifi守护程序和ipa手机版手机助手守护程序),若本地守护文件有更新,需要调用这个同时更新手机端程序,必须为usb连接
	//************************************
	int iphoneinterface_install91HelperService(pIphoneInterfaceType iType);
	//************************************
	// Method   : iphoneinterface_uninstall91HelperService
	// Returns  : int 原先没有安装的返回-1
	// Note     : 删除ipa手机版手机助手插件(wifi守护程序和ipa手机版手机助手守护程序)
	//************************************
	int iphoneinterface_uninstall91HelperService(pIphoneInterfaceType iType);

	//************************************
	// Method   : iphoneinterface_chmod
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: pIphoneInterfaceType iType
	// Parameter: PTCHAR remoteFile 远程文件路径 本地unicode编码
	// Parameter: PTCHAR mode 权限rwxr-xr-x为0755 rw-r--r--为0644 最大权限为rwsrwsrwt 07777
	// Parameter: bool recursive 是否也修改子目录文件
	// Parameter: callBack 回调函数,执行进度,回调函数体中返回非0则本函数立即返回该非0值,服务端仍在运行
	//************************************
	int iphoneinterface_chmod(pIphoneInterfaceType iType, const PTCHAR remoteFile, const PTCHAR mode = _T("rwxr-xr-x"), bool recursive = false, int (*callBack)(CFDictionaryRef) = NULL);
	//************************************
	// Method   : iphoneinterface_chown
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: pIphoneInterfaceType iType
	// Parameter: PTCHAR remoteFile 远程文件路径 本地unicode编码
	// Parameter: int uid 0或501
	// Parameter: int gid 0或501
	// Parameter: bool recursive 是否也修改子目录文件
	// Parameter: callBack 回调函数,执行进度,回调函数体中返回非0则本函数立即返回该非0值,服务端仍在运行
	//************************************
	int iphoneinterface_chown(pIphoneInterfaceType iType, const PTCHAR remoteFile, int uid = 501, int gid = 501, bool recursive = false, int (*callBack)(CFDictionaryRef) = NULL);
	
	//************************************
	// Method   : iphoneinterface_pxlInstall iphoneinterface_special_pxlInstall
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: pIphoneInterfaceType iType
	// Parameter: PTCHAR remotePxlFile 远程文件绝对路径 本地unicode编码
	// Parameter: bool isZip 是否是zip压缩包
	// Parameter: PTCHAR * error 安装过程中生成的具体失败原因,执行完需要调用iphoneinterface_delete释放非空的*error,不需要错误原因可以传NULL
	// Parameter: callBack 回调函数,执行进度和具体失败原因,安装超过60秒的需要使用,否则会超时返回失败,回调函数体中返回非0则本函数立即返回该非0值,服务端仍在运行
	// Note     : pxl安装,软件已存在会先卸载再安装,若app下的Info.plist的CFBundleIdentifier跟PxlPkg.plist里边的不一致,则修改成PxlPkg.plist里边的CFBundleIdentifier(百宝箱软件管理需要)
	//			  iphoneinterface_special_pxlInstall()在iphoneinterface_install91HelperService()安装了ipa手机版手机助手插件才能使用,用于pxl脚本中安装卸载deb的软件包.
	//************************************
	int iphoneinterface_pxlInstall(pIphoneInterfaceType iType, const PTCHAR remotePxlFile, bool isZip = true, PTCHAR *error = NULL, int (*callBack)(CFDictionaryRef) = NULL);
	int iphoneinterface_special_pxlInstall(pIphoneInterfaceType iType, const PTCHAR remotePxlFile, bool isZip = true, PTCHAR *error = NULL, int (*callBack)(CFDictionaryRef) = NULL);
	//************************************
	// Method   : iphoneinterface_pxlUninstall iphoneinterface_special_pxlUninstall
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: PTCHAR CFBundleIdentifier PxlPkg.plist里的CFBundleIdentifier
	// Parameter: PTCHAR * error 卸载过程中生成的具体失败原因,执行完需要调用iphoneinterface_delete释放非空的*error,不需要错误原因可以传NULL
	// Parameter: callBack 回调函数,执行进度和具体失败原因,卸载超过30秒的需要使用,否则会超时返回失败,回调函数体中返回非0则本函数立即返回该非0值,服务端仍在运行
	// Note     : pxl卸载
	//			  iphoneinterface_special_pxlUninstall()在iphoneinterface_install91HelperService()安装了ipa手机版手机助手插件才能使用,用于pxl脚本中安装卸载deb的软件包.
	//************************************
	int iphoneinterface_pxlUninstall(pIphoneInterfaceType iType, const PTCHAR CFBundleIdentifier, PTCHAR *error = NULL, int (*callBack)(CFDictionaryRef) = NULL);
	int iphoneinterface_special_pxlUninstall(pIphoneInterfaceType iType, const PTCHAR CFBundleIdentifier, PTCHAR *error = NULL, int (*callBack)(CFDictionaryRef) = NULL);

	//************************************
	// Method   : iphoneinterface_runProgram iphoneinterface_special_runProgram
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: PTCHAR program 用system()执行的参数
	// Parameter: long waitSeconds 执行等待时间,秒
	// Parameter: int * status 执行返回值,program程序返回值的低8位(0x00000000~0x000000ff),若不需要则传NULL
	// Note     : 运行程序或脚本
	//			  iphoneinterface_special_runProgram()在iphoneinterface_install91HelperService()安装了ipa手机版手机助手插件才能使用,主要用于安装deb.
	//************************************
	int iphoneinterface_runProgram(pIphoneInterfaceType iType, const PTCHAR program, long waitSeconds, int *status = NULL);
	int iphoneinterface_special_runProgram(pIphoneInterfaceType iType, const PTCHAR program, long waitSeconds, int *status = NULL);

	//************************************
	// Method   : iphoneinterface_reload
	// Returns  : int 返回0成功,小于0具体的失败值
	// Note     : 注销
	//************************************
	int iphoneinterface_reload(pIphoneInterfaceType iType);

	//************************************
	// Method   : iphoneinterface_reboot
	// Note     : 重启
	//************************************
	int iphoneinterface_reboot(pIphoneInterfaceType iType);
	//************************************
	// Method   : iphoneinterface_shutdown
	// Note     : 关机
	//************************************
	int iphoneinterface_shutdown(pIphoneInterfaceType iType);
	//************************************
	// Method   : iphoneinterface_kill
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: PTCHAR processName 进程名称如SpringBoard
	// Note     : 结束进程
	//************************************
	int iphoneinterface_kill(pIphoneInterfaceType iType, const PTCHAR processName);
	
	//************************************
	// Method   : iphoneinterface_getImsi
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: pIphoneInterfaceType iType
	// Parameter: PTCHAR * imsi 取得的imsi,执行完需要调用iphoneinterface_delete释放非空的*imsi
	//************************************
	int iphoneinterface_getImsi(pIphoneInterfaceType iType, PTCHAR *imsi);
	//************************************
	// Method   : iphoneinterface_getType
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: PTCHAR * type 取得的设备类型,执行完需要调用iphoneinterface_delete释放非空的*type
	//************************************
	int iphoneinterface_getType(pIphoneInterfaceType iType, PTCHAR *type);
	//************************************
	// Method   : iphoneinterface_iconsRepair
	// Returns  : int 返回0成功,小于0具体的失败值
	// Note     : 图标修复
	//************************************
	int iphoneinterface_iconsRepair(pIphoneInterfaceType iType);
	//************************************
	// Method   : iphoneinterface_screenshot
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: BYTE * * pngBytesPtr 截图文件的头指针,执行完需要调用iphoneinterface_delete释放非空的*pngBytesPtr
	// Parameter: UINT * pngLength 截图文件的大小,字节
	// Note     : 高清截屏
	//************************************
	int iphoneinterface_screenshot(pIphoneInterfaceType iType, BYTE **pngBytesPtr, UINT *pngLength); 

	//************************************
	// Method   : iphoneinterface_changeCallRingtone
	// Note     : 发送铃声修改消息
	//************************************
	int iphoneinterface_changeCallRingtone(pIphoneInterfaceType iType);
	//************************************
	// Method   : iphoneinterface_mome
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: BYTE * cmd PC端要发送的已加密的指令
	// Parameter: UINT cmdLength 指令长度
	// Parameter: BYTE * * resultBytesPtr 执行结果头指针,执行完需要调用iphoneinterface_delete释放非空的*resultBytesPtr
	// Parameter: UINT * resultLength 执行结果长度,字节
	// Note     : mome主题
	//************************************
	int iphoneinterface_mome(pIphoneInterfaceType iType, const BYTE *cmd, UINT cmdLength, BYTE **resultBytesPtr, UINT *resultLength); 

	//************************************
	// Method   : iphoneinterface_sendSmsSingle
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: pIphoneInterfaceType iType
	// Parameter: PTCHAR aTelephoneNumber 短信的一个接收号码
	// Parameter: PTCHAR content 短信内容
	// Note     : 发送一条短信到一个接收号码
	//************************************
	int iphoneinterface_sendSmsSingle(pIphoneInterfaceType iType, const PTCHAR aTelephoneNumber, const PTCHAR content);
	//************************************
	// Method   : iphoneinterface_setSmsFlag
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: PTCHAR smsid 短信id
	// Parameter: PTCHAR flag 短信标示位
	// Note     : 设置短信状态
	//************************************
	int iphoneinterface_setSmsFlag(pIphoneInterfaceType iType, const PTCHAR smsid, const PTCHAR flag);
	//************************************
	// Method   : iphoneinterface_smsRepair
	// Note     : 短信修复
	//************************************
	int iphoneinterface_smsRepair(pIphoneInterfaceType iType);

	typedef enum SET_BACKGROUND_IMAGE 
	{
		BACKGROUND_IMAGE_LOCK,					//锁屏背景
		BACKGROUND_IMAGE_DESKTOP,				//桌面背景
		BACKGROUND_IMAGE_DOUBLE					//同时设置锁屏背景和桌面背景
	}SET_BACKGROUND_IMAGE;
	//************************************
	// Method   : iphoneinterface_setBackgroundImage
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: PTCHAR image 远程壁纸文件绝对路径
	// Parameter: SET_BACKGROUND_IMAGE action 锁屏背景 桌面背景 同时设置
	// Note     : 设置壁纸
	//************************************
	int iphoneinterface_setBackgroundImage(pIphoneInterfaceType iType, const PTCHAR image, SET_BACKGROUND_IMAGE action); 
	//************************************
	// Method   : iphoneinterface_installTheme
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: PTCHAR themeName 主题名称
	// Parameter: PTCHAR themePath 远程主题文件目录
	// Parameter: PTCHAR themePreview 远程主题预览图
	// Note     : 设置主题
	//************************************
	int iphoneinterface_installTheme(pIphoneInterfaceType iType, const PTCHAR themeName, const PTCHAR themePath, const PTCHAR themePreview);
	//************************************
	// Method   : iphoneinterface_setSmsRingTone
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: PTCHAR sourcePath 远程铃声源文件
	// Parameter: PTCHAR destPath 远程铃声目标文件
	// Parameter: PTCHAR fileName 远程铃声配置文件
	// Parameter: PTCHAR key 铃声配置键值
	// Parameter: PTCHAR name 铃声名称
	// Note     : 设置短信铃声
	//************************************
	int iphoneinterface_setSmsRingTone(pIphoneInterfaceType iType, const PTCHAR sourcePath, const PTCHAR destPath, const PTCHAR fileName, const PTCHAR key, const PTCHAR name);
	//************************************
	// Method   : iphoneinterface_setCallRing
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: PTCHAR ringID 铃声id
	// Parameter: PTCHAR ringPath 远程铃声源文件
	// Parameter: PTCHAR chinaName 铃声名称
	// Note     : 设置来电铃声
	//************************************
	int iphoneinterface_setCallRing(pIphoneInterfaceType iType, const PTCHAR ringID, const PTCHAR ringPath, const PTCHAR chinaName);
	
	//************************************
	// Method   : iphoneinterface_copyFile
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: PTCHAR from 远程源文件或目录绝对路径
	// Parameter: PTCHAR to 目标文件或目录绝对路径,注意不能只传入目标的上一个目录
	// Parameter: PTCHAR mode 生成的文件或目录权限,注意是9个字符，无相应权限位的用'-',rwx小写, rwxr-xr-x 0755, rwxrwxrwx 0777
	// Parameter: PTCHAR umask 创建目录umask使用的参数,创建目录前拿走相应的权限位, ----w--w- 0022, --------- 0000
	// Parameter: int uid 生成的文件或目录的属主 501 mobile  0 root
	// Parameter: int gid 生成的文件或目录的属组 501 mobile  0 root
	// Parameter: callBack 回调函数,执行进度,若不需要则传NULL,回调函数体中返回非0则本函数立即返回该非0值,服务端仍在运行
	// Note     : 复制文件,绝对路径以'/'开头,目标路径必须带有目标文件名或目录名,目标路径的上个路径不存在则创建,若目标已存在则会被删除再复制;若源文件包含链接,则只会复制链接,不会复制链接目录下的文件,并且链接会变成一个普通文件;在ipa沙盒里建立root属主的文件或目录会导致ipa卸载不了,升级时会提示错误.
	//************************************
	int iphoneinterface_copyFile(pIphoneInterfaceType iType, const PTCHAR from, const PTCHAR to, const PTCHAR mode = _T("rwxr-xr-x"), const PTCHAR umask = _T("----w--w-"), int uid = 501, int gid = 501, int (*callBack)(CFDictionaryRef) = NULL);
	//************************************
	// Method   : iphoneinterface_removeFile
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: PTCHAR path
	// Parameter: callBack 回调函数,执行进度,若不需要则传NULL,回调函数体中返回非0则本函数立即返回该非0值,服务端仍在运行
	// Note     : 删除普通文件和目录,若传人的文件不存在也会返回成功,传人的是目录时则会删除目录本身和目录下所有文件
	//************************************
	int iphoneinterface_removeFile(pIphoneInterfaceType iType, const PTCHAR path, int (*callBack)(CFDictionaryRef) = NULL);
	//************************************
	// Method   : iphoneinterface_copyFile
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: PTCHAR from 远程源文件或目录绝对路径
	// Parameter: PTCHAR to 目标文件或目录绝对路径
	// Note     : 文件重命名
	//************************************
	int iphoneinterface_renameFile(pIphoneInterfaceType iType, const PTCHAR from, const PTCHAR to);
	//************************************
	// Method   : iphoneinterface_getDirectorySize
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: CFArrayRef dirs 要统计的路径数组
	// Parameter: CFArrayRef * sizes 统计结果数组,不存在的路径-1,空目录大小0,需要用iphoneinterface_CFRelease释放非空的*sizes
	// Note     : 统计普通文件或目录大小
	//************************************
	int iphoneinterface_getDirectorySize(pIphoneInterfaceType iType, CFArrayRef dirs, CFArrayRef *sizes);
	//************************************
	// Method   : iphoneinterface_createDirectory
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: PTCHAR path 目录绝对路径
	// Parameter: PTCHAR mode 生成的文件或目录权限,注意是9个字符，无相应权限位的用'-',rwx小写, rwxr-xr-x 0755, rwxrwxrwx 0777
	// Parameter: PTCHAR umask 创建目录umask使用的参数,创建目录前拿走相应的权限位, ----w--w- 0022, --------- 0000
	// Parameter: int uid 生成的目录的属主 501 mobile  0 root
	// Parameter: int gid 生成的目录的属组 501 mobile  0 root
	// Note     : 创建目录
	//************************************
	int iphoneinterface_createDirectory(pIphoneInterfaceType iType, const PTCHAR path, const PTCHAR mode = _T("rwxr-xr-x"), const PTCHAR umask = _T("----w--w-"), int uid = 501, int gid = 501);
	
	//************************************
	// Method   : iphoneinterface_getFileInfo
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: pIphoneInterfaceType iType
	// Parameter: PTCHAR remoteFile 远程目录或文件或链接绝对路径
	// Parameter: INT64 * size 大小字节
	// Parameter: UINT16 * mode 文件类型和权限st_mode
	// Parameter: UINT32 * uid 属主0或者501或者其他
	// Parameter: UINT32 * gid 属组0或者501或者其他
	// Parameter: LONG * mtime 文件修改时间st_mtime
	// Parameter: LONG * birthtime 文件创建时间
	// Note     : 取文件状态
	//************************************
	int iphoneinterface_getFileInfo(pIphoneInterfaceType iType, const PTCHAR remoteFile, INT64 *size, UINT16 *mode, UINT32 *uid, UINT32 *gid, LONG *mtime, LONG *birthtime);
	//************************************
	// Method   : iphoneinterface_getFileSystemInfo
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: PTCHAR remoteFile 远程目录或文件绝对路径
	// Parameter: UINT32 * blockSize 分区的块大小
	// Parameter: UINT64 * totalBlocks 分区的块数量
	// Parameter: UINT64 * freeBlocks 未使用的块数量
	// Note     : 取分区大小剩余空间
	//************************************
	int iphoneinterface_getFileSystemInfo(pIphoneInterfaceType iType, const PTCHAR remoteFile, UINT32 *blockSize, UINT64 *totalBlocks, UINT64 *freeBlocks);

	//************************************
	// Method   : iphoneinterface_getFileList
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: pIphoneInterfaceType iType
	// Parameter: PTCHAR remoteFile 远程目录或文件绝对路径 
	// Parameter: CFMutableDictionaryRef * list 文件列表,使用完后需要用iphoneinterface_CFRelease释放非空的*fileList
	// Note     : 取普通文件属性或目录属性和文件列表
	//************************************
	int iphoneinterface_getFileList(pIphoneInterfaceType iType, const PTCHAR remoteFile, CFMutableDictionaryRef *fileList);
	//************************************
	// Method   : iphoneinterface_getListFirstFile
	// Parameter: CFMutableDictionaryRef fileList 通过iphoneinterface_getFileList取得文件列表
	// Parameter: PTCHAR * fileName	文件名称,需要用iphoneinterface_delete释放非空的*fileName
	// Parameter: PTCHAR * link 若是连接则给出链接的源路径,否则为NULL,需要用iphoneinterface_delete释放非空的*link
	// Parameter: INT64 * size 大小字节
	// Parameter: UINT16 * mode 文件类型和权限st_mode
	// Parameter: UINT32 * uid 属主0或者501或者其他
	// Parameter: UINT32 * gid 属组0或者501或者其他
	// Parameter: LONG * mtime 文件修改时间st_mtime
	// Parameter: LONG * birthtime 文件创建时间
	// Parameter: INT32 * subCount 目录下第一层文件和目录总个数,大等于0
	// Note     : 取第一个文件属性,iphoneinterface_getFileList给的路径本身属性,并重置fileList里的偏移到第一个子文件或子目录
	//************************************
	int iphoneinterface_getListFirstFile(CFMutableDictionaryRef fileList, PTCHAR *fileName, PTCHAR *link, INT64 *size, UINT16 *mode, UINT32 *uid, UINT32 *gid, LONG *mtime, LONG *birthtime, INT32 *subCount);
	//************************************
	// Method   : iphoneinterface_getListNextFile
	// Parameter: CFMutableDictionaryRef fileList 通过iphoneinterface_getFileList取得文件列表
	// Parameter: PTCHAR * fileName 文件名称,需要用iphoneinterface_delete释放非空的*fileName
	// Parameter: PTCHAR * link 若是连接则给出链接的源路径,否则为NULL,需要用iphoneinterface_delete释放非空的*link
	// Parameter: INT64 * size 大小字节
	// Parameter: UINT16 * mode 文件类型和权限st_mode
	// Parameter: UINT32 * uid 属主0或者501或者其他
	// Parameter: UINT32 * gid 属组0或者501或者其他
	// Parameter: LONG * mtime 文件修改时间st_mtime
	// Parameter: LONG * birthtime 文件创建时间
	// Parameter: INT32 * lastFiles 剩余的文件或目录个数
	// Note     : 取目录下第一层下一个文件或目录属性,若第一层没有文件和目录则fileName为NULL,lastFiles为0,偏移记录在fileList
	//************************************
	int iphoneinterface_getListNextFile(CFMutableDictionaryRef fileList, PTCHAR *fileName, PTCHAR *link, INT64 *size, UINT16 *mode, UINT32 *uid, UINT32 *gid, LONG *mtime, LONG *birthtime, INT32 *lastFiles);

	//************************************
	// Method   : iphoneinterface_getFile
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: pIphoneInterfaceType iType
	// Parameter: PTCHAR remoteFile 远程文件绝对路径,'/'开头,不能是目录或链接
	// Parameter: PTCHAR localFile 本地文件绝对路径,目标的上一个目录必须已存在并且有写权限,不能只传入目标的上一个目录
	// Note     : 下载普通文件,不能是目录或链接
	//************************************
	int iphoneinterface_getFile(pIphoneInterfaceType iType, const PTCHAR remoteFile, const PTCHAR localFile);
	//************************************
	// Method   : iphoneinterface_putFile
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: pIphoneInterfaceType iType
	// Parameter: PTCHAR remoteFile 远程文件绝对路径,'/'开头,不能是目录或链接
	// Parameter: PTCHAR localFile 本地文件绝对路径,文件要有读权限
	// Note     : 上传普通文件,不能是目录或链接
	//************************************
	int iphoneinterface_putFile(pIphoneInterfaceType iType, const PTCHAR remoteFile, const PTCHAR localFile); 

	typedef enum IOSNotifyType
	{
		IOSNotifyType_none,						//每4秒收到一次
		IOSNotifyType_WiFiDisconnect,			//手机端关闭WiFi连接
		IOSNotifyType_SystemWillSleep,			//手机即将休眠
		IOSNotifyType_SystemSleep,				//手机开始休眠(目前手机端在有连接下会取消休眠)
		IOSNotifyType_LockScreen,				//手机锁屏
		IOSNotifyType_DeviceWillShutdown,		//手机即将关机
	}IOSNotifyType;
	//************************************
	// Method   : iphoneinterface_notify
	// Returns  : int 返回0 ERR_IPHONEINTERFACE_WAIT_SERVER_RESULT服务端断连,其它小于0具体的失败值
	// Parameter: pIphoneInterfaceType iType
	// Parameter: callBack 回调函数参数为enum IOSNotifyType,在回调函数中返回非0并且不是本文件定义的错误码-7****则退出这个函数
	// Note     : 手机端消息通知
	//************************************
	int iphoneinterface_notify(pIphoneInterfaceType iType, int (*callBack)(IOSNotifyType));

	typedef enum ConfigureActionType
	{
		ConfigureActionType_wifiEnable,				//WiFi连接开关,值为BOOL,TRUE为打开,FALSE拒绝本手机外的连接
		ConfigureActionType_wifiPassword,			//WiFi连接密码,值为PTCHAR,空密码_T("")
		ConfigureActionType_wifiConfirm,			//WiFi连接是否需要确认,值为BOOL
		ConfigureActionType_getDeviceVersion,		//获取设备版本字符串,值为PTCHAR如3.0/4.3.3/5.0.1/6.0,只在iphoneinterface_configureGet()中使用
	}ConfigureActionType;
	//************************************
	// Method   : iphoneinterface_configureSet iphoneinterface_configureGet
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: pIphoneInterfaceType iType
	// Parameter: int count 可变参数的个数,偶数
	// Parameter: ... 可变参数一个或多个成对的设置
	// Note     : 保存和获取手机端WiFi连接参数,使用方法如:
	// iphoneinterface_configureSet(iType, 2, ConfigureActionType_wifiEnable, BOOL wifiEnable)
	// iphoneinterface_configureSet(iType, 2, ConfigureActionType_wifiPassword, const PTCHAR password)
	// iphoneinterface_configureSet(iType, 2, ConfigureActionType_wifiConfirm, BOOL isNeedConfirm)
	// iphoneinterface_configureSet(iType, 4, ConfigureActionType_wifiEnable, BOOL wifiEnable, ConfigureActionType_wifiPassword, const PTCHAR password)
	// iphoneinterface_configureSet(iType, 6, ConfigureActionType_wifiEnable, BOOL wifiEnable, ConfigureActionType_wifiPassword, const PTCHAR password, ConfigureActionType_wifiConfirm, BOOL isNeedConfirm)
	// ...
	// iphoneinterface_configureGet(iType, 2, ConfigureActionType_wifiEnable, BOOL *wifiEnable)
	// iphoneinterface_configureGet(iType, 2, ConfigureActionType_wifiPassword, PTCHAR *password) ---需要用iphoneinterface_delete释放返回的*password
	// iphoneinterface_configureGet(iType, 2, ConfigureActionType_wifiConfirm, BOOL *isNeedConfirm)
	// iphoneinterface_configureGet(iType, 2, ConfigureActionType_getDeviceVersion, PTCHAR *versionString) ---需要用iphoneinterface_delete释放返回的*versionString
	// iphoneinterface_configureGet(iType, 4, ConfigureActionType_wifiConfirm, BOOL *isNeedConfirm, ConfigureActionType_getDeviceVersion, PTCHAR *versionString) ---需要用iphoneinterface_delete释放返回的*versionString
	// iphoneinterface_configureGet(iType, 6, ConfigureActionType_wifiEnable, BOOL *wifiEnable, ConfigureActionType_wifiPassword, PTCHAR *password, ConfigureActionType_wifiConfirm, BOOL *isNeedConfirm) ---需要用iphoneinterface_delete释放返回的*password
	// ...
	// 在WiFi连接时iphoneinterface_configureGet()不验证密码
	//************************************
	int iphoneinterface_configureSet(pIphoneInterfaceType iType, int count, ...);
	int iphoneinterface_configureGet(pIphoneInterfaceType iType, int count, ...);

	typedef struct IOSConnectAlert				//wifi连接提示
	{
		TCHAR alertHeader[64];					//提示的标题,'\0'结束
		TCHAR alertContent[256];				//提示的内容,'\0'结束
		TCHAR alertAllow[16];					//允许按钮,'\0'结束
		TCHAR alertRefuse[16];					//拒绝按钮,'\0'结束
		double alertTimeOut;					//提示显示时间,单位秒,0.0表示永久显示,负值默认8.0
	}IOSConnectAlert, *pIOSConnectAlert;
	//************************************
	// Method   : iphoneinterface_connectAlert
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: pIphoneInterfaceType iType
	// Parameter: const pIOSConnectAlert connectAlert 手机端开启了wifi连接提示时弹出的提示框设置,NULL使用默认
	// Note     : wifi连接提示,手机端设置了WiFi连接确认时将弹出对话框
	//************************************
	int iphoneinterface_connectAlert(pIphoneInterfaceType iType, const pIOSConnectAlert connectAlert = NULL);

	//************************************
	// Method   : iphoneinterface_getHelperServiceVersion
	// Parameter: PTCHAR version 需要用iphoneinterface_delete释放放回的*version
	// Note     : 取当前程序对应的手机端程序版本(usb连接时取本地守护文件版本),WiFi连接时取ipa手机版手机助手插件(守护程序)版本
	//************************************
	int iphoneinterface_getHelperServiceVersion(pIphoneInterfaceType iType, PTCHAR *version);

	//************************************
	// Method   : iphoneinterface_CFStringRefCreateFromTCHAR
	// Returns  : CFStringRef 需要用iphoneinterface_CFRelease返回的非空指针(DLL内部分配内存)
	// Parameter: const PTCHAR string
	// Note     : TCHAR to CFStringRef
	//************************************
	CFStringRef iphoneinterface_CFStringRefCreateFromTCHAR(const PTCHAR string);
	//************************************
	// Method   : iphoneinterface_TCHARCreateFromCFStringRef
	// Returns  : PTCHAR 需要用iphoneinterface_delete返回的非空指针,若unicode工程返回TCHAR是宽字符,要_tprintf、_fputts等函数显示中文前边用_tsetlocale(LC_CTYPE, _T("chs"))
	// Parameter: CFStringRef cfstring
	// Note     : CFStringRef to TCHAR
	//************************************
	PTCHAR iphoneinterface_TCHARCreateFromCFStringRef(CFStringRef cfstring);

	//************************************
	// Method   : iphoneinterface_CFRelease
	// Parameter: CFTypeRef cf
	// Note     : DLL内部CFRelease
	//************************************
	void iphoneinterface_CFRelease(CFTypeRef cf);
	//************************************
	// Method   : iphoneinterface_delete
	// Parameter: void * buffer
	// Note     : 用delete[]释放DLL内部new[]的内存
	//************************************
	void iphoneinterface_delete(void *buffer);

	//************************************
	// Method   : iphoneinterface_postNotification
	// Returns  : int 返回0成功,小于0具体的失败值
	// Parameter: pIphoneInterfaceType iType 执行完后需要用freeIphoneInterfaceType(iType)释放内存
	// Parameter: CFStringRef notification
	// Note     : 往手机上发消息通知,仅在usb连接下使用
	//************************************
	int iphoneinterface_postNotification(pIphoneInterfaceType iType, CFStringRef notification);

	//************************************
	// Method   : iphoneinterface_getDeviceVersionInt
	// Returns  : int 30103为3.1.3固件,40000为4.0 50101为5.1.1,小于0具体的失败值
	// Parameter: pIphoneInterfaceType iType 执行完后需要用freeIphoneInterfaceType(iType)释放内存,仅在usb连接下使用
	//************************************
	int iphoneinterface_getDeviceVersionInt(pIphoneInterfaceType iType); 

	int iphoneinterface_connectDeviceAfc(struct am_device *iPhoneDev, struct afc_connection **afcConnection, int *serviceAfc2Socket);
	int iphoneinterface_disconnectDeviceAfc(struct am_device *iPhoneDev, struct afc_connection *afcConnection);

	//************************************
	// Note     : 下边char * 带中文时必须是utf8编码
	//************************************
	int iphoneinterface_createAfcDir(struct afc_connection * afcConnection, char *dirname);
	int iphoneinterface_putAfcFile(struct afc_connection *afcConnection, char *remoteFile, PTCHAR localFile);
	int iphoneinterface_getAfcFile(struct afc_connection *afcConnection, char *remoteFile, PTCHAR localFile);
	LONGLONG iphoneinterface_getAfcFileSize(struct afc_connection *afcConnection, char *remoteFile);
	LONGLONG iphoneinterface_getAfcFileModifiedTime(struct afc_connection *afcConnection, char *remoteFile);
	int iphoneinterface_getAfcFileSizeMtime(struct afc_connection *afcConnection, char *remoteFile, INT64 *size, INT64 *mtime);

	//************************************
	// Note     : 需要用iphoneinterface_delete释放放回的内存指针
	//************************************
	char *mbcsToUtf8(const PSTR mbcsString);
	PSTR utf8ToMbcs(const char *utf8String);
	char *unicodeToUtf8(const WCHAR* unicodeString);
	WCHAR *utf8ToUnicode(const char *utf8String);
	char *TCHARCovertToUtf8(const PTCHAR tcharString);
	PTCHAR Utf8CovertToTCHAR(const char *utf8String);
	PSTR unicodeToAnsi(const WCHAR* unicodeString);

	//************************************
	// Method   : CFStringRefToUtf8CString
	// Returns  : char * 需要用iphoneinterface_delete释放放回的非空内存指针
	// Parameter: CFStringRef cfstring
	// Note     : CFStringRef to utf8
	//************************************
	char *CFStringRefToUtf8CString(CFStringRef cfstring);

#ifdef __cplusplus
}
#endif

//====================================================================================
//stat extension
/*
 * [XSI] The following are symbolic names for the values of type mode_t.  They
 * are bitmap values.
 */
/* File type */
#ifndef S_IFMT
#define	S_IFMT		0170000		/* [XSI] type of file mask */
#endif
#ifndef S_IFIFO
#define	S_IFIFO		0010000		/* [XSI] named pipe (fifo) */
#endif
#ifndef S_IFCHR
#define	S_IFCHR		0020000		/* [XSI] character special */
#endif
#ifndef S_IFDIR
#define	S_IFDIR		0040000		/* [XSI] directory */
#endif
#ifndef S_IFBLK
#define	S_IFBLK		0060000		/* [XSI] block special */
#endif
#ifndef S_IFREG
#define	S_IFREG		0100000		/* [XSI] regular */
#endif
#ifndef S_IFLNK
#define	S_IFLNK		0120000		/* [XSI] symbolic link */
#endif
#ifndef S_IFSOCK
#define	S_IFSOCK	0140000		/* [XSI] socket */
#endif
#ifndef S_IFWHT
#define	S_IFWHT		0160000		/* whiteout */
#endif
		
/* File mode */
/* Read, write, execute/search by owner */
#ifndef S_IRWXU
#define	S_IRWXU		0000700		/* [XSI] RWX mask for owner */
#endif
#ifndef S_IRUSR
#define	S_IRUSR		0000400		/* [XSI] R for owner */
#endif
#ifndef S_IWUSR
#define	S_IWUSR		0000200		/* [XSI] W for owner */
#endif
#ifndef S_IXUSR
#define	S_IXUSR		0000100		/* [XSI] X for owner */
#endif
/* Read, write, execute/search by group */
#ifndef S_IRWXG
#define	S_IRWXG		0000070		/* [XSI] RWX mask for group */
#endif
#ifndef S_IRGRP
#define	S_IRGRP		0000040		/* [XSI] R for group */
#endif
#ifndef S_IWGRP
#define	S_IWGRP		0000020		/* [XSI] W for group */
#endif
#ifndef S_IXGRP
#define	S_IXGRP		0000010		/* [XSI] X for group */
#endif
/* Read, write, execute/search by others */
#ifndef S_IRWXO
#define	S_IRWXO		0000007		/* [XSI] RWX mask for other */
#endif
#ifndef S_IROTH
#define	S_IROTH		0000004		/* [XSI] R for other */
#endif
#ifndef S_IWOTH
#define	S_IWOTH		0000002		/* [XSI] W for other */
#endif
#ifndef S_IXOTH
#define	S_IXOTH		0000001		/* [XSI] X for other */
#endif

#ifndef S_ISUID
#define	S_ISUID		0004000		/* [XSI] set user id on execution */
#endif
#ifndef S_ISGID
#define	S_ISGID		0002000		/* [XSI] set group id on execution */
#endif
#ifndef S_ISVTX
#define	S_ISVTX		0001000		/* [XSI] directory restrcted delete */
#endif
		
#ifndef S_ISTXT
#define	S_ISTXT		S_ISVTX		/* sticky bit: not supported */
#endif
#ifndef S_IREAD
#define	S_IREAD		S_IRUSR		/* backward compatability */
#endif
#ifndef S_IWRITE
#define	S_IWRITE	S_IWUSR		/* backward compatability */
#endif
#ifndef S_IEXEC
#define	S_IEXEC		S_IXUSR		/* backward compatability */
#endif

/*
 * [XSI] The following macros shall be provided to test whether a file is
 * of the specified type.  The value m supplied to the macros is the value
 * of st_mode from a stat structure.  The macro shall evaluate to a non-zero
 * value if the test is true; 0 if the test is false.
 */
#define	S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)	/* block special */
#define	S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)	/* char special */
#define	S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)	/* directory */
#define	S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)	/* fifo or socket */
#define	S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)	/* regular file */
#define	S_ISLNK(m)	(((m) & S_IFMT) == S_IFLNK)	/* symbolic link */
#define	S_ISSOCK(m)	(((m) & S_IFMT) == S_IFSOCK)/* socket */
#define	S_ISWHT(m)	(((m) & S_IFMT) == S_IFWHT)	/* whiteout */

//====================================================================================

#pragma pack(pop)

#endif
