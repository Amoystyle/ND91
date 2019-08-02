#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    // ��������(�����߳�)
    // ���ڲ�ʹ�����ӳ��Ҹ���ͨ������ϴ�
    // �����������̳�BaseCommand !!!
    class CreateLongConnectionCommand
    {
    public:
        // ����Android������
        // ���� port : �����ӵı��ض˿�
        static std::string GetAndroidDatagram(short port, wstring pcName=L"");

        void setLongConnectSign();

        // �����������ݰ�
        void ParseturnDatagram(std::string data);

        // ������ݰ�ͷ�ĸ�ʽ
        // ����true��ʾ��ȷ������false��ʾ����
        bool VerifyPackageHead();

        // ����������תΪ�ַ������
        std::wstring ToString();

        int GetDataLength() { return _nDataLength; }

        int GetCommandId() { return _nCommandID; }
    private:
        char _pHead[5];      // ��ͷ�ַ���
        int _nFlag;          // ���ݰ���־
        int _nDataLength;    // ���ݰ�����
        int _nCommandID;     // ��������
    };
}