#pragma  once

#include "Module/BaseCommand.h"
#include <vector>
using namespace std;

namespace ND91Assistant
{
    class ContactData;

    // 更新联络人命令
    class UpdateContactCommand : public BaseCommand
    {
    public:
        UpdateContactCommand(const DeviceData* pDevice, ContactData* pContact, bool bVersion20, bool bOMS);
        virtual ~UpdateContactCommand();

        // 执行命令
        virtual void Execute();

        // 命令是否执行成功
        bool Success();

    private:

        // 执行IOS命令
        bool IosExecute();

    private:

        // 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
        virtual std::string GetAndroidBodyData();

        // 解析返回数据包
        virtual void SetAndroidReturnData(std::string data);

        // 命令传送到IOS设备上的文件名（全路径）
        virtual std::wstring GetIosCommandFilePath(){ return L""; }

        // 从IOS上获取的命令结果文件名（全路径）
        virtual std::wstring GetIosResultFilePath(){ return L""; }

        // 生成ios文件
        virtual std::wstring GetIosCommandFileContent(){ return L""; }

        // 解析ios设备的返回文件，解析后删除文件
        virtual void SetIosReturnFile(std::wstring filename){}


        ContactData* _pContactData; // 待更新的联络人数据

        bool _success;              // 是否执行成功

        bool _bVersion20;           // 安卓的系统的版本是否为2.0以上
        bool _bOMS;                 // 是否为OMS系统
    };
}