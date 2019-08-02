#pragma once

namespace ND91Assistant
{
    // 与设备收发消息
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
        string                      _serviceName;             // 服务名称
        IosUsbConnection*           _device;                  // USB连接
        void*                       _serviceSock;             // 套接字指针
        void*                       _securitySocketCookie;    
        
    };
}
