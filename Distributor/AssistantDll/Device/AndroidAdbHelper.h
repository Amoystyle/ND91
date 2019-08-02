///////////////////////////////////////////////////////////
//  AndroidAdbHelper.h
//  Implementation of the Class AndroidAdbHelper
//  Created on:      01-六月-2011 9:42:46
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_D47CEEA2_C50C_46f0_B89D_1E6638920EF4__INCLUDED_)
#define EA_D47CEEA2_C50C_46f0_B89D_1E6638920EF4__INCLUDED_

#include "common/BaseSocket.h"
#include "Core/DeviceInfo.h"
#include <string>
#include <iostream>
#include <vector>
#ifdef WIN32
    #include <windows.h>
#endif
using namespace std;

 
namespace ND91Assistant
{
#define ADB_CMD_WAIT_TIME		11000
#define ADB_CMD_START_SERVER	" start-server"		//启动Adb服务
//#define ADB_CMD_START_SERVER  " force_startserver"
//#define ADB_CMD_KILL_SERVER		" kill-server"		//停止Adb服务
#define ADB_CMD_KILL_SERVER		" nd-kill-server"	
//#define ADB_CMD_LIST_DEVICE		"host:track-devices"	//枚举当前可连接的Android设备
#define ADB_CMD_LIST_DEVICE		"host:devices"
#define	ADB_CMD_FORWARD_TCP		"host-serial:%s:forward:tcp:%d;tcp:%d"	//映射端口
#define	ADB_CMD_REBOOT	" reboot" //重启
    // ADB基础命令
    // 通过CMD命令执行
    // 主要用于执行Adb服务处理
    class AndroidAdbHelper
    {
    public:
        AndroidAdbHelper();
        virtual ~AndroidAdbHelper();

        //执行一个ADB命令
		void ExecuteCmd(string strCmd, int nWaitTime=5000);
		//adb是否启动
		bool isAdbWorking();
		//启动Adb
		bool startServer();
		//停止Adb
		bool stopServer();

	private:
		wstring _strAdbExe;			//Adb程序路径		
    };
}

#endif // !defined(EA_D47CEEA2_C50C_46f0_B89D_1E6638920EF4__INCLUDED_)
