#pragma once

#include "AssistantDll.h"
#include "DeviceData.h"

#include <vector>
#include <map>
#include <string>
using namespace std;

#include "Monitor/DeviceSubject.h"

#define DEVICE_MANAGER  DeviceManager::GetInstance()
#define Wifi_Connect_Psw    L"nd.com.cn"

namespace ND91Assistant
{
	class ISubject;
	class DeviceSubject;
	class IosjailBreakMonitorThread;
    // 91�ֻ����ֵײ��������
    class CLASSINDLL_CLASS_DECL  DeviceManager : public DeviceSubject
    {
	friend class MonitorThread;
    private:
        static DeviceManager* _pInstance;   // ����

        DeviceManager();
        virtual ~DeviceManager();
    public:
        // ȡ�õ�������
        static DeviceManager* GetInstance();

        static void Release();

		//adb�Ƿ���
		bool isAdbWork();

		//����adb
        void StopAdb();
		bool RestartADB();

        // �������豸���������ӣ�
        DeviceData* AddDevice(const std::wstring id, const DeviceInfo* pInfo);

        // ɾ���豸�����ȶϿ����ӣ�
        void RemoveDevice(wstring ID);

		void OnLinkout(wstring ID);

        // ȡ�������豸ID���б�
        vector<wstring> GetAllDevice() const;

		//���ios�豸�б�
		std::string GetIosDevice() const;

        // ȡ���豸���Ӹ���
        int GetDeviceCount();
        int GetDeviceCount(ND91Assistant::DEVICE_OS_TYPE nType);

		// ͨ��ID�����豸
		DeviceData* FindDevice(std::wstring id) const;

        // IOS�豸���ң�ͨ��USB�ص����ص��豸��������豸
        DeviceData* FindIosDevice(void* pDevPtr) const;

		//WIFI����ʱ��ͨ��IP�����豸
		DeviceData* FindIosDevice(wstring strIP) const;

        // ios��ʼ��������������ע��ص�����
        bool IosInit();

        // ɨ��USBӲ���豸
        // ����true��ʾ�豸�б仯��false��ʾ�ޱ仯
        bool ScanUsbDevice();

        //���ӵ�ָ����IP��ַ
		// strPreIP : ������IP��ǰ��λ
		// strCheckCode : У����
		bool Wifi_Connect(wstring strPreIP, wstring strCheckCode);
        int Wifi_IsNeedConnectConfirm(wstring strPreIP, wstring  strCheckCode);

        // �����߳�֪ͨ��Ϣ
        static void WifiTickCallBack(int nNotify, void* pVoid);
		// Android �豸��wifi����
        bool Android_Wifi_Connect(wstring strHost);

		//IOS�豸��wifi����
        bool Ios_WiFi_Connect(wstring strIP, wstring strPSW = Wifi_Connect_Psw, int nConfig=-1);

		//�Ƴ�������USB��ʽ���ӵ�IOS�豸���ݣ�IOS������ж�ص�ʱ��(���ȶϿ�����)
		void RemoveAllIosUSBDevice();

		//ע��ʧȥ���ӵ��豸
		void ReleaseDisconnectDevice(wstring id);
		void ReleaseAllDisconnectDevice();

		// ��ȡiTunes���ص�ַ
		wstring GetITunesUrl();

        // ��ȡRootData
        RootData* GetRootData(const DeviceData* pDeviceData);
		//�����豸����
		 void PassWordLockDeviceConnect(wstring devicename);
		//�������δ���������IOS�豸
		void AddPasswordLockIosDevice(void* pConnHandle);

		//�Ƴ�����δ��������IOS�豸
		void RemovePasswordLockIosDevice(void* pConnHandle, bool bReleaseConn = false);

		//����������������δ���������IOS�豸
		void ReConnectAllPasswordLockIosDevice();

        // ��ȡAndroid�豸�����б�����״̬��
        const map<wstring, wstring>* GetAndroidDeviceMap() { return &_androidDeviceMap;};

		//������̨�豸�������򵯳��豸
		int GetSuitPlateForm(DEVICE_OS_TYPE eType);
    private:
        // ��ʼ��
        void Init();		

        // ɾ�������豸����
        void RemoveAllDevice();

		void OnDeviceDisconnect(wstring id);

		//ɾ����������δ���������IOS�豸
		void RemoveAllPasswordLockIosDevice();

#pragma warning(disable : 4251)     // ���þ���4251

        typedef map<wstring, DeviceData*>    DEVICE_MAP;

        // �豸�б�
        DEVICE_MAP  _devices;

		// �Ͽ����ӵ��豸�б�
		vector<DeviceData*>	_disConnectDevices;

		bool _bIosInitSuccess;

		vector<void*> _iosPasswordLockDevices;

        // android �豸�б� <id,����״̬>
        map<wstring, wstring> _androidDeviceMap;

#pragma warning(default : 4251)

		// Android �豸��wifi����
		bool Android_Wifi_Connect(wstring strHost, wstring strCheckCode);

		void InsertDisconnectDevices(DeviceData* pDeviceData);

		//Խ������߳�
		IosjailBreakMonitorThread* _IosjailBreakMonitorThread;

		DWORD _AdbScanTimer;

	public:
		//adb��������������
		CRITICAL_SECTION _csADBSSessionLock;
		CRITICAL_SECTION _csIosInitLock;


    };
}


