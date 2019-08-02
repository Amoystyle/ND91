#pragma once

#include "Module/IManager.h"

namespace ND91Assistant
{
    class CLASSINDLL_CLASS_DECL SystemManager : public IManager
    {
    public:
        SystemManager (const DeviceData* pDevice);
        virtual ~SystemManager();

        // ��ȡIMEI��Ϣ
        wstring GetIMEI();

        // ��ȡIMSI��Ϣ
        wstring GetIMSI();

        // ��ȡ��ǰ�豸��ص���
        int  GetBatteryLever();
    
        // ��ȡSDCard״̬
        SDCardState GetSDCardState();

        // ��ȡ�豸����
        wstring GetDeviceName();

        // �����豸����
        bool SetDevName(wstring strNewName);

		// ��ȡandroid�豸�Ƿ���root
		bool GetIsRoot();

		// �޸�android�豸system/appĿ¼Ϊ��д
		bool GetIsRemount();

        // ע���ֻ�
        bool LogoffDev();

        // �����豸
        bool RebootDev();

        // �ر��豸
        bool ShoutdownDev();
	

		// ��ȡ�豸�ֱ���
		wstring GetDeviceResolution();
#ifndef ASSISTANT_LITE
		//����ָ�ģʽ
		bool EnterRecoveryMode();
#endif
		//�˳��ָ�ģʽ
		bool RebootToNormal();

#ifndef ASSISTANT_LITE
        // �������ݿ��޸�
        bool FixMessageDB();
#endif
#ifdef PLATFORM_IOS
        // ��ƻ���޸�����ios�豸��
        void RepairIdleDevice();

        //��ȡ�豸����
        wstring GetIosDeviceType();
#endif
#ifdef PLATFORM_ANDROID
#endif
        

    };
}