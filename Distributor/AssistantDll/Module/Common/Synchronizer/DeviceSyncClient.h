#pragma once
#include "Module/Common/Synchronizer/ServiceClientBase.h"

namespace ND91Assistant
{
    class DeviceLink;

    // ��Ҫ��������<see cref="DeviceSyncType"/>�õ�
    struct DeviceSyncAnchors
    {
        string _deviceAnchor;
        string _computerAnchor;
        DeviceSyncAnchors()
        {
            _deviceAnchor   = "";
            _computerAnchor = "";
        }
    };

    class DeviceSyncClient : public ServiceClientBase
    {
    public:
        DeviceSyncClient(IosUsbConnection* device);
        ~DeviceSyncClient();

        // ͬ������
        enum DeviceSyncType
        {
            Fast,    // Fast-sync ֻ��Ҫ�����ϴ�ͬ����������������ݼ���
            Slow,    // Slow-sync ��Ҫ������������
            Reset,   // Reset-sync signals that the computer should send all data again.(�豸ĳ��DataClass��һ��ͬ����ʱ�����������)
            Unknow
        };

        // ���ݵķ���
        enum DeviceSyncDirection
        {
            DeviceToComputer,  // �豸��PC
            ComputerToDevice,  // PC���豸
            Unknown
        };

        string GetEmptyParameterString() { return EMPTY_PARAMETER_STRING; }
        string GetRecordEntityNameKey() { return RECORD_ENTITY_NAME_KEY; }
        string GetDataClass() { return _dataClass; }
        DeviceSyncDirection GetDeviceSyncDirection() { return _syncDirection; }
        string GetDeviceAnchor() { return _deviceAnchor; }
        DeviceSyncType GetDeviceSyncType() { return _syncType; }

        // �жϵ�ǰ�豸MobileSync�����Ƿ��Ѿ�׼����������PC��������.
        // Ҫ�����������,��Ҫ����GetAllRecordsFromDevice����GetChangesFromDevice��������Ӧ����
        bool IsReadyToReceiveChangesFromComputer();

        // Summary  : �����豸��mobilesync ����,���ں����������
        // Returns  : bool
        // Parameter: string dataClass ָ����ǰҪ�����Ǹ�dataClass(��ϵ��,������,������ǩ��)
        // Parameter: DeviceSyncAnchors anchors ͬ��˫������ȷ��ͬ�������õ�ʱ��ê
        // Parameter: int computerDataClassVersion ��ǰpc����dataClass��Ӧ�İ汾
        // Parameter: int * deviceDataClassVersion ��ǰ�豸dataClass��Ӧ�İ汾
        bool StartSync(string dataClass, DeviceSyncAnchors anchors, int computerDataClassVersion,int* deviceDataClassVersion);

        // Summary  : ֹͣͬ��
        // Returns  : void
        void StopSync();

        // Summary  : �ж�ĳ��DataClass�Ƿ��ֹ��PCͬ��
        // Returns  : bool
        // Parameter: string dataClass
        // Parameter: bool tryEnableIfDisabled �����ֹ,���Կ���
        bool IsDataClassDisableSync(string dataClass, bool tryEnableIfDisabled);

        // Summary  : �ж�ĳ��DataClass�Ƿ���iCloudͬ����������IMAP �ʼ��˻�ͬ��
        // Returns  : bool
        // Parameter: string dataClass
        bool IsDataClassEnableiCloud(string dataClass);

        // Summary  : �����豸PCͬ���Ѿ����,һ����ø÷�����,�豸�ͻ���ʾ���µ�����
        // Returns  : bool
        bool FinishSync();

        // Summary  : �����豸,PC��Ҫ��ȡ���ϴ�ͬ���������������
        // Returns  : bool
        bool GetChangesFromDevice();

        // Summary  : �����豸,PC��Ҫ��ȡ�豸����������
        // Returns  : bool
        bool GetAllRecordsFromDevice();

        // Summary  : ���շ���GetAllRecordsFromDevice����GetChangesFromDevice���ú�,�豸ʵ�ʴ��͹���������
        // Returns  : bool
        // Parameter: map<string, map<string,void*>>& mapEntities ���յ��豸���ص�����
        // Parameter: bool& isHaveRecord �Ƿ�������
        bool ReceiveChangesEntitiesFromDevice(map< string, map<string,void*> >& mapEntities,bool& isHaveRecord);

        // Summary  : ÿ����ReceiveChangesEntitiesFromDevice һ��Ҫ�����±�����,�����豸,
                        // PC�Ѿ��������豸�ϴδ��͹�����������,�豸���Խ��Ŵ�����һ������
        // Returns  : bool
        bool AcknowledgeChangesFromDevice();

        // Summary  : ����PC�ϱ���������ݸ��豸
        // Returns  : bool
        // Parameter: vector<void*> sendInfo
        bool SendProcessChangesToDevice(vector<void*> sendInfo);    // ���鷽ʽ
        bool SendProcessChangesToDevice(map<string, void*> entities,bool isHaveRecord,map< string, vector<void*> > actions,vector<void*>& sendInfo);    // ���鷽ʽ

        // Summary  : �������ݸ��豸��,һ��Ҫ���ø÷���,���ڻ�ȡ�豸������ӳ���������ID.����PC ĳ����¼id��226===>�豸ӳ�����12��...
        // Returns  : bool
        // Parameter: map<string, string>& mapIds �豸���ص�IDӳ������
        bool GetRemapIdentifiersFromDevice(map<string, string>& mapIds);

        //vector<string> CreateProcessChangesMessage(map<string,string> entities, bool isHaveRecord, map<string,string> actions, wstring xmlFilePath);

        // Summary  : ����豸�ϵ�ǰDataClass��Ӧ����������
        bool ClearAllRecordsOnDevice();

        static string   EMPTY_PARAMETER_STRING;
        static string   RECORD_ENTITY_NAME_KEY;

    private:
        // Summary  : �����豸��ͬ������
        // Returns  : bool �ɹ�����TRUE
        bool StartMobileSyncService();

        // Summary  : �����豸,PC��Ҫ��ȡ������������(��������,�����ϴ�ͬ������������)
        // Returns  : bool
        // Parameter: string operation all ����changes
        bool GetRecordsFromDevice(string operation);

        void CreateProcessChangesMessage(string dataClass,map<string, void*> entities,bool isHaveRecord,map< string, vector<void*> > actions,vector<void*>& sendInfo);

    private:
        int             MSYNC_VERSION_INT1;     // ͬ���汾����1
        int             MSYNC_VERSION_INT2;     // ͬ���汾����2

        bool            _isServiceRunning;      // �豸�ϵķ����Ƿ���������
        DeviceLink*     _dlink;                 // �豸����

        string          _dataClass;             // ��������
        DeviceSyncDirection _syncDirection;     // ���ݷ���
        string          _deviceAnchor;          // �豸���صı�ʶ
        DeviceSyncType  _syncType;              // ͬ������
    };
}