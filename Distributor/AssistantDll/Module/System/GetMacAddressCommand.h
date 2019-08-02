#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    // ��ȡMAC��ַ
    class GetMacAddressCommand : public BaseCommand
    {
    public:
        GetMacAddressCommand(const DeviceData* pDevice);
        virtual ~GetMacAddressCommand();

        // ִ������
        virtual void Execute();

        // ����Android�������ݰ�
        virtual std::string GetAndroidBodyData();

        // ����Android���ؽ�����ݰ�
        virtual void SetAndroidReturnData(std::string data);

		virtual void SetIosReturnFile(std::wstring filename) {;}

        wstring GetMacAddress(){ return _macAddress; }

    private:
        
		wstring _macAddress;
    };
}