#pragma once

#include "Module/Message/MessageData.h"
#include "Module/Message/MessageGroup.h"
#include "Module/Message/MessageReceiptData.h"

namespace ND91Assistant
{
	class DeviceData;
	class MessageData;
	class MessageGroup;

	// ���Ź���
	class CLASSINDLL_CLASS_DECL MessageManager
	{
	public:
		MessageManager(const DeviceData* pDevice);
		virtual ~MessageManager(void);

		// Summary  : ���ض���Ϣ
		// Returns	: void
		CommandExecuteResult Load(bool bReload = false, bool redownIosSmsdbFile = false);

		// Summary  : ���������Ϣ
		// Returns	: vector<MessageData*>
		void GetAllMessages(vector<const MessageData*>& msglist);

		// Summary  : ��÷���
		// Returns	: vector<MessageGroup*>
		const vector<MessageGroup*>* GetMessageGroups();

		// ���ָ�������µĶ���
		void GetMessagesByGroup(MessageGroup* pGroup, vector<const MessageData*>& msglist);


		// ����id��ȡ����
		MessageData* GetMessageById(int msgId);


		// Summary  : ɾ������
		// Returns	: void
		// Parameter: vector<int> msgIds
		// Parameter: ProgressObserver * pObserver
		void DeleteMessage(vector<int> msgIds, ProgressObserver* pObserver);

		// Summary  : ���¶���
		// Returns	: bool
		// Parameter: vector<int> msgIds
		// Parameter: ProgressObserver * pObserver
		bool UpdateMessage(int msgId, wstring text, ProgressObserver* pObserver);


		// ���ö����Ѷ�δ����־
		bool UpdateMessageByIsRead(vector<int> msgIds, ProgressObserver* pObserver);


		// Summary  : ���Ͷ���
		// Returns	: bool
		// Parameter: vector<MessageData * > msgs
		// Parameter: ProgressObserver * pObserver
		bool SendMessages(vector<string> friendNumbers, wstring content, ProgressObserver* pObserver);


		// Summary  : ���·���
		// Returns	: bool
		// Parameter: vector<MessageData * > msgs
		// Parameter: ProgressObserver * pObserver
		// Parameter: time_t startTime
		// Parameter: time_t endTime
		bool ReSendSMS( vector<int> msgIds, ProgressObserver* pObserver,time_t startTime,time_t endTime);

		// Summary  : �ͷ�
		// Returns	: void
		void Release();

        // �������Ϣ����
        bool ImportMessageData( const vector<const MessageData*>* pDataList,ProgressObserver* pObserver = NULL);

        //��ȡ��������
        int GetSMSCount();

        int SuccessCount() { return _successCount;};

#ifdef PLATFORM_IOS
        // Iosȡ��Ios����Ϣ�ļ�
        bool GetIosSmsData( wstring path, DeviceInfo* devInfo );
#endif
	private:

		// Summary  : ����ios�������ݿ�
		// Returns	: void
		// Parameter: bool ReDown �Ƿ���������
		void DownLoadIosDb(bool ReDown);

	private:

		const DeviceData*   _pDevice;

		map<int, MessageData*>  _messages;	//ȫ������

		vector<MessageGroup*>   _groups;	//ȫ��Ⱥ���imsg������

        int _successCount;

        bool _bLoad;
	};

}
