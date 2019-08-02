#include "pch_module.h"

#ifdef MODULE_MESSAGE

#include "Module/Message/IosMessageDB.h"
#include "Module/Message/MessageData.h"

#include "Device/CFConverter.h"

#define PRODUCTVERSIONL            113               // IOS设备低版本号
#define PRODUCTVERSIONH            300               // IOS设备高版本号
#define MADRID_FLAGS_RECEIVE	   32773             // imessage接收标志
#define MARDID_STYLE_CHATROOM	   43				 // 聊天类型，多人聊天室
#define MADRID_RECEIVE_ATATCHGUID_STARTPOS	104		 //	imessage收件，附件GUID在madrid_attachmentInfo字段内容的起始位置
#define MADRID_RECEIVE_ATATCHGUID_COUNTPOS	65		 //	imessage收件，附件GUID个数在madrid_attachmentInfo字段内容的起始位置
#define MADRID_SEND_ATATCHGUID_STARTPOS		67		 //	imessage发件，附件GUID在madrid_attachmentInfo字段内容的起始位置
#define MADRID_SEND_ATATCHGUID_COUNTPOS		47		 //	imessage收件，附件GUID个数在madrid_attachmentInfo字段内容的起始位置
#define MADRID_ATTACHGUID_LENGTH			36		 // imessage附件Guid长度 
#define MESSAGE_INSERT_FIELDNAMES  "address,date,text,flags,group_id,country,read,UIFlags,subject"

IosMessageDB::IosMessageDB(IosDBPath iosDBPath)
: IosDBBase(iosDBPath)
{
    _mapHash.clear();
    _addressDic.clear();
    _dicGroupTels.clear();
}

IosMessageDB::~IosMessageDB(void)
{
}

bool IosMessageDB::GetMessages(vector<MessageData*> *messageVec, vector<MessageGroup*>* groupsVec)
{
    CppSQLite3Buffer sql;
    sql.Format("select * from message ORDER BY ROWID DESC");
    CppSQLite3Query q = ExecQuery((const char*)sql);

	map<string, MessageGroup*>  groups;
    while (!q.Eof())
    {
		vector<string> numbers;
		string key = "";
		int is_madrid = 0 ;
		if(_iosDBPath.GetProductVersion() >= 500)
			is_madrid = q.GetIntField("is_madrid");

		MESSAGE_GROUP_TYPE grouptype = ParseMessageGroupValues(is_madrid == 1, q, key);
		if(key.empty())
		{
			q.NextRow();
			continue;
		}
		
		MessageGroup* group = NULL;
		map<string, MessageGroup*>::iterator it= groups.find(key);
		if ( it != groups.end())
			group = it->second;
		else
		{
			group = new MessageGroup();
			int groupId = q.GetIntField("group_id");
			group->SetGroupId(groupId);
			if(!is_madrid)
				GetGroupInfo(group);
			else
			{
				group->_groupType = grouptype;
				string chat_id = q.GetStringField("madrid_handle");
				group->_chatRoomName = q.GetStringField("madrid_roomname");
				GetMadridChatInfos(chat_id.empty()?group->_chatRoomName:chat_id, group);
			}
			groups[key]= group;
			if(groupsVec)
				groupsVec->push_back(group);
		}
		numbers = group->_members;
		MessageData* msg = NULL;
		msg = new MessageData();
		messageVec->push_back(msg);
		if(is_madrid==1)
			msg->_msgType = MessageData::EIMS;
		ParseMessageDataValues(msg, q);
		if(group)
			msg->_groupId = group->GroupId();

		int len=0;
		string tittle = q.GetStringField("address");
		tittle = GetPhoneNumberFormat(tittle, true);
		if(!tittle.empty())
			msg->InsertNumber(tittle);

		string recipients(q.GetStringField("recipients"));
		if(!recipients.empty())
		{
			msg->_msgType = MessageData::EMMS;
			ParseNumberPlist(recipients, numbers);
		}
		for (size_t i = 0; i < numbers.size(); i++)
		{
			msg->InsertNumber(GetPhoneNumberFormat(numbers[i], true));
		}
		msg->SetStrNumber(tittle);

		if(group)
			group->InsertMessage(msg);

        q.NextRow();
    }
    return true;
}

MESSAGE_GROUP_TYPE IosMessageDB::ParseMessageGroupValues(bool is_madrid, CppSQLite3Query& q, string& key)
{
	MESSAGE_GROUP_TYPE grouptype=MESSAGE_GROUP_NONE;
	if( is_madrid)
	{
		string handle = q.GetStringField("madrid_handle");
		string roomname = q.GetStringField("madrid_roomname");
		if(!roomname.empty())
		{
			key = "madrid_roomname"+roomname;
			grouptype = MESSAGE_MADRID_CHATROOM;
		}
		else if(!handle.empty())
		{
			key = "madrid_handle"+handle;
			grouptype = MESSAGE_MADRID_HANDLE;
		}
	}
	else
	{
		string group_id = q.GetStringField("group_id");
		key = "group_id"+group_id;
	}
	return grouptype;
}

void IosMessageDB::ParseMessageDataValues(MessageData* msg, CppSQLite3Query& q)
{
	int ROWID = q.GetIntField("ROWID");
	msg->SetId(ROWID);

	string text = q.GetStringField("subject");
	if(text.empty())
		text=q.GetStringField("text");
	msg->SetText(CCodeOperation::UTF_8ToUnicode(text));

	const char* pdate = q.FieldValue("date");
	long datetime = 0;
	if(pdate && strlen(pdate)>0)
		datetime = atol(pdate);

	msg->SetTime(datetime);
	msg->SetRead(q.GetIntField("read")==1);
	if(msg->_msgType == MessageData::EIMS)
	{
		//msg->_time += CDealTime::IosStartTime().GetTime();
        msg->_time += CDealTime(1970,1,1,0,0,0).GetTime()-CDealTime::GetTimeZone();
		msg->_category = q.GetIntField("madrid_flags")==MADRID_FLAGS_RECEIVE?MessageData::INBOX:MessageData::OUTBOX;
		return;
	}
	int flag = q.GetIntField("flags");
	switch(flag)
	{
	case 0:
	case 2:
		msg->SetCategory(MessageData::INBOX);
		break;
	case 1:
		msg->SetCategory(MessageData::DRAFT);
		break;
	case 3:
		msg->SetCategory(MessageData::OUTBOX);
		break;
	default:
		msg->SetCategory(MessageData::FAIL);
		break;
	}
}

void IosMessageDB::GetMadridChatInfos(string chat_id, MessageGroup* pGroup)
{
	CppSQLite3Buffer sql;
	sql.Format("select style,account_id,participants,account_login from madrid_chat where chat_identifier='%s' ORDER BY ROWID DESC", chat_id.c_str());
	CppSQLite3Query q = ExecQuery((const char*)sql);
	while (!q.Eof())
	{
		pGroup->_madridChat_accountId = q.GetStringField(1);
		pGroup->_madridchat_accountLogin = q.GetStringField(3);
		if(q.GetIntField(0) == MARDID_STYLE_CHATROOM)
		{
			int length = 0;
			const unsigned char* bin = q.GetBlobField(2, length);
			string pants; pants.assign((char*)bin, length);
			if(!pants.empty())
			{
				pants = CCFConverter::ConvertPlist(pants,false);//二进制转xml
				ParseNumberPlist(pants, pGroup->_members);
			}
		}
		else
			pGroup->_members.push_back(chat_id);
		q.NextRow();
	}
}

