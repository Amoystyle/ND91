#pragma once

#include "Module/Driver/HardwareInfo.h"

#ifdef WIN32
	#include <cfgmgr32.h>
	#include <windows.h>
	#include <setupapi.h>
	#pragma comment(lib,"setupapi.lib")
#else
	typedef void* HDEVINFO;
	#define SPDRP_HARDWAREID		(0x00000001)  // HardwareID (R/W)
	
	#ifndef GUID_DEFINED
	#define GUID_DEFINED
	#if defined(__midl)
		typedef struct 
		{
		    unsigned long  Data1;
		    unsigned short Data2;
		    unsigned short Data3;
		    byte           Data4[ 8 ];
		} GUID;
	#else
		typedef struct _GUID 
		{
		    unsigned long  Data1;
		    unsigned short Data2;
		    unsigned short Data3;
		    unsigned char  Data4[ 8 ];
		} GUID;
	#endif
	#endif

	typedef struct _SP_DEVINFO_DATA 
	{
    unsigned long cbSize;
    GUID  ClassGuid;
    unsigned long DevInst;    // DEVINST handle
    unsigned long* Reserved;
	} SP_DEVINFO_DATA, *PSP_DEVINFO_DATA;
#endif

namespace ND91Assistant
{
    // ��������������
	class DriverFunction
	{
	public:
		// ��ȡ�豸����
		static string getProperty( HDEVINFO hDevinfo, SP_DEVINFO_DATA device, int nInfo);

		//��ȡVID
		static string GetVID(const string strHardID);

		//��ȡPID
		static string GetPID(const string strHardID);

		//��ȡMI
		static string GetMI(const string strHardID);

        static string GetUSDVIDPID(const string strHardID);

		// ����HardwareInfo������
		static HardwareInfo* CreateHardwareInfo(HARDWARE_TYPE nType, HDEVINFO hDevinfo, SP_DEVINFO_DATA device);

		//����Ƿ���Ҫ��װiTurns 2012-09-27 �Ƶ�systeminfo
		//static bool IsNeedInstalliTunes();

		// ��ȡ����״̬ 0��ʶ����Ҫ��װ������Ϊ��Ҫ��װ
		static unsigned long GetDriverState( HARDWARE_TYPE nType, SP_DEVINFO_DATA device );
	};
}