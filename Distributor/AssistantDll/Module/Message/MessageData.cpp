#include "pch_module.h"

#ifdef MODULE_MESSAGE

#include "Module/Message/MessageData.h"
#include "Module/Message/IosMessageDB.h"

#include "Module/Common/IosDB/IosDBPath.h"

MMSItem::MMSItem()
{
	_rowId  = -1;
	_partId = -1;
	_type	= Unknown;
}
MMSItem::~MMSItem()
{

}
MMSItem::MMSItem(MMSItem* pItem)
{
	_rowId = pItem->_rowId;
	_partId = pItem->_partId;
	_type = pItem->_type;
	_fileName = pItem->_fileName;
	_binaryData = pItem->_binaryData;
	_previewDataBin = pItem->_previewDataBin;
}

ImessageInfo::ImessageInfo()
{
	_isChatRoom = false;    // 是否多人聊天
	_madrid_flags = 36869;    // 
	_madrid_date_deliveredp = 0;//发送时间，备份导入用
}
ImessageInfo::~ImessageInfo()
{

}
ImessageInfo::ImessageInfo(ImessageInfo* pInfo)
{
	_chat_id = pInfo->_chat_id;       // 聊天id
	_madrid_guid = pInfo->_madrid_guid;   // 备份导入用
	_isChatRoom = pInfo->_isChatRoom;    // 是否多人聊天
	_madrid_attributedBody = pInfo->_madrid_attributedBody;		// 备份导入用
	_madrid_flags = pInfo->_madrid_flags;    // 
	_madrid_date_deliveredp = pInfo->_madrid_date_deliveredp;//发送时间，备份导入用
	_madrid_attachmentInfo = pInfo->_madrid_attachmentInfo; //导入用
	for (size_t i =0; i < pInfo->_attachmentFiles.size(); i++)
	{
		_attachmentFiles.push_back(pInfo->_attachmentFiles[i]);
	}
}


MessageData::MessageData()
{
    _id                 = 0;
    _threadID           = 0;
    _read               = false;
    _strNumber          = "";
    _text               = L"";
    _time               = 0;
    _category           = INBOX;
	_msgType            = ESMS;
	_groupId            = 0;
	_iMsgInfo           = NULL;
}

MessageData::MessageData(MessageData* pMessageData)
{
    _id                 = pMessageData->_id;
    _threadID           = pMessageData->_threadID;
    _read               = pMessageData->_read;
    _strNumber          = pMessageData->_strNumber;
    _text               = pMessageData->_text;
    _time               = pMessageData->_time;
    _category           = pMessageData->_category;
	_groupId            = pMessageData->_groupId;
	_msgType			= pMessageData->_msgType;

	for(size_t i = 0; i < pMessageData->_numbers.size(); i++)
	{
		_numbers.push_back(pMessageData->_numbers[i]);
	}

	for(size_t i =0; i < pMessageData->_mmsItems.size(); i++)
	{
		_mmsItems.push_back(new MMSItem(pMessageData->_mmsItems[i]));
	}
	_iMsgInfo = NULL;
	if(pMessageData->_iMsgInfo)
		_iMsgInfo = new ImessageInfo(pMessageData->_iMsgInfo);
}

MessageData::MessageData(const char* iosData[6])
{
	_msgType          = ESMS;
    _id               = atoi(iosData[0]);
    _strNumber        = iosData[1];
    _time             = atoi(iosData[2]);
    _text             = CCodeOperation::UTF_8ToUnicode(iosData[3]);

    // message表的字段flags: 3:发件 2:收件 33:发送失败 1:草稿箱 0:未读并且message表的read字段为0
    if (atoi(iosData[4]) == 2 || atoi(iosData[4]) == 0)
    {
        if (atoi(iosData[5]) == 0)
            _read = false;
        else
            _read = true;
        _category = INBOX;
    }
    else if(atoi(iosData[4]) == 3)
        _category = OUTBOX;
    else if(atoi(iosData[4]) == 1)
        _category = DRAFT;
    else
        _category = FAIL;
}

MessageData::~MessageData()
{
	for (size_t i = 0; i< _mmsItems.size(); i++)
	{
		SAFE_DELETE(_mmsItems[i]);
	}
	_mmsItems.clear();

	SAFE_DELETE(_iMsgInfo);
}

int MessageData::GetId() const
{
    return _id;
}

void MessageData::SetId(int id)
{
    _id = id;
}

int MessageData::GetThreadID() const
{
    return _threadID;
}

void MessageData::SetThreadID(int threadID)
{
    _threadID = threadID;
}

bool MessageData::GetRead() const
{
    return _read;
}

void MessageData::SetRead(bool read)
{
    _read = read;
}

