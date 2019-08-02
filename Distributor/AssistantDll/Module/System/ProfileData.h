#pragma once
#include "Module/BaseData.h"
#include "Common/BinaryBufferReader.h"
//#include <xstring>

using namespace std;
namespace ND91Assistant
{

class ProfileData :public BaseData
{
	friend class GetProfilesCommand;
public:
	ProfileData(void);
	virtual ~ProfileData(void);

	int     _id;
	wstring _name;               // 情景名称
	wstring _ringmode;           // 响铃模式
	wstring _volume;             // 音量
	wstring _line1_ringtone;     // 来电铃声1
	wstring _line2_ringtone;     // 来电铃声2
	wstring _message_alert;      // 短信铃声
	wstring _email_alert;        // Email铃声
	wstring _calendar_alert;     // 日程铃声
	wstring _alarm_alert;        // 闹钟铃声
	wstring _reminder_alert;     // 提醒铃声
	wstring _fetion_alert;       // 飞信铃声

	int		_ringModeIndex;      // 响铃模式枚举值
	bool	_preload;
	bool	_modify;

private:

	void ParseAndroidData(BinaryBufferReader& reader);
    
};

}
