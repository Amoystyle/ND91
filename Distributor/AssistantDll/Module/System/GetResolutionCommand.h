#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    // ��ȡandroid�豸�Ƿ���root
    class GetResolutionCommand : public BaseCommand
    {
    public:
        GetResolutionCommand(const DeviceData* pDevice);
        virtual ~GetResolutionCommand();

        // ִ������
        virtual void Execute();

        // ����Android�������ݰ�
        virtual std::string GetAndroidBodyData();

        // ����Android���ؽ�����ݰ�
        virtual void SetAndroidReturnData(std::string data);

		virtual void SetIosReturnFile(std::wstring filename) {;}


        wstring GetResult(){return _strSize;}

    private:
        
		wstring _strSize;
    };
}