#include "pch_module.h"

#ifdef MODULE_MESSAGE

#include "Module/Message/MessageGroup.h"
#include "Module/Message/MessageData.h"

using namespace ND91Assistant;

ND91Assistant::MessageGroup::MessageGroup()
{
	_groupId = -1;
	_hash    = 0;
	_groupType = MESSAGE_GROUP_SINGLE_MEMBER;//×éÀàÐÍ
}

ND91Assistant::MessageGroup::MessageGroup( const MessageGroup* pGroup )
{
	_groupId = pGroup->_groupId;
	_hash    = pGroup->_hash;
	_groupType = pGroup->_groupType;
	_chatRoomName = pGroup->_chatRoomName;
	_madridChat_accountId = pGroup->_madridChat_accountId;
	_madridchat_accountLogin = pGroup->_madridchat_accountLogin;
	for(size_t i = 0; i < pGroup->_messageIds.size(); i++)
	{
		_messageIds.push_back( pGroup->_messageIds[i] );
	}
	for(size_t i = 0; i < pGroup->_members.size(); i++)
	{
		_members.push_back(pGroup->_members[i]);
	}
}
ND91Assistant::MessageGroup::~MessageGroup()
{

}

void ND91Assistant::MessageGroup::InsertMessage(MessageData* msg )
{
	bool found = false;
	for(size_t i = 0; i < _messageIds.size(); i++)
	{
		if(_messageIds[i] == msg->GetId())
		{
			found = true;
			break;
		}
	}
	if(!found)
		_messageIds.push_back(msg->GetId());

}

#endif