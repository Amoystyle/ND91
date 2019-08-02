#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    // ����������ȷ�ϣ�����ǰ���ж��Ƿ���Ҫ����ȷ��
    // ���ڲ�ʹ�����ӳ��Ҹ���ͨ������ϴ�
    // �����������̳�BaseCommand !!!
    class CheckNeedConnectConfirmCommand
    {
    public:
        static bool IsNeedConnectConfirm(wstring strHost, int nPort);

        // ����Android������
        // ���� port : �����ӵı��ض˿�
        static std::string GetAndroidDatagram();

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