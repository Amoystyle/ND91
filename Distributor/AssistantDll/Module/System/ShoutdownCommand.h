#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
	//�ر��豸����
	class ShoutdownCommand: public BaseCommand
	{
	public:
		ShoutdownCommand(const DeviceData* pDevice);
		~ShoutdownCommand(void);

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

    private:
        void IosExecute();

#pragma endregion
	};
}