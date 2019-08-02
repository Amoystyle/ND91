///////////////////////////////////////////////////////////
//  SendMessageCommand.h
//  Implementation of the Class SendMessageCommand
//  Created on:      01-六月-2011 9:43:03
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_40988EBC_C77A_4b99_A80E_C737A09B85DA__INCLUDED_)
#define EA_40988EBC_C77A_4b99_A80E_C737A09B85DA__INCLUDED_

#include "Module/BaseCommand.h"
#include "Common/BinaryBufferWriter.h"
#include <string>
#include <vector>
using namespace std;

namespace ND91Assistant
{
    class MessageData;

    class SendMessageCommand : public BaseCommand
    {

    public:
        SendMessageCommand(const DeviceData* pDevice);
        virtual ~SendMessageCommand();

        // 执行命令
        virtual void Execute();

        // 生成Android数据体，
        virtual std::string GetAndroidBodyData();

        // 解析Android返回数据包
        virtual void SetAndroidReturnData(std::string data);

        // 命令传送到IOS设备上的文件名（全路径）
        virtual std::wstring GetIosCommandFilePath();

        // 从IOS上获取的命令结果文件名（全路径）
        virtual std::wstring GetIosResultFilePath();

        // 生成ios文件。返回文件名，调用者负责删除文件
        virtual std::wstring GetIosCommandFileContent();

        // 解析返回文件，解析后删除文件
        virtual  void SetIosReturnFile(std::wstring filename);

        // 设置要发送的信息
        void SetDataList( const std::vector<MessageData*>* DataList);

        // 发送消息是否成功
        bool Success();

    private:

        // 执行Ios命令
        bool IosExecute();

        // 有守护模式下执行Ios命令
        bool IosDeamonExecut();

        // 无守护模式下执行Ios命令
        bool IosNoDeamonExecut();

        // 执行Android命令
        bool AndroidExecute();

        // Summary  : 获取电话号码字符串
        // Returns  : string 电话号码字符串
        string GetPhoneNumberStr();


		void WaiteForResult();


        // Summary  : 发送短信回执
        // Parameter: int id 短信ID
        // Parameter: string phoneNumber 发送短信的电话号码
        // Parameter: bool success 发送是否成功
        void ReceiptMessage(int id, string phoneNumber, bool success);


        // Summary  : 获取IOS设备的产品序列
        // Returns  : std::wstring 产品序列
        // Parameter: const DeviceData * pDevice 设备信息
        wstring GetSerial(const DeviceData * pDevice);

    private:
        // 当前发送的短信息
        MessageData* _pMessageData;

        // 需要发送的短信息
        const vector<MessageData*>* _dataList;

        // 发送成功的ID集合
        vector<int> _sendSuccessIds;

        // 需发送短信的电话号码容器
        vector<string> _phoneVec;

        // 命令执行是否成功
        bool _success;
    };
}

#endif // !defined(EA_40988EBC_C77A_4b99_A80E_C737A09B85DA__INCLUDED_)