void IosMessageDB::GetGroupInfo(MessageGroup* pGroup)
{
	CppSQLite3Buffer sql;
	sql.Format("select type,hash from msg_group where ROWID=%d", pGroup->_groupId);
	CppSQLite3Query q = ExecQuery((const char*)sql);
	if (!q.Eof())
	{
		pGroup->_groupType = q.GetIntField(0)==1?MESSAGE_GROUP_MULII_MEMBER:MESSAGE_GROUP_SINGLE_MEMBER;
		pGroup->_hash = atol(q.GetStringField(1));
	}
	sql.Format("select address from group_member where group_id=%d ORDER BY ROWID DESC", pGroup->_groupId);
	q = ExecQuery((const char*)sql);
	while(!q.Eof())
	{
		pGroup->_members.push_back(GetPhoneNumberFormat(q.GetStringField(0), true));
		q.NextRow();
	}
}


void IosMessageDB::ParseNumberPlist(string plistcontent, vector<string>& nums)
{
	xml_document<char> doc;
	doc.parse<0>(const_cast<char *>(plistcontent.c_str()));
	xml_node<char> * nodeplist = doc.first_node("plist");
	xml_node<char> * nodearray = nodeplist->first_node("array");
	if(nodearray)
	{
		xml_node<char> * node= nodearray->first_node();
		while(node)
		{
			string number = node->value();
			string name = node->name();
			if(name == "string")
				nums.push_back(number);
			node = node->next_sibling();
		}
	}
}
bool IosMessageDB::GetMessagesSize(int* size)
{
    CppSQLite3Buffer sql;
    sql.Format("SELECT count(*) as MessageNums from message;");
    int count = ExecScalar((const char*)sql);
    if (count == -1)
        *size = 0;
    else
        *size = count;

    return true;
}

bool IosMessageDB::GetMessageSendFail(vector<MessageData*> *messageVec,time_t startTime,time_t endTime)
{
    CppSQLite3Buffer sql;
    sql.Format("select ROWID,address,date,text,flags,read from [message] where flags = 33 and date between %d and %d;",(int)startTime,(int)endTime);
    CppSQLite3Query q = ExecQuery((const char*)sql);

    while (!q.Eof())
    {
        const char* iosData[6] = {0};
        for (int i = 0; i < 6; i++)
            iosData[i] = q.GetStringField(i);

        MessageData* pApp = new MessageData(iosData);
        messageVec->push_back(pApp);

        q.NextRow();
    }
    return true;

}

int IosMessageDB::AddMessage(const vector<const MessageData*> *messageVec, ProgressSubject * pProgressSubject)
{
    // 判断用户是否取消，若取消则立即返回
    ProgressObserver* pObserver = NULL;
    if (!pProgressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(pProgressSubject->_observers[0]);

	CppSQLite3Buffer sql;

	vector<int> datatypes;
	map<string, int> namevalues;
	string  insertfields= GetTableFieldsAndDataTypes("message", namevalues, datatypes);
	int recipientFieldIndex = -1;
	map<string, int>::iterator it = namevalues.find("recipients");
	if(it != namevalues.end())
		recipientFieldIndex = it->second;

	CreateReadFunction();
    //DropSmsTriggers();
	vector<string> fields = CStrOperation::parseStrings(MESSAGE_INSERT_FIELDNAMES,',');
    size_t i = 0;
    for (i = 0; i < messageVec->size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)
        
        const MessageData *pMessage = messageVec->at(i);
		vector<string> values;
		MessageGetFieldValues(values, pMessage);

		map<int, string> indexValues = SetMessageValues(namevalues, fields, values);

		string  insertvalue;
		map<int, string>::iterator itd = indexValues.begin();
		for(size_t k = 0; k < datatypes.size(); k++)
		{
			if (itd != indexValues.end())
			{
				insertvalue +=CovertByType(datatypes[k],itd->second)+",";
			}
			itd++;
		}
		insertvalue = CStrOperation::trimRight(insertvalue, ",");

		
		sql.Format("insert or replace into [message](%s) Values(%s)",insertfields.c_str(), insertvalue.c_str());
		ExecDML((const char*)sql);
		
		sql.Format("SELECT seq from sqlite_sequence where name = 'message';");
		int newmessageId = ExecScalar((const char*)sql);

		// 插入彩信
		if(recipientFieldIndex>-1 && pMessage->_msgType == MessageData::EMMS)
		{
			UpdateMMSRecipients(newmessageId, pMessage);
			const vector<MMSItem*>* mmsitems = &pMessage->_mmsItems;
			for (size_t i = 0; i < mmsitems->size();i++)
			{
				InsertMMSItem(newmessageId, mmsitems->at(i), pMessage->GetCategory() != MessageData::INBOX);
			}
		}
		// 插入imessage
		UpdateImessageInfoInMessageTable(newmessageId, pMessage);
        
        pProgressSubject->IncreaseTaskNumberComplete();
        pProgressSubject->SendProgress();

        pProgressSubject->SetCurrentTaskSum(1);
        pProgressSubject->SetCurrentTaskName(CCodeOperation::UTF_8ToUnicode(pMessage->GetStrNumber()));

        pProgressSubject->AddTaskProgress(1);
        pProgressSubject->SendProgress();
    }
    //CreateSmsTriggers();
    //RETURN_FALSE_IF(pObserver);
    return (int)i;
}

void IosMessageDB::UpdateMMSRecipients(int msgId, const MessageData* pMessage)
{
	if(pMessage->_msgType != MessageData::EMMS)
		return;
	string recipients = "";
	vector<string> numbers = pMessage->GetNumbers();
	if (pMessage->GetCategory() == MessageData::INBOX)
		numbers.erase(numbers.begin());
	recipients = CreateRecipientsPlist(numbers);

	CppSQLite3Buffer sql;
	sql.Format("update [message] set recipients=? where ROWID = %d;", msgId);
	SaveBlob((const char*)sql, recipients.c_str(), recipients.length());
}

string IosMessageDB::GetTableFieldsAndDataTypes(string tablename, map<string, int>& namevalues, vector<int>& datatypes)
{
	CppSQLite3Buffer sql;
	sql.Format("select * from %s ORDER BY ROWID DESC", tablename.c_str());
	CppSQLite3Query q = ExecQuery((const char*)sql);

	string  insertfields;
	int columns = q.NumFields();
	for(int j =1; j < q.NumFields();j++)
	{
		string fieldname = q.FieldName(j);
		insertfields += "[";
		insertfields += fieldname;
		insertfields += "],";
		datatypes.push_back(q.FieldDataRealType(j));
		namevalues[fieldname] = j-1;
	}
	insertfields = CStrOperation::trimRight(insertfields, ",");
	return insertfields;
}


void IosMessageDB::MessageGetFieldValues(vector<string>& values, const MessageData *pMessage)
{
	CppSQLite3Buffer sql;
	int groupId = 0; 
	string subject = "";
	string address = pMessage->GetStrNumber();
	time_t tm=pMessage->GetTime();
	vector<string> numbers = pMessage->_numbers;
	if(pMessage->_msgType == MessageData::EMMS)
	{
		if (pMessage->GetCategory() == MessageData::INBOX)
			numbers.erase(--numbers.end());
		subject = MyCodeOperation::UnicodeToUTF_8SQL(pMessage->GetText());
	}
	if(pMessage->_msgType != MessageData::EIMS)
	{
		groupId = GetGroupIdsByNumber(numbers);
		if (groupId == -1&&!numbers.empty())
		{
			groupId = InsertGroup(numbers, 0);
		}
	}
	else
		tm -= CDealTime::IosStartTime().GetTime();//imessage为 ios时间，groupId为0
	int flags = GetCategoryFlags(pMessage);
	values.push_back(address.c_str());//address
	values.push_back((const char*)sql.Format("%d",(int)tm));//date
	values.push_back(pMessage->_msgType == MessageData::EMMS?"":MyCodeOperation::UnicodeToUTF_8SQL(pMessage->GetText()).c_str());//text
	values.push_back((const char*)sql.Format("%d", flags));//flags
	values.push_back((const char*)sql.Format("%d", groupId));//groupid
	values.push_back(pMessage->_msgType != MessageData::EIMS?"cn":"");//country
	values.push_back((const char*)sql.Format("%d",(flags == 0 ? 0 : 1)));//read
	values.push_back(pMessage->_msgType == MessageData::ESMS?"4":"0");//UIFlags
	values.push_back(subject);//subject
}


