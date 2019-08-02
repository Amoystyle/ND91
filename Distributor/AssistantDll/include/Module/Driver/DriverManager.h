#pragma once

#include "Module/Driver/Driver.h"

#define DRIVER_MANAGER  DriverManager::GetInstance()

// ����������
namespace ND91Assistant
{
    class DeviceHardIDList;
	class HardwareInfo;    
    class CLASSINDLL_CLASS_DECL DriverManager
    {
	friend class HardwareInfo;	
	private:
		static DriverManager* _pInstance;   // ����

        DriverManager();
        ~DriverManager();

	public:

		// ȡ�õ�������
		static DriverManager* GetInstance();

		static void Release();

        //ɨ��Ӳ���豸���õ��ֻ��豸�б�
		//���strHardID��ֵ����ֻɨ���strHardID���豸		
        int ScanHardware(wstring wstrHardID = L"");
		//��õ�ǰ���豸�б�
        vector<Driver*> GetDriverList() { return _DriverList;};
        //�Ƿ����豸������
        int HasDeviceLinkin();
 		//���� VID, PID�ж��ǲ���Ios�豸
		bool IsIosDevice(wstring wstrVID, wstring wstrPID);
        bool IsIosDevice(string strVID, string strPID);

		// ���strHardID��Ӧ��Driver
		Driver* GetDriver(wstring wstrHardID);
		Driver* GetDriver(wstring wstrVID, wstring wstrPID, wstring wstrMI=L"");
		
		// �Ƿ���Ҫ��װiTunes
		bool IsNeedInstalliTunes();

        //�����豸ɨ��
        bool ScanForHardwareChange();

        // ȡiTunes����·��
        wstring GetITunesUrl();
        //��װ���� Inf ��ʽ
        bool InstallInf(wstring strInf);
        //��װ���� Exe ��ʽ
        bool InstallExe(wstring strExe, wstring strParam=L"");
        //��װ���� Msi ��ʽ
        bool InstallMsi(wstring strMsi, wstring strParam=L"");
    private:
		//����ֻɨ�� ����wstrVID, wstrPID, wstrMI�������豸
		int ScanHardware(wstring wstrHardID, wstring wstrVID, wstring wstrPID, wstring wstrMI);

        //��ȡ�豸����
        HARDWARE_TYPE GetHardwareType(string strClassName, string strDesc, string strHardID);
		HARDWARE_TYPE GetHardwareType(string strHardID);

		//�ͷ��豸
		void ReleaseDriverList();

    private:
		DeviceHardIDList* pDeviceHardIDList;
        //�洢ɨ�赽���豸
#pragma warning(disable : 4251)     // ���þ���4251
        vector<Driver*> _DriverList;
#pragma warning(default : 4251)
    };

}

