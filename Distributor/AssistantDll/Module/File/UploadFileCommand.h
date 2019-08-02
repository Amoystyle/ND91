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

    // �ϴ��ļ�����
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

        // ָ��Դ�ļ���Ŀ���ļ�
        void setfilename(const wstring& srcFile, const wstring& destFile);

		TransferResult  GetUploadResult();

		// ִ������
		virtual void Execute();

	
		// ���������µ�ַ
		wstring GetDestFilePath();

		//�����Ƿ�����Ȩ�ޣ������ã���Ĭ��Ϊ��Ҫ����Ȩ��
		void SetIsSetAttr(bool bSetAttr){_bSetAttr = bSetAttr;}

		void SetParam(int64_t OffsetAppInExe =0,int64_t len=0);

    private:

        // ����Android�������ݰ�
        virtual std::string GetAndroidBodyData();

        // ����Android���ؽ�����ݰ�
        virtual void SetAndroidReturnData(std::string data);

		bool parseAndroidPackageHead( BinaryBufferReader* pReader);

        // ����͵�IOS�豸�ϵ��ļ�����ȫ·����
        virtual std::wstring GetIosCommandFilePath() { return L""; }

        // ��IOS�ϻ�ȡ���������ļ�����ȫ·����
        virtual std::wstring GetIosResultFilePath() { return L""; }

        // ����ios�ļ��������ļ����������߸���ɾ���ļ�
        virtual std::wstring GetIosCommandFileContent() { return L""; }

        // ���������ļ���������ɾ���ļ�
        virtual  void SetIosReturnFile(std::wstring filename) { }

		std::string GetWifiHeadData();

		// android wifi���ݷ���������
		string WifiExecute();

        bool CheckExistFile();

        IIosFileSystem* GetIosFile();

        DirectoryOperationCommand* GetDirCmd();

        /*SetAttributeCommand* GetSetAttrCmd();*/

        AdbUploadFileCommand* GetUploadCmd();

	private:
		
		TransferResult   _uploadSuccess;  //���ͽ��

		wstring _fileOnPc;      //���ϴ���PC���ļ�
		wstring _fileonphone;   //�ֻ���Ŀ���ļ�
        int     _fileSize;      //�ļ���С
		unsigned __int64 _OffsetAppInExe;

		AdbUploadFileCommand*       _pAdbUpload;
        DirectoryOperationCommand*  _pDirCmd;
        //SetAttributeCommand*        _pSetAttrCmd;
        IIosFileSystem*             _pIosFile;

		bool _bSetAttr; //�Ƿ�Ҫ����Ȩ�ޣ�Ĭ��ΪTRUE
    };
}
