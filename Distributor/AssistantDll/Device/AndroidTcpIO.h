#pragma once
#include "BaseIO.h"
#include "ConnectionPool.h"
namespace ND91Assistant
{
    // ��׿ TCP ��ʽ�շ�
    class AndroidTcpIO : public BaseIO
    {
    public:
        AndroidTcpIO(int nConnectionID, ConnectionPool* pConnectionPool );

        ~AndroidTcpIO();

        // summary  : ���Ͱ�׿������ؽ��
        // return   : std::string         : ������ݰ��������ƣ�
        // parameter: std::string data    : �������ݰ�
        // parameter: bool bReceiveResult : true=���շ���ֵ false=�����շ���ֵ
        virtual std::string SendAndroidDatagram(std::string data, bool bReceiveResult = true);

        // ios�õķ�������ʵ��
		virtual std::wstring SendIosFile(std::wstring strCmdFileOnPhone, std::wstring strResultFileOnPhone, std::wstring strCmdFileContent, bool bReceiveResult = true);

        // ��������
        // ���� size : ָ�����յ��ֽ�����0�����ȫ��
        virtual std::string Receive(unsigned int size = 0);

        // ��������Ƿ���Ч�������Ч��Ӧ��ֹ�������Բ���
        bool IsConnectionAvailable();

    private:
        int _nConnectionID;
        ConnectionPool* _pConnectionPool;
    };
}
