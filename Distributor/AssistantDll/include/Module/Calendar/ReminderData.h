#pragma once
#include "Module/BaseData.h"

namespace ND91Assistant
{

	//class BinaryBufferReader;
	//class BinaryBufferWriter;
	// 事件提醒
	class CLASSINDLL_CLASS_DECL ReminderData :public BaseData
	{
	public:
		ReminderData(void);
		virtual ~ReminderData(void);

		ReminderData* Clone();

		int		_id;

		int		_eventId;	// 事件

		int		_minutes;	// 提前几分钟

		int		_method;	// 方式

		//void ParseAndroidData(BinaryBufferReader& reader);

		//void AndroidDataToStream(BinaryBufferWriter& writer);

	};

}
