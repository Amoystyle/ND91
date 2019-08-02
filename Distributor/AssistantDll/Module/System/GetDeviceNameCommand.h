#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    // ��ȡandroid�豸����
    class GetDeviceNameCommand : public BaseCommand
    {
    public:
        GetDeviceNameCommand(const DeviceData* pDevice);
        virtual ~GetDeviceNameCommand();

        // ִ������
        virtual void Execute();

        // ����Android�������ݰ�
        virtual std::string GetAndroidBodyData();

        // ����Android���ؽ�����ݰ�
        virtual void SetAndroidReturnData(std::string data);

		virtual void SetIosReturnFile(std::wstring filename) {;}

        wstring GetDeviceName(){ return _strDeviceName; }

    private:
        
		wstring _strDeviceName;
    };
}