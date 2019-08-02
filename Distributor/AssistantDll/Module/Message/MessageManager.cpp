#include "pch_module.h"

#ifdef MODULE_MESSAGE

#include "Module/Message/MessageManager.h"
#include "Module/Message/MessageData.h"
#include "Module/Message/MessageGroup.h"
#include "Module/Message/IosMessageDB.h"
#include "Module/Message/GetMessageCommand.h"
#include "Module/Message/DeleteMessageCommand.h"
#include "Module/Message/UpdateMessageCommand.h"
#include "Module/Message/SetMessageReadCommand.h"
#include "Module/Message/SendMessageCommand.h"
#include "Module/Message/ReSendMessageCommand.h"
#include "Module/Message/AddMessageCommand.h"

#include "Module/Common/IosDB/IosDBPath.h"
#include "Device/IosUsbConnection.h"
#include "Common/7zip/SevenZipWrapper.h"

using namespace ND91Assistant;
MessageManager::MessageManager(const DeviceData* pDevice)
{
	_pDevice = pDevice;
    _bLoad = false;
}

MessageManager::~MessageManager(void)
{
	Release();
}

CommandExecuteResult ND91Assistant::MessageManager::Load(bool bReload, bool redownIosSmsdbFile)
{
    RETURN_VALUE_IF(!bReload && _bLoad, COMMAND_EXECUTE_SUCCESS);

	Release();
	DownLoadIosDb(redownIosSmsdbFile);
	GetMessageCommand cmd(_pDevice);
	vector<MessageData*> msgs;
	cmd.SetDataList(&msgs, &_groups);
	cmd.Execute();
	for (size_t i = 0; i < msgs.size(); i++)
	{
		_messages[msgs[i]->GetId()] = msgs[i];
	}
    _bLoad = true;
    return cmd.Result();
}

void ND91Assistant::MessageManager::GetAllMessages(vector<const MessageData*>& msglist)
{
	map<int, MessageData*>::iterator it = _messages.begin();
	while(it != _messages.end())
	{
		msglist.push_back(it->second);
		it++;
	}
}

const vector<MessageGroup*>* ND91Assistant::MessageManager::GetMessageGroups()
{
	return &_groups;
}

void MessageManager::GetMessagesByGroup(MessageGroup* pGroup, vector<const MessageData*>& msglist)
{
	if(!pGroup)
	{
		GetAllMessages(msglist);
		return;
	}

	time_t time = 0;
	for(size_t i =0; i < pGroup->Getmessages().size(); i++)
	{
		int msgId=pGroup->Getmessages()[i];
		map<int, MessageData*>::iterator it = _messages.find(msgId);
		if(it != _messages.end())
		{
			if (pGroup->GetGroupType() == MESSAGE_GROUP_MULII_MEMBER && time == it->second->GetTime())
				continue;
			msglist.push_back(it->second);
			time = it->second->GetTime();
		}
	}
}

MessageData* MessageManager::GetMessageById(int msgId)
{
	map<int, MessageData*>::iterator it = _messages.find(msgId);
	if(it != _messages.end())
		return it->second;
	return NULL;
}

void ND91Assistant::MessageManager::DeleteMessage( vector<int> msgIds, ProgressObserver* pObserver)
{
	vector<const MessageData*> msgs;
	for(size_t i = 0; i < msgIds.size(); i++)
	{
		map<int, MessageData*>::iterator it = _messages.find(msgIds[i]);
		if(it != _messages.end())
			msgs.push_back(it->second);
	}
	DeleteMessageCommand deletecmd(_pDevice);
	deletecmd.Attach(pObserver);
	deletecmd.SetDataList(&msgs);
	deletecmd.Execute();
}

void ND91Assistant::MessageManager::Release()
{
	map<int, MessageData*>::iterator it = _messages.begin(); 
	while(it != _messages.end())
	{
		SAFE_DELETE(it->second);
		it++;
	}
	_messages.clear();

	for(size_t i = 0 ; i < _groups.size(); i++)
	{
		SAFE_DELETE(_groups[i]);
	}
	_groups.clear();
}

bool ND91Assistant::MessageManager::UpdateMessage(int msgId, wstring text, ProgressObserver* pObserver )
{
	MessageData* oldmsg = GetMessageById(msgId);
	RETURN_FALSE_IF(!oldmsg);
	vector<const MessageData*> msgs;
	MessageData* newMsg = new MessageData(oldmsg);
	newMsg->SetText(text);
	msgs.push_back(newMsg);
	UpdateMessageCommand updatecmd(_pDevice);
	updatecmd.Attach(pObserver);
	updatecmd.SetDataList(&msgs);
	updatecmd.Execute();
	SAFE_DELETE(newMsg);
	return updatecmd.Success();

}

bool MessageManager::UpdateMessageByIsRead(vector<int> msgIds, ProgressObserver* pObserver)
{
	vector<const MessageData*> msgs;
	for(size_t i = 0; i < msgIds.size(); i++)
	{
		map<int, MessageData*>::iterator it = _messages.find(msgIds[i]);
		if(it != _messages.end())
		{
			if(it->second->GetCategory() == MessageData::INBOX)
				msgs.push_back(it->second);
		}
	}
	SetMessageReadCommand cmd(_pDevice);
	cmd.Attach(pObserver);
	cmd.SetDataList(&msgs);
	cmd.Execute();
	return cmd.Success();
}