map<int, string> IosMessageDB::SetMessageValues(map<string, int> nameValues, vector<string> filedNames, vector<string> values)
{
	map<int, string> indexValues;
	for(size_t i = 0; i < nameValues.size(); i++)
	{
		indexValues[i] =  "";
	}
	for(size_t i = 0; i < filedNames.size(); i++)
	{
		map<string, int>::iterator it = nameValues.find(filedNames[i]);
		if(it != nameValues.end())
		{
			if(i< values.size())
				indexValues[it->second] = values[i];
		}
	}
	return indexValues;
}


string IosMessageDB::CovertByType(int type, string fieldvalue)
{
	string result;
	CppSQLite3Buffer sql;
	switch(type)
	{
		case SQLITE_INTEGER: 
			{
				result = (const char*)sql.Format("%d",fieldvalue.empty()?0:atoi(fieldvalue.c_str()));
			}
			break;
		case SQLITE_FLOAT:
			{
				result = (const char*)sql.Format("%f",fieldvalue.empty()?0.0:atof(fieldvalue.c_str()));
			}
			break;
		case SQLITE_BLOB: 
			{
				result = (const char*)sql.Format("'%s'",fieldvalue.empty()?"":fieldvalue.c_str());
			}
			break;
		case SQLITE_NULL: 
			{
				result = (const char*)sql.Format("%d",fieldvalue.empty()?0:atoi(fieldvalue.c_str()));
			}
			break;
		case SQLITE_TEXT:
			{
				result = sql.Format("'%s'",fieldvalue.empty()?"":fieldvalue.c_str());
			}
			break;
		default:
			break;
	}
	return result;
}

int IosMessageDB::DeleteMessage(const MessageData* pMsg)
{
	CreateReadFunction();
    CppSQLite3Buffer sql;
    string sqlstr = sql.Format("Delete From [message] where ROWID = %d;", pMsg->_id);
    int row = ExecDML((const char*)sql);
	if(_iosDBPath.GetProductVersion() >= PRODUCTVERSIONH && pMsg->_msgType == MessageData::EMMS)
	{
		sql.Format("delete from [msg_pieces] where message_id=%d;",pMsg->_id);
		ExecDML((const char*)sql);
	}
		
	sql.Format("delete from [msg_group];");
	ExecDML((const char*)sql);

	sql.Format("delete from [group_member];");
	ExecDML((const char*)sql);

    return row;
}

int IosMessageDB::UpdateMessageByContent(const vector<const MessageData*> *messageVec,ProgressSubject * pProgressSubject)
{
    // 判断用户是否取消，若取消则立即返回
    ProgressObserver* pObserver = NULL;
    if (!pProgressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(pProgressSubject->_observers[0]);

    //DropSmsTriggers();
    CreateReadFunction();
    size_t i = 0;
    for (i = 0; i < messageVec->size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)

        const MessageData *pMessage = messageVec->at(i);

        CppSQLite3Buffer sql;
        if (_iosDBPath.GetProductVersion() >= PRODUCTVERSIONL && _iosDBPath.GetProductVersion() < 500)
            sql.Format("update [message] set text = '%s', height = 0 where ROWID = '%d';",MyCodeOperation::UnicodeToUTF_8SQL(pMessage->GetText()).c_str(),pMessage->GetId());
        else
            sql.Format("update [message] set text = '%s' where ROWID = '%d';",MyCodeOperation::UnicodeToUTF_8SQL(pMessage->GetText()).c_str(),pMessage->GetId());
        ExecDML((const char*)sql);

        pProgressSubject->IncreaseTaskNumberComplete();
        pProgressSubject->SendProgress();

        pProgressSubject->SetCurrentTaskSum(1);
		pProgressSubject->SetCurrentTaskName(CStrOperation::IntToWString(pMessage->GetId()));

        pProgressSubject->AddTaskProgress(1);
        pProgressSubject->SendProgress();
    }
    //CreateSmsTriggers();
    return (int)i;
}

int IosMessageDB::UpdateMessageByIsRead(const vector<const MessageData*> *messageVec,ProgressSubject * pProgressSubject)
{
    // 判断用户是否取消，若取消则立即返回
    ProgressObserver* pObserver = NULL;
    if (!pProgressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(pProgressSubject->_observers[0]);

    //DropSmsTriggers();
    CreateReadFunction();
    size_t i = 0;
    for (i = 0; i < messageVec->size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)

        const MessageData *pMessage = messageVec->at(i);

        CppSQLite3Buffer sql;
        int flags = (pMessage->GetRead() == true ? 0 : 2);
        if (_iosDBPath.GetProductVersion() >= PRODUCTVERSIONL)
            sql.Format("update [message] set flags = %d,read = '%d' where ROWID = '%d';",
            flags,
            (flags == 0 ? 0 : 1),
            pMessage->GetId());
        ExecDML((const char*)sql);

        pProgressSubject->IncreaseTaskNumberComplete();
        pProgressSubject->SendProgress();

        pProgressSubject->SetCurrentTaskSum(1);
		pProgressSubject->SetCurrentTaskName(CStrOperation::IntToWString(pMessage->GetId()));

        pProgressSubject->AddTaskProgress(1);
        pProgressSubject->SendProgress();
    }
    //CreateSmsTriggers();
    return (int)i;
}

// 修复数据库
bool IosMessageDB::FixDB()
{
	if(_iosDBPath.GetProductVersion() >= 500)
		return false;

    // 修正短信记录可能为负数的bug
    bool needRepairCount = CheckSmsCount();
    bool needRepairGroup = CheckSmsGroup();

    if (needRepairCount || needRepairGroup)
        RecountSms();

    // 删除无用的冗余记录
    /*if (_iosDBPath.GetProductVersion() < PRODUCTVERSIONH)
    {
        string sql = "Delete From [message] where trim(address)='' OR address is null";
        //DropSmsTriggers();
        CreateReadFunction();
        ExecDML(sql.c_str());
        //CreateSmsTriggers();
    }*/

    return true;
}

// 检查短信记数是否正确
bool IosMessageDB::CheckSmsCount()
{
    if (_iosDBPath.GetProductVersion() < PRODUCTVERSIONL)
        return false;

    // 清除号码为空的冗余数据
    string sql = "DELETE from message where ifnull(address,'')='' ;";
    //DropSmsTriggers();
    CreateReadFunction();
    ExecDML(sql.c_str());
    //CreateSmsTriggers();

    // 未读短信数为负数
    sql = "SELECT ifnull( sum( CASE WHEN unread_count < 0 THEN 1 ELSE 0 END ) , 0 ) AS unread_count FROM [msg_group] ;";
    if (GetSmsCount(sql.c_str()) > 0)
        return true;

    // group_member 中有不存在与于msg_group中的groupid
    sql = "select count(group_id) as no_group from [group_member] where group_id not in (select rowid from [msg_group]) ;";
    if (GetSmsCount(sql.c_str()) > 0)
        return true;

    // message中有有不存在与于msg_group中的groupid
    sql = "SELECT count(group_id) as grp_count FROM (SELECT DISTINCT [message].group_id AS group_id FROM [message] ) t1 WHERE t1.group_id NOT IN ( SELECT ROWID AS group_id FROM [msg_group] ) ;";
    if (GetSmsCount(sql.c_str()) > 0)
        return true;

    // 未读短信数变化
    sql = "SELECT t1.unread_count - t2.unread_count AS unread_count FROM ( SELECT IFNULL( sum( unread_count ) , 0 ) AS unread_count FROM [msg_group] ) t1 , ( SELECT IFNULL( sum( CASE WHEN [message].flags = 0 THEN 1 ELSE 0 END ) , 0 ) AS unread_count FROM [message] ) t2 ;";
    if (GetSmsCount(sql.c_str()) != 0)
        return true;

    return false;
}

