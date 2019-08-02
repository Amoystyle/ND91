///////////////////////////////////////////////////////////
//  ReceiveMessageCommand.h
//  Implementation of the Class ReceiveMessageCommand
//  Created on:      01-����-2011 9:43:02
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_97D6206D_9831_45c5_A5C3_2726AD0C2494__INCLUDED_)
#define EA_97D6206D_9831_45c5_A5C3_2726AD0C2494__INCLUDED_

#include "Module/BaseCommand.h"

namespace ND91Assistant
{

    class ReceiveMessageCommand : public BaseCommand
    {

    public:
        ReceiveMessageCommand(const DeviceData* pDevice);
        virtual ~ReceiveMessageCommand();

        // ִ������
        virtual void Execute() {}

        // ����Android�������ݰ�
        virtual std::string GetAndroidBodyData() { return ""; }

        // ����Android���ؽ�����ݰ�
        virtual void SetAndroidReturnData(std::string data) {}

        // ����͵�IOS�豸�ϵ��ļ�����ȫ·����
        virtual std::wstring GetIosCommandFilePath() { return L""; }

        // ��IOS�ϻ�ȡ���������ļ�����ȫ·����
        virtual std::wstring GetIosResultFilePath() { return L""; }

        // ����ios�ļ��������ļ����������߸���ɾ���ļ�
        virtual std::wstring GetIosCommandFileContent() { return L""; }

        // ���������ļ���������ɾ���ļ�
        virtual  void SetIosReturnFile(std::wstring filename) { }

    private:
        BinaryBufferWriter _BodyWriter;
    };
}

#endif // !defined(EA_97D6206D_9831_45c5_A5C3_2726AD0C2494__INCLUDED_)
