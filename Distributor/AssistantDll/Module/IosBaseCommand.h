#pragma once

#include "IBaseCommand.h"
namespace ND91Assistant
{
    class IosBaseCommand : public IBaseCommand
    {
    public:
        IosBaseCommand(const DeviceData* pDevice);
        ~IosBaseCommand(void);

        virtual void Execute();

        // ����͵�IOS�豸�ϵ��ļ�����ȫ·��, unicode��
        // Ĭ�Ϸ���IOS_COMMOND_NORMAL_FOLDER·���£�����ԭ�����ļ�����CreatDaemonCypher���м��ܴ���
        virtual std::wstring GetIosCommandFilePath();

        // ��IOS�ϻ�ȡ���������ļ�����ȫ·��, unicode��
        // Ĭ����IOS_COMMOND_NORMAL_FOLDER·���£��ļ���Ϊԭ�����ļ���+".done"
        virtual std::wstring GetIosResultFilePath();

        // ����ios�ļ����ݡ�Ĭ��Ϊ��
        virtual std::wstring GetIosCommandFileContent();

        // ����ios�豸�ķ����ļ���ȫ·��, unicode����ɾ���ļ�
        virtual void SetIosReturnFile(std::wstring filename) = 0;

    protected:
        std::wstring GetIosDoneInfoFromFileName();
        
    private:
        bool LookForResultFile( IIosFileSystem* pFileStream, std::wstring filePrefix, wstring& strInfo);

    protected:
        std::wstring _strIosCmdFileName; //IOS�����ļ�������_strIosCmdFileName = L"kill." + CFileOperation::GetGuidW(); ������Ĺ��캯���и�ֵ
    };
}