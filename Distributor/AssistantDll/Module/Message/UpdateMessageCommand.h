#pragma  once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    class DeviceData;
    class MessageData;

    // 更新短消息命令
    class UpdateMessageCommand : public BaseCommand
    {

    public:
        UpdateMessageCommand(const DeviceData* pDevice);
        virtual ~UpdateMessageCommand();

        // 执行命令
        virtual void Execute();

        // 装入待更新的短信数据
        virtual void SetDataList(const vector<const MessageData*>* DataList);

        // 生成Android数据体，
        virtual std::string GetAndroidBodyData();

        // 解析Android返回数据包
        virtual void SetAndroidReturnData(std::string data);

        // 命令传送到IOS设备上的文件名（全路径）
        virtual std::wstring GetIosCommandFilePath() { return L""; }

        // 从IOS上获取的命令结果文件名（全路径）
        virtual std::wstring GetIosResultFilePath() { return L""; }

        // 生成ios文件。返回文件名，调用者负责删除文件
        virtual std::wstring GetIosCommandFileContent() {throw "方法待实现";return NULL;}

        // 解析ios返回文件，解析后删除文件
        virtual void SetIosReturnFile(std::wstring filename){throw "方法待实现";}

        // 更新是否成功
        bool Success();

    private:

        // 执行IOS命令
        bool IosExecute();

        // 执行Android命令
        bool AndroidExecute();

    private:

        // 需要更新的短消息（（用SetDataList方法设置）
        const std::vector<const MessageData*>* _dataList;

        int _executeCount;// 实际执行的条数
        
        const MessageData* _pMessageData;
        
        // 命令执行是否成功
        bool _success;
    };
}