// 检查短信分组是否正确
bool IosMessageDB::CheckSmsGroup()
{
    if (_iosDBPath.GetProductVersion() < PRODUCTVERSIONL)
        return false;

    bool isNeedGroup = false;

    //113~2.2.1区间固件需要判断type字段，若为null表示需要进行分组修复
    string sql = "select count(type) from [msg_group] where ifnull(type,'')='' ;";
    if (GetSmsCount(sql.c_str()) > 0)
        isNeedGroup = true;

    //3.X之后的固件还需要判断Hash字段若为null或0表示需要进行分组修复
    if (_iosDBPath.GetProductVersion() < PRODUCTVERSIONH)
        isNeedGroup = false;

    map<string, long> mapHash;
    sql = "Select [msg_group].hash, [group_member].address From [msg_group] Left Join [group_member] On [msg_group].ROWID = [group_member].group_id where [msg_group].type != 1 AND [msg_group].hash is not null";
    GetSmsGroup(sql.c_str(), &mapHash);

    std::map<string, long>::iterator iter;
    for(iter = mapHash.begin(); iter != mapHash.end(); iter++)
    {
        string address = iter->first;
		long hash = iter->second < 0 ? iter->second*-1 : iter->second;
        address = GetPhoneNumberFormat(address,true);

        std::map<string,long>::iterator iter = _mapHash.find(address);
        if (hash != 0 && iter == _mapHash.end())
            _mapHash.insert(pair<string, long>(address, hash));
        else if (hash == 0 && iter != _mapHash.end())
            isNeedGroup = true;
    }

    if (isNeedGroup != false)
        return isNeedGroup;

    //检测同一个联络人是否存在两个以上的对话框
    vector<string> vecAddress;
    sql = "SELECT [group_member].address FROM [group_member] LEFT JOIN [msg_group] ON [group_member].group_id = [msg_group].rowid WHERE [msg_group].type = 0 ;";
    GetSmsGroup(sql.c_str(), &vecAddress);

    map<string, string> mapAddress;
    for (size_t i = 0; i < vecAddress.size(); i++)
    {
        string temp = vecAddress.at(i);
        if (temp.length() <= 0)
            continue ;

        std::map<string, string>::iterator iter = mapAddress.find(temp);
        if (iter == mapAddress.end())
            mapAddress.insert(pair<string, string>(temp, temp));
        else
            isNeedGroup = true;  
    }

    return isNeedGroup;
}

// 获取短信记数
int IosMessageDB::GetSmsCount(const char* sql)
{    
    int count = 0;
    CppSQLite3Query q = ExecQuery(sql);
    while (!q.Eof())
    {
        count = q.GetIntField(0);
        q.NextRow();
    }
    return count;
}

// 获取短信分组
bool IosMessageDB::GetSmsGroup(const char* sql, map<string, long> *mapHash)
{
    CppSQLite3Query q = ExecQuery(sql);
    while (!q.Eof())
    {
        mapHash->insert(pair<string, long>(q.GetStringField(1), q.GetIntField(0)));
        q.NextRow();
    }
    return true;
}

bool IosMessageDB::GetSmsGroup(const char* sql, vector<string> *vecAddress)
{
    CppSQLite3Query q = ExecQuery(sql);
    while (!q.Eof())
    {
        vecAddress->push_back(q.GetStringField(0));
        q.NextRow();
    }
    return true;
}

string IosMessageDB::GetPhoneNumberFormat(string tel, bool trimPrefixion)
{
	if (tel.empty()||IsEmail(tel))
		return tel;
    string startStr[8] = {"+86","+852","+886","+853","+1","+7","+44","+61"};
    for (int i = 0; i < 8; i++)
    {
        if(CStrOperation::startsWith(tel, startStr[i]))
            tel = CStrOperation::trimLeft(tel, startStr[i]);
    }

    // 去除非数字，比如电话号码里有空格或者‘-’之类的 包括开头的那个+
    string strTemp = "";
    char strChar;
    for (int i = 0; i < (int)tel.size(); i++)
    {
        strChar = tel[i];
        if (strChar != ' ' && strChar != '+' && strChar != '-' && strChar != ',')
        {
            strTemp += strChar; 
        }
    }

    // 去除常用电话号码前缀
    string tmpTel = strTemp;
    if (trimPrefixion)
    {
        string startStr[13] = {"12593","17909","79458","11618","197202","12591","17911","17951","17996","11808","10131","10193","12520"};
        for (int i = 0; i < 13; i++)
        {
            if(CStrOperation::startsWith(strTemp, startStr[i]))
                strTemp = CStrOperation::trimLeft(strTemp, startStr[i]);
        }
    }

    //如果在剔除了上述前缀后，发现号码为空，则还原剔除前缀前的号码，此时其并不为前缀
    if (strTemp.size() == 0)
        strTemp = tmpTel;

    strTemp = CStrOperation::trimRight(strTemp, " ");
    strTemp = CStrOperation::trimLeft(strTemp, " ");

    return strTemp;
}

// message表的字段flags: 3:发件 2:收件 33:发送失败 1:草稿箱 0:未读并且message表的read字段为0
int IosMessageDB::GetCategoryFlags(const MessageData* pData)
{
	if(pData->_msgType == MessageData::EIMS)
		return 0;
    if (pData->GetCategory() == MessageData::INBOX)
	{
		if (!pData->GetRead())
			return 0;
		else
			return 2;
	}
    else if (pData->GetCategory() == MessageData::OUTBOX)
        return 3;
    else if (pData->GetCategory() == MessageData::DRAFT)
		return 1;
	else if (pData->GetCategory() == MessageData::FAIL)
		return 33;
    else
        return 3;
}

//修复短信分组或计数问题for 113+
void IosMessageDB::RecountSms()
{
    //113固件之前不存在msg_group
    if (_iosDBPath.GetProductVersion() < PRODUCTVERSIONL) 
        return;

	if (_iosDBPath.GetProductVersion() > 300)
	{
		//TODO:

	}

    CppSQLite3Buffer sql;
    //sql.Format("DELETE FROM [msg_group];DELETE FROM [group_member];");
    //DropSmsTriggers();
    CreateReadFunction();
    //ExecDML((const char*)sql);
    //CreateSmsTriggers();

    //[message].association_id>0时，说明这两种短信为群发短信
    sql.Format(" SELECT [message].rowid , [message].address , [message].flags , \
               CASE WHEN ifnull([message].association_id,0)<>0 THEN [message].association_id ELSE 0 END AS groupid \
                FROM \
                [message] \
                ORDER BY \
                [message].group_id ;\
				DELETE FROM [msg_group] ;\
				DELETE FROM [group_member] ;");

    int rowId, flags,oldGroupId, newGroupId = 1;
    string address = "";

    CppSQLite3Query q = ExecQuery((const char*)sql);
    while (!q.Eof())
    {
        rowId = q.GetIntField(0);
        address = q.GetStringField(1);
        flags = q.GetIntField(2);
        oldGroupId = q.GetIntField(3);

        string key = GetPhoneNumberFormat(address, true);
        string prefixAddress = ReplaceSpaceChar(address);
        if (address == "" || rowId == 0 || flags == -1)
            continue ;

        //大于0为群发短信
        if (oldGroupId > 0)
            DealGroupMessage(oldGroupId, rowId, address, key, &newGroupId);
        else
            DealSingleMessage(oldGroupId, rowId, flags, address, key, prefixAddress, &newGroupId);

        q.NextRow();
    }

    //修复相同群发号码不在同一个分组问题
    DealGroupMessageNotInGroup(&newGroupId);

    // 修复短信数据库各个表
    DealMessageDB();
}

string IosMessageDB::ReplaceSpaceChar(string tel)
{
    string ret = "";
    for (size_t i = 0; i < tel.length(); i++)
    {
        //c>="0" && c<="9" || c=="+"
        if (tel[i] >= 48 && tel[i] <= 57 || tel[i] == 43)
            ret += tel[i];
    }
    return ret;
}

