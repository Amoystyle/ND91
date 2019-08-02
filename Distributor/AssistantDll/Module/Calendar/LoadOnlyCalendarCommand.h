#pragma once
#include "Module/TableQueryCommand.h"

namespace ND91Assistant
{
	// �����˻����¼��ֿ���������
	class CalendarLibrary;
	class BinaryBufferReader;
	class LoadOnlyCalendarCommand :public TableQueryCommand
	{
	public:
		LoadOnlyCalendarCommand(const DeviceData* pDevice, CalendarLibrary* pLibrary);
		virtual ~LoadOnlyCalendarCommand(void);

		// Summary  : �������˻�
		// Returns	: void
		void LoadCalendars();

		// Summary  : �����˻�Id�����¼�
		// Returns	: void
		// Parameter: int calendarId�����˻�Id
		void LoadCalendarEventsByCalendarId(int calendarId);

	private:

		// �����������ݰ�
		virtual void SetAndroidReturnData(std::string data);

		// Summary  : �����˻�
		// Returns	: void
		// Parameter: BinaryBufferReader & reader
		void ParseCalendars(BinaryBufferReader& reader);

		// Summary  : �����¼�
		// Returns	: void
		// Parameter: BinaryBufferReader & reader
		void ParseCalendarEvents(BinaryBufferReader& reader);

	private:

		enum ActionType			// ��������
		{
			ELoadCalendars,		// �����������˻�
			ELoadCalendarEvents,// �����˻�Id�����¼�
		};

		ActionType		 _type;		// ��������

		CalendarLibrary* _pLibrary; // �������ݲ�����
	};

}
