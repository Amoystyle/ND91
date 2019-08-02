#pragma  once

#include "Module/Call/CallData.h"

namespace ND91Assistant
{
    // 添加通话记录命令
    class AddCallDataCommand : public BaseCommand
    {
    public:
        AddCallDataCommand(const DeviceData* pDevice);
        virtual ~AddCallDataCommand();

        // 执行命令
        virtual void Execute();

        // 装入待添加的呼叫记录
        void SetDataList(const vector<const CallData*>* DataList);

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

        int  ExecuteCount() { return _executeCount;};

    private:

        // 执行安卓命令
        bool AndroidExecute();
        
        // 执行IOS命令
        bool IosExecute();

        // 重设未执行数据的状态
        void ResetDataStatus();

    private: 
         
        // 需要添加的通话记录（用SetDataList方法设置）
        const vector<const CallData*>* _dataList;
        
        // 待执行添加命令的通话记录容器
        vector<const CallData*> _executeDataList;
        int _executeCount;          // 实际执行的条数
        int _executeCountEveryTime; // 每次执行的条数

        int _maxCount; // 每次命令允许添加的最大数量

        // 命令执行是否成功
        bool _success;
    };
}
