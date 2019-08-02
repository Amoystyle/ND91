#pragma once

namespace ND91Assistant
{
	class NoDeamonInterface
	{
	public:
        static void Uninitialize();
        static bool Initialize();

        //DWORD g_ipAddress;
        //CIPAddressCtrl m_ctl_ipaddress.SetAddress(10, 0, 2, 9);
        //m_ctl_ipaddress.GetAddress(g_ipAddress);

        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: void* devicePointer 设备指针
        // Parameter: BYTE** pngBytesPtr 截图文件的头指针,执行完需要调用iphoneinterface_delete释放非空的*pngBytesPtr
        // Parameter: UINT* pngLength 截图文件的大小,字节
        // Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001  为0则为USB连接
        // Note     : 高清截屏
        static int Screenshot(void* devicePointer, BYTE** pngBytesPtr, UINT* pngLength, DWORD iosIp = 0);

        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: void* buffer
        // Note     : 用delete[]释放DLL内部new[]的内存
        static void Delete(void* buffer);

        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: void* devicePointer 设备指针
        // Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001  为0则为USB连接
        // Note     : 注销
        static int Reload(void* devicePointer, DWORD iosIp = 0);

        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: void* devicePointer 设备指针
        // Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001  为0则为USB连接
        // Note     : 重启
        static int Reboot(void* devicePointer, DWORD iosIp = 0);

        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: void* devicePointer 设备指针
        // Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001  为0则为USB连接
        // Note     : 关机
        static int Shutdown(void* devicePointer, DWORD iosIp = 0);

        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: void* devicePointer 设备指针
        // Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001  为0则为USB连接
        // Note     : 图标修复
        static int IconsRepair(void* devicePointer, DWORD iosIp = 0);

        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: void* devicePointer 设备指针
        // Parameter: PTCHAR* imsi 取得的imsi,执行完需要调用iphoneinterface_delete释放非空的imsi
        // Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001  为0则为USB连接
        static int GetImsi(void* devicePointer, PTCHAR* imsi, DWORD iosIp = 0);

        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: void* devicePointer 设备指针
        // Parameter: PTCHAR* type 取得的imsi,执行完需要调用iphoneinterface_delete释放非空的imsi
        // Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001  为0则为USB连接
        static int GetType(void* devicePointer, PTCHAR* type, DWORD iosIp = 0);

        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: PTCHAR processName 进程名称如SpringBoard
        // Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001  为0则为USB连接
        // Note     : 结束进程
        static int Kill(void* devicePointer, PTCHAR processName, DWORD iosIp = 0);

        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: void* devicePointer 设备指针
        // Parameter: PTCHAR remotePxlFile 远程文件全路径 本地unicode编码
        // Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001  为0则为USB连接
		//backupfile: 备份文件夹位置 deondir：守护位置
        // Note     : pxl安装,软件已存在会先卸载再安装,若app下的Info.plist的CFBundleIdentifier跟PxlPkg.plist里边的不一致,则修改成PxlPkg.plist里边的CFBundleIdentifier(百宝箱软件管理需要)
        static int PxlInstall(void* devicePointer, PTCHAR remotePxlFile, DWORD iosIp = 0);

        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: void* devicePointer 设备指针
        // Parameter: PTCHAR CFBundleIdentifier PxlPkg.plist里的CFBundleIdentifier
        // Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001  为0则为USB连接
        // Note     : pxl卸载
        static int PxlUninstall(void* devicePointer, PTCHAR CFBundleIdentifier, DWORD iosIp = 0);
       
        // Returns  : int 返回ERR_IPHONERUN_LAUNCHD_FILE_LOAD_ERROR需要提示用户重启手机
        // Parameter: void* devicePointer 设备指针
        // Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001  为0则为USB连接
        // Note     : 安装ipa手机版手机助手插件(wifi守护程序和ipa手机版手机助手守护程序),若本地守护文件有更新,需要调用这个同时更新手机端程序,必须为usb连接
        static int Install91HelperService(void* devicePointer, DWORD iosIp = 0);

