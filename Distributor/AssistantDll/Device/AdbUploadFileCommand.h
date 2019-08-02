#pragma once

#include "AdbRemoteCommand.h"
#include "Monitor/ProgressSubject.h"
#include "Module/AndroidDefine.h"

namespace ND91Assistant
{
    class AdbUploadFileCommand : public AdbRemoteCommand
    {
    public:
        AdbUploadFileCommand(wstring sn,wstring id, wstring strPCPath, wstring strPhonePath, ProgressSubject& subject = NullProgressSubject::Instance());
        ~AdbUploadFileCommand();

		void SetParam(wstring strPCPath, wstring strPhonePath,unsigned __int64 OffsetAppInExe =0,unsigned __int64 len=0);

		bool Success(){return _success==Transfer_Success;}

		TransferResult upLoadResult(){return _success;}

    protected:

		virtual void InternalExecute();

       // 解析返回数据
        // 返回值：true 表示成功，false 表示失败
        virtual bool ParseReturnData();

		void CreateSyncReq();

		// 创建【发送请求】数据包
		std::string CreateSendFileReqPkg();

		// 创建【文件数据】数据包
		std::string CreateSendDataPkg(const char* pbuffer, size_t buffsize);

		// 创建【发送完毕】数据包
		std::string CreateSendDonePkg();

    private:
		wstring _fileOnPc;//待上传的PC端文件
		wstring _fileonphone;//手机端目标文件

		int    _fileSize;//文件大小

		TransferResult   _success;

		ProgressSubject* _pSubject;

		unsigned __int64 _OffsetAppInExe;
    };
}