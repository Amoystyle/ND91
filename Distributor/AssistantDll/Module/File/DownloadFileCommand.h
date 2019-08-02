///////////////////////////////////////////////////////////
//  DownloadFileCommand.h
//  Implementation of the Class DownloadFileCommand
//  Created on:      01-六月-2011 9:42:54
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_8AA4E3E1_0E3A_4be7_AFB0_8C25852EAC04__INCLUDED_)
#define EA_8AA4E3E1_0E3A_4be7_AFB0_8C25852EAC04__INCLUDED_

#include "Module/BaseCommand.h"
#include "Common/BinaryBufferWriter.h"
using namespace std;

namespace ND91Assistant
{
	class AdbDownloadFileCommand;

	// 下载文件命令
    class DownloadFileCommand : public BaseCommand
    {
        friend class FileData;
    public:
        DownloadFileCommand(const DeviceData* pDevice);
        virtual ~DownloadFileCommand();

        bool Download(wstring localFile, wstring remoteFile)
        {
            SetFileNames(localFile, remoteFile);
            Execute();

			TransferResult ret = getResult();
            if( Transfer_Success != ret)
			{
				LOG->WriteDebug(L"DownloadFile Fail name=" + remoteFile);
				return false;
			}
			else
				return true;
        }

        wstring GetFileToPC()
        {
            return _DownResult == Transfer_Success ? _FileToPc : L"";
        }

    private:
		// 设置要下载的文件路径filefromPhone，目标文件路径filetoPc
        void  SetFileNames(const wstring& filetoPc, const wstring& filefromPhone);

		// 获得下载结果
		TransferResult getResult(){return _DownResult;}

		// 执行命令
		virtual void Execute();

        // 若文件是否已存在，是否继续处理
        // 返回true表示继续，返回false表示取消
        bool CheckFileExist();

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

	private:

		// wifi下载
		void  WifiExecute();

		// USB下载
		//void  USBExecute();

	private:
		wstring         _FileToPc;      // pc 端目标文件路径
		wstring         _FilefromPhone; // 要下载的手机端文件路径
		TransferResult  _DownResult;    // 下载结果

		AdbDownloadFileCommand* _pAdbDown;        
    };
}

#endif // !defined(EA_8AA4E3E1_0E3A_4be7_AFB0_8C25852EAC04__INCLUDED_)
