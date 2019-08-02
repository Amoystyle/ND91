#pragma  once

#include "Module/BaseCommand.h"
#include <vector>
using namespace std;

namespace ND91Assistant
{
    class ContactData;

    // 删除联系人命令
    class DeleteContactCommand : public BaseCommand
    {
    public:
        DeleteContactCommand(const DeviceData* pDevice);
        virtual ~DeleteContactCommand();

        // 执行命令
        virtual void Execute();

        // 装入待操作的数据集合
        virtual void SetDataList(const vector<ContactData*>* pDataList);

        // 命令是否执行成功
        bool Success();

    private:

        // 执行IOS命令
        bool IosExecute();

        // 获取RowId集合
        void GetRowIdVector(vector<int> *vecRowid);

        // 重设未执行数据的状态
        void ResetDataStatus();

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

        const vector<ContactData*>* _pDataList; // 待删除的联系人数据集合

        int _executeCount;// 实际执行的条数

        bool _success; // 是否执行成功
    };
}