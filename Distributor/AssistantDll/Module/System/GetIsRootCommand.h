#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    // ��ȡandroid�豸�Ƿ���root
    class GetIsRootCommand : public BaseCommand
    {
    public:
        GetIsRootCommand(const DeviceData* pDevice);
        virtual ~GetIsRootCommand();

        // ִ������
        virtual void Execute();

        // ����Android�������ݰ�
        virtual std::string GetAndroidBodyData();

        // ����Android���ؽ�����ݰ�
        virtual void SetAndroidReturnData(std::string data);

		virtual void SetIosReturnFile(std::wstring filename) {;}


        bool GetResult(){return _isRoot;}

    private:
        
		bool _isRoot;
    };
}