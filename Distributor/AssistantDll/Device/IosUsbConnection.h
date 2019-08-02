///////////////////////////////////////////////////////////
//  IosUsbConnection.h
//  Implementation of the Class IosUsbConnection
//  Created on:      01-����-2011 9:42:58
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_6D93A705_E3B8_43d8_8C3E_380CA26E0552__INCLUDED_)
#define EA_6D93A705_E3B8_43d8_8C3E_380CA26E0552__INCLUDED_

#include "IConnection.h"
#include "Core/DeviceInfo.h"
#include "AssistantDll.h"
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

namespace ND91Assistant
{
#define PASSWORD_PROTECTED_CODE -402653158

	typedef struct IosDiskMemoInfo
	{
		/// �ܵĴ��̿ռ�
		INT64 nTotalDiskCapacity;

		/// ��ϵͳ�ռ�
		INT64 nTotalSystemCapacity;

		/// �ܵ�ϵͳ���ÿռ�
		INT64 nTotalSystemAvailable;

		/// �����ݿռ�
		INT64 nTotalDataCapacity;

		/// �ܿ������ݿռ� = ���������ݿռ� + �������ݿռ�
		INT64 nTotalDataAvailable;

		/// ���������ݿռ�
		INT64 nAmountDataReserved;

		/// ���õ����ݿռ�
		INT64 nAmountDataAvailable;

		/// Ӧ�ó���ռ��
		INT64 nMobileApplicationUsage;

		IosDiskMemoInfo()
		{
			nTotalDiskCapacity = 0;
			nTotalSystemCapacity = 0;
			nTotalSystemAvailable = 0;
			nTotalDataCapacity = 0;
			nTotalDataAvailable = 0;
			nAmountDataReserved = 0;
			nAmountDataAvailable = 0;
			nMobileApplicationUsage = 0;
		}
	}IosDiskMemoInfo;

    enum RelayType
    {
        AppleSupport,
        Network,
        VPN,
        WiFi,
        UserDatabases,
        CrashReporter,
        tmp,
        SystemConfiguration
    };

    // ���ػ�����ͨ�ţ�ios�豸 usb���ӣ�
    class IosUsbConnection : public IConnection
    {
    public:
        IosUsbConnection(DeviceInfo* devInfo);//(void *dev);
        virtual ~IosUsbConnection();

		// ����׼��
		virtual bool PreActivate(){return true;};

		// ����
		virtual bool Activate();

		virtual bool Connect();
		virtual void Disconnect();
        virtual const DeviceInfo* GetDeviceInfo();

        // ��ǰ�豸�Ƿ��ֹ����װ,Ҳ�������豸���������濪�������Ƴ���װ.wifi���ӵ��Ƿ�����жϲ�?
        virtual bool IsProhibitAppInstall();

		// ���ӵ�ĳ��Ӧ�ó���Ĺ����ĵ�Ŀ¼
		// strIdentifier		: ��Ӧ�ó����ʶ��
		// ����					: �ɹ�ΪTURE��������ͨ�ž���ͷ���������DeviceInfo�ڣ�ʧ��ΪFALSE
		// ע					: �ݶ�Ϊÿ̨�ֻ�����Ӧһ�������л�Ӧ�ó���ʱ��Ὣ��һ������ر�
		// �̼�3.x��ʼ�����������
		bool Connect2AppSharingDirectory(wstring strIdentifier);

        // ��װ��������(���̲߳����������ʹ��)
        bool StartInstallationProxyService();
        // ��װ����ֹͣ
        void StopInstallationProxyService();

		//��������ͼ��״̬����رմ˷��񣬺���ʹ�������¿���������Ӵ˺���
		//���캯�����贫���豸���_ios_devptr��_deviceProductVersion�����ص�ServiceSocket�ɵ��ô������ͷ�
		bool StartSpringBoardService(void** ServiceSocket);

		// Summary  : ��ȡ��ص���
		// Returns  : int ��ص���ֵ����80�������Ϊ80%��
		int GetBetteryState();

		// Summary  : ��ȡ�豸������Ϣ
		// Returns  : bool ��ȡ�ɹ�����TRUE
		// Parameter: IosDiskMemoInfo & diskInfo ���洢������Ϣ
		bool GetDiskMemoInfo(IosDiskMemoInfo& diskInfo);

		// Summary  : �޸��豸����
		// Returns  : bool �ɹ�����true
		// Parameter: wstring strName ���޸ĺ������
		bool SetDeviceName(wstring strName);

		// Summary  : ��ȡ�豸�Ƿ�����
		// Returns  : bool true����
		bool GetIsDeviceLock(){return _bDeviceLock;}

        // Summary  : ��ȡ�ļ�
        // Returns  : bool
        // Parameter: RelayType eType 
        // Parameter: wstring strSavePath
        bool FileRelay(RelayType eType, wstring strSavePath);

		// Summary  : ����IOS����
		// Returns  : int ����������
		// Parameter: char* service ��������
		// Parameter: void** ServiceSocket �׽���ָ��
		int StartIOSService(char* service, void **ServiceSocket);

		//����ָ�ģʽ
		int EnterRecoveryMode();

        // Summary  : ֹͣIOS����
        // Returns  : void 
        // Parameter: void** pSocket Ҫֹͣ���׽���ָ��
        void StopIOSService(void** pSocket);

