#pragma  once

#include "Module/BaseCommand.h"
#include "Common/BinaryBufferWriter.h"
#include <vector>
#include <string>
using namespace std;

namespace ND91Assistant
{
    class MessageData;

    // 删除短消息命令
    class DeleteMessageCommand : public BaseCommand
    {

    public:
        DeleteMessageCommand(const DeviceData* pDevice);
        virtual ~DeleteMessageCommand();

        // 执行命令
        virtual void Execute();
        
        // 装入待删除的短信数据
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

        // 删除是否成功
        bool Success();

    private:

        // 执行IOS命令
        bool IosExecute();

        // 删除彩信文件
        void DeleteMMSFile(const MessageData* pMessageData);

    private:
        // 需要删除的消息（（用SetDataList方法设置）
        const vector<const MessageData*>* _dataList;

        int _executeCount;// 实际执行的条数

        // 命令执行是否成功
        bool _success;
    };
}
