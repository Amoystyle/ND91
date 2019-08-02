#pragma once
#include "Module/BaseCommand.h"
#include "Common/BinaryBufferWriter.h"

#include "Common/greta/RegexprHelper.h"

namespace ND91Assistant
{
	class AdbDirOperateCommand;
    class DirectoryOperationCommand;

    class CopyFileCommand :public BaseCommand
    {
    public:
        CopyFileCommand(const DeviceData* pDevice);
        ~CopyFileCommand(void);

        CopyInfo  GetCopyInfo(){return _retCopyInfo;}

        bool     IsCopyDoneWell(){return _doneSuccess;}

        // 执行命令
        virtual void Execute();

        void setFileNames(const wstring remoteFromPath, const wstring remoteToPath);
    private:

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

        bool CheckFileExist(const wstring& aRemotefile);
        void ParseResult(const string& result);
		
		bool CheckExistFile();
		DirectoryOperationCommand* GetDirCmd();

    private:

        wstring    _source;
        wstring    _dest;

        CopyInfo  _retCopyInfo;
        bool      _doneSuccess;

		DirectoryOperationCommand*  _pDirCmd;

		AdbDirOperateCommand*  _adbCmd;

        RegexprHelper _rh;
    };

}
