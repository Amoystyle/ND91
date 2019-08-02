
#pragma  once

#include "Module/BaseCommand.h"
#include "Common/BinaryBufferWriter.h"

namespace ND91Assistant
{

    class ReSendMessageCommand : public BaseCommand
    {

    public:
        ReSendMessageCommand(const DeviceData* pDevice,time_t startTime,time_t endTime);
        virtual ~ReSendMessageCommand();

        // 执行命令
        virtual void Execute();

        // 生成Android数据体，
        virtual std::string GetAndroidBodyData();

        // 解析Android返回数据包
        virtual void SetAndroidReturnData(std::string data);

        // 命令传送到IOS设备上的文件名（全路径）
        virtual std::wstring GetIosCommandFilePath() { return L""; }

        // 从IOS上获取的命令结果文件名（全路径）
        virtual std::wstring GetIosResultFilePath() { return L""; }

        // 生成ios文件。返回文件名，调用者负责删除文件
        virtual std::wstring GetIosCommandFileContent() { return L""; }

        // 解析返回文件，解析后删除文件
        virtual  void SetIosReturnFile(std::wstring filename) { }

        // 设置要重发的信息
        void SetDataList(vector<int>* delmsgIds);

        // 发送消息是否成功
        bool Success();

    private:
        // 执行Ios命令
        bool IosExecute();

        // 执行Android命令
        bool AndroidExecute();

        // 获取RowId集合
        void GetRowIdVector(vector<int> *vecRowid);

    private:
        // 当前重发的信息
        int _curMessageId;

		// 要重发的短信id
		vector<int>*  _delmsgIds;

        // 最后一次发送开始时间
        time_t _startTime;

        // 最后一次发送结束时间
        time_t _endTime;

        // 命令执行是否成功
        bool _success;
    };
}

