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
        UnKnow          = -1,       // 未知类型
        PersonSingle    = 0,        // 普通单人对话
        PersonGroup     = 1         // 群组对话聊天
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

    #pragma region 短信操作

        // 取所有短信
        bool GetMessages(vector<MessageData*> *messageVec, vector<MessageGroup*>* groupsVec);

		// 取彩信内容
		bool GetMessageMMSItem(int msgId, vector<MMSItem*>* itemVec);

		// 获取imessage详细信息和附件，IsBack备份时获取需要的字段，否则仅取附件
		void GetImessageInfo(int msgId, ImessageInfo* pInfo, const DeviceData* pDevice, bool IsBak=false);

        // 取短信数量
        bool GetMessagesSize(int* size);

		// 导入时插入组信息
		int InsertGroup(const MessageGroup* pGroup);

        // 新增短信（发送成功、收到新短信后调用）
        // 返回实际执行数据条数
        int AddMessage(const vector<const MessageData*> *messageVec, ProgressSubject * pProgressSubject);

        // 删除短信
        // 返回实际执行数据条数
        int DeleteMessage(const MessageData* pMsg);

		// 删除imessage
		int DeleteMadridMessage(const MessageData* pImsg,  const DeviceData* pDevice);

        // 通过短信内容更新短信
        // 返回实际执行数据条数
        int UpdateMessageByContent(const vector<const MessageData*> *messageVec,ProgressSubject * pProgressSubject);

        // 通过是否已读标志更新短信
        // 返回实际执行数据条数
        int UpdateMessageByIsRead(const vector<const MessageData*> *messageVec,ProgressSubject * pProgressSubject);

        // 修复数据库
        bool FixDB();

        // Summary  : 获取发送失败的短信数据
        // Returns  : bool 是否成功
        // Parameter: vector<MessageData * > * messageVec 发送失败的短息数据集合
        // Parameter: time_t startTime 最后一次发送开始时间
        // Parameter: time_t endTime 最后一次发送结束时间
        bool GetMessageSendFail(vector<MessageData*> *messageVec,time_t startTime,time_t endTime);

    #pragma endregion

    private:

        void CreateReadFunction();

        // Summary  : 获取邮箱标志
        // Returns  : int 
        // Parameter: const MessageData * pData 短信数据
        int GetCategoryFlags(const MessageData* pData);

		// Summary  : 获取格式化后的电话号码(去除不符合规范的字符，保留规范的电话号码)
		// Returns  : string 格式化后的电话号码
		// Parameter: string tel 格式前后的电话号码
		// Parameter: bool trimPrefixion 是否去除常用电话号码前缀
		string GetPhoneNumberFormat(string tel, bool trimPrefixion);

		// 是否email账号
		bool IsEmail(string tel);

		// 处理手机号码，去除非数字
		string ReplaceSpaceChar(string tel);


/************************************数据库修复相关****************************/

        // Summary  : 检查短信记数是否正确
        // Returns  : bool 是否需要修复
        bool CheckSmsCount();

        // Summary  : 检查短信分组是否正确
        // Returns  : bool 是否需要修复
        bool CheckSmsGroup();

        // 获取短信记数
        int GetSmsCount(const char* sql);

        // 获取短信分组
        bool GetSmsGroup(const char* sql, map<string, long> *mapHash);
        bool GetSmsGroup(const char* sql, vector<string> *vecAddress);

        //修复短信分组或计数问题for 113+
        void RecountSms();
        // 处理群发情况
        void DealGroupMessage(int oldGroupId, int rowId, string address, string key, int * newGroupId);
        // 处理单发情况
        void DealSingleMessage(int oldGroupId, int rowId, int flags, string address, string key, string prefixAddress, int * newGroupId);

        // 修复相同群发号码不在同一个分组问题
        void DealGroupMessageNotInGroup(int *newGroupId);
        // 处理群发号码数是一的情况
        void DealAddressCountIsOne(string keyTmp,int *newGroupId, vector<string> *lstDel);
        // 处理群发号码数不是一的情况
        void DealAddressCountIsNotOne(string keyTmp);
        // 清除群发号码
        void ClearGroupMessageAddress();

        // 修复短信数据库各个表
        void DealMessageDB();

