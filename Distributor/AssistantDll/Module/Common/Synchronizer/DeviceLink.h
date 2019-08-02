#pragma once

namespace ND91Assistant
{
    // һЩ����ͨѶ����DL***message��ʽͨѶ��,����Screenshotr������Ҫ

    /* ͬ������
    Sync Process
    Initial handshake/session start:
    1. host -> device: SDMessageSyncDataClassWithDevice
    2. device -> host: SDMessageSyncDataClassWithComputer (sync can start) or SDMessageRefuseToSyncDataClassWithComputer (refusing to sync)

    Request records:
    3. host -> device: SDMessageGetAllRecordsFromDevice (first or full sync) or SDMessageGetChangesFromDevice (following sync) or 

    Retrieve records from device:
    4. device -> host: SDMessageProcessChanges
    5. host -> device: SDMessageAcknowledgeChangesFromDevice
    6. repeat 4 and 5 for each entity type for the schema until
    7. device-> host: SDMessageDeviceReadyToReceiveChanges (end of device -> host changes transfer)

    Change sync direction / should be handled by inherited DeviceLinkService interface:
    8. host -> device: DLMessagePing

    Send records from host:
    9. host -> device: SDMessageProcessChanges
    10. device -> host: SDMessageRemapRecordIdentifiers
    11. repeat 10 and 11 for each entity type for the schema until no more changes to send
    12. host -> device: SDMessageFinishSessionOnDevice (end of host -> device changes transfer)

    Finished session (allows to start from 1. again for syncing a different data class):
    13. device -> host: SDMessageDeviceFinishedSession
    */
    class ServiceClientBase;
    class DeviceLink
    {
    public:
        DeviceLink(ServiceClientBase* client);
        ~DeviceLink();

        // �˶�У��DL�汾ֵ�Ƿ�һ��
        bool VersionExchange(int versionMajor, int versionMinor);

        bool Send(vector<string> sendInfo);
        bool Send(vector<void*> sendInfo);
		bool Send(map<string, string> sendInfo);
		bool Send(map<string, void*> sendInfo);

		bool Receive(wstring filepath);
        bool Receive(vector<string>& replyInfo);
		bool Receive(map<string, string>& mapInfos);
        bool Receive(vector<string>& replyInfo,map<string, string>& mapIds);
        bool Receive(vector<string>& replyInfo,map< string, map<string,void*> >& mapEntities);
		bool Receive(vector<string>& replyInfo,map<string, string>& mapIds, map<string,string>& mapEntities);
        

        bool SendPing(string message);

        // ʹ��DL*Message�ķ����ڶϿ�֮ǰ,��Ҫ�ȷ��ͶϿ�dl��Ϣ���ܹرշ���
        bool Disconnect(string message);

    private:
        string GetMessageFromDLMSG(vector<string> replyInfo);// ���豸��ȡ��dl��Ϣ������ǰ����Ӧ����Ϣ�ı�(��������Ϣ��ɶ)

    private:
        ServiceClientBase* _client;
    };
}
