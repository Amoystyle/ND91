#pragma once

#include "Module/BaseCommand.h"
#include "Common/BinaryBufferWriter.h"

namespace ND91Assistant
{
    //���ɺʹ���������
	class SystemCommand
	{
	public:

		// ����Android������
        // ���� port : �����ӵı��ض˿�
        static std::string GetAndroidDatagram(short port);

        void setLongConnectSign();

		// �����������ݰ�
        void ParseturnDatagram(std::string data);

		// ����ios�ļ��������ļ����������߸���ɾ���ļ�
        std::string GetIosCommandFile() { return ""; }

		// ���������ļ���������ɾ���ļ�
        void SetIosReturnFile(std::string filename) {}

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