void IosMessageDB::DealGroupMessage(int oldGroupId, int rowId, string address, string key, int * newGroupId)
{
	string keyType = CStrOperation::Int2string(oldGroupId) + "tmp";

    std::map<string, SMSInfoClass>::iterator iter = _addressDic.find(keyType);
    if (iter == _addressDic.end())
    {
        SMSInfoClass smsInfo;
        smsInfo.GroupID = *newGroupId;
        smsInfo.GroupType = PersonGroup;
        smsInfo.NewestMsgID = rowId;
        smsInfo.UnreadCount = 0;
        smsInfo.AddressList.push_back(address);
        smsInfo.MsgIdList.push_back(rowId);
        _addressDic.insert(pair<string, SMSInfoClass>(keyType, smsInfo));

        (*newGroupId)++;
    }
    else
    {
        if (find(_addressDic[keyType].AddressList.begin(),_addressDic[keyType].AddressList.end(),key) == _addressDic[keyType].AddressList.end())
            _addressDic[keyType].AddressList.push_back(key);
        _addressDic[keyType].MsgIdList.push_back(rowId);
    } 
}

void IosMessageDB::DealSingleMessage(int oldGroupId, int rowId, int flags, string address, string key, string prefixAddress, int * newGroupId)
{
    std::map<string, SMSInfoClass>::iterator iter = _addressDic.find(key);
    if (iter == _addressDic.end())
    {
        SMSInfoClass smsInfo;
        smsInfo.GroupID = *newGroupId;
        smsInfo.GroupType = PersonSingle;
        smsInfo.NewestMsgID = rowId;
        smsInfo.UnreadCount = (flags == 0 ? 1 : 0);

        std::map<string, long>::iterator iter = _mapHash.find(key);
        if (iter != _mapHash.end())
            smsInfo.Hash = _mapHash[key];

        smsInfo.AddressList.push_back(address);
        smsInfo.MsgIdList.push_back(rowId);

        if (CStrOperation::startsWith(prefixAddress, "+"))
            smsInfo.IncludePrefixAddress = prefixAddress;
        else
            smsInfo.IncludePrefixAddress = key;

        _addressDic.insert(pair<string, SMSInfoClass>(key, smsInfo));
        (*newGroupId)++;
    }
    else
    {
        if (find(_addressDic[key].AddressList.begin(),_addressDic[key].AddressList.end(),address) == _addressDic[key].AddressList.end())
            _addressDic[key].AddressList.push_back(address);
        _addressDic[key].MsgIdList.push_back(rowId);
        _addressDic[key].NewestMsgID = rowId;
        _addressDic[key].UnreadCount += (flags == 0 ? 1 : 0);
        if (CStrOperation::startsWith(prefixAddress, "+") && _addressDic[key].IncludePrefixAddress.length() < prefixAddress.length())
            _addressDic[key].IncludePrefixAddress = prefixAddress;
    }
}

void IosMessageDB::DealGroupMessageNotInGroup(int *newGroupId)
{
    vector<string> lstDel;
    vector<string> lstKeys;

    for (std::map<string, SMSInfoClass>::iterator iter = _addressDic.begin(); iter != _addressDic.end(); ++iter)
        lstKeys.push_back(iter->first);

    for (size_t i = 0; i < lstKeys.size(); i++)
    {
        string keyTmp = lstKeys.at(i);
        if (_addressDic[keyTmp].GroupType != PersonGroup)
            continue ;

        if (_addressDic[keyTmp].AddressList.size() == 1)
            DealAddressCountIsOne(keyTmp, newGroupId, &lstDel);// 处理群发号码数是一的情况
        else
            DealAddressCountIsNotOne(keyTmp);// 处理群发号码数不是一的情况
    }

    for (size_t i = 0; i < lstDel.size(); i++)
        _addressDic.erase(lstDel.at(i));

    ClearGroupMessageAddress();// 清除群发号码
}

void IosMessageDB::DealAddressCountIsOne(string keyTmp,int *newGroupId, vector<string> *lstDel)
{
    string address = _addressDic[keyTmp].AddressList[0];
    int rowId = _addressDic[keyTmp].MsgIdList[0];
    string key = GetPhoneNumberFormat(address, true);
    lstDel->push_back(keyTmp);

    std::map<string, SMSInfoClass>::iterator iter = _addressDic.find(key);
    if (iter == _addressDic.end())
    {
        SMSInfoClass smsInfo;
        smsInfo.GroupID = *newGroupId;
        smsInfo.GroupType = PersonSingle;
        smsInfo.NewestMsgID = rowId;

        std::map<string, long>::iterator iter = _mapHash.find(key);
        if (iter != _mapHash.end())
            smsInfo.Hash = _mapHash[key];

        smsInfo.AddressList.push_back(address);
        smsInfo.MsgIdList.push_back(rowId);

        _addressDic.insert(pair<string, SMSInfoClass>(key, smsInfo));
        (*newGroupId)++;
    }
    else
    {
        if (find(_addressDic[key].AddressList.begin(),_addressDic[key].AddressList.end(),address) == _addressDic[key].AddressList.end())
            _addressDic[key].AddressList.push_back(address);
        _addressDic[key].MsgIdList.push_back(rowId);
    }
}

void IosMessageDB::DealAddressCountIsNotOne(string keyTmp)
{
    string tels = "";
    for (size_t i = 0; i < _addressDic[keyTmp].AddressList.size(); i ++)
    {
        string tel = _addressDic[keyTmp].AddressList.at(i);
        tels += (tels.length() == 0 ? "" : ",") + GetPhoneNumberFormat(tel, true);
    }

    std::map<string, vector<string>>::iterator iter = _dicGroupTels.find(tels);
    if (iter == _dicGroupTels.end())
    {
        vector<string> lstTel;
        lstTel.push_back(keyTmp);
        _dicGroupTels.insert(pair<string, vector<string>>(tels, lstTel));
    }
    else
    {
        _dicGroupTels[tels].push_back(keyTmp);
    }
}

void IosMessageDB::ClearGroupMessageAddress()
{
    for (std::map<string, vector<string>>::iterator iter = _dicGroupTels.begin(); iter != _dicGroupTels.end(); ++iter)
    {
        if ((iter->second).size() <= 1)
            continue ;

        SMSInfoClass info = _addressDic[(iter->second).at(0)];
        for (size_t K = 1; K < (iter->second).size(); K++)
        {
            SMSInfoClass otherInfo = _addressDic[(iter->second).at(K)];  
            for (size_t i = 0; i < otherInfo.MsgIdList.size(); i++)
            {
                int id = otherInfo.MsgIdList.at(i);
                if (find(info.MsgIdList.begin(),info.MsgIdList.end(),id) == info.MsgIdList.end())
                    info.MsgIdList.push_back(id);
                info.NewestMsgID = id;
            }
            _addressDic.erase((iter->second).at(K));
        }
    }
}

