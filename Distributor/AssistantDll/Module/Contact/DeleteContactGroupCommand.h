#pragma  once

#include "Module/BaseCommand.h"
#include <vector>

using namespace std;

namespace ND91Assistant
{
    class ContactGroupData;

    // 删除联系人分组命令
    class DeleteContactGroupCommand : public BaseCommand
    {

    public:
        DeleteContactGroupCommand(const DeviceData* pDevice, const ContactGroupData* pGroupData);
        virtual ~DeleteContactGroupCommand();

        // 执行命令
        virtual void Execute();

        // 返回删除是否成功
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


        ContactGroupData* _pGroupData; // 待删除的联系人分组数据

        bool _success;  // 是否执行成功
    };
}

