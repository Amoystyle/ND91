#pragma once

namespace ND91Assistant
{
    // ���豸�շ���Ϣ
    class ServiceClientBase
    {
    public:
        ServiceClientBase(IosUsbConnection* device);
        ~ServiceClientBase();

        bool ReadMessageFromService(vector<string>& replyInfo);
        bool ReadMessageFromService(vector<string>& replyInfo,map<string, map<string,void*>>& mapEntities);
        bool ReadMessageFromService(vector<string>& replyInfo,map<string, string>& mapIds);

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
