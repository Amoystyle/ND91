#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
	class GetIMSICommand : public BaseCommand
	{
	public:
		GetIMSICommand(const DeviceData* pDevice);
		~GetIMSICommand(void);

		// ִ������
		virtual void Execute();

		// ����Android�������ݰ�
		virtual std::string GetAndroidBodyData(){ return "";}

		// ����Android���ؽ�����ݰ�
		virtual void SetAndroidReturnData(std::string data){}

		// ��IOS�ϻ�ȡ���������ļ�����ȫ·����
		virtual std::wstring GetIosResultFilePath() { return L""; }

		// ����ios�豸�ķ����ļ���������ɾ���ļ�
		virtual void SetIosReturnFile(std::wstring filename);

		std::wstring GetIMSI(){return _strIMSI;};

	private:
		std::wstring _strIMSI;
	};
}
