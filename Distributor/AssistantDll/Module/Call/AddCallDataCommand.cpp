#include "pch_module.h"

#ifdef MODULE_CALL

#include "AddCallDataCommand.h"
#include "IosCallDataDB.h"

#include "Module/Common/IosDB/IosDBPath.h"
#include "Module/AndroidDefine.h"

// 该最大值可在Option中设置，option中找不到则使用此默认值
#define ANDROID_DEFAULT_IMPORT_CALLDATA_COUNT  30

AddCallDataCommand::AddCallDataCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _success                = false;
    _dataList               = NULL;
    
    _nAndroidChannelID      = PIM;
    _nAndroidCommandID      = ADD_CALL;
    _executeCount           = 0;
    _executeCountEveryTime  = 0;
    
    if ( ! OPTION->Get(GlobalOption::INT_ANDROID_IMPORT_CALLDATA_COUNT, _maxCount) )
        _maxCount = ANDROID_DEFAULT_IMPORT_CALLDATA_COUNT;
}

AddCallDataCommand::~AddCallDataCommand()
{
    _executeDataList.clear();
}

std::string AddCallDataCommand::GetAndroidBodyData()
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
        const CallData* pData = _executeDataList.at(i);
        writer.WriteInt(pData->_rowId);
        writer.WriteString(pData->_phoneNumber);
        writer.WriteTime(pData->_date*1000);
		int flag=pData->_flags;
		time_t duration= pData->_duration;
		if(pData->_flags==CallData::MISSEDOUT)
		{
			flag=CallData::OUTGOING;
			duration=0;
		}
        writer.WriteInt(flag);
        writer.WriteTime(duration);

        IncreaseTaskNumberComplete();
        SendProgress();

        SetCurrentTaskSum(1);
        SetCurrentTaskName(CCodeOperation::UTF_8ToUnicode(pData->_phoneNumber));

        AddTaskProgress(1);
        SendProgress();
    }
    _executeCountEveryTime = (int)i;
    LOG->WriteDebug(L"std::string AddCallDataCommand::GetAndroidBodyData():_executeCountEveryTime:"+CStrOperation::IntToWString(_executeCountEveryTime));

    if (_executeCountEveryTime != (int)_executeDataList.size())
        writer.ReplaceInt(_executeCountEveryTime,8);

    return writer.GetBuffer();
}

void AddCallDataCommand::SetAndroidReturnData(std::string data)
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
    LOG->WriteDebug(L"void AddCallDataCommand::SetAndroidReturnData(std::string data):_executeCount:"+CStrOperation::IntToWString(_executeCount));
}

void AddCallDataCommand::SetDataList( const std::vector<const CallData*>* DataList )
{
    _dataList = DataList;
}

bool AddCallDataCommand::Success()
{
    return _success;
}

void AddCallDataCommand::Execute()
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

bool AddCallDataCommand::AndroidExecute()
{
    int count = 0;
    vector<const CallData*>::const_iterator it = _dataList->begin();
    while (  it != _dataList->end() )
    {
        IF_CANCEL_RETURN_FALSE(dynamic_cast<ProgressObserver*>(GetFirstObserver()));
        // 加此短消息放入待执行队列
        _executeDataList.push_back(*it);
        ++count;
		++it; 

		if(_pDevice->IsConnectRetry())
		{
			return false;
		}

        // 判断短消息的数量是否达到当条命令的上限，达到便执行命令，并将相关变量清零
        if ( count == _maxCount || (it == _dataList->end() && count >0) )
        {
            DefaultExecute();
            count = 0;
            _executeDataList.clear();
            RETURN_FALSE_IF(!_success)
			RETURN_FALSE_IF(!_executesuccess)
        }
    }
    return true;
}

bool AddCallDataCommand::IosExecute()
{
    // 没连接上设备则返回
    if (_pDevice->GetDevInfo() == NULL || !_pDevice->GetDevInfo()->_ios_bAFCConn)
        return false;

    // 修改数据库文件
    IosDBPath dbPath(_pDevice,ISCALLDATADB);
    IosCallDataDB callDataDB(dbPath);
    callDataDB.OpenDB();
    callDataDB.BeginTransaction();
    try
    {
        _executeCount = callDataDB.AddCallData(_dataList,(ProgressSubject*)this);
        callDataDB.Commit();
        callDataDB.CloseDB();
        return callDataDB.SyncToIos(_pDevice);// 同步到IOS设备
    }
    catch (CppSQLite3Exception& e)
    {
        int iError = e.ErrorCode();
        const char* szError = e.ErrorMessage();
        callDataDB.Rollback();
    }
    callDataDB.CloseDB();

    return false;
}

void AddCallDataCommand::ResetDataStatus()
{
    for (size_t i = _executeCount; i < _dataList->size(); i++)
    {
        const CallData* pCallData = _dataList->at(i);
        BaseData* pBase = (BaseData*)pCallData;
        pBase->SetStatus(BaseData::NO_CHANGE);
    }
}


#endif