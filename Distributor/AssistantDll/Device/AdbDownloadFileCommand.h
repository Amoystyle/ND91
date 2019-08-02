#pragma once

#include "AdbRemoteCommand.h"
#include "Monitor/ProgressSubject.h"
#include "Module/AndroidDefine.h"

using namespace std;
namespace ND91Assistant
{
    //����ǰ��ȡ���ֻ����ļ�����
    struct  TFileState
    {
        int _mReadMode;//�ɶ�ģʽ
        int _mSize;    //�ļ���С
        int _mModified;//���޸�
    };

    class AdbDownloadFileCommand : public AdbRemoteCommand
    {
    public:
        AdbDownloadFileCommand(wstring sn,wstring id, ProgressSubject* pSubject = NULL);

        void SetParam(wstring strPCPath, wstring strPhonePath);

        virtual ~AdbDownloadFileCommand(void);

        // ִ������
        // ����ʵ��ʱ���������⴦��ֻ�����CommonExecute����

        TransferResult GetDownloadResult(){return _success;}

    protected:

        // ������������
        // ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
        virtual bool ParseReturnData();

    private:
        virtual void InternalExecute();

        // ���ɻ�ȡԶ���ļ������������ݰ�
        string CreateFileStateReqPkg(const string& afilepath);

        // ���ɽ���Զ���ļ����������
        string CreateReceiveRegPkg();

        // ����Զ���ļ�����
        TFileState ParseFileStateResult(const string& result);


        wstring _FileToPc;     // PC���ļ�
        wstring _FilefromPhone;// �ֻ����ļ�

        TransferResult   _success;

        ProgressSubject* _pSubject;

        NullProgressSubject _nullSubject;
    };
}
