#pragma once
#include "Module/BaseCommand.h"

namespace ND91Assistant
{
	// ����ȫ�������˻����¼�����������
	class CalendarLibrary;
	class LoadCalendarCommand :public BaseCommand
	{
	public:
		LoadCalendarCommand(const DeviceData* pDevice, CalendarLibrary* pLibrary);
		virtual ~LoadCalendarCommand(void);

		// ִ������
		virtual void Execute() ;

	protected:

#pragma region ��6��������Command����Ҫ���ݣ��������ʽ������ʽ

		// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
		virtual std::string GetAndroidBodyData();

		// �����������ݰ�
		virtual void SetAndroidReturnData(std::string data);

		// ����ios�豸�ķ����ļ���ȫ·��, unicode����ɾ���ļ�
		virtual void SetIosReturnFile(std::wstring filename);

	private:

		CalendarLibrary*  _pLibrary;  // �������ݲ�����
	};

}