// 修复短信数据库各个表
void IosMessageDB::DealMessageDB()
{
    CppSQLite3Buffer sql;
    CreateReadFunction();
    //DropSmsTriggers();
    for (std::map<string, SMSInfoClass>::iterator iter = _addressDic.begin(); iter != _addressDic.end(); ++iter)
    {
		sql.Format("select count(*) from [group_member] where rowid = %d;", iter->second.GroupID);
		int count = ExecScalar((const char*)sql);

		sql.Format("select count(*) from [msg_group] where rowid = %d;", iter->second.GroupID);
		int gCount = ExecScalar((const char*)sql);
		if ( count > 0 || gCount > 0 ) continue;
		
        if (iter->second.GroupType == PersonGroup)
        {
            for (size_t i = 0; i < iter->second.AddressList.size(); i++)
            {
                sql.Format("insert into [group_member](group_id,address) values(%d,'%s');",iter->second.GroupID, iter->second.AddressList.at(i).c_str());
                ExecDML((const char*)sql);
            }
        }
        else
        {
            //非群发短信：[group_member]的address字段用IncludePrefixAddress填充(带前缀如：+86的电话号码)
            sql.Format("insert into [group_member](group_id,address) values(%d,'%s');",iter->second.GroupID, iter->second.IncludePrefixAddress.c_str());
            ExecDML((const char*)sql);
        }

		if (_iosDBPath.GetProductVersion() >= PRODUCTVERSIONH)
			sql.Format("insert into [msg_group](rowid,type,newest_message,unread_count,hash) values(%d,%d,%d,%d,%d);",iter->second.GroupID,(int)iter->second.GroupType,iter->second.NewestMsgID,iter->second.UnreadCount,iter->second.Hash);
		else
			sql.Format("insert into [msg_group](rowid,type,newest_message,unread_count) values(%d,%d,%d,%d);",iter->second.GroupID,(int)iter->second.GroupType,iter->second.NewestMsgID,iter->second.UnreadCount);
		ExecDML((const char*)sql);

        for (size_t i = 0; i < iter->second.MsgIdList.size(); i++)
        {
            sql.Format("update [message] set group_id = %d where ROWID = %d;", iter->second.GroupID, iter->second.MsgIdList.at(i));
            ExecDML((const char*)sql);
        }
    }
    //CreateSmsTriggers();
}

static void Read(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    RETURN_IF( 1 != argc );
    RETURN_IF( SQLITE_INTEGER != sqlite3_value_type(argv[0]) );

    __int64 flag = sqlite3_value_int64(argv[0]);

    sqlite3_result_int64(context, flag & 2);
}

void IosMessageDB::CreateReadFunction()
{
    RETURN_IF ( ! IsOpen() );

    sqlite3_create_function(
        GetSqlite3DB(),
        "read",
        1,
        SQLITE_UTF8,
        NULL,
        &Read,
        NULL,
        NULL
        );
}

bool ND91Assistant::IosMessageDB::IsEmail( string tel )
{
	match_results results;
	REGEX_FLAGS dw = GLOBAL | ALLBACKREFS;
	rpattern url_pattern(L"^([\\w-\\.]+)@((\\[[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.)|(([\\w-]+\\.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\\]?)$", dw);
	match_results::backref_type br = url_pattern.match(CCodeOperation::UTF_8ToUnicode(tel), results);
	return br.matched; 
}

const char* contentTypes[]={
	"application/smil",
	"image/jpeg",
	"text/plain",
	"text/x-vcard",
	"image/gif",
	"image/png",
	"text/vcard"
};

bool ND91Assistant::IosMessageDB::GetMessageMMSItem( int msgId, vector<MMSItem*>* itemVec )
{
	CppSQLite3Buffer sql;
	sql.Format("select * from msg_pieces where message_id=%d;", msgId);
	
	CppSQLite3Query q = ExecQuery((const char*)sql);
	map<int, MMSItem*> items;
	while(!q.Eof())
	{
		int partid = q.GetIntField("part_id");
		string  contentType = q.GetStringField("content_type");
		int found = -1;
		for(int i= 0; i < 7; i++)
		{
			if(contentType == contentTypes[i])
			{
				found = i;
				break;
			}
		}
		MMSItem* item = NULL;
		map<int, MMSItem*>::iterator it = items.find(partid);
		if (it != items.end())
		{
			item = it->second;
		}
		else
		{
			item = new MMSItem();
			items[partid] = item;
			item->_rowId = q.GetIntField("ROWID");
			item->_partId = partid;
			itemVec->push_back(item);
		}
		if(item->_fileName.empty())
			item->_fileName = CCodeOperation::UTF_8ToUnicode(q.GetStringField("content_loc"));
		int len = 0;
		const unsigned char* bin = q.GetBlobField("data", len);
		switch(found)
		{
			case 0:
				{
					item->_binaryData.assign((char*)bin, len);
					item->_type = SMSSMIL;
				}
				break;
			case 1:
			case 4:
				{
					item->_binaryData.assign((char*)bin, len);
					item->_type = SMSImage;
				}
				break;
			case 5:
				{
					item->_previewDataBin.assign((char*)bin, len);
					if(item->_type == Unknown)
						item->_type = SMSImage;
				}
				break;
			case 2:
				{
					item->_binaryData.assign((char*)bin, len);
					item->_type = SMSText;
				}
				break;
			case 3:
			case 6:
				{
					item->_binaryData.assign((char*)bin, len);
					item->_type = SMSVCard;
				}
				break;
			default:
				{
					item->_previewDataBin.assign((char*)bin, len);
				}
				break;
		}
		q.NextRow();
	}
	return true; 
}


void IosMessageDB::InsertMMSItem(int messageId, MMSItem* item, bool isOut)
{
	// 判断是否有 msg_pieces表
	CppSQLite3Buffer sql;
	sql.Format("select 1 from sqlite_master where type = 'table' and name = 'msg_pieces';");
	CppSQLite3Query q = ExecQuery((const char*)sql);
	RETURN_IF(q.Eof());

	string data = item->_binaryData;
	int flags = item->_type==SMSText?4:0;
	string preData, contentId;
	string content_type = item->_type != Unknown?contentTypes[item->_type]:"";
	switch(item->_type)
	{
		case SMSImage:
			{
				contentId = isOut?CStrOperation::Int2string(item->_partId+1):"<cidImage>";
				preData = item->_previewDataBin;
				if (preData.empty() && CStrOperation::endsWithW(item->_localFileName,L"-preview"))
					preData = CFileOperation::ReadFile(item->_localFileName, true);
				if (data.empty())
					data = CFileOperation::ReadFile(CStrOperation::ReplaceW(item->_localFileName,L"-preview",L".jpg"), true);
			}
			break;
		case SMSSMIL:
			contentId = isOut?CStrOperation::Int2string(item->_partId+1):"<start>";
			break;
		case SMSText:
		case SMSVCard:
			contentId = isOut?CStrOperation::Int2string(item->_partId+1):"<cidText>";
			break;
	}
	
	sql.Format("insert into [msg_pieces] (message_id,data,part_id,preview_part,content_type,height,version,flags,content_id,content_loc,headers)\
			   values (%d,?,%d,%d,'%s',0,%d,%d,'%s','%s','%s')", messageId, /*data.c_str(),*/ item->_partId, -1, content_type.c_str(), isOut?1:0, flags, contentId.c_str(), MyCodeOperation::UnicodeToUTF_8SQL(item->_fileName).c_str(),"");
	//ExecDML((const char*)sql);
	SaveBlob((const char*)sql,data.c_str(), data.length());

	sql.Format("select group_id from [message] where rowid = %d;", messageId);
	int groupId = ExecuteAndGetId(sql);
	wstring mmspath=MyFileOperation::ConvertMMSItemFolder(messageId, groupId);
	if(item->_type == SMSImage)
	{
		wstring newlocpath = _iosDBPath.GetTempFolder()+L"\\sms\\Parts\\"+mmspath+
			CStrOperation::IntToWString(messageId)+L"-"+CStrOperation::IntToWString(item->_partId);
		if(!data.empty())
			CFileOperation::WriteBinFile(newlocpath + L"."+CFileOperation::GetFileExt(item->_localFileName),data.c_str(), data.length());
		if(!preData.empty())
		{
			sql.Format("insert into [msg_pieces] (message_id,data,part_id,preview_part,content_type,height,version,flags,content_id,content_loc,headers)\
				    values (%d,?,%d,%d,'%s',0,%d,%d,'%s','%s','%s')", messageId, /*preData.c_str(),*/ item->_partId, 0, "image/png", 0, 0, "", "","");
			SaveBlob((const char*)sql,preData.c_str(), preData.length());
			CFileOperation::WriteBinFile(newlocpath+L"-preview",preData.c_str(), preData.length());
		}
	}
}

