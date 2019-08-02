#pragma once

#include "Module/BaseCommand.h"
#include "Module/File/DirData.h"
#include "Module/File/FileData.h"
#include "Common/BinaryBufferWriter.h"
#include "Common/greta/RegexprHelper.h"

#include <algorithm>
#include <deque>
#include <string>

using namespace std;
 
namespace ND91Assistant
{
	class AdbDirOperateCommand;

	// 目录操作
    class DirectoryOperationCommand : public BaseCommand
    {

    public:
        DirectoryOperationCommand(const DeviceData* pDevice);
        virtual ~DirectoryOperationCommand();

		bool Exist(wstring dir);

		bool MakeDir(wstring dir);

		bool DeleteDir(wstring dir);

        bool DeleteFileLink(wstring file);

		bool Rename(wstring source, wstring dest);

		// Summary  : 修改权限和组，若想仅修改owner，则将mode设空
		// Returns	: bool 
		// Parameter: wstring dir 路径
		// Parameter: string mode 权限（777格式)，
		// Parameter: wstring owner 组，若仅修改owner，为空则给默认值501
		bool ChangeMode(wstring dir, string mode="", wstring owner=L"");

        // Summary  : 获取权限和组
        bool GetMode(wstring dir, string& mode, wstring& owner);

		bool Move(wstring sourceDir, wstring destDir);

		// 逐级创建目录
		bool AndroidCreateDir(const wstring & path);
		
		// Ios获取文件大小
		int IosGetFileSize(wstring file);

		bool IsSupportColor();

		
		bool IsMountSystem();

	private:

        // 执行命令
        virtual void Execute();

		bool DoneSuccess(){return _doneSuccess;}

		enum COMMAND_TYPE
		{
            EDirExist,			    // 路径是否存在
			ECreateDir,             // 创建目录
			EDeleteDir,             // 删除空目录
			EDeleteFile,            // 删除手机端文件
			EMoveRemotefile,        // 移动手机端文件
			ERenameRemotefile,      // 重命名手机端文件
			EChmod,                 // 设置权限
			ESupportColor,          // 是否支持颜色
			EGetFileSize,			// 获取文件大小
		};

		// 设置命令参数，aCommandType = 命令类型，
		// source=源路径， dest=目标路径，移动和重命名需传
        // 对chmod命令，必须先设置mode
		void SetParams(COMMAND_TYPE aCommandType, const wstring source, const wstring dest = L"");

        // 保存一个权限值（设置权限命令需要）
        void SetMode(const string& mode);

        // 生成Android命令数据包
        virtual std::string GetAndroidBodyData();

        // 解析Android返回结果数据包
        virtual void SetAndroidReturnData(std::string data);

        // 生成ios文件。返回文件名，调用者负责删除文件
        virtual std::wstring GetIosCommandFileContent();

        // 解析返回文件，解析后删除文件
        virtual  void SetIosReturnFile(std::wstring filename);

		//返回值为false 则认为操作失败了  receivedData = 接收到的整块结果数据， shellresult=shell命令返回的纯结果
		bool ExtractAndroidResult(const string& receivedData, string& shellresult);

        // ls 命令是否需要增加颜色选项
        bool HasColor() { return false; }

		void ParseResult(const string result);

        // 转换权限格式（例如："755" 转换为 "rwxr-xr-x"）
        wstring FormatMode(string mode);

	private:

		COMMAND_TYPE _commandType;		//命令类型


		wstring  _source;				// 被操作的手机端路径
		wstring  _dest;					// 手机端目标路径，移动和重命名需传
        string  _mode;					// 权限（777格式）
		wstring _strOwner;              // 
		string  _androidCommand;		// shell命令内容

		bool  _doneSuccess;				// 操作结果
		int   _fileSize;				// 获取文件大小	

		bool  _createDirbyShellFail;

		AdbDirOperateCommand* _pAdbDirOp;
		
        RegexprHelper _rh;
    };
}
