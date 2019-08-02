#pragma  once

namespace ND91Assistant
{
	class ContactAccount;
    class ContactData;

    // 添加联络人命令
    class AddContactDataCommand : public BaseCommand
    {
    public:
        AddContactDataCommand(const DeviceData* pDevice, ContactAccount* pAccount);
        virtual ~AddContactDataCommand();

        // 执行命令
        virtual void Execute();

        // 解析返回数据包
        virtual void SetAndroidReturnData(std::string data);

        // 装入待操作的数据集合
        void SetDataList(const vector<ContactData*>* pDataList);

        // 命令是否执行成功
        bool Success();
        int ExecuteCount() { return _executeCount;};

    private:
        // 执行安卓命令
        bool AndroidExecute();

        // 执行IOS命令
        bool IosExecute();

        // 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
        virtual std::string GetAndroidBodyData();

        void WriteAndroidGroupMemberShip(BinaryBufferWriter& writer, ContactData* pContactData);
        // 命令传送到IOS设备上的文件名（全路径）
        virtual std::wstring GetIosCommandFilePath(){ return L""; }

        // 从IOS上获取的命令结果文件名（全路径）
        virtual std::wstring GetIosResultFilePath(){ return L""; }

        // 生成ios文件
        virtual std::wstring GetIosCommandFileContent(){ return L""; }

        // 解析ios设备的返回文件，解析后删除文件
        virtual void SetIosReturnFile(std::wstring filename){}

    private:

        // 待添加的联络人
        const vector<ContactData*>* _pDataList;

        // 与添加的联络人相关的账户信息
        ContactAccount*      _pAccount;

        // 待执行添加命令的联络人
        vector<ContactData*> _executeDataList;
        int _executeCount;          // 实际执行的条数
        int _executeCountEveryTime; // 每次执行的条数

        int  _maxCount;         // 每次命令允许添加的最大数量
        bool _success;          // 是否执行成功
        bool _bVersion20;       // 安卓的系统的版本是否为2.0以上
        bool _bOMS;             // 是否为OMS系统
    };
}