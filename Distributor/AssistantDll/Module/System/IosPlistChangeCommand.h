#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
	//IOS���޸�PLIST�ļ�ΪXML��ʽ�������ΪĳЩ�ļ����ص����غ��޸ĳ�XML�ļ�������
	class IosPlistChangeCommand : public BaseCommand
	{
	public:
		// Parameter: const DeviceData * pDevice
		// Parameter: std::wstring strPlistFileFullName �����޸ĵ�PLIST�ļ�������·�������豸�ϣ�
		IosPlistChangeCommand(const DeviceData* pDevice, std::wstring strPlistFileFullName);
		~IosPlistChangeCommand(void);

		bool GetIsComplete(){return _bComplete;}

		// ִ������
		virtual void Execute();

#pragma region ���ĸ�������Command����Ҫ���ݣ��������ʽ������ʽ

		// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
		virtual std::string GetAndroidBodyData(){return "";};

		// �����������ݰ�
		virtual void SetAndroidReturnData(std::string data) {}

		// ����ios�ļ��������ļ����������߸���ɾ���ļ�
		virtual std::wstring GetIosCommandFileContent();

		// ����ios�豸�ķ����ļ���������ɾ���ļ�
		virtual void SetIosReturnFile(std::wstring filename);

#pragma endregion

	private:
		bool _bComplete;		//�Ƿ��޸ĳɹ�
		std::wstring _strPlistFileFullName; //���޸ĵ�PLIST�ļ�������·�������豸�ϣ�
	};
}