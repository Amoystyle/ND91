#pragma once

#include "AdbShellCommand.h"
#include "Core/DeviceInfo.h"
#include <map>
#include <string>
#include "Device/AndroidPropertyData.h"

#define ADB_CMD_GET_PROP    "getprop"

using namespace std;
namespace ND91Assistant
{
    // 获取设备信息
    class AdbGetPropertyCommand : public AdbShellCommand
    {
    public:
        AdbGetPropertyCommand(const wstring sn)
        {
			_sn = CCodeOperation::UnicodeToUTF_8(sn);
            _strCommand = ADB_CMD_GET_PROP;
        };

        const DeviceInfo* GetDeviceInfo()
        {
            return &_DeviceInfo;
        };

    protected:
        // 解析返回数据
        // 返回值：true 表示成功，false 表示失败
        virtual bool ParseReturnData()
        {
			int retrycount=0;
			//读取失败增加5次重试
			while(_returnData=="" && retrycount++<5)
			{
				Execute();
				Sleep(20);
			}

            AndroidPropertyData parsePropertyData(_returnData.c_str(), _returnData.length());

            _DeviceInfo._strSerialNumberForShow        = parsePropertyData.GetPropertyValue("ro.serialno");
			if(_DeviceInfo._strSerialNumber.empty())
				_DeviceInfo._strSerialNumber = _DeviceInfo._strSerialNumberForShow;
            _DeviceInfo._deviceIMEI             = parsePropertyData.GetPropertyValue("ro.gsm.imei");
            _DeviceInfo._manufacturer           = parsePropertyData.GetPropertyValue("ro.product.manufacturer");
            _DeviceInfo._deviceHWModel          = parsePropertyData.GetPropertyValue("ro.product.model");
            _DeviceInfo._deviceProductVersion   = parsePropertyData.GetPropertyValue("ro.build.version.release");
            _DeviceInfo._language               = parsePropertyData.GetPropertyValue("ro.product.locale.region");
			_DeviceInfo._deviceCoreVersion		= parsePropertyData.GetPropertyValue("ro.build.display.id");
			_DeviceInfo._deviceBaseVersion		= parsePropertyData.GetPropertyValue("gsm.version.baseband");
			_DeviceInfo._deviceInsdeVersion		= parsePropertyData.GetPropertyValue("ro.build.description");

			wstring strPlatform = parsePropertyData.GetPropertyValue("apps.setting.platformversion");
			if (strPlatform.find(L"OMS") == 0 || strPlatform.find(L"OPhone") == 0)
				_DeviceInfo._platform = Platform_OMS;
			else
				_DeviceInfo._platform = Platform_Android;

            return false;
        };

    private:
        DeviceInfo _DeviceInfo;
    };
}