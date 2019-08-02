#pragma  once

#include "Module/BaseCommand.h"
#include <vector>

using namespace std;

namespace ND91Assistant
{
    class ContactData;

    // 设置联系人收藏属性命令
    class SetFavoriteCommand : public BaseCommand
    {
    public:
        SetFavoriteCommand(const DeviceData* pDevice, bool bFavorite);
        virtual ~SetFavoriteCommand();

        // 执行命令
        virtual void Execute() {}

        // 装入待操作的数据集合
        virtual void SetDataList(const vector<ContactData*>* pDataList);

        // 命令是否执行成功
        bool Success();

        // 执行命令
        int ExecuteSetFavorite();

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

        const vector<const ContactData*>* _pDataList; // 待设置收藏属性的联系人集合

        int _executeCount;// 实际执行的条数

        bool _bFavorite; // 是否为收藏

        bool _success; // 是否执行成功

    };
}
