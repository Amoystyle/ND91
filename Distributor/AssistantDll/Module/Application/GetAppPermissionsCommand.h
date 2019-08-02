#pragma once

#include "Module/BaseCommand.h"
#include <string>
using namespace std;
namespace ND91Assistant
{
	// ��ȡ����߱���Ȩ���б����ȡ�����б�ֿ���������Ҫ��������
	class GetAppPermissionsCommand : public BaseCommand
	{
	public:
		// packageName : App �İ���(���ֻ��е�Ψһ��ʶ)
		GetAppPermissionsCommand(const DeviceData* pDevice, wstring packageName);
		~GetAppPermissionsCommand();

#pragma region
		// ִ������
		virtual void Execute();

		// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
		virtual std::string GetAndroidBodyData();

		// �����������ݰ�
		virtual void SetAndroidReturnData(std::string data);

		// ����͵�IOS�豸�ϵ��ļ�����ȫ·����
		virtual std::wstring GetIosCommandFilePath(){ return L""; }

		// ��IOS�ϻ�ȡ���������ļ�����ȫ·����
		virtual std::wstring GetIosResultFilePath(){ return L""; }

		// ����ios�ļ�
		virtual std::wstring GetIosCommandFileContent(){ return L""; }

		// ����ios�豸�ķ����ļ���������ɾ���ļ�
		virtual void SetIosReturnFile(std::wstring filename){}

#pragma endregion
		vector<wstring>* GetPerminssionList() {return &_PermissionList;};
	protected:
		bool parseAndroidPackageHead( BinaryBufferReader* pReader );
	private:
		vector<wstring> _PermissionList;	//��õ�Ȩ���б���
		wstring _packageName;	//App �İ���(���ֻ��е�Ψһ��ʶ)
	};
}