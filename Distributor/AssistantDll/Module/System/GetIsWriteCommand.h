#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    // ��ȡandroid�豸system/app Ŀ¼�Ƿ��д
    class GetIsWriteCommand : public BaseCommand
    {
    public:
        GetIsWriteCommand(const DeviceData* pDevice);
        virtual ~GetIsWriteCommand();

        // ִ������
        virtual void Execute();

        // ����Android�������ݰ�
        virtual std::string GetAndroidBodyData();

        // ����Android���ؽ�����ݰ�
        virtual void SetAndroidReturnData(std::string data);

		virtual void SetIosReturnFile(std::wstring filename) {;}


        bool GetResult(){return _bWrite;}

    private:
        
		bool _bWrite;
    };
}