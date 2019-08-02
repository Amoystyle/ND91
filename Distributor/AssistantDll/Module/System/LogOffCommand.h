#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
#define Ios_SB_Shutdown_Cookie 

	//ע���ֻ�����
	class LogOffCommand: public BaseCommand
	{
	public:
		LogOffCommand(const DeviceData* pDevice);
		~LogOffCommand(void);

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