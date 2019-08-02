#pragma once

#include "Module/Message/MessageData.h"
#include "Module/Message/MessageGroup.h"
#include "Module/Message/MessageReceiptData.h"

namespace ND91Assistant
{
	class DeviceData;
	class MessageData;
	class MessageGroup;

	// 短信管理
	class CLASSINDLL_CLASS_DECL MessageManager
	{
	public:
		MessageManager(const DeviceData* pDevice);
		virtual ~MessageManager(void);

		// Summary  : 加载短信息
		// Returns	: void
		CommandExecuteResult Load(bool bReload = false, bool redownIosSmsdbFile = false);

		// Summary  : 获得所有信息
		// Returns	: vector<MessageData*>
		void GetAllMessages(vector<const MessageData*>& msglist);

		// Summary  : 获得分组
		// Returns	: vector<MessageGroup*>
		const vector<MessageGroup*>* GetMessageGroups();

		// 获得指定分组下的短信
		void GetMessagesByGroup(MessageGroup* pGroup, vector<const MessageData*>& msglist);


		// 根据id获取短信
		MessageData* GetMessageById(int msgId);


		// Summary  : 删除短信
		// Returns	: void
		// Parameter: vector<int> msgIds
		// Parameter: ProgressObserver * pObserver
		void DeleteMessage(vector<int> msgIds, ProgressObserver* pObserver);

		// Summary  : 更新短信
		// Returns	: bool
		// Parameter: vector<int> msgIds
		// Parameter: ProgressObserver * pObserver
		bool UpdateMessage(int msgId, wstring text, ProgressObserver* pObserver);


		// 设置短信已读未读标志
		bool UpdateMessageByIsRead(vector<int> msgIds, ProgressObserver* pObserver);


		// Summary  : 发送短信
		// Returns	: bool
		// Parameter: vector<MessageData * > msgs
		// Parameter: ProgressObserver * pObserver
		bool SendMessages(vector<string> friendNumbers, wstring content, ProgressObserver* pObserver);


		// Summary  : 重新发送
		// Returns	: bool
		// Parameter: vector<MessageData * > msgs
		// Parameter: ProgressObserver * pObserver
		// Parameter: time_t startTime
		// Parameter: time_t endTime
		bool ReSendSMS( vector<int> msgIds, ProgressObserver* pObserver,time_t startTime,time_t endTime);

		// Summary  : 释放
		// Returns	: void
		void Release();

        // 导入短消息数据
        bool ImportMessageData( const vector<const MessageData*>* pDataList,ProgressObserver* pObserver = NULL);

        //获取短信数量
        int GetSMSCount();

        int SuccessCount() { return _successCount;};

#ifdef PLATFORM_IOS
        // Ios取得Ios短消息文件
        bool GetIosSmsData( wstring path, DeviceInfo* devInfo );
#endif
	private:

		// Summary  : 下载ios短信数据库
		// Returns	: void
		// Parameter: bool ReDown 是否重新下载
		void DownLoadIosDb(bool ReDown);

	private:

		const DeviceData*   _pDevice;

		map<int, MessageData*>  _messages;	//全部短信

		vector<MessageGroup*>   _groups;	//全部群组和imsg聊天室

        int _successCount;

        bool _bLoad;
	};

}
