#pragma once


namespace ND91Assistant
{
    class IosUsbConnection;

    // ���豸�շ���Ϣ
    class CLASSINDLL_CLASS_DECL ServiceClientBase
    {
		friend class BackupRestoreBase;
    public:
        ServiceClientBase(IosUsbConnection* device);
        ~ServiceClientBase();

        bool ReadMessageFromService(vector<string>& replyInfo);
        bool ReadMessageFromService(vector<string>& replyInfo,map< string, map<string,void*> >& mapEntities);
		bool ReadMessageFromService(vector<string>& replyInfo,map<string, string>& mapIds, map<string,string>& mapEntities);
        bool ReadMessageFromService(vector<string>& replyInfo,map<string, string>& mapIds);
		bool ReadMessageFromService(map<string, string> &mapMsg);
		bool ReadMessageFromService(vector<string>& replyInfo, wstring filepath);

		bool SendMessageToService(map<string, void*> sendInfo);
		bool SendMessageToService(map<string, string> sendInfo);
        bool SendMessageToService(vector<string> sendInfo);
        bool SendMessageToService(vector<void*> sendInfo);

    protected:
        bool StartService();
        void CloseService();

    protected:
        string                      _serviceName;             // ��������
        IosUsbConnection*           _device;                  // USB����
        void*                       _serviceSock;             // �׽���ָ��
        void*                       _securitySocketCookie;    
        
    };
}