string IosMessageDB::CreateRecipientsPlist(vector<string> numbers)
{
	IosPlistHelper helper(XML_PI, XML_DOCTYPE, eXmlType_UTF8, XML_ELEMENT(L"plist", L"", L"version", L"1.0"));
	XML_NODE_INDEX index;
	helper.AddElement(index, XML_ELEMENT(XML_ARRAY));
	index.AddChild(0);
	for (size_t i = 0; i < numbers.size(); i++)
	{
		helper.AddElement(index, XML_ELEMENT(XML_STRING, CCodeOperation::UTF_8ToUnicode(numbers[i])));
	}
	return MyCodeOperation::UnicodeToUTF_8SQL(helper.GetXmlContent());
}

int ND91Assistant::IosMessageDB::DeleteMadridMessage(const MessageData* pImsg, const DeviceData* pDevice)
{
	CppSQLite3Buffer sql;
	sql.Format("select madrid_handle,madrid_roomname,madrid_attachmentInfo,madrid_flags from message where ROWID=%d",pImsg->_id);
	CppSQLite3Query q = ExecQuery(sql);
	string chat_id = "";
	string attach;
	int madridflags=0;
	if(!q.Eof())
	{
		chat_id = q.GetStringField(0);
		if(chat_id.empty())
			chat_id = q.GetStringField(1);
		int len = 0;
		const unsigned char* bin = q.GetBlobField(2, len);
		attach.assign((const char*)bin,len);
		madridflags = q.GetIntField(3);
	}
	CreateReadFunction();
	//删除imessage和聊天室信息？一个聊天室有多条imessage，删掉聊天室不妥
	sql.Format("delete from message where ROWID=%d;",pImsg->_id);
	ExecDML(sql);
	sql.Format("select count(*) from message where madrid_handle = '%s' OR madrid_roomname = '%s'", chat_id.c_str(), chat_id.c_str());
	int count = ExecuteAndGetId(sql);
	if (count > 0)
	{
		sql.Format("delete from madrid_chat where chat_identifier='%s';",chat_id.c_str());
		ExecDML(sql);
	}
	// 取得附件guid，删除附件
	vector<string> madridGuids= GetAttachmentGuid(attach, madridflags);
	for (size_t i=0; i< madridGuids.size();i++)
	{
		sql.Format("delete from [madrid_attachment] where attachment_guid='%s';", madridGuids[i].c_str());
		ExecDML(sql);
		FileHelper::Delete(pDevice,_iosDBPath.GetDBFolder()+L"/"+CCodeOperation::UTF_8ToUnicode(madridGuids[i]));
	}

	return 0;
}

vector<string> ND91Assistant::IosMessageDB::GetAttachmentGuid(string madrid_attachmentInfo, int madridflags)
{
	vector<string> guidvec;
	if(madrid_attachmentInfo.empty())
		return guidvec;
	int countpos= 0, startpos=0;
	if(madridflags == MADRID_FLAGS_RECEIVE)
	{
		countpos = MADRID_RECEIVE_ATATCHGUID_COUNTPOS;
		startpos = MADRID_RECEIVE_ATATCHGUID_STARTPOS;
	}
	else //if(madridflags == 36869)
	{
		countpos = MADRID_SEND_ATATCHGUID_COUNTPOS;
		startpos = MADRID_SEND_ATATCHGUID_STARTPOS;
	}
	int len = madrid_attachmentInfo.length();
	if(len < startpos)
		return guidvec;
	int imgcount = (unsigned short)(madrid_attachmentInfo[countpos]);
	for (int index = 0; index < imgcount; index++)
	{
		int startIndex = startpos + index*42;
		string guid= madrid_attachmentInfo.substr(startIndex,MADRID_ATTACHGUID_LENGTH);
		guidvec.push_back(guid);
	}
	return guidvec;
}

int IosMessageDB::GetGroupIdsByNumber(vector<string>numbers)
{
	if(numbers.empty())
		return -1;
	CppSQLite3Buffer sql;
	string likefieldstr;
	vector<string> formatNumbers;
	for (size_t i=0; i < numbers.size();i++)
	{
		string likestr= GetPhoneNumberFormat(numbers[i],true);
		formatNumbers.push_back(likestr);
		if(likestr.length() >= 7)
		{
			likestr = likestr.substr(0,3)+"%"+likestr.substr(likestr.length()-4,4);
			likestr = "%"+likestr;
		}
		likefieldstr += sql.Format("group_member.address like '%s' OR ",likestr.c_str());
	}
	likefieldstr = CStrOperation::trimRight(likefieldstr," OR ");
	sql.Format("select group_member.address,group_member.group_id from group_member,msg_group\
		   where msg_group.ROWID=group_member.group_id AND msg_group.type=%d AND (%s) order by group_member.group_id;",
		   numbers.size() < 2?0:1, likefieldstr.c_str());

	CppSQLite3Query q = ExecQuery((const char*)sql);
	int groupId = -1;
	map<int,vector<int>> ids;
	while(!q.Eof())
	{
		string number = GetPhoneNumberFormat(q.GetStringField(0),true);
		for (size_t i=0; i < formatNumbers.size();i++)
		{
			if(number == formatNumbers[i])
			{
				groupId = q.GetIntField(1);
				if(numbers.size()==1)
					return groupId;
				map<int,vector<int>>::iterator it= ids.find(groupId);
				if(it ==ids.end())
				{
					vector<int> nums;nums.push_back(i);
					ids[groupId]=nums;
				}
				else
					it->second.push_back(i);
			}
		}
		q.NextRow();
	}
	map<int,vector<int>>::iterator it= ids.begin();
	while(it!=ids.end())
	{
		if(it->second.size() == numbers.size())
			return it->first;
		it++;
	}
	return groupId;
}


int ND91Assistant::IosMessageDB::InsertGroup( const MessageGroup* pGroup )
{
	int groupId = 0;
	switch(pGroup->_groupType)
	{
		case MESSAGE_GROUP_MULII_MEMBER:
		case MESSAGE_GROUP_SINGLE_MEMBER:
			{
				int type = pGroup->_groupType ==MESSAGE_GROUP_MULII_MEMBER?1:0 ;
				int groupId = GetGroupIdsByNumber(pGroup->_members);
				if(groupId == -1)
				{
					groupId = InsertGroup(pGroup->_members, pGroup->_hash);
				}
			}
			break;
		case MESSAGE_MADRID_CHATROOM:
		case MESSAGE_MADRID_HANDLE:
			{
				InsertMadridChat(pGroup);
			}
			break;
		default:
			break;
	}
	return groupId;
}

int ND91Assistant::IosMessageDB::InsertGroup(vector<string> members, int hash)
{
	CreateReadFunction();
	int groupId=-1;
	try
	{
		CppSQLite3Buffer sql;
		sql.Format("insert into msg_group (type,newest_message,unread_count,hash) values (%d,%d,0,%d);",members.size()>1?1:0, 0, hash);
		ExecDML(sql);
		groupId=ExecuteAndGetId(sql.Format("select seq from sqlite_sequence where name='msg_group'"));
		if ( groupId ==-1 )
			return groupId;
		for (size_t i=0; i<members.size();i++)
		{
			sql.Format("insert into group_member (group_id,address,country) values (%d,'%s','%s');",groupId, members[i].c_str(),"cn");
			ExecDML(sql);
		}
	}
	catch(CppSQLite3Exception& e)
	{
		int iError = e.ErrorCode();
		const char* szError = e.ErrorMessage();
	}
	return groupId;
}

