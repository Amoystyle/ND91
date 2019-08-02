#pragma once

#include "AssistantDll.h"

#include <map>
#include <string>

using namespace std;

namespace ND91Assistant
{
	// �ַ���ID��ע�����Ƕ�Ӧ�����ַ���������
	// ������Ҫ���ݵ�ǰ���֣��������ú������ַ���������
	enum STRING_ID
	{
		PHONE_LOCK_RING_NAME,       // Ĭ��ֵ "Phone lock"
		PHONE_UNLOCK_RING_NAME,     // Ĭ��ֵ "Phone unlock"
		CHARGING_RING_NAME,         // Ĭ��ֵ "Charging"
		LOW_ENERGY_RING_NAME,       // Ĭ��ֵ "�͵���"
		CALENDER_ALARM_RING_NAME,   // "��������"
		KEY_PRESS_RING_NAME,        // "���̰�����"
		TAKE_PHOTO_RING_NAME,       // "���������"
		NEW_MAIL_RING_NAME,         // "���ʼ�"
		SEND_MAIL_RING_NAME,        // "�ʼ�����"
		ALTERNATE_1,				// "��ȫ��"
		ALTERNATE_2,				// "������"
		ALTERNATE_3,				// "����"				
		ALTERNATE_4,				// "Բ��"			
		ALTERNATE_5,				// "����"		
		ALTERNATE_6,				// "������"	
		DIAL_0_RING_NAME,		    // 0 ������
		DIAL_1_RING_NAME,		    // 1 ������
		DIAL_2_RING_NAME,		    // 2 ������
		DIAL_3_RING_NAME,		    // 3 ������
		DIAL_4_RING_NAME,		    // 4 ������
		DIAL_5_RING_NAME,		    // 5 ������
		DIAL_6_RING_NAME,		    // 6 ������
		DIAL_7_RING_NAME,		    // 7 ������
		DIAL_8_RING_NAME,		    // 8 ������
		DIAL_9_RING_NAME,		    // 9 ������
		DIAL_POUND_RING_NAME,       // #�� ������
		DIAL_STAR_RING_NAME,		// *�� ������
		VOICEMAIL_RING_NAME,		// �������� ������
        DEFAULT_ACCOUNT_NAME,       // Ĭ���ʻ�
        PICTURE_CATEGORY_PHOTO,     // ��Ƭ
        PICTURE_CATEGORY_WALLPAPER, // ��ֽ
        PICTURE_CATEGORY_SCREENSHOT,// ��ͼ
		DEFAULT_RING_TITTLE,        // ��׿ϵͳĬ����������
		IOS_DEFAULT_RING_TITTLE,	// iosĬ��������������

        // IOSϵͳ��������
        system_Null,
		system_Marimba,
        system_Boing,
        system_Trill,
        system_Pinball,
        system_PianoRiff,
        system_Bark,
        system_Robot,
        system_Strum,
        system_Ascending,
        system_Alarm,
        system_SciFi,
        system_Blues,
        system_OldPhone,
        system_OldCarHorn,
        system_default,
        system_Doorbell,
        system_Motorcycle,
        system_Xylophone,
        system_Sonar,
        system_TimePassing,
        system_Harp,
        system_Digital,
        system_Timba,
        system_Crickets,
        system_Duck,
        system_BellTower,

        // IOSϵͳ��������
        texttone_none,
        texttone_TriTone,
        texttone_Chime,
        texttone_Glass,
        texttone_Horn,
        texttone_Bell,
        texttone_Electronic,
        texttone_Anticipate,
        texttone_Bloom,
        texttone_Calypso,
        texttone_ChooChoo,
        texttone_Descent,
        texttone_Fanfare,
        texttone_Ladder,
        texttone_Minuet,
        texttone_NewsFlash,
        texttone_Noir,
        texttone_SherwoodForest,
        texttone_Spell,
        texttone_Suspense,
        texttone_Telegraph,
        texttone_Tiptoes,
        texttone_Typewriters,
        texttone_Update,

        // �ļ���������
        FILE_COPY_NAME,
	};

    // ���ʻ�֧���ࣨĿǰ��������֧�֣�
    class CLASSINDLL_CLASS_DECL International
    {
    public:

        // �����ַ���
		static void SetString(STRING_ID id, wstring value);

        // ȡ���ַ���
		static std::wstring GetString(STRING_ID id);

		// ȡ��iosϵͳ������������
		static std::wstring GetIosRingNameByGuid(wstring guid);

        // ȡ���ַ���
        static void Release();

#pragma warning (disable: 4251)
    private:
		static std::map<STRING_ID, std::wstring> _strings;
    };    
}
