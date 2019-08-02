#pragma once

#include "Module/BaseCommand.h"
#include "Module/File/DirectoryOperationCommand.h"
#include "Common/BinaryBufferWriter.h"
#include "Device/IIosFileSystem.h"
#include "Device/IosWiFiFileSystem.h"
#include "Device/AFCFileSysemBase.h"
#include <fstream>
#include <deque>
#include <vector>

using namespace std;

namespace ND91Assistant
{
	class AdbUploadFileCommand;
    class SetAttributeCommand;

    // 上传文件命令
    class UploadFileCommand : public BaseCommand
    {
    public:

        UploadFileCommand(const DeviceData* pDevice);
        virtual ~UploadFileCommand();

        bool Upload(const wstring& srcFile, const wstring& destFile)
        {
            setfilename(srcFile, destFile);
            Execute();
            return Transfer_Success == GetUploadResult();
        }

        // 指定源文件和目标文件
        void setfilename(const wstring& srcFile, const wstring& destFile);

		TransferResult  GetUploadResult();

		// 执行命令
		virtual void Execute();

	
		// 重命名后新地址
		wstring GetDestFilePath();

		//设置是否设置权限，不调用，则默认为需要设置权限
		void SetIsSetAttr(bool bSetAttr){_bSetAttr = bSetAttr;}

		void SetParam(int64_t OffsetAppInExe =0,int64_t len=0);

    private:

        // 生成Android命令数据包
        virtual std::string GetAndroidBodyData();

        // 解析Android返回结果数据包
        virtual void SetAndroidReturnData(std::string data);

		bool parseAndroidPackageHead( BinaryBufferReader* pReader);

        // 命令传送到IOS设备上的文件名（全路径）
        virtual std::wstring GetIosCommandFilePath() { return L""; }

        // 从IOS上获取的命令结果文件名（全路径）
        virtual std::wstring GetIosResultFilePath() { return L""; }

        // 生成ios文件。返回文件名，调用者负责删除文件
        virtual std::wstring GetIosCommandFileContent() { return L""; }

        // 解析返回文件，解析后删除文件
        virtual  void SetIosReturnFile(std::wstring filename) { }

		std::string GetWifiHeadData();

		// android wifi数据发送整过程
		string WifiExecute();

        bool CheckExistFile();

        IIosFileSystem* GetIosFile();

        DirectoryOperationCommand* GetDirCmd();

        /*SetAttributeCommand* GetSetAttrCmd();*/

        AdbUploadFileCommand* GetUploadCmd();

	private:
		
		TransferResult   _uploadSuccess;  //传送结果

		wstring _fileOnPc;      //待上传的PC端文件
		wstring _fileonphone;   //手机端目标文件
        int     _fileSize;      //文件大小
		unsigned __int64 _OffsetAppInExe;

		AdbUploadFileCommand*       _pAdbUpload;
        DirectoryOperationCommand*  _pDirCmd;
        //SetAttributeCommand*        _pSetAttrCmd;
        IIosFileSystem*             _pIosFile;

		bool _bSetAttr; //是否要设置权限，默认为TRUE
    };
}
