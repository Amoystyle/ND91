#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
	class AppData;

	// ��ȡandroid�豸�Ƿ���root
	class GetNdShStateCommand : public BaseCommand
	{
	public:
		GetNdShStateCommand(const DeviceData* pDevice);
		virtual ~GetNdShStateCommand();

		// ִ������
		virtual void Execute();

		// ����Android�������ݰ�
		virtual std::string GetAndroidBodyData();

		// ����Android���ؽ�����ݰ�
		virtual void SetAndroidReturnData(std::string data);

		virtual void SetIosReturnFile(std::wstring filename) {;}


		int GetHasNdsh(){return _bHasNdsh;}
		int GetSuAuth(){return _bSuAuth;}

	private:

		bool _bHasNdsh;
		bool _bSuAuth;

	};
}