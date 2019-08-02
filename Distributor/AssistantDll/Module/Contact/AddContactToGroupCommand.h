#pragma once

namespace ND91Assistant
{
    class ContactData;
    class ContactGroupData;

    class AddContactToGroupCommand : public BaseCommand
    {

    public:
        AddContactToGroupCommand(const DeviceData* pDevice, ContactGroupData* pGroupData);
        virtual ~AddContactToGroupCommand();

        // 执行命令
        virtual void Execute();

#pragma region

        // 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
        virtual std::string GetAndroidBodyData();

        // 解析返回数据包
        virtual void SetAndroidReturnData(std::string data);

        // 装入待操作的数据集合
        virtual void SetDataList(const vector<ContactData*>* pDataList);

        // 命令传送到IOS设备上的文件名（全路径）
        virtual std::wstring GetIosCommandFilePath(){ return L""; }

        // 从IOS上获取的命令结果文件名（全路径）
        virtual std::wstring GetIosResultFilePath(){ return L""; }

        // 生成ios文件
        virtual std::wstring GetIosCommandFileContent(){ return L""; }

        // 解析ios设备的返回文件，解析后删除文件
        virtual void SetIosReturnFile(std::wstring filename){}

        // 命令是否执行成功
        bool Success();

#pragma endregion

    private:

        ContactGroupData* _pGroupData;

        const vector<const ContactData*>* _pDataList;

        bool _success;

    };
}
