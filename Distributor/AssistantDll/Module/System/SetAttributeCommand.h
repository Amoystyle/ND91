#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
#define IOS_NORMAL_OWNER L"501" //�����ļ�������ֵ
#define IOS_LOWER_OWNER  L"0"   //�Ͱ汾IOS�豸������ֵ

	//�����ļ�Ȩ������,ios�����ú��޸��Ƿֿ�����������
	class SetAttributeCommand : public BaseCommand
	{
	public:
		// Parameter: const DeviceData * pDevice 
		// Parameter: std::wstring strPath			: ������Ȩ�޵��ļ�������·�������豸���ļ�
		// Parameter: std::wstring strRights		: Ȩ�ޣ���L"777"
		// Parameter: std::wstring strOwner			: ��������L"501"
		SetAttributeCommand(const DeviceData* pDevice, std::wstring strPath, std::wstring strRights, std::wstring strOwner = L"");
		~SetAttributeCommand(void);

        // ����Ŀ���ļ�����ȫ·����
        void SetPath(wstring path);

		// ִ������
		virtual void Execute();

        // �Ƿ�ɹ�
		bool Success();

    private:

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

		bool _bSuccess;			//�Ƿ����óɹ�
		std::wstring _strRights; //Ȩ��ֵ
		std::wstring _strPath; //�豸��������Ȩ�޵��ļ�������·����
		std::wstring _strOwner; //����ֵ
	};
}
