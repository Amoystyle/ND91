#pragma once

#include "IBaseCommand.h"
namespace ND91Assistant
{
    class IosBaseCommand : public IBaseCommand
    {
    public:
        IosBaseCommand(const DeviceData* pDevice);
        ~IosBaseCommand(void);

        virtual void Execute();

        // 命令传送到IOS设备上的文件名（全路径, unicode）
        // 默认放在IOS_COMMOND_NORMAL_FOLDER路径下，并对原命令文件名用CreatDaemonCypher进行加密处理
        virtual std::wstring GetIosCommandFilePath();

        // 从IOS上获取的命令结果文件名（全路径, unicode）
        // 默认在IOS_COMMOND_NORMAL_FOLDER路径下，文件名为原命令文件名+".done"
        virtual std::wstring GetIosResultFilePath();

        // 生成ios文件内容。默认为空
        virtual std::wstring GetIosCommandFileContent();

        // 解析ios设备的返回文件（全路径, unicode）并删除文件
        virtual void SetIosReturnFile(std::wstring filename) = 0;

    protected:
        std::wstring GetIosDoneInfoFromFileName();
        
    private:
        bool LookForResultFile( IIosFileSystem* pFileStream, std::wstring filePrefix, wstring& strInfo);

    protected:
        std::wstring _strIosCmdFileName; //IOS命令文件名，如_strIosCmdFileName = L"kill." + CFileOperation::GetGuidW(); 在子类的构造函数中赋值
    };
}