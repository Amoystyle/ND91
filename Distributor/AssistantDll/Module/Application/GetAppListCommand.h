#pragma once

#include "Module/BaseCommand.h"
#include <vector>
#include <map>
using namespace std;

namespace ND91Assistant
{
    class AppData;

    //��ȡ������Ϣ�б�
    class GetAppListCommand : public BaseCommand
    {
    public:
		// appType : Ҫ��ȡ��App����
		// appOwner : Ҫ��ȡ��App���������ͣ� user / system
        GetAppListCommand(const DeviceData* pDevice, APP_TYPE appType, APP_OWNER appOwner);
        virtual ~GetAppListCommand();
		
        // ִ������
        virtual void Execute();

		int GetIosAppCount();

#pragma region

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
		// ��ȡȡ����App�б�
		const vector<AppData*> GetDataList();
        // ����
        void SortData();

	private:
		vector<AppData*> _DataList;
		wstring m_TempOnComputerFolder;

		APP_TYPE _appType;
		APP_OWNER _appOwner;

	private:
		void GetIosDataList(APP_TYPE appType, vector<AppData*>& vtData);

        // ��ȡ��׿�û������С
        void GetAndroidUserAppSize();

//         // ��ȡ��׿�����С
//         void GetAndroidAppSize();
//         // ��ȡ��׿ϵͳ����MAP
//         void GetAndroidSystemAppMap(map<wstring, long>& appMap);
//         // ��ȡ��׿�û�����MAP
//         void GetAndroidUserAppMap(map<wstring, long>& appMap);
// 
//         // ��ȡ��׿����MAP
//         void GetAndroidAppMap(map<wstring, long>& appMap, wstring path);
    };

}