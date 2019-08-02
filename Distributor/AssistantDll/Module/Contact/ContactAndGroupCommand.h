#pragma  once

#include "Module/BaseCommand.h"
#include <vector>

using namespace std;

namespace ND91Assistant
{
    class ContactData;
    class ContactGroupData;

    // 操作联络人与分组所属关系的命令
    class ContactAndGroupCommand : public BaseCommand
    {
    public:
        ContactAndGroupCommand(const DeviceData* pDevice);
        virtual ~ContactAndGroupCommand();

        // 命令是否执行成功
        bool Success();
 
        // Summary  : 从分组中移除联络人
        // Returns  : int 实际执行的条数
        // Parameter: const vector<ContactData * > * pDataList  需要操作的联络人集合
        // Parameter: ContactGroupData * pGroupData 需要操作的分组
        int ExecuteRemoveContactFromGroup(const vector<ContactData*>* pDataList, ContactGroupData* pGroupData);

        // Summary  : 将联络人添加到分组
        // Returns  : int 实际执行的条数
        // Parameter: const vector<ContactData * > * pDataList  需要操作的联络人集合
        // Parameter: ContactGroupData * pGroupData 需要操作的分组
        int ExecuteAddContactToGroup(const vector<ContactData*>* pDataList, ContactGroupData* pGroupData);

    private:
        // 执行命令
        virtual void Execute() {}

    private:
        // 执行IOS添加联络人到群组命令
        bool IosExecuteAddContactToGroup();

        // 执行IOS从群组移除联络人命令
        bool IosExecuteRemoveContactFromGroup();

        // 获取RowId集合
        void GetRowIdVector(vector<int> *vecRowid);

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

    private:      
      
        ContactGroupData* _pGroupData; // 待操作的分组信息

        const vector<const ContactData*>* _pDataList; // 待操作的联络人集合
        int _executeCount;// 实际执行的条数

        bool _success; // 是否执行成功

    };
}
