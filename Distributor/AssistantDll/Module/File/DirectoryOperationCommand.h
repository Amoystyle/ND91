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

	// Ŀ¼����
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

		// Summary  : �޸�Ȩ�޺��飬������޸�owner����mode���
		// Returns	: bool 
		// Parameter: wstring dir ·��
		// Parameter: string mode Ȩ�ޣ�777��ʽ)��
		// Parameter: wstring owner �飬�����޸�owner��Ϊ�����Ĭ��ֵ501
		bool ChangeMode(wstring dir, string mode="", wstring owner=L"");

        // Summary  : ��ȡȨ�޺���
        bool GetMode(wstring dir, string& mode, wstring& owner);

		bool Move(wstring sourceDir, wstring destDir);

		// �𼶴���Ŀ¼
		bool AndroidCreateDir(const wstring & path);
		
		// Ios��ȡ�ļ���С
		int IosGetFileSize(wstring file);

		bool IsSupportColor();

		
		bool IsMountSystem();

	private:

        // ִ������
        virtual void Execute();

		bool DoneSuccess(){return _doneSuccess;}

		enum COMMAND_TYPE
		{
            EDirExist,			    // ·���Ƿ����
			ECreateDir,             // ����Ŀ¼
			EDeleteDir,             // ɾ����Ŀ¼
			EDeleteFile,            // ɾ���ֻ����ļ�
			EMoveRemotefile,        // �ƶ��ֻ����ļ�
			ERenameRemotefile,      // �������ֻ����ļ�
			EChmod,                 // ����Ȩ��
			ESupportColor,          // �Ƿ�֧����ɫ
			EGetFileSize,			// ��ȡ�ļ���С
		};

		// �������������aCommandType = �������ͣ�
		// source=Դ·���� dest=Ŀ��·�����ƶ����������贫
        // ��chmod�������������mode
		void SetParams(COMMAND_TYPE aCommandType, const wstring source, const wstring dest = L"");

        // ����һ��Ȩ��ֵ������Ȩ��������Ҫ��
        void SetMode(const string& mode);

        // ����Android�������ݰ�
        virtual std::string GetAndroidBodyData();

        // ����Android���ؽ�����ݰ�
        virtual void SetAndroidReturnData(std::string data);

        // ����ios�ļ��������ļ����������߸���ɾ���ļ�
        virtual std::wstring GetIosCommandFileContent();

        // ���������ļ���������ɾ���ļ�
        virtual  void SetIosReturnFile(std::wstring filename);

		//����ֵΪfalse ����Ϊ����ʧ����  receivedData = ���յ������������ݣ� shellresult=shell����صĴ����
		bool ExtractAndroidResult(const string& receivedData, string& shellresult);

        // ls �����Ƿ���Ҫ������ɫѡ��
        bool HasColor() { return false; }

		void ParseResult(const string result);

        // ת��Ȩ�޸�ʽ�����磺"755" ת��Ϊ "rwxr-xr-x"��
        wstring FormatMode(string mode);

	private:

		COMMAND_TYPE _commandType;		//��������


		wstring  _source;				// ���������ֻ���·��
		wstring  _dest;					// �ֻ���Ŀ��·�����ƶ����������贫
        string  _mode;					// Ȩ�ޣ�777��ʽ��
		wstring _strOwner;              // 
		string  _androidCommand;		// shell��������

		bool  _doneSuccess;				// �������
		int   _fileSize;				// ��ȡ�ļ���С	

		bool  _createDirbyShellFail;

		AdbDirOperateCommand* _pAdbDirOp;
		
        RegexprHelper _rh;
    };
}