        // Method   : iphoneinterface_runProgram iphoneinterface_special_runProgram
        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: PTCHAR program 用system()执行的参数
        // Parameter: long waitSeconds 执行等待时间,秒
        // Parameter: int * status 执行返回值,program程序返回值的低8位(0x00000000~0x000000ff),若不需要则传NULL
        // Note     : 运行程序或脚本
        static int RunProgram(void* devicePointer, const PTCHAR program, long waitSeconds, int* status = NULL, DWORD iosIp = 0);
        static int SpecialRunProgram(void* devicePointer, const PTCHAR program, long waitSeconds, int* status = NULL, DWORD iosIp = 0);
        
        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: void* devicePointer 设备指针
        // Parameter: PTCHAR remoteFile 远程文件路径 本地unicode编码
        // Parameter: PTCHAR mode 权限rwxr-xr-x为0755 rw-r--r--为0644 最大权限为rwsrwsrwt 07777
        // Parameter: bool recursive 是否也修改子目录文件
        // Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001  为0则为USB连接
        static int Chmod(void* devicePointer, PTCHAR remoteFile, PTCHAR mode = _T("rwxr-xr-x"), bool recursive = false, DWORD iosIp = 0);

        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: void* devicePointer 设备指针
        // Parameter: PTCHAR remoteFile 远程文件路径 本地unicode编码
        // Parameter: PTCHAR uid 0或501
        // Parameter: PTCHAR gid 0或501
        // Parameter: bool recursive 是否也修改子目录文件
        // Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001  为0则为USB连接
        static int Chown(void* devicePointer, PTCHAR remoteFile, PTCHAR uid = _T("501"), PTCHAR gid = _T("501"), bool recursive = false, DWORD iosIp = 0);

        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: void* devicePointer 设备指针
        // Parameter: PTCHAR aTelephoneNumber 短信的一个接收号码
        // Parameter: PTCHAR content 短信内容
        // Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001  为0则为USB连接
        // Note     : 发送一条短信到一个接收号码
        static int SendSmsSingle(void* devicePointer, PTCHAR aTelephoneNumber, PTCHAR content, DWORD iosIp = 0);

        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: void* devicePointer 设备指针
        // Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001  为0则为USB连接
        // Note     : 短信修复
        static int SmsRepair(void* devicePointer, DWORD iosIp = 0);

        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: void* devicePointer 设备指针
        // Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001  为0则为USB连接
        // Note     : 发送铃声修改消息
        static int ChangeCallRingtone(void* devicePointer, DWORD iosIp = 0);

        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: void* devicePointer 设备指针
        // Parameter: PTCHAR smsid 短信id
        // Parameter: PTCHAR flag 短信标示位
        // Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001  为0则为USB连接
        // Note     : 设置短信状态
        static int SetSmsFlag(void* devicePointer, PTCHAR smsid, PTCHAR flag, DWORD iosIp = 0);

        // Returns  : int 返回0成功,小于0具体的失败值
        // Parameter: void* devicePointer 设备指针
        // Parameter: PTCHAR remoteFile 远程文件全路径
        // Parameter: INT64 * size 大小字节
        // Parameter: UINT16 * mode 权限struct _stat64里的st_mode
        // Parameter: UINT32 * uid 属主0或者501或者其他
        // Parameter: UINT32 * gid 数组0或者501或者其他
        // Parameter: LONG * mtime 文件修改时间struct _stat64里的st_mtime
        // Parameter: LONG * birthtime 文件创建时间
        // Parameter: DWORD iosIp 设备ip地址,主机字节如127.0.0.1-->iosIp=0x7F000001  为0则为USB连接
        // Note     : 取文件状态
        static int GetFileInfo(void* devicePointer, PTCHAR remoteFile, INT64 *size, UINT16 *mode, UINT32 *uid, UINT32 *gid, LONG *mtime, LONG *birthtime, DWORD iosIp = 0);

		static void SetPath(wstring lbackupfile,wstring ldeamonDir);
	private:
        static wstring backDir;
        static wstring deamonDir;
	};
}