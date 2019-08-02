#pragma once

#include "Module/Common/IosDB/IosDBBase.h"
#include "Module/Message/MessageGroup.h"

namespace ND91Assistant
{
    class MessageData;
    class MMSItem;
    class ImessageInfo;
    class ProgressSubject;

    enum MessageGroupType
    {
        UnKnow          = -1,       // δ֪����
        PersonSingle    = 0,        // ��ͨ���˶Ի�
        PersonGroup     = 1         // Ⱥ��Ի�����
    };

    struct SMSInfoClass
    {
        vector<string>      AddressList;
        vector<int>         MsgIdList;
        int                 GroupID;
        MessageGroupType    GroupType;
        int                 NewestMsgID;
        int                 UnreadCount;
        long                Hash;
        string              IncludePrefixAddress;
        SMSInfoClass()
        {
            AddressList.clear();
            MsgIdList.clear();
            GroupID                 = 0;
            GroupType               = UnKnow;
            NewestMsgID             = 0;
            UnreadCount             = 0;
            Hash                    = 0;
            IncludePrefixAddress    = "";
        }
    };

    class IosMessageDB : public IosDBBase
    {
    public:
        IosMessageDB(IosDBPath iosDBPath);
        ~IosMessageDB(void);

    #pragma region ���Ų���

        // ȡ���ж���
        bool GetMessages(vector<MessageData*> *messageVec, vector<MessageGroup*>* groupsVec);

		// ȡ��������
		bool GetMessageMMSItem(int msgId, vector<MMSItem*>* itemVec);

		// ��ȡimessage��ϸ��Ϣ�͸�����IsBack����ʱ��ȡ��Ҫ���ֶΣ������ȡ����
		void GetImessageInfo(int msgId, ImessageInfo* pInfo, const DeviceData* pDevice, bool IsBak=false);

        // ȡ��������
        bool GetMessagesSize(int* size);

		// ����ʱ��������Ϣ
		int InsertGroup(const MessageGroup* pGroup);

        // �������ţ����ͳɹ����յ��¶��ź���ã�
        // ����ʵ��ִ����������
        int AddMessage(const vector<const MessageData*> *messageVec, ProgressSubject * pProgressSubject);

        // ɾ������
        // ����ʵ��ִ����������
        int DeleteMessage(const MessageData* pMsg);

		// ɾ��imessage
		int DeleteMadridMessage(const MessageData* pImsg,  const DeviceData* pDevice);

        // ͨ���������ݸ��¶���
        // ����ʵ��ִ����������
        int UpdateMessageByContent(const vector<const MessageData*> *messageVec,ProgressSubject * pProgressSubject);

        // ͨ���Ƿ��Ѷ���־���¶���
        // ����ʵ��ִ����������
        int UpdateMessageByIsRead(const vector<const MessageData*> *messageVec,ProgressSubject * pProgressSubject);

        // �޸����ݿ�
        bool FixDB();

        // Summary  : ��ȡ����ʧ�ܵĶ�������
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: vector<MessageData * > * messageVec ����ʧ�ܵĶ�Ϣ���ݼ���
        // Parameter: time_t startTime ���һ�η��Ϳ�ʼʱ��
        // Parameter: time_t endTime ���һ�η��ͽ���ʱ��
        bool GetMessageSendFail(vector<MessageData*> *messageVec,time_t startTime,time_t endTime);

    #pragma endregion

    private:

        void CreateReadFunction();

        // Summary  : ��ȡ�����־
        // Returns  : int 
        // Parameter: const MessageData * pData ��������
        int GetCategoryFlags(const MessageData* pData);

		// Summary  : ��ȡ��ʽ����ĵ绰����(ȥ�������Ϲ淶���ַ��������淶�ĵ绰����)
		// Returns  : string ��ʽ����ĵ绰����
		// Parameter: string tel ��ʽǰ��ĵ绰����
		// Parameter: bool trimPrefixion �Ƿ�ȥ�����õ绰����ǰ׺
		string GetPhoneNumberFormat(string tel, bool trimPrefixion);

		// �Ƿ�email�˺�
		bool IsEmail(string tel);

		// �����ֻ����룬ȥ��������
		string ReplaceSpaceChar(string tel);


/************************************���ݿ��޸����****************************/

        // Summary  : �����ż����Ƿ���ȷ
        // Returns  : bool �Ƿ���Ҫ�޸�
        bool CheckSmsCount();

        // Summary  : �����ŷ����Ƿ���ȷ
        // Returns  : bool �Ƿ���Ҫ�޸�
        bool CheckSmsGroup();

        // ��ȡ���ż���
        int GetSmsCount(const char* sql);

        // ��ȡ���ŷ���
        bool GetSmsGroup(const char* sql, map<string, long> *mapHash);
        bool GetSmsGroup(const char* sql, vector<string> *vecAddress);