		//ʹ�����ģʽ����������
		bool RebootWithDiagnosticsRelay();

		//ʹ�����ģʽ���رշ���
		bool ShutDownWithDiagnosticsRelay();

		// Summary  : �������ݵ�iDevice�豸
		// Returns  : bool �Ƿ��ȡ������
		// Parameter: void* pSocket �׽���ָ��
		// Parameter: map<string,string> mapMsg ������ݵ�����
		bool SendMessageToService(void* pSocket, map<string, void*> mapMsg);
		bool SendMessageToService(void* pSocket, map<string, string> mapMsg);
        bool SendMessageToService(void* pSocket, vector<string> vecMsg);
        bool SendMessageToService(void* pSocket, vector<void*> vecMsg);

        // Summary  : ��iDevice�豸��������
        // Returns  : bool �Ƿ�ɹ���������
        // Parameter: void* pSocket �׽���ָ��
        // Parameter: map<string, string> & mapMsg �����������ݵ�����
        bool ReadMssageFromService(void* pSocket, map<string, string> &mapMsg);
        bool ReadMssageFromService(void* pSocket, vector<string> &vecMsg);
		bool ReadMssageFromService(void* pSocket, vector<void*> &vecMsg);
        bool ReadMssageFromService(void* pSocket, vector<string> &vecMsg, map< string, map<string,void*> > &mapEntities);
		bool ReadMssageFromService(void* pSocket, vector<string> &vecMsg, map<string, string>& mapIds, map<string,string> &mapEntities);
        bool ReadMssageFromService(void* pSocket, vector<string> &vecMsg, map<string, string>& mapIds);
		bool ReadMssageFromService(void* pSocket, vector<string> &vecMsg, wstring path);

        // Summary  : �ж�ĳ��DataClass�Ƿ��ֹ��PCͬ��
        // Returns  : bool
        // Parameter: string dataClass ��������
        // Parameter: bool tryEnableIfDisabled �����ֹ,���Կ���
        bool IsDataClassDisableTetheredSync(string dataClass, bool tryEnableIfDisabled);

        // Summary  : �ж�ĳ��DataClass�Ƿ���iCloud��������IMAP �ʼ��˻�ͬ��
        // Returns  : bool
        // Parameter: string dataClass ��������
        bool IsDataClassEnableiCloud(string dataClass);

		// ͨ��Ӳ���ͺ���n90ap��ȡ�豸DeviceType ��iPhone3,1
		// ��n90ap,iphone4
		// һЩ5.x�豸lockdown��ȡDeviceType������;����ֹ�ά�����ű�
		// http://theiphonewiki.com/wiki/index.php?title=Models
		wstring GetDeviceTypeByHardwareModel(wstring strHardware);

		//�ж��豸�Ƿ�Խ�� 
		int IsJailBreak();

        // ����ػ��Ƿ����У�����ü���key
        bool PhoneDaemonIsStillRunning();

    private:
        enum DEVICE_COMMAND_TYPE
        {
            Device_Connect,
            Device_Disconnect,
            Device_Pair,
            Device_UnPair,
            Device_Validate_Pairing,
            Device_Start_Session,
            Device_Stop_Session,
			Device_is_Pair,
        };

        int iTunesCommand(DEVICE_COMMAND_TYPE command);

        // ȡ�豸��Ϣ����
        void GetDevInfo();

        // �����Ի�
		int StartSession();

        // ֹͣ�Ի�
		int StopSession();

        // ȡint���͵��豸����
        /*INT64*/int64_t GetDeivceValueAsInt64(const char* pDomainKey, const char* pKey);

		// ȡbyte���͵��豸���Ե�base64���봮
		/*INT64*/wstring GetDeivceValueAsBase64(const char* pDomainKey, const char* pKey);

        // ȡstring���͵��豸����
		wstring GetDeviceValueAsString(const char* pDomainKey, const char* pKey);

        // ȡmap���͵��豸����
        map<string, string>* GetDeviceValueAsDict(const char* pDomainKey, const char* pKey);

        // Summary  : �ж�ĳ��DataClass�Ƿ��ֹ��PCͬ��
        // Returns  : bool
        // Parameter: map<string,string> mapDataClass ���� "com.apple.mobile.tethered_sync","com.apple.bookmarks.Bookmark"
        bool IsDisableTethered(map<string, string> mapDataClass);

        // Summary  : �Ƿ���iCloud,�������ʾ����ͬ��.
        // Returns  : bool
        // Parameter: string dataClass ��������
        bool IsDataSyncContainiCloudSources(string dataClass);

        // ȡ�豸ID
        wstring GetDeviceIdentifier();

        // ����iTunes_AFCConnectionOpen
        void OpenConnection();

	private:		
		bool                    _sessionStarted;       // �Ի��Ƿ���
		DeviceInfo*             _pDeviceInfo;          // �豸��Ϣ
		bool                    _bDeviceLock;          // �豸�Ƿ�����
        map<string, string>     _mapDeviceInfo;        // �豸���ص���Ϣ����
		CRITICAL_SECTION        _lock; 
    };
}

#endif // !defined(EA_6D93A705_E3B8_43d8_8C3E_380CA26E0552__INCLUDED_)
