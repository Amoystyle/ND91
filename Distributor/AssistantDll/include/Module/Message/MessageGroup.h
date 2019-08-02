#pragma once
#include "Module/BaseData.h"
#include <string>
#include <vector>

#pragma warning (disable:4251)

using namespace std;
namespace ND91Assistant
{
    class MessageData;

	enum MESSAGE_GROUP_TYPE // 组类型
	{
		MESSAGE_GROUP_NONE = -1,
		MESSAGE_GROUP_SINGLE_MEMBER = 0, // 单人对话
		MESSAGE_GROUP_MULII_MEMBER = 1,  // 群组对话
		MESSAGE_MADRID_CHATROOM = 2,	 // imessage聊天室
		MESSAGE_MADRID_HANDLE = 3,       // imessage单个
	};
	// 组
	class CLASSINDLL_CLASS_DECL MessageGroup
	{
		friend class MessageManager;
		friend class IosMessageDB;
		private:
			MessageGroup();
			~MessageGroup();
			MessageGroup(const MessageGroup* pGroup);
		public:
			int GroupId(){return _groupId;} // 组Id

			vector<int> Getmessages(){return _messageIds;}

			vector<string>  GetMembers(){return _members;}

			void InsertMessage(MessageData* msg);

			void SetGroupId(int groupId){_groupId = groupId;}

			MESSAGE_GROUP_TYPE GetGroupType(){return _groupType;}

			string GetChatRoomName(){return _chatRoomName;}

			string GetMadridchatAccount(){return _madridchat_accountLogin;}

		private:
			int				_groupId;     // 组Id

			MESSAGE_GROUP_TYPE   _groupType;//组类型

			long            _hash;        // hash值

			vector<string>  _members;     // 组成员号码

			vector<int>		_messageIds;  // 该组所有短信id

			string          _chatRoomName; //聊天室名称

			string          _madridChat_accountId;//聊天发起者帐号GUID，仅备份恢复用

			string          _madridchat_accountLogin;// 聊天发起者账号
	};

}