        //�޸����ŷ�����������for 113+
        void RecountSms();
        // ����Ⱥ�����
        void DealGroupMessage(int oldGroupId, int rowId, string address, string key, int * newGroupId);
        // ���������
        void DealSingleMessage(int oldGroupId, int rowId, int flags, string address, string key, string prefixAddress, int * newGroupId);

        // �޸���ͬȺ�����벻��ͬһ����������
        void DealGroupMessageNotInGroup(int *newGroupId);
        // ����Ⱥ����������һ�����
        void DealAddressCountIsOne(string keyTmp,int *newGroupId, vector<string> *lstDel);
        // ����Ⱥ������������һ�����
        void DealAddressCountIsNotOne(string keyTmp);
        // ���Ⱥ������
        void ClearGroupMessageAddress();

        // �޸��������ݿ������
        void DealMessageDB();

/*****************************���Ż�ȡ���****************************************/

		// ��ȡȺ�鸨������
		MESSAGE_GROUP_TYPE ParseMessageGroupValues(bool is_madrid, CppSQLite3Query& q, string& key);

		// ����groupIdȡ�����Ա�����hashֵ
		void GetGroupInfo(MessageGroup* pGroup);

		// ��������ҵĳ�Ա�������Ϣ
		void GetMadridChatInfos(string chat_id, MessageGroup* pGroup);

        // ��ȡplist�����еĺ���
		void ParseNumberPlist(string plistcontent, vector<string>& nums);

		// ��ȡ���ŵĸ�����������õ���message�ĸ���ֵ
		void ParseMessageDataValues(MessageData* msg, CppSQLite3Query& q);


/**********************************���Ų������***************************************/

		// �������ݿ�field����ת����value
		string CovertByType(int type, string fieldvalue);

		// ��Ӷ��Ÿ���������ȡ����Ҫ������ֶε�value
		void MessageGetFieldValues(vector<string>& values, const MessageData *pMessage);

		// ����Message����ֶε�ֵ
		map<int,string> SetMessageValues(map<string, int> nameValues, vector<string> filedNames, vector<string> values);

		// Summary  : ��ȡָ�����ȫ���ֶκ��ֶ�����
		// Returns	: string �ֶ����
		// Parameter: string tablename ����
		// Parameter: map<string, string> & namevalues //������
		// Parameter: vector<int> & datatypes ��������
		string GetTableFieldsAndDataTypes(string tablename, map<string, int>& namevalues, vector<int>& datatypes);

/***************************************���Ų������********************************/

		// Summary  : ���ɲ���Recipients�ֶ�plist����
		// Returns	: string
		// Parameter: vector<string> numbers
		string CreateRecipientsPlist(vector<string> numbers);
		
		// Summary  : ���ò���Recipients�ֶ�ֵ
		// Returns	: void
		// Parameter: int msgId Ҫ���õĲ��ŵ�id
		// Parameter: const MessageData * pMessage
		void UpdateMMSRecipients(int msgId, const MessageData* pMessage);

		// Summary  : ������Ÿ���
		// Returns	: void
		// Parameter: int messageId 
		// Parameter: MMSItem * item
		// Parameter: bool isOut �Ƿ���
		void InsertMMSItem(int messageId, MMSItem* item, bool isOut);

/********************************imessage���*********************************/

		// Summary  : ����imessage��message���и�ֵ
		// Returns	: void
		// Parameter: int msgId Ҫ���õ�imessage��id
		// Parameter: const MessageData * pMessage 
		void UpdateImessageInfoInMessageTable(int msgId, const MessageData* pMessage);

		// Summary  : ����imessage������Ϣ
		// Returns	: void
		// Parameter: string guid ����Guid
		// Parameter: wstring filePath�������ֻ���·��
		// Parameter: bool Iscoming �ǽ��ܻ��Ƿ���
		void InsertMadridAttachment(string guid, wstring filePath, bool Iscoming);


		// Summary  :����������guid
		// Returns	: vector<string> ����guid
		// Parameter: string madrid_attachmentInfo  message����madrid_attachmentInfo�ֶ�����
		// Parameter: int madridflags message����madridflags�ֶ�ֵ
		vector<string> GetAttachmentGuid(string madrid_attachmentInfo, int madridflags);

/********************************Ⱥ�飬�����Ҳ������****************************/

		// ����Ⱥ���Ա��ȡ��id
		int GetGroupIdsByNumber(vector<string>numbers);
	
		// Summary  : ����Ⱥ��
		// Returns	: int ����Ⱥ��id
		// Parameter: vector<string> members Ⱥ���Ա
		// Parameter: int hash ��ϣֵ
		int InsertGroup(vector<string> members, int hash);
		
		// Summary  : ����imessage��������Ϣ
		// Returns	: void
		// Parameter: const MessageGroup * pGroup 
		void InsertMadridChat(const MessageGroup* pGroup);

    private:
        map<string, long> _mapHash;             // ��¼3.X�绰��������Ӧ��Hashֵ
        map<string, SMSInfoClass> _addressDic;
        map<string, vector<string>> _dicGroupTels;
    };
}