bool ND91Assistant::MessageManager::SendMessages(vector<string> friendNumbers, wstring content, ProgressObserver* pObserver )
{
	vector<MessageData*> msgs;
	time_t mt;
	for (size_t i = 0; i < friendNumbers.size(); i++)
	{
		MessageData* msg = new MessageData();
		msg->SetStrNumber(friendNumbers[i]);
		msg->SetRead(true);
		msg->SetText(content);
		time(&mt);
		msg->SetTime(mt);
		msgs.push_back(msg);
	}
	SendMessageCommand cmd(_pDevice);
	cmd.Attach(pObserver);
	cmd.SetDataList(&msgs);
	cmd.Execute();
	for(size_t i = 0; i < msgs.size(); i++)
	{
		SAFE_DELETE(msgs[i]);
	}
	msgs.clear();
	return cmd.Success();
}

void ND91Assistant::MessageManager::DownLoadIosDb(bool ReDown)
{
	RETURN_IF(_pDevice->GetDevInfo()->_deviceType != Ios);

	IosDBPath dbPath(_pDevice,ISMESSAGEDB);
	IosMessageDB messageDB(dbPath);

	// 不需重新下载且已经存在则返回
	RETURN_IF(!ReDown && CFileOperation::IsFileExist(messageDB.GetTempDBFilePath().c_str()))

	DirData* dir = new DirData(_pDevice);
	dir->_name = CStrOperation::trimRightW(dbPath.GetDBFolder(),L"/");
	vector<NodeData*> nodes;
	deque<NodeData*> children = dir->GetChildrens();
	for (size_t i=0; i < children.size(); i++)
	{
		int found = children[i]->_name.find(L"sms.db");
		if(found > -1)
		{
			wstring localpath = CFileOperation::GetFileDir(messageDB.GetTempDBFilePath())+children[i]->_name;
            CFileOperation::DeleteFile(localpath);
			nodes.push_back(children[i]);
		}
	}
	FileHelper::Download(&nodes, CFileOperation::GetFileDir(messageDB.GetTempDBFilePath()), NULL);

	SAFE_DELETE(dir);

	if(!_pDevice->GetDevInfo()->_ios_bJailBreaked)//非越狱，通过守护获取
	{
		CFileOperation::MakeLocalDir(CFileOperation::GetFileDir(messageDB.GetTempDBFilePath()));
		IosUsbConnection iosUsb((DeviceInfo*)_pDevice->GetDevInfo());
		if(iosUsb.FileRelay(UserDatabases, messageDB.GetTempDBFilePath()+L".gz"))
		{
			if(SevenZipWrapper::Extract(messageDB.GetTempDBFilePath()+L".gz",dbPath.GetTempFolder()+L"sms\\"))
			{
				if(SevenZipWrapper::Extract(messageDB.GetTempDBFilePath(), dbPath.GetTempFolder()+L"sms\\"))
				{
					CFileOperation::DeleteFile(messageDB.GetTempDBFilePath());
					CFileOperation::DeleteFile(messageDB.GetTempDBFilePath()+L".gz");
					vector<wstring> filepaths;
					CFileOperation::GetMatchFileFullName(dbPath.GetTempFolder()+L"sms\\private\\var\\mobile\\Library\\SMS",L"",filepaths);
					for (size_t i = 0; i < filepaths.size(); i++)
					{
						wstring despath = CStrOperation::ReplaceW(filepaths[i],L"sms\\private\\var\\mobile\\Library\\SMS",L"sms");
						::MoveFile(filepaths[i].c_str(), despath.c_str());
					}
					CFileOperation::DeleteDirectory(dbPath.GetTempFolder()+L"sms\\private");	
				}
			}
		}
	}
}

bool MessageManager::ReSendSMS( vector<int> msgIds, ProgressObserver* pObserver,time_t startTime,time_t endTime)
{
	ReSendMessageCommand cmd(_pDevice,startTime,endTime);
	cmd.Attach(pObserver);
	cmd.SetDataList(&msgIds);
	cmd.Execute();
	return cmd.Success();
}


bool ND91Assistant::MessageManager::ImportMessageData( const vector<const MessageData*>* pDataList,ProgressObserver* pObserver )
{
    AddMessageCommand command(_pDevice);
    command.Attach(pObserver);
    command.SetDataList(pDataList);
    command.Execute();
    _successCount = command.ExecuteCount();
    LOG->WriteDebug(L"MessageManager::ImportMessageData:_successCount:"+CStrOperation::IntToWString(_successCount));
    LOG->WriteDebug(L"MessageManager::ImportMessageData:result:"+CStrOperation::IntToWString(command.Success()));
    return command.Success();
}

int ND91Assistant::MessageManager::GetSMSCount()
{
    return GetMessageCommand(_pDevice).GetSize();
}

bool ND91Assistant::MessageManager::GetIosSmsData( wstring path, DeviceInfo* devInfo )
{
    //IosUsbConnection conn(((DeviceData*)_pDevice)->GetMutableDevInfo());
    IosUsbConnection conn(devInfo);
    return conn.FileRelay(UserDatabases, path);
}


#endif
