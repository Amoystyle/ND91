#pragma once

namespace ND91Assistant
{
    class DeviceSyncClient;
    class IosUsbConnection;
    class DeviceData;

    // ͬ��������
    class SynchronizerBase
    {
    public:
        SynchronizerBase(const DeviceData* pDevice);
        ~SynchronizerBase();

        // Summary  : ��ǰ�Ƿ��ֹ��PCͬ����DataClass,�����True...
        // Returns  : bool
        // Parameter: bool tryEnableIfDisabled �����True,�Ƿ�Ҫ���Թر�
        bool IsDisableSync(bool tryEnableIfDisabled);

        // Summary  : �жϵ�ǰDataClass�Ƿ���iCloud��������IMAP �ʼ��˻�ͬ��
                    //(����¼ͬ��(over air sync)����Ҫ3��(4.0�̼�)�Ժ�Ļ�����֧���˻�ͬ��,3gs����֮��,iTouch 3��... iPad)
        // Returns  : bool
        bool IsEnableiCloud();

        // Summary  : ���ص�ǰdata-class��Ӧ����������
        // Returns  : bool
        bool LoadAllData();

        // Summary  : ɾ����dataclass��Ӧ����������(����ȵ���SaveChanges������)
        bool RemoveAllData();

    protected:
        
        // Summary  : �����޸Ĺ�������(����,�޸�,ɾ��)
        // Returns  : bool �ɹ�����TRUE
        bool SaveChanges();

        // Summary  : ���ص�ǰdata-class��Ӧ����������
        // Returns  : bool �ɹ�����TRUE
        // Parameter: bool useFastSync �Ƿ�ʹ�ÿ���ͬ��ģʽ(�������ݵ�ʱ��һ���ÿ���ͬ��ģʽ)
        // Parameter: bool parseData �Ƿ�Ҫ�����豸���ݹ���������(�������ݵ�ʱ��,����Ҫ����)
        bool LoadAllData(bool useFastSync, bool parseData);

        // Summary  : ͬ��ǰ��׼��
        // Returns  : bool �ɹ�����TRUE
        // Parameter: bool useFastSync �Ƿ�ʹ�ÿ���ͬ��ģʽ
        bool PrepareSync(bool useFastSync);

        //map<string,string> BuildActions(vector<string> linkEntityNamesKey, bool linkAllRecordsOfPulledEntityTypeSentKey);

        // �����������ݵ��豸,�̳�����Ҫʵ���Լ���Ҫ�ı��淽��
        virtual void DoSaveChanges() = 0;

        // �̳�����Ҫ��������໺������,�����
        virtual void ClearCachedData() = 0;

        // �����豸���͹���������,�̳�����Ҫʵ���Լ��Ľ�������
        virtual void ParsePlistEntities() = 0;

        // �ͷŷ��͵��豸������
        void ReleaseSendInfo();

    private:
        // �ͷŴ��豸������������
        void ReleaseEntities();

    protected:
        string  _shortDataClass;    // �̳�����Ҫ�ṩDataClass��Ӧ�Ķ���,�����ж��Ƿ����ͬ����,DataClass���Ĵ�
        string  _dataClass;         // �̳�����Ҫ�ṩ��ǰҪͬ����DataClassȫ��,��ʽ����:com.apple.xxx
        string  _dataClassAnchor;   // �̳�����Ҫ�ṩ��ǰDataClass�õ���ͬ��Anchor,���ں��豸ƥ��ͬ��ʱ����
        int     _dataClassVersion;  // �̳�����Ҫ�ṩ��ǰ֧�ֵ�DataClass�汾,һ����10x ������(��ͬ�̼�,��ͬdataclass,���ֵ��һ��,�������ַ�ͳһ��ֵ���豸,��ǰû������.)
//         bool    _useCache;          // �Ƿ�ʹ�û���,��ǰֻ����ϵ��ʵ��
//         bool    _isCacheExpired;    // �Ƿ����¼�������
        map< string, map<string,void*> > _mapEntities;
        vector<void*> _sendInfo;
        map<string, string> _actions; // һ�㴫��һ��ָʾ�豸�Ƿ�Ҫ�ϲ���Ӧ��Ŀ����

        DeviceSyncClient* _syncClient;
        IosUsbConnection* _device;
        const DeviceData* _pDevice;
    };
}
