///////////////////////////////////////////////////////////
//  DownloadFileCommand.h
//  Implementation of the Class DownloadFileCommand
//  Created on:      01-����-2011 9:42:54
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

	// �����ļ�����
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
		// ����Ҫ���ص��ļ�·��filefromPhone��Ŀ���ļ�·��filetoPc
        void  SetFileNames(const wstring& filetoPc, const wstring& filefromPhone);

		// ������ؽ��
		TransferResult getResult(){return _DownResult;}

		// ִ������
		virtual void Execute();

        // ���ļ��Ƿ��Ѵ��ڣ��Ƿ��������
        // ����true��ʾ����������false��ʾȡ��
        bool CheckFileExist();

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

	private:

		// wifi����
		void  WifiExecute();

		// USB����
		//void  USBExecute();

	private:
		wstring         _FileToPc;      // pc ��Ŀ���ļ�·��
		wstring         _FilefromPhone; // Ҫ���ص��ֻ����ļ�·��
		TransferResult  _DownResult;    // ���ؽ��

		AdbDownloadFileCommand* _pAdbDown;        
    };
}

#endif // !defined(EA_8AA4E3E1_0E3A_4be7_AFB0_8C25852EAC04__INCLUDED_)
