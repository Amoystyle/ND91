///////////////////////////////////////////////////////////
//  Implementation of the Class MessageData
//  Created on:      01-六月-2011 9:43:00
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
	// 彩信附件信息
	class DeviceData;
	class CLASSINDLL_CLASS_DECL MMSItem
	{
		public:
			MMSItem();
			~MMSItem();
			MMSItem(MMSItem* pItem);
			int          _rowId;			// 
			long         _partId;			// 
			MMSItemType  _type;				// 附件类型
			wstring      _fileName;			// 附件名
			string       _binaryData;		//内容
			string       _previewDataBin;	//图片
			wstring      _localFileName;	//附件本地路径

	};

	// imessage信息
	class CLASSINDLL_CLASS_DECL ImessageInfo
	{
	public:
		ImessageInfo();
		~ImessageInfo();
		ImessageInfo(ImessageInfo* pInfo);
		string      _chat_id;       // 聊天id
		string      _madrid_guid;   // 备份导入用
		bool		_isChatRoom;    // 是否多人聊天
		string		_madrid_attributedBody;		// 备份导入用
		int			_madrid_flags;    // 
		time_t      _madrid_date_deliveredp;//发送时间，备份导入用
		string      _madrid_attachmentInfo; //导入用
		vector<wstring>  _attachmentFiles; // 附件文件本地路径
	};

    // 短信数据类
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
            INBOX   = 1,    // 收件箱
            SENT    = 2,    // 已发送
            DRAFT   = 3,    // 草稿箱
            OUTBOX  = 4,    // 发件箱
            FAIL    = 5,    // 发送失败
		    Unknown = 8
        };

		enum MESSAGE_TYPE
		{
			ESMS = 1, //短信
			EMMS = 2, //彩信
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
		
		// 获得彩信附件信息
		const vector<MMSItem*>* GetMMSItems(const DeviceData* pDevice);

		// 获得imsg信息
		// Summary  :
		// Returns	: const ImessageInfo*
		// Parameter: const DeviceData * pDevice
		// Parameter: bool IsBak 备份时，设为true
		const ImessageInfo* GetImessageInfo(const DeviceData* pDevice, bool IsBak = false);

		void GetMessageData( const DeviceData* pDevice, bool isGetAllData );

	private:

		// Summary  : 下载彩信图片
		// Returns	: void
		// Parameter: const DeviceData * pDevice
		wstring DownMMSItemImageFiles(const DeviceData* pDevice);

		// Summary  : 彩信vcard文件
		// Returns	: std::wstring
		// Parameter: const DeviceData * pDevice
		wstring DownMMSItemVcardFile(const DeviceData* pDevice);

        //读取所有短信时的格式
        static MessageData* FromBuffer(string buffer);
        static MessageData* FromBuffer(BinaryBufferReader* pReader);

        //从心跳线程通知来的，格式不一样！！！
        static MessageData* FromDeamonNotifyBuffer(BinaryBufferReader* pReader);

    private:
        int						_id;        // id
        int						_threadID;  // 未知含义，数据包有该数据
        bool					_read;      // 是否已读
        string					_strNumber; // 电话号码
        wstring					_text;      // 彩信为subject字段内容，其他为text字段内容
        time_t					_time;      // 发送/接收时间
        MESSAGE_CATEGORY		_category;  //  信箱

		MESSAGE_TYPE			_msgType;	// 信息类型
		int                     _groupId;	// 组Id

		vector<string>			_numbers;	// 接收号码

		vector<MMSItem*>		_mmsItems;	// 彩信附件

		ImessageInfo*           _iMsgInfo;	// imessage详细信息
    };
}

#endif // !defined(EA_7E29138F_02E1_4f36_B62D_9B006F11CC92__INCLUDED_)
