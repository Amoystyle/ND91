#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    class AppData;

    // ��ȡandroid�豸�Ƿ���root
    class UninstallSystemAppCommand : public BaseCommand
    {
    public:
        UninstallSystemAppCommand(const DeviceData* pDevice, const AppData* appData);
        virtual ~UninstallSystemAppCommand();

        // ִ������
        virtual void Execute();

        // ����Android�������ݰ�
        virtual std::string GetAndroidBodyData();

        // ����Android���ؽ�����ݰ�
        virtual void SetAndroidReturnData(std::string data);

		virtual void SetIosReturnFile(std::wstring filename) {;}


        bool GetResult(){return _bSuccess;}

    private:
        
		bool _bSuccess;

		const AppData* _appData;
    };
}