#pragma once

#include "Module/Driver/Driver.h"

#define DRIVER_MANAGER  DriverManager::GetInstance()

// 驱动管理类
namespace ND91Assistant
{
    class DeviceHardIDList;
	class HardwareInfo;    
    class CLASSINDLL_CLASS_DECL DriverManager
    {
	friend class HardwareInfo;	
	private:
		static DriverManager* _pInstance;   // 单例

        DriverManager();
        ~DriverManager();

	public:

		// 取得单例对象
		static DriverManager* GetInstance();

		static void Release();

        //扫描硬件设备，得到手机设备列表
		//如果strHardID有值，则只扫描该strHardID的设备		
        int ScanHardware(wstring wstrHardID = L"");
		//获得当前的设备列表
        vector<Driver*> GetDriverList() { return _DriverList;};
        //是否有设备连进来
        int HasDeviceLinkin();
 		//根据 VID, PID判断是不是Ios设备
		bool IsIosDevice(wstring wstrVID, wstring wstrPID);
        bool IsIosDevice(string strVID, string strPID);

		// 获得strHardID对应的Driver
		Driver* GetDriver(wstring wstrHardID);
		Driver* GetDriver(wstring wstrVID, wstring wstrPID, wstring wstrMI=L"");
		
		// 是否需要安装iTunes
		bool IsNeedInstalliTunes();

        //触发设备扫描
        bool ScanForHardwareChange();

        // 取iTunes下载路径
        wstring GetITunesUrl();
        //安装驱动 Inf 方式
        bool InstallInf(wstring strInf);
        //安装驱动 Exe 方式
        bool InstallExe(wstring strExe, wstring strParam=L"");
        //安装驱动 Msi 方式
        bool InstallMsi(wstring strMsi, wstring strParam=L"");
    private:
		//或者只扫描 符合wstrVID, wstrPID, wstrMI条件的设备
		int ScanHardware(wstring wstrHardID, wstring wstrVID, wstring wstrPID, wstring wstrMI);

        //获取设备类型
        HARDWARE_TYPE GetHardwareType(string strClassName, string strDesc, string strHardID);
		HARDWARE_TYPE GetHardwareType(string strHardID);

		//释放设备
		void ReleaseDriverList();

    private:
		DeviceHardIDList* pDeviceHardIDList;
        //存储扫描到的设备
#pragma warning(disable : 4251)     // 禁用警告4251
        vector<Driver*> _DriverList;
#pragma warning(default : 4251)
    };

}

