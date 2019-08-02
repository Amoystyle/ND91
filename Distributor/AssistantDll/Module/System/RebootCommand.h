#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
	//�����豸����
	class RebootCommand: public BaseCommand
	{
	public:
		RebootCommand(const DeviceData* pDevice);
		~RebootCommand(void);

		// ִ������
		virtual void Execute();

#pragma region ���ĸ�������Command����Ҫ���ݣ��������ʽ������ʽ

		// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
		virtual std::string GetAndroidBodyData(){return "";};

		// �����������ݰ�
		virtual void SetAndroidReturnData(std::string data) {}

		// ����ios�豸�ķ����ļ���������ɾ���ļ�
		virtual void SetIosReturnFile(std::wstring filename){};

		virtual std::wstring GetIosResultFilePath(){return L"";}

		void SetReootRecovery(bool recovery){_EnterRecivery = recovery;}

    private:
        void IosExecute();
		bool _EnterRecivery;
#pragma endregion
	};
}