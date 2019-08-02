#pragma once

#include "AdbRemoteCommand.h"

//#define ADB_CMD_LIST_DEVICE		"host:track-devices"	//枚举当前可连接的Android设备
#define ADB_CMD_LIST_DEVICE		"host:devices"
namespace ND91Assistant
{
    // 通过ADB 获取连接PC的手机设备列表(SerialNumber列表)
    class AdbGetDeviceCommand : public AdbRemoteCommand
    {
    public:
        AdbGetDeviceCommand(string cmd = ADB_CMD_LIST_DEVICE);

        

        const vector<wstring>* GetSnList();
        int GetAllDeviceSize() {return _snMap.size();}; //取所有设备个数
        const map<wstring, wstring>* GetSnMap() {return &_snMap; };
		string GetReturnData();
    protected:
		virtual void InternalExecute();
        // 解析返回数据
        // 返回值：true 表示成功，false 表示失败
        virtual bool ParseReturnData();

    private:
        vector<wstring> _snList; //已连接设备列表
        map<wstring, wstring> _snMap; //所有设备列表，包含各种状态 <设备序列号, 设备状态>
		string _strCommand;
    };

}