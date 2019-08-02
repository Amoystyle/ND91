#pragma once
#include "Module/BaseCommand.h"

namespace ND91Assistant
{
	//�����¼���������
	class CalendarEventData;
	class CalendarEventActionCommand : public BaseCommand
	{
	public:
		CalendarEventActionCommand(const DeviceData* pDevice);
		virtual ~CalendarEventActionCommand(void);

		// Summary  : ����¼�
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent
		bool AddEvent(CalendarEventData* pEvent);

		// Summary  : ɾ���¼�
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent
		// Parameter: int deleteType ɾ������
		bool DeleteEvent(CalendarEventData* pEvent, int deleteType);

		// Summary  : �޸��¼�
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent
		// Parameter: int modifyType �޸�����
		bool EditEvent(CalendarEventData* pEvent, int modifyType);

		// Summary  : ȡ���¼�����
		// Returns	: int
		int  GetEventCount();

		// Summary  : ����������ص�Id
		// Returns	: int
		int  GetRetId(){return _retValue;}

	protected:

		// ִ������
		virtual void Execute();

#pragma region ��6��������Command����Ҫ���ݣ��������ʽ������ʽ

		// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
		virtual std::string GetAndroidBodyData();

		// �����������ݰ�
		virtual void SetAndroidReturnData(std::string data);

		// ����ios�豸�ķ����ļ���ȫ·��, unicode����ɾ���ļ�
		virtual void SetIosReturnFile(std::wstring filename){}

	private:
		enum ActionType				// ��������
		{
			EAddEvent		= 1,	// ���
			EDeleteEvent	= 2,	// ɾ��
			EEditEvent		= 3,	// �༭
			EGetEventCount	= 5		// ��ȡ����
		};

		ActionType			_type;			// ��������

		CalendarEventData*	_pEvent;		// ���������������¼�

		int					_modifyType;	// �������޸�����

		int					_retValue;		// ����ֵ��EGetEventCountʱ���ظ��������������¼�Id

	};

}
