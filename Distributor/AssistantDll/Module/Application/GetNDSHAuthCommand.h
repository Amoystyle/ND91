#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
	class AppData;

	// ��ȡandroid�豸rootȨ��
	class GetNdShAuthCommand : public BaseCommand
	{
	public:
		GetNdShAuthCommand(const DeviceData* pDevice);
		virtual ~GetNdShAuthCommand();

		// ִ������
		virtual void Execute();

		// ����Android�������ݰ�
		virtual std::string GetAndroidBodyData();

		// ����Android���ؽ�����ݰ�
		virtual void SetAndroidReturnData(std::string data);

		virtual void SetIosReturnFile(std::wstring filename) {;}

		bool GetResult(){return _bSucess;}
	

	private:

		
		bool _bSucess;

	};
}