/*****************************短信获取相关****************************************/

		// 获取群组辅助方法
		MESSAGE_GROUP_TYPE ParseMessageGroupValues(bool is_madrid, CppSQLite3Query& q, string& key);

		// 根据groupId取得组成员号码和hash值
		void GetGroupInfo(MessageGroup* pGroup);

		// 获得聊天室的成员号码等信息
		void GetMadridChatInfos(string chat_id, MessageGroup* pGroup);

        // 获取plist内容中的号码
		void ParseNumberPlist(string plistcontent, vector<string>& nums);

		// 获取短信的辅助方法，获得单个message的各项值
		void ParseMessageDataValues(MessageData* msg, CppSQLite3Query& q);


/**********************************短信插入相关***************************************/

		// 根据数据库field类型转换其value
		string CovertByType(int type, string fieldvalue);

		// 添加短信辅助方法，取得需要插入的字段的value
		void MessageGetFieldValues(vector<string>& values, const MessageData *pMessage);

		// 设置Message表各字段的值
		map<int,string> SetMessageValues(map<string, int> nameValues, vector<string> filedNames, vector<string> values);

		// Summary  : 获取指定表的全部字段和字段类型
		// Returns	: string 字段组合
		// Parameter: string tablename 表名
		// Parameter: map<string, string> & namevalues //各列名
		// Parameter: vector<int> & datatypes 各列类型
		string GetTableFieldsAndDataTypes(string tablename, map<string, int>& namevalues, vector<int>& datatypes);

/***************************************彩信插入相关********************************/

		// Summary  : 生成彩信Recipients字段plist内容
		// Returns	: string
		// Parameter: vector<string> numbers
		string CreateRecipientsPlist(vector<string> numbers);
		
		// Summary  : 设置彩信Recipients字段值
		// Returns	: void
		// Parameter: int msgId 要设置的彩信的id
		// Parameter: const MessageData * pMessage
		void UpdateMMSRecipients(int msgId, const MessageData* pMessage);

		// Summary  : 插入彩信附件
		// Returns	: void
		// Parameter: int messageId 
		// Parameter: MMSItem * item
		// Parameter: bool isOut 是否发送
		void InsertMMSItem(int messageId, MMSItem* item, bool isOut);

/********************************imessage相关*********************************/

		// Summary  : 设置imessage在message表中各值
		// Returns	: void
		// Parameter: int msgId 要设置的imessage的id
		// Parameter: const MessageData * pMessage 
		void UpdateImessageInfoInMessageTable(int msgId, const MessageData* pMessage);

		// Summary  : 插入imessage附件信息
		// Returns	: void
		// Parameter: string guid 附件Guid
		// Parameter: wstring filePath附件在手机端路径
		// Parameter: bool Iscoming 是接受还是发送
		void InsertMadridAttachment(string guid, wstring filePath, bool Iscoming);


		// Summary  :解析出附件guid
		// Returns	: vector<string> 附件guid
		// Parameter: string madrid_attachmentInfo  message表中madrid_attachmentInfo字段内容
		// Parameter: int madridflags message表中madridflags字段值
		vector<string> GetAttachmentGuid(string madrid_attachmentInfo, int madridflags);

/********************************群组，聊天室插入相关****************************/

		// 根据群组成员获取组id
		int GetGroupIdsByNumber(vector<string>numbers);
	
		// Summary  : 插入群组
		// Returns	: int 返回群组id
		// Parameter: vector<string> members 群组成员
		// Parameter: int hash 哈希值
		int InsertGroup(vector<string> members, int hash);
		
		// Summary  : 插入imessage聊天室信息
		// Returns	: void
		// Parameter: const MessageGroup * pGroup 
		void InsertMadridChat(const MessageGroup* pGroup);

    private:
        map<string, long> _mapHash;             // 记录3.X电话号码所对应的Hash值
        map<string, SMSInfoClass> _addressDic;
        map<string, vector<string>> _dicGroupTels;
    };
}