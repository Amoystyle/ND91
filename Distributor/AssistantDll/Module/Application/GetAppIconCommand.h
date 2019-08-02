#pragma once

#include "Module/BaseCommand.h"
#include <string>
using namespace std;

namespace ND91Assistant
{
    // ��ȡ����ͼ��, ���ȡ�����б�ֿ���������Ҫ��������
    class GetAppIconCommand : public BaseCommand
    {
    public:
		// packageName : App �İ���(���ֻ��е�Ψһ��ʶ)
		// strSavePath : ȡ�õ�IconҪ���浽��PC�е�·��
        GetAppIconCommand(const DeviceData* pDevice, wstring packageName, wstring strSavePath);
        ~GetAppIconCommand();
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

	private:
		bool parseAndroidPackageHead( BinaryBufferReader* pReader );
		bool SaveIconData(wstring strFilePath, const char* pIconData, int nIconSize);
   private:
		wstring _strSavePath;	// ȡ�õ�IconҪ���浽��PC�е�·��	
        wstring _packageName;	// App �İ���(���ֻ��е�Ψһ��ʶ)
    };
}