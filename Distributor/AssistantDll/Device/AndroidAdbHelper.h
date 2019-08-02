///////////////////////////////////////////////////////////
//  AndroidAdbHelper.h
//  Implementation of the Class AndroidAdbHelper
//  Created on:      01-����-2011 9:42:46
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
#define ADB_CMD_START_SERVER	" start-server"		//����Adb����
//#define ADB_CMD_START_SERVER  " force_startserver"
//#define ADB_CMD_KILL_SERVER		" kill-server"		//ֹͣAdb����
#define ADB_CMD_KILL_SERVER		" nd-kill-server"	
//#define ADB_CMD_LIST_DEVICE		"host:track-devices"	//ö�ٵ�ǰ�����ӵ�Android�豸
#define ADB_CMD_LIST_DEVICE		"host:devices"
#define	ADB_CMD_FORWARD_TCP		"host-serial:%s:forward:tcp:%d;tcp:%d"	//ӳ��˿�
#define	ADB_CMD_REBOOT	" reboot" //����
    // ADB��������
    // ͨ��CMD����ִ��
    // ��Ҫ����ִ��Adb������
    class AndroidAdbHelper
    {
    public:
        AndroidAdbHelper();
        virtual ~AndroidAdbHelper();

        //ִ��һ��ADB����
		void ExecuteCmd(string strCmd, int nWaitTime=5000);
		//adb�Ƿ�����
		bool isAdbWorking();
		//����Adb
		bool startServer();
		//ֹͣAdb
		bool stopServer();

	private:
		wstring _strAdbExe;			//Adb����·��		
    };
}

#endif // !defined(EA_D47CEEA2_C50C_46f0_B89D_1E6638920EF4__INCLUDED_)
