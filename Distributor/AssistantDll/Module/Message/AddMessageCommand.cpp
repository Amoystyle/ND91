#include "pch_module.h"

#ifdef MODULE_MESSAGE

#include "Module/Message/AddMessageCommand.h"
#include "Module/Message/MessageData.h"
#include "Module/Message/IosMessageDB.h"

#include "Module/Common/IosDB/IosDBPath.h"
#include "Module/AndroidDefine.h"

// 该最大值可在Option中设置，option中找不到则使用此默认值
#define ANDROID_DEFAULT_IMPORT_MESSAGE_COUNT  20

AddMessageCommand::AddMessageCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _success                = false;
    _dataList               = NULL;

    _nAndroidChannelID      = PIM;
    _nAndroidCommandID      = ADD_SMS;
    _executeCount           = 0;
    _executeCountEveryTime  = 0;
    if ( ! OPTION->Get(GlobalOption::INT_ANDROID_IMPORT_MESSAGE_COUNT, _maxCount) )
        _maxCount = ANDROID_DEFAULT_IMPORT_MESSAGE_COUNT;
}

AddMessageCommand::~AddMessageCommand()
{
    _executeDataList.clear();
}

std::string AddMessageCommand::GetAndroidBodyData()
{
    // 判断用户是否取消，若取消则立即返回
    ProgressObserver* pObserver = NULL;
    if (!_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

    BinaryBufferWriter writer;
    writer.Clear();
    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);
    writer.WriteInt(_executeDataList.size());

    IF_CANCEL(pObserver)
    {
        writer.ReplaceInt(0,8);
        return writer.GetBuffer();
    }

    size_t i = 0;
    for (i = 0; i < _executeDataList.size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)
        const MessageData* pData = _executeDataList.at(i);
        writer.WriteInt(pData->GetId());
        writer.WriteString(pData->GetStrNumber());
        writer.WriteTime(pData->GetTime()*1000);
        writer.WriteInt(pData->GetCategory());
        writer.WriteStringAsUTF8(pData->GetText());
        writer.WriteBool(pData->GetRead());

        IncreaseTaskNumberComplete();
        SendProgress();

        SetCurrentTaskSum(1);
        SetCurrentTaskName(CCodeOperation::UTF_8ToUnicode(pData->GetStrNumber()));

        AddTaskProgress(1);
        SendProgress();
    }
    _executeCountEveryTime = (int)i;
    LOG->WriteDebug(L"std::string AddMessageCommand::GetAndroidBodyData():_executeCountEveryTime:"+CStrOperation::IntToWString(_executeCountEveryTime));
    
    if (_executeCountEveryTime != (int)_executeDataList.size())
        writer.ReplaceInt(_executeCountEveryTime,8);

    return writer.GetBuffer();
}

void AddMessageCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader reader(data.c_str(), data.length());
    if (!parseAndroidPackageHead(&reader))
    {
        _success = false;
        return;
    }
    reader.ReadBool(_success);
    RETURN_IF(!_success);
    _executeCount += _executeCountEveryTime;
    LOG->WriteDebug(L"void AddMessageCommand::SetAndroidReturnData(std::string data):_executeCount:"+CStrOperation::IntToWString(_executeCount));
}

void AddMessageCommand::SetDataList( const std::vector<const MessageData*>* DataList )
{
    _dataList = DataList;
}

bool AddMessageCommand::Success()
{
    return _success;
}

void AddMessageCommand::Execute()
{
    if ( _dataList == NULL )
    {
        _success = true;
        Complete();
        return;
    }
    
    // 设置所有任务总量
    SetAllTaskSum(_dataList->size());
    // 设置任务总数
    SetTaskNumberSum(_dataList->size());

    if ( _pDevice->GetDevInfo()->_deviceType == Android )
        _success = AndroidExecute();        
    else if ( _pDevice->GetDevInfo()->_deviceType == Ios )
        _success = IosExecute();
}

bool AddMessageCommand::AndroidExecute()
{
    int count = 0;
    vector<const MessageData*>::const_iterator it = _dataList->begin();

    while (  it != _dataList->end() )
    {        
        IF_CANCEL_RETURN_FALSE(dynamic_cast<ProgressObserver*>(GetFirstObserver()));
        // 加此短消息放入待执行队列
        _executeDataList.push_back(*it);
        ++count;
		++it; 

		if(_pDevice->IsConnectRetry())
		{
			LOG->WriteDebug(L"设备连接失败");
			return false;
		}

        // 判断短消息的数量是否达到当条命令的上限，达到便执行命令，并将相关变量清零
        if ( count == _maxCount || (it == _dataList->end() && count >0) )
        {
            DefaultExecute();
			count = 0;
            _executeDataList.clear(); 
            RETURN_FALSE_IF(!_success)
        }
        
	}
    return true;
}

bool AddMessageCommand::IosExecute()
{
    // 没连接上设备则返回
    if (_pDevice->GetDevInfo() == NULL || !_pDevice->GetDevInfo()->_ios_bAFCConn)
        return false;

    // 修改数据库文件
    IosDBPath dbPath(_pDevice,ISMESSAGEDB);
    IosMessageDB messageDB(dbPath);
    messageDB.OpenDB();
    messageDB.BeginTransaction();
    try
    {
        // 取消还没添加的
        _executeCount = messageDB.AddMessage(_dataList, (ProgressSubject*)this);
        messageDB.Commit();
		messageDB.FixDB();
        messageDB.CloseDB();

		_executeCount = _dataList->size();
        return messageDB.SyncToIos(_pDevice);// 同步到IOS设备
    }
    catch (CppSQLite3Exception& e)
    {
        int iError = e.ErrorCode();
        const char* szError = e.ErrorMessage();
        messageDB.Rollback();

		_executeCount = 0;
    }
    messageDB.CloseDB();

    return false;
}


#endif