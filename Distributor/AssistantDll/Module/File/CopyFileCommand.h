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

        // ִ������
        virtual void Execute();

        void setFileNames(const wstring remoteFromPath, const wstring remoteToPath);
    private:

        // ����Android�������ݰ�
        virtual std::string GetAndroidBodyData();

        // ����Android���ؽ�����ݰ�
        virtual void SetAndroidReturnData(std::string data);

        // ����͵�IOS�豸�ϵ��ļ�����ȫ·����
        virtual std::wstring GetIosCommandFilePath() { return L""; }

        // ��IOS�ϻ�ȡ���������ļ�����ȫ·����
        virtual std::wstring GetIosResultFilePath() { return L""; }

        // ����ios�ļ��������ļ����������߸���ɾ���ļ�
        virtual std::wstring GetIosCommandFileContent() { return L""; }

        // ���������ļ���������ɾ���ļ�
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
