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
	wstring _name;               // �龰����
	wstring _ringmode;           // ����ģʽ
	wstring _volume;             // ����
	wstring _line1_ringtone;     // ��������1
	wstring _line2_ringtone;     // ��������2
	wstring _message_alert;      // ��������
	wstring _email_alert;        // Email����
	wstring _calendar_alert;     // �ճ�����
	wstring _alarm_alert;        // ��������
	wstring _reminder_alert;     // ��������
	wstring _fetion_alert;       // ��������

	int		_ringModeIndex;      // ����ģʽö��ֵ
	bool	_preload;
	bool	_modify;

private:

	void ParseAndroidData(BinaryBufferReader& reader);
    
};

}
