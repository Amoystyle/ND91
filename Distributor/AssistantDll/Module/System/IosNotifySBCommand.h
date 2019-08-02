#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
	// IOS�޸ĳ��������ļ��󣬶�ϵͳ�ĸ�������
	class IosNotifySBCommand : public BaseCommand
	{
	public:
		
		// Parameter: const DeviceData * pDevice
		// Parameter: unsigned long nTimeOut ����ʱ�ȴ�ʱ�䣬�Ժ���Ϊ��λ
		IosNotifySBCommand(const DeviceData* pDevice, unsigned long nTimeOut);
		~IosNotifySBCommand(void);

		bool GetIsComplete(){return _bComplete;}

		// ִ������
		virtual void Execute();

#pragma region ���ĸ�������Command����Ҫ���ݣ��������ʽ������ʽ

		// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
		virtual std::string GetAndroidBodyData(){return "";};

		// �����������ݰ�
		virtual void SetAndroidReturnData(std::string data) {}

		// ����ios�豸�ķ����ļ���������ɾ���ļ�
		virtual void SetIosReturnFile(std::wstring filename);

#pragma endregion

	private:
		bool _bComplete; //����ִ���Ƿ�ɹ�
		unsigned long _nTimeOut; //��ʱ�ȴ�ʱ��
	};
}