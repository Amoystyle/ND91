#pragma once

#include "Core/DeviceInfo.h"

namespace ND91Assistant
{
    // �����豸�Ľӿ�
    class IConnection
    {
    public:
        // �������������������
        virtual ~IConnection() {}

        // ����׼��
        virtual bool PreActivate(){return false;};

        // ����
        virtual bool Activate(){return false;};

        // ����
        virtual bool Connect() = 0;

        // �Ͽ�
        virtual void Disconnect() = 0;

        // �������ӳɹ����õ�Ӳ����Ϣ
        virtual const DeviceInfo* GetDeviceInfo() = 0;
        
        // ��ǰIOS�豸�Ƿ��ֹ����װ,Ҳ�������豸���������濪�������Ƴ���װ.wifi���ӵ��Ƿ�����жϲ�?
        virtual bool IsProhibitAppInstall() { return false; };

		//��ʼ���豸����
		virtual void InitDeviceData(){};


        void SetConfirmConfig(int nConfig) {_nConfirmConfig = nConfig;};
    protected:
        int _nConfirmConfig;    // ����ȷ������
    };
}
