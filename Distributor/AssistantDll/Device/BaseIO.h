#pragma  once

#include "Core/DeviceInfo.h"

#include <string>
namespace ND91Assistant
{

    class CBaseSocket;

    // IO����
    class BaseIO
    {
    public:
        BaseIO();
        virtual ~BaseIO();

        // summary  : ���Ͱ�׿������ؽ��
        // return   : std::string         : �������ݰ�
        // parameter: std::string data    : ������ݰ��������ƣ�
        // parameter: bool bReceiveResult : true=���շ���ֵ false=�����շ���ֵ
        virtual std::string SendAndroidDatagram(std::string data, bool bReceiveResult = true) = 0;

        // summary  : ����ios������ؽ��
        // return   : std::wstring						: ����ļ���(unicode)
        // parameter: std::wstring strCmdFileOnPhone	: �ϴ����豸�ϵ������ļ���(ȫ·����unicode)
		// parameter: std::wstring strResultFileOnPhone	: ������PC�ϵĽ���ļ���(ȫ·����unicode)
		// parameter: std::wstring strCmdFileContent	: �����ļ�����(unicode)
        // parameter: bool bReceiveResult				: true=���շ���ֵ false=�����շ���ֵ
		virtual std::wstring SendIosFile(std::wstring strCmdFileOnPhone, std::wstring strResultFileOnPhone, std::wstring strCmdFileContent, bool bReceiveResult = true) = 0;

    protected:
    };
}

