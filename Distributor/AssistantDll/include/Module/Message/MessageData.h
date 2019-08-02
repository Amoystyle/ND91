///////////////////////////////////////////////////////////
//  Implementation of the Class MessageData
//  Created on:      01-����-2011 9:43:00
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_7E29138F_02E1_4f36_B62D_9B006F11CC92__INCLUDED_)
#define EA_7E29138F_02E1_4f36_B62D_9B006F11CC92__INCLUDED_

#include "Module/BaseData.h"
#include <string>
#include <deque>
#include <vector>
#include <common/BinaryBufferReader.h>
using namespace std;

#pragma warning (disable:4251)

namespace ND91Assistant
{

	enum MMSItemType
	{
		Unknown  = -1,
		SMSSMIL  = 0,
		SMSImage = 1,
		SMSText  = 2,
		SMSVCard = 3
	};
	// ���Ÿ�����Ϣ
	class DeviceData;
	class CLASSINDLL_CLASS_DECL MMSItem
	{
		public:
			MMSItem();
			~MMSItem();
			MMSItem(MMSItem* pItem);
			int          _rowId;			// 
			long         _partId;			// 
			MMSItemType  _type;				// ��������
			wstring      _fileName;			// ������
			string       _binaryData;		//����
			string       _previewDataBin;	//ͼƬ
			wstring      _localFileName;	//��������·��

	};

	// imessage��Ϣ
	class CLASSINDLL_CLASS_DECL ImessageInfo
	{
	public:
		ImessageInfo();
		~ImessageInfo();
		ImessageInfo(ImessageInfo* pInfo);
		string      _chat_id;       // ����id
		string      _madrid_guid;   // ���ݵ�����
		bool		_isChatRoom;    // �Ƿ��������
		string		_madrid_attributedBody;		// ���ݵ�����
		int			_madrid_flags;    // 
		time_t      _madrid_date_deliveredp;//����ʱ�䣬���ݵ�����
		string      _madrid_attachmentInfo; //������
		vector<wstring>  _attachmentFiles; // �����ļ�����·��
	};

    // ����������
    class CLASSINDLL_CLASS_DECL MessageData
    {
		friend class MessageManager;
        friend class IosMessageDB;
		friend class GetMessageCommand;
		friend class ReSendMessageCommand;
		friend class SMSBackupAndRestore;
        friend class AndroidMonitorThread;

    public:

        enum MESSAGE_CATEGORY
        {
            INBOX   = 1,    // �ռ���
            SENT    = 2,    // �ѷ���
            DRAFT   = 3,    // �ݸ���
            OUTBOX  = 4,    // ������
            FAIL    = 5,    // ����ʧ��
		    Unknown = 8
        };

		enum MESSAGE_TYPE
		{
			ESMS = 1, //����
			EMMS = 2, //����
			EIMS = 3  //iMessage
		};

	private:
        MessageData();
        MessageData(MessageData* pMessageData);
        MessageData(const char* iosData[6]);
        virtual ~MessageData();
	public:
        int GetId() const;
        void SetId(int id);

        int GetThreadID() const;
        void SetThreadID(int threadID);

        bool GetRead() const;
        void SetRead(bool read);

        string GetStrNumber() const;
        void SetStrNumber(string strNumber);

		vector<string> GetNumbers() const;
		void InsertNumber(string number);

        wstring GetText() const;
        void SetText(wstring text);

        time_t GetTime() const;
        void SetTime(time_t time);

        MESSAGE_CATEGORY GetCategory() const;
        void SetCategory(MESSAGE_CATEGORY category);

		MESSAGE_TYPE GetMessageType() const;

		int GetGroupId() const;
		
		// ��ò��Ÿ�����Ϣ
		const vector<MMSItem*>* GetMMSItems(const DeviceData* pDevice);

		// ���imsg��Ϣ
		// Summary  :
		// Returns	: const ImessageInfo*
		// Parameter: const DeviceData * pDevice
		// Parameter: bool IsBak ����ʱ����Ϊtrue
		const ImessageInfo* GetImessageInfo(const DeviceData* pDevice, bool IsBak = false);

		void GetMessageData( const DeviceData* pDevice, bool isGetAllData );

	private:

		// Summary  : ���ز���ͼƬ
		// Returns	: void
		// Parameter: const DeviceData * pDevice
		wstring DownMMSItemImageFiles(const DeviceData* pDevice);

		// Summary  : ����vcard�ļ�
		// Returns	: std::wstring
		// Parameter: const DeviceData * pDevice
		wstring DownMMSItemVcardFile(const DeviceData* pDevice);

        //��ȡ���ж���ʱ�ĸ�ʽ
        static MessageData* FromBuffer(string buffer);
        static MessageData* FromBuffer(BinaryBufferReader* pReader);

        //�������߳�֪ͨ���ģ���ʽ��һ��������
        static MessageData* FromDeamonNotifyBuffer(BinaryBufferReader* pReader);

    private:
        int						_id;        // id
        int						_threadID;  // δ֪���壬���ݰ��и�����
        bool					_read;      // �Ƿ��Ѷ�
        string					_strNumber; // �绰����
        wstring					_text;      // ����Ϊsubject�ֶ����ݣ�����Ϊtext�ֶ�����
        time_t					_time;      // ����/����ʱ��
        MESSAGE_CATEGORY		_category;  //  ����

		MESSAGE_TYPE			_msgType;	// ��Ϣ����
		int                     _groupId;	// ��Id

		vector<string>			_numbers;	// ���պ���

		vector<MMSItem*>		_mmsItems;	// ���Ÿ���

		ImessageInfo*           _iMsgInfo;	// imessage��ϸ��Ϣ
    };
}

#endif // !defined(EA_7E29138F_02E1_4f36_B62D_9B006F11CC92__INCLUDED_)
