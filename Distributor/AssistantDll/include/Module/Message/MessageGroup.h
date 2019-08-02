#pragma once
#include "Module/BaseData.h"
#include <string>
#include <vector>

#pragma warning (disable:4251)

using namespace std;
namespace ND91Assistant
{
    class MessageData;

	enum MESSAGE_GROUP_TYPE // ������
	{
		MESSAGE_GROUP_NONE = -1,
		MESSAGE_GROUP_SINGLE_MEMBER = 0, // ���˶Ի�
		MESSAGE_GROUP_MULII_MEMBER = 1,  // Ⱥ��Ի�
		MESSAGE_MADRID_CHATROOM = 2,	 // imessage������
		MESSAGE_MADRID_HANDLE = 3,       // imessage����
	};
	// ��
	class CLASSINDLL_CLASS_DECL MessageGroup
	{
		friend class MessageManager;
		friend class IosMessageDB;
		private:
			MessageGroup();
			~MessageGroup();
			MessageGroup(const MessageGroup* pGroup);
		public:
			int GroupId(){return _groupId;} // ��Id

			vector<int> Getmessages(){return _messageIds;}

			vector<string>  GetMembers(){return _members;}

			void InsertMessage(MessageData* msg);

			void SetGroupId(int groupId){_groupId = groupId;}

			MESSAGE_GROUP_TYPE GetGroupType(){return _groupType;}

			string GetChatRoomName(){return _chatRoomName;}

			string GetMadridchatAccount(){return _madridchat_accountLogin;}

		private:
			int				_groupId;     // ��Id

			MESSAGE_GROUP_TYPE   _groupType;//������

			long            _hash;        // hashֵ

			vector<string>  _members;     // ���Ա����

			vector<int>		_messageIds;  // �������ж���id

			string          _chatRoomName; //����������

			string          _madridChat_accountId;//���췢�����ʺ�GUID�������ݻָ���

			string          _madridchat_accountLogin;// ���췢�����˺�
	};

}