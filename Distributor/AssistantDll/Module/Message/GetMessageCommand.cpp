#include "pch_module.h"

#ifdef MODULE_MESSAGE

#include "Module/Message/GetMessageCommand.h"
#include "Module/Message/MessageData.h"
#include "Module/Message/MessageGroup.h"
#include "Module/Message/IosMessageDB.h"

#include "Module/Common/IosDB/IosDBPath.h"
#include "Module/AndroidDefine.h"

GetMessageCommand::GetMessageCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = PIM;
    _nAndroidCommandID = GET_SMS;
    _onlySize          = false;
    _size              = 0;
	_dataList		   = NULL;
	_groupList         = NULL;
}

GetMessageCommand::~GetMessageCommand()
{

}

std::string GetMessageCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;

    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);

    return writer.GetBuffer();
}

void ND91Assistant::GetMessageCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader reader(data.c_str(), data.length());
    
    if (!parseAndroidPackageHead(&reader))
    {
        _result = COMMAND_EXECUTE_FAIL;
        return;
    }

    _size = 0;		//nCount 标识接下来记录的条数
    reader.ReadInt32(_size);
	if(_onlySize)
		return;
    for (int i=0; i<_size; i++)
    {
        _dataList->push_back(MessageData::FromBuffer(&reader));
    }
}


void ND91Assistant::GetMessageCommand::SetDataList( vector<MessageData*>* dataList, vector<MessageGroup*>* groupList )
{
	_dataList = dataList;
	_groupList = groupList;
}

void GetMessageCommand::SetOnlySize(bool onlySize)
{
    _onlySize = onlySize;
}

void GetMessageCommand::Execute()
{
    _result = COMMAND_EXECUTE_SUCCESS;
    if ( _pDevice->GetDevInfo()->_deviceType == Android )
        DefaultExecute();
    else if ( _pDevice->GetDevInfo()->_deviceType == Ios )
    {
        if (!IosExecute())
            _result = COMMAND_EXECUTE_FAIL;  
    }
}

int GetMessageCommand::GetSize()
{
	_onlySize = true;
	Execute();
    return _size;
}

bool GetMessageCommand::IosExecute()
{
    bool result = false;
    // 没连接上设备则返回
    if (_pDevice->GetDevInfo() == NULL || !_pDevice->GetDevInfo()->_ios_bAFCConn)
        return result;
	
	//__time64_t tm64;
	//_time64(&tm64);
    IosDBPath dbPath(_pDevice,ISMESSAGEDB);
    IosMessageDB messageDB(dbPath);
    // 打开数据库
    if (!messageDB.OpenDB())
        return result;
	//__time64_t endtm64;
	//_time64(&endtm64);

	//LOG->WriteDebug(L"down smsdb time "+CStrOperation::Int64ToWString(endtm64-tm64));

	if (_onlySize)
	{
		// 获取短信数量
		if (messageDB.GetMessagesSize(&_size))
			result = true;
		else
			result = false;
	}
	else
	{
		// 获取所有短信记录
		if (messageDB.GetMessages(_dataList, _groupList))
			result = true;
		else
			result = false;
		//_time64(&endtm64);

		//LOG->WriteDebug(L"load sms time "+CStrOperation::Int64ToWString(endtm64-tm64));
	}

    // 关闭数据库
    messageDB.Close();

    return result;
}

#endif