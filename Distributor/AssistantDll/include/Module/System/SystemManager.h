#pragma once

#include "Module/IManager.h"

namespace ND91Assistant
{
    class CLASSINDLL_CLASS_DECL SystemManager : public IManager
    {
    public:
        SystemManager (const DeviceData* pDevice);
        virtual ~SystemManager();

        // 获取IMEI信息
        wstring GetIMEI();

        // 获取IMSI信息
        wstring GetIMSI();

        // 获取当前设备电池电量
        int  GetBatteryLever();
    
        // 获取SDCard状态
        SDCardState GetSDCardState();

        // 获取设备名称
        wstring GetDeviceName();

        // 设置设备名称
        bool SetDevName(wstring strNewName);

		// 获取android设备是否已root
		bool GetIsRoot();

		// 修改android设备system/app目录为可写
		bool GetIsRemount();

        // 注销手机
        bool LogoffDev();

        // 重启设备
        bool RebootDev();

        // 关闭设备
        bool ShoutdownDev();
	

		// 获取设备分辨率
		wstring GetDeviceResolution();
#ifndef ASSISTANT_LITE
		//进入恢复模式
		bool EnterRecoveryMode();
#endif
		//退出恢复模式
		bool RebootToNormal();

#ifndef ASSISTANT_LITE
        // 短信数据库修复
        bool FixMessageDB();
#endif
#ifdef PLATFORM_IOS
        // 白苹果修复（仅ios设备）
        void RepairIdleDevice();

        //获取设备类型
        wstring GetIosDeviceType();
#endif
#ifdef PLATFORM_ANDROID
#endif
        

    };
}