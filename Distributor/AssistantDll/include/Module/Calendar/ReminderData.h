#pragma once
#include "Module/BaseData.h"

namespace ND91Assistant
{

	//class BinaryBufferReader;
	//class BinaryBufferWriter;
	// �¼�����
	class CLASSINDLL_CLASS_DECL ReminderData :public BaseData
	{
	public:
		ReminderData(void);
		virtual ~ReminderData(void);

		ReminderData* Clone();

		int		_id;

		int		_eventId;	// �¼�

		int		_minutes;	// ��ǰ������

		int		_method;	// ��ʽ

		//void ParseAndroidData(BinaryBufferReader& reader);

		//void AndroidDataToStream(BinaryBufferWriter& writer);

	};

}
