#pragma  once

namespace ND91Assistant
{
    class MessageData;

    // 添加通话记录命令
    class AddMessageCommand : public BaseCommand
    {

    public:
        AddMessageCommand(const DeviceData* pDevice);
        virtual ~AddMessageCommand();

        // 执行命令
        virtual void Execute();

        // 装入待添加的短消息
        void SetDataList(const vector<const MessageData*>* DataList);

        // 生成Android命令数据包
        virtual std::string GetAndroidBodyData();

        // 解析Android返回结果数据包
        virtual void SetAndroidReturnData(std::string data);

        // 命令传送到IOS设备上的文件名（全路径）
        virtual std::wstring GetIosCommandFilePath() { return L""; }

        // 从IOS上获取的命令结果文件名（全路径）
        virtual std::wstring GetIosResultFilePath() { return L""; }

        // 生成ios文件。返回文件名，调用者负责删除文件
        virtual std::wstring GetIosCommandFileContent() { return L""; }

        // 解析返回文件，解析后删除文件
        virtual  void SetIosReturnFile(std::wstring filename) { }
        
        // 命令执行结果：是否成功
        bool Success();

        int ExecuteCount() {return _executeCount;}
    private:
        // 执行安卓命令
        bool AndroidExecute();

        // 执行IOS命令
        bool IosExecute();

    private:

        // 需要添加的短消息（用SetDataList方法设置）
        const vector<const MessageData*>* _dataList;

        // 待执行添加命令的短消息容器
        vector<const MessageData*> _executeDataList;
        int _executeCount;          // 实际执行的条数
        int _executeCountEveryTime; // 每次执行的条数

        int _maxCount; // 每次命令允许添加的最大数量

        // 命令执行是否成功
        bool _success;
    };
}