string MessageData::GetStrNumber() const
{
    return _strNumber;
}

void MessageData::SetStrNumber(string strNumber)
{
    _strNumber = strNumber;
}

wstring MessageData::GetText() const
{
    return _text;
}
void MessageData::SetText(wstring text)
{
    _text = text;
}

time_t MessageData::GetTime() const
{
    return _time;
}

void MessageData::SetTime(time_t time)
{
    _time = time;
}

MessageData::MESSAGE_CATEGORY MessageData::GetCategory() const
{
    return _category;
}

void MessageData::SetCategory(MESSAGE_CATEGORY category)
{
    _category = category;
}

MessageData* MessageData::FromBuffer(BinaryBufferReader* pReader)
{
    MessageData* pMessage = new MessageData();

    pReader->ReadInt32(pMessage->_id);
    pReader->ReadInt32(pMessage->_threadID);
    pReader->ReadString(pMessage->_strNumber);
    pReader->ReadInt64(pMessage->_time);
    pMessage->_time /= 1000;
    int nType = Unknown;
    pReader->ReadInt32(nType);
    switch (nType)
    {
    case 1:
        pMessage->_category = INBOX;
        break;
    case 2:
        pMessage->_category = SENT;
        break;
    case 3:
        pMessage->_category = DRAFT;
        break;
    case 4:
        pMessage->_category = OUTBOX;
        break;
    case 5:
        pMessage->_category = FAIL;
        break;
    case 6:
        pMessage->_category = Unknown;
        break;
    default:
        break;
    }
    pReader->ReadStringFromUTF8(pMessage->_text);
    pReader->ReadBool(pMessage->_read);

    return pMessage;
}

MessageData* MessageData::FromDeamonNotifyBuffer(BinaryBufferReader* pReader)
{
    MessageData* pMessage = new MessageData();
    //线程id - 8
    __int64 nThreadID64;
    pReader->ReadInt64(nThreadID64);
    pMessage->_threadID = (int)nThreadID64;
    //短信id - 4
    pReader->ReadInt32(pMessage->_id);
    //类型 - 4
    int nType = Unknown;
    pReader->ReadInt32(nType);
    switch (nType)
    {
    case 1:
        pMessage->_category = INBOX;
        break;
    case 2:
        pMessage->_category = SENT;
        break;
    case 3:
        pMessage->_category = DRAFT;
        break;
    case 4:
        pMessage->_category = OUTBOX;
        break;
    case 5:
        pMessage->_category = FAIL;
        break;
    case 6:
        pMessage->_category = Unknown;
        break;
    default:
        break;
    }
    //号码 - 
    pReader->ReadString(pMessage->_strNumber);
    string strText;
    //内容
    pReader->ReadStringFromUTF8(pMessage->_text);
    //时间 - 8
    pReader->ReadInt64(pMessage->_time);

    return pMessage;
}

MessageData* MessageData::FromBuffer(string buffer)
{
    BinaryBufferReader reader(buffer.c_str(), buffer.size());

    return FromBuffer(&reader);
}

const vector<MMSItem*>* ND91Assistant::MessageData::GetMMSItems(const DeviceData* pDevice)
{
	if(!pDevice || pDevice->GetDevInfo()->_deviceType != Ios || !_mmsItems.empty())
		return &_mmsItems;
	IosDBPath dbPath(pDevice,ISMESSAGEDB);
	IosMessageDB messageDB(dbPath);
	if(messageDB.OpenDB())
	{
		messageDB.GetMessageMMSItem(_id, &_mmsItems);
		messageDB.Close();
		DownMMSItemImageFiles(pDevice);
		DownMMSItemVcardFile(pDevice);
	}
	return &_mmsItems;
}

