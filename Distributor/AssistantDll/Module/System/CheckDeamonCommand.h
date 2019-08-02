///////////////////////////////////////////////////////////
//  CheckDeamonCommand.h
//  Implementation of the Class CheckDeamonCommand
//  Created on:      01-����-2011 9:42:51
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_2C5E8742_3520_4e6c_9E51_84FF83E6B33B__INCLUDED_)
#define EA_2C5E8742_3520_4e6c_9E51_84FF83E6B33B__INCLUDED_

#include "Module/BaseCommand.h"
namespace ND91Assistant
{
    // ����ػ�����
    class CheckDeamonCommand : public BaseCommand
    {

    public:
        CheckDeamonCommand(const DeviceData* pDevice);
        virtual ~CheckDeamonCommand();

        // ִ������
        virtual void Execute();

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
        //�ػ��İ汾��
        std::string _strVersion;
        BinaryBufferWriter _BodyWriter;

    public:
        std::string getVersion()
        {
            return _strVersion;
        };

    };
}

#endif // !defined(EA_2C5E8742_3520_4e6c_9E51_84FF83E6B33B__INCLUDED_)
