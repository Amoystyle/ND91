#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    // ��ȡandroid�豸 �޸�system/appȨ���Ƿ�ɹ� 
    class GetIsRemountCommand : public BaseCommand
    {
    public:
        GetIsRemountCommand(const DeviceData* pDevice);
        virtual ~GetIsRemountCommand();

        // ִ������
        virtual void Execute();

        // ����Android�������ݰ�
        virtual std::string GetAndroidBodyData();

        // ����Android���ؽ�����ݰ�
        virtual void SetAndroidReturnData(std::string data);

		virtual void SetIosReturnFile(std::wstring filename) {;}


        bool GetResult(){return _bRemount;}

    private:
        
		bool _bRemount;
    };
}