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

       // ������������
        // ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
        virtual bool ParseReturnData();

		void CreateSyncReq();

		// �����������������ݰ�
		std::string CreateSendFileReqPkg();

		// �������ļ����ݡ����ݰ�
		std::string CreateSendDataPkg(const char* pbuffer, size_t buffsize);

		// ������������ϡ����ݰ�
		std::string CreateSendDonePkg();

    private:
		wstring _fileOnPc;//���ϴ���PC���ļ�
		wstring _fileonphone;//�ֻ���Ŀ���ļ�

		int    _fileSize;//�ļ���С

		TransferResult   _success;

		ProgressSubject* _pSubject;

		unsigned __int64 _OffsetAppInExe;
    };
}