wstring ND91Assistant::MessageData::DownMMSItemImageFiles( const DeviceData* pDevice )
{
	if(_msgType != MessageData::EMMS || _mmsItems.empty())
		return L"";
	int partId =-1;
	MMSItem* item = NULL;
	for(size_t i =0; i < _mmsItems.size(); i++)
	{
		item = _mmsItems[i];
		if(item->_type == SMSImage)
		{
			partId = item->_partId;
			break;
		}
	}
	if(partId == -1)
		return L"";
	wstring mmspath= MyFileOperation::ConvertMMSItemFolder(_id, _groupId);
	IosDBPath dbPath(pDevice,ISMESSAGEDB);
	//缩略图本地路径
	wstring foldername = dbPath.GetTempFolder()+L"sms\\Parts\\"+mmspath+
						 CStrOperation::IntToWString(_id)+L"-"+CStrOperation::IntToWString(partId)+ L"-preview";
	//缩略图手机端路径
	wstring remotepath = dbPath.GetDBFolder()+L"/Parts/"+CStrOperation::ReplaceW(mmspath,L"\\",L"/")+
						 CStrOperation::IntToWString(_id)+L"-"+CStrOperation::IntToWString(partId)+ L"-preview";
	DownloadFileCommand cmd(pDevice);
	if(!CFileOperation::IsFileExist(foldername.c_str()))//缩略图不存在下载
	{
		if(!item->_previewDataBin.empty())
			CFileOperation::WriteBinFile(foldername, item->_previewDataBin.c_str(), item->_previewDataBin.length());
		else
			cmd.Download(foldername, remotepath);
	}
	if(CFileOperation::IsFileExist(foldername.c_str()))
		item->_localFileName = foldername;

	// 下载原图
	foldername = CStrOperation::ReplaceW(foldername, L"-preview",L".jpg");
	remotepath = CStrOperation::ReplaceW(remotepath, L"-preview",L".jpg");
	if(!CFileOperation::IsFileExist(foldername.c_str()))
	{
		if(!item->_binaryData.empty())
			CFileOperation::WriteBinFile(foldername, item->_binaryData.c_str(), item->_binaryData.length());
		else
		{
			cmd.Download(foldername, remotepath);
			if(!CFileOperation::IsFileExist(foldername.c_str()))// 不存在尝试gif图
			{
				foldername = CStrOperation::ReplaceW(foldername, L".jpg",L".gif");
				remotepath = CStrOperation::ReplaceW(remotepath, L".jpg",L".gif");
				if(!CFileOperation::IsFileExist(foldername.c_str()))
					cmd.Download(foldername, remotepath);
			}
		}
	}
	if(item->_localFileName.empty() && CFileOperation::IsFileExist(foldername.c_str()))
		item->_localFileName = foldername;

	return item->_localFileName;
}

std::wstring ND91Assistant::MessageData::DownMMSItemVcardFile( const DeviceData* pDevice )
{
	if(_msgType != MessageData::EMMS || _mmsItems.empty())
		return L"";
	string vcardByte;
	int partId =-1;
	wstring filename;
	MMSItem* item = NULL;
	for(size_t i =0; i < _mmsItems.size(); i++)
	{
		item = _mmsItems[i];
		if(item->_type == SMSVCard)
		{
			vcardByte = item->_binaryData;
			partId = item->_partId;
			filename = item->_fileName;
			break;
		}
	}
	if(partId == -1)
		return L"";
	IosDBPath dbPath(pDevice,ISMESSAGEDB);
	wstring foldername = dbPath.GetTempFolder()+L"sms\\Parts\\"+MyFileOperation::ConvertMMSItemFolder(_id, _groupId)+filename;
	if(!vcardByte.empty())
	{
		CFileOperation::WriteBinFile(foldername, vcardByte.c_str(), vcardByte.length());
		item->_localFileName = foldername;
		return foldername;
	}
	return L"";
}

vector<string> ND91Assistant::MessageData::GetNumbers() const
{
	return _numbers;
}

void ND91Assistant::MessageData::InsertNumber(string number)
{
	for (size_t i = 0; i < _numbers.size(); i++)
	{
		if(_numbers[i]== number)
			return;
	}
	_numbers.push_back(number);
}

ND91Assistant::MessageData::MESSAGE_TYPE ND91Assistant::MessageData::GetMessageType() const
{
	return _msgType;
}

int ND91Assistant::MessageData::GetGroupId() const
{
	return _groupId;
}

const ImessageInfo* ND91Assistant::MessageData::GetImessageInfo( const DeviceData* pDevice, bool IsBak /*= false*/ )
{
	RETURN_NULL_IF(!pDevice || pDevice->GetDevInfo()->_deviceType != Ios);

	if(_msgType != EIMS)	// 不是iMessage固定返回NULL
		return NULL;

	if ( IsBak ) // 备份就重新取值（多次备份就重复取值，这里不予优化）
	{
		GetMessageData(pDevice, true);
		return _iMsgInfo;
	}

	// 不是备份：显示用
	if( ! _iMsgInfo )	// 没有就取内容
	{
		GetMessageData(pDevice, false);
	}

	return _iMsgInfo;
}

void ND91Assistant::MessageData::GetMessageData( const DeviceData* pDevice, bool isGetAllData )
{
	if( !_iMsgInfo )
	{
		_iMsgInfo = new ImessageInfo();
	}

	IosDBPath dbPath(pDevice,ISMESSAGEDB);
	IosMessageDB messageDB(dbPath);
	if(messageDB.OpenDB())
	{
		messageDB.GetImessageInfo(_id, _iMsgInfo, pDevice, isGetAllData);
		messageDB.Close();
	}
}


#endif