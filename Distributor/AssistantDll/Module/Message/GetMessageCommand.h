///////////////////////////////////////////////////////////
//  GetMessageCommand.h
//  Implementation of the Class GetMessageCommand
//  Created on:      01-六月-2011 9:42:55
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_D0425901_C899_407d_B915_875FBE1A48A0__INCLUDED_)
#define EA_D0425901_C899_407d_B915_875FBE1A48A0__INCLUDED_

#include "Module/BaseCommand.h"
#include <vector>
#include <string>
using namespace std;

namespace ND91Assistant
{
    class MessageData;
    class MessageGroup;

    // 取短消息命令
    class GetMessageCommand : public BaseCommand
    {
    public:
        GetMessageCommand(const DeviceData* pDevice);
        virtual ~GetMessageCommand();

		// 返回数据数量
		int GetSize();

        // 设置是否只获取数据数量
        void SetOnlySize(bool onlySize);

        // 执行命令
        virtual void Execute();

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

    public:
		void SetDataList(vector<MessageData*>* dataList, vector<MessageGroup*>* groupList);

    private:

        // 执行IOS命令
        bool IosExecute();

    private:
        vector<MessageData*>* _dataList;

		vector<MessageGroup*>* _groupList;

        bool _onlySize;// 是否只获取数量

        int _size;     // 数据数量
    };
}

#endif // !defined(EA_D0425901_C899_407d_B915_875FBE1A48A0__INCLUDED_)