void ND91Assistant::IosMessageDB::UpdateImessageInfoInMessageTable( int msgId, const MessageData* pMessage )
{
	RETURN_IF(_iosDBPath.GetProductVersion() < 500 || !pMessage ||!pMessage->_iMsgInfo);
	CppSQLite3Buffer sql;
	ImessageInfo* pInfo = pMessage->_iMsgInfo;
	string fields = sql.Format("madrid_attributedBody =?,");
	string madrid_handle = pInfo->_isChatRoom?"":pInfo->_chat_id;
	fields += sql.Format("madrid_handle = '%s',", madrid_handle.c_str());
	fields += sql.Format("madrid_guid = '%s',", pInfo->_madrid_guid.c_str());
	fields += sql.Format("madrid_type=%d,", pInfo->_isChatRoom?1:0);
	string madrid_roomname = pInfo->_isChatRoom?pInfo->_chat_id:"";
	fields += sql.Format("madrid_roomname='%s',", madrid_roomname.c_str());
	fields += sql.Format("madrid_service='%s',", "Madrid");
	sql.Format("select account_id,account_login from madrid_chat where chat_identifier='%s'",pInfo->_chat_id.c_str());
	CppSQLite3Query q = ExecQuery(sql);
	if(!q.Eof())
	{
		string madrid_account_guid = q.GetStringField(0);
		string madrid_account = q.GetStringField(1);
		fields += sql.Format("madrid_account='%s',", madrid_account.c_str());
		fields += sql.Format("madrid_account_guid='%s',", madrid_account_guid.c_str());
	}
	fields += sql.Format("madrid_flags=%d,", pInfo->_madrid_flags);
	fields += sql.Format("is_madrid=%d,", 1);
	fields += sql.Format("madrid_date_delivered=%d", pInfo->_madrid_date_deliveredp);
	sql.Format("Update [message] Set %s where ROWID=%d", fields.c_str(), msgId);
	SaveBlob(sql, pInfo->_madrid_attributedBody.c_str(), pInfo->_madrid_attributedBody.length());

	if(!pInfo->_madrid_attachmentInfo.empty())
	{
		sql.Format("Update [message] Set madrid_attachmentInfo=? where ROWID=%d", msgId);
		SaveBlob(sql, pInfo->_madrid_attachmentInfo.c_str(), pInfo->_madrid_attachmentInfo.length());
		vector<string> guids = GetAttachmentGuid(pInfo->_madrid_attachmentInfo, pInfo->_madrid_flags);
		if(guids.size()>0 && pInfo->_attachmentFiles.size()>0)
			InsertMadridAttachment(guids[0],pInfo->_attachmentFiles[0],pInfo->_madrid_flags==MADRID_FLAGS_RECEIVE);
	}

}

void ND91Assistant::IosMessageDB::GetImessageInfo(int msgId, ImessageInfo* pInfo, const DeviceData* pDevice, bool IsBak/*=false*/ )
{
	RETURN_IF(!pInfo);
	string fields="madrid_handle,madrid_guid,madrid_type,madrid_roomname,madrid_flags,madrid_attachmentInfo,madrid_date_delivered";
	if(IsBak)
		fields += ",madrid_attributedBody";
	CppSQLite3Buffer sql;
	sql.Format("select %s from message where ROWID=%d", fields.c_str(), msgId);
	CppSQLite3Query q = ExecQuery((const char*)sql);
	if(!q.Eof())
	{
		pInfo->_madrid_guid = q.GetStringField(1);
		pInfo->_isChatRoom = q.GetIntField(2)==1;
		int chatIdIndex=!pInfo->_isChatRoom?0:3;
		pInfo->_chat_id = q.GetStringField(chatIdIndex);
		pInfo->_madrid_flags = q.GetIntField(4);
		string attachmentInfo;
		int len = 0;
		const unsigned char* bin= q.GetBlobField(5,len);
		attachmentInfo.assign((const char*)bin,len);
		pInfo->_madrid_date_deliveredp = atol(q.GetStringField(6));
		if(IsBak)
		{
			pInfo->_madrid_attachmentInfo = attachmentInfo;
			bin= q.GetBlobField(7,len);
			pInfo->_madrid_attributedBody.assign((const char*)bin,len);
		}


		if(!pInfo->_attachmentFiles.empty() || attachmentInfo.empty())
			return;
		vector<string> attachguids = GetAttachmentGuid(attachmentInfo, pInfo->_madrid_flags);
		for (size_t i = 0; i< attachguids.size();i++)
		{
			sql.Format("select filename from madrid_attachment where attachment_guid ='%s'", attachguids[i].c_str());
			q= ExecQuery(sql);
			if(!q.Eof())
			{
				wstring remotepath= CCodeOperation::UTF_8ToUnicode(q.GetStringField(0));
				wstring localpath= _iosDBPath.GetTempFolder()+L"sms\\Attachments\\"+CCodeOperation::UTF_8ToUnicode(attachguids[i]);
				localpath += L"\\"+CFileOperation::GetFileName(remotepath);
				remotepath = CStrOperation::ReplaceW(localpath,_iosDBPath.GetTempFolder()+L"sms",_iosDBPath.GetDBFolder());
				remotepath = CStrOperation::ReplaceW(remotepath, L"\\",L"/");
				DownloadFileCommand cmd(pDevice);
				cmd.Download(localpath,remotepath);
				if(CFileOperation::IsFileExist(localpath.c_str()))
					pInfo->_attachmentFiles.push_back(localpath);
			}
		}

	}
}

void ND91Assistant::IosMessageDB::InsertMadridChat( const MessageGroup* pGroup )
{
	CreateReadFunction();
	string chat_id = pGroup->_chatRoomName;
	if(chat_id.empty())
		chat_id = pGroup->_members[0];
	int style= pGroup->_groupType == MESSAGE_MADRID_CHATROOM?43:45;
	string guid = pGroup->_groupType == MESSAGE_MADRID_CHATROOM?"+":"-"+CStrOperation::toUpper(chat_id);
	CppSQLite3Buffer sql;
	sql.Format("select * from madrid_chat where chat_identifier='%s';", chat_id.c_str());
	CppSQLite3Query q = ExecQuery(sql);
	if(!q.Eof())
		return;
	sql.Format("insert into [madrid_chat] (style,state,account_id,properties,chat_identifier,service_name,guid,room_name,account_login,participants) \
			   values (%d,3,'%s','','%s','Madrid','%s','%s','%s',?);",
			   style, pGroup->_madridChat_accountId.c_str(), chat_id.c_str(), guid.c_str(),
			   pGroup->_chatRoomName.c_str(), pGroup->_madridchat_accountLogin.c_str());

	string participants = CreateRecipientsPlist(pGroup->_members);

	//xml转二进制
	string plistbin= CCFConverter::ConvertPlist(participants,true);
	
	SaveBlob(sql, plistbin.c_str(), plistbin.length());
}

void ND91Assistant::IosMessageDB::InsertMadridAttachment( string guid, wstring filePath, bool Iscoming)
{
	time_t nowtime;
	time(&nowtime);
	nowtime -= CDealTime::IosStartTime().GetTime();

	wstring filename = CFileOperation::GetFileName(filePath);
	string remotepath =	CCodeOperation::UnicodeToUTF_8(_iosDBPath.GetDBFolder())+"/Attachments/";
	remotepath += guid+"/"+CCodeOperation::UnicodeToUTF_8(filename);

	string utitype= "public."+CCodeOperation::UnicodeToUTF_8(CFileOperation::GetFileExt(filePath));

	CppSQLite3Buffer sql;
	sql.Format("insert into [madrid_attachment] \
			   (attachment_guid,created_date,start_date,filename,\
			    uti_type,mime_type,transfer_state,is_incoming,message_id) \
				values ('%s',%d,0,'%s','%s','%s',5,%d,-1);",
				guid.c_str(),nowtime,remotepath.c_str(),
				utitype.c_str(),"image/jpeg",Iscoming?1:0);
	ExecDML(sql);

	wstring localpath = _iosDBPath.GetTempFolder()+L"sms\\Attachments\\"+CCodeOperation::UTF_8ToUnicode(guid);
	CFileOperation::MakeLocalDir(localpath);
	localpath += L"\\"+filename;
    CFileOperation::CopyFile( filePath, localpath, true );
}


#endif