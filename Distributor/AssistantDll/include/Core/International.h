#pragma once

#include "AssistantDll.h"

#include <map>
#include <string>

using namespace std;

namespace ND91Assistant
{
	// 字符串ID，注释中是对应中文字符串的内容
	// 界面需要根据当前语种，事先设置好所有字符串的内容
	enum STRING_ID
	{
		PHONE_LOCK_RING_NAME,       // 默认值 "Phone lock"
		PHONE_UNLOCK_RING_NAME,     // 默认值 "Phone unlock"
		CHARGING_RING_NAME,         // 默认值 "Charging"
		LOW_ENERGY_RING_NAME,       // 默认值 "低电量"
		CALENDER_ALARM_RING_NAME,   // "日历提醒"
		KEY_PRESS_RING_NAME,        // "键盘按键声"
		TAKE_PHOTO_RING_NAME,       // "相机拍照声"
		NEW_MAIL_RING_NAME,         // "新邮件"
		SEND_MAIL_RING_NAME,        // "邮件发送"
		ALTERNATE_1,				// "三全音"
		ALTERNATE_2,				// "管钟琴"
		ALTERNATE_3,				// "玻璃"				
		ALTERNATE_4,				// "圆号"			
		ALTERNATE_5,				// "铃声"		
		ALTERNATE_6,				// "电子乐"	
		DIAL_0_RING_NAME,		    // 0 按键音
		DIAL_1_RING_NAME,		    // 1 按键音
		DIAL_2_RING_NAME,		    // 2 按键音
		DIAL_3_RING_NAME,		    // 3 按键音
		DIAL_4_RING_NAME,		    // 4 按键音
		DIAL_5_RING_NAME,		    // 5 按键音
		DIAL_6_RING_NAME,		    // 6 按键音
		DIAL_7_RING_NAME,		    // 7 按键音
		DIAL_8_RING_NAME,		    // 8 按键音
		DIAL_9_RING_NAME,		    // 9 按键音
		DIAL_POUND_RING_NAME,       // #号 按键音
		DIAL_STAR_RING_NAME,		// *号 按键音
		VOICEMAIL_RING_NAME,		// 语言信箱 按键音
        DEFAULT_ACCOUNT_NAME,       // 默认帐户
        PICTURE_CATEGORY_PHOTO,     // 照片
        PICTURE_CATEGORY_WALLPAPER, // 壁纸
        PICTURE_CATEGORY_SCREENSHOT,// 截图
		DEFAULT_RING_TITTLE,        // 安卓系统默认铃声名称
		IOS_DEFAULT_RING_TITTLE,	// ios默认来电铃声名称

        // IOS系统来电铃声
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

        // IOS系统短信铃声
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

        // 文件‘复件’
        FILE_COPY_NAME,
	};

    // 国际化支持类（目前仅多语言支持）
    class CLASSINDLL_CLASS_DECL International
    {
    public:

        // 设置字符串
		static void SetString(STRING_ID id, wstring value);

        // 取得字符串
		static std::wstring GetString(STRING_ID id);

		// 取得ios系统来电铃声标题
		static std::wstring GetIosRingNameByGuid(wstring guid);

        // 取得字符串
        static void Release();

#pragma warning (disable: 4251)
    private:
		static std::map<STRING_ID, std::wstring> _strings;
    };    
}
