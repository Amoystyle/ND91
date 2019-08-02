#include "pch_module.h"

#ifdef MODULE_CALL

#include "DeleteCallDataCommand.h"
#include "IosCallDataDB.h"

#include "Module/Common/IosDB/IosDBPath.h"
#include "Module/AndroidDefine.h"

DeleteCallDataCommand::DeleteCallDataCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _success           = false;
    _dataList          = NULL;
    _nAndroidChannelID = PIM;
    _nAndroidCommandID = DEL_CALL;
    _executeCount      = 0;
}

DeleteCallDataCommand::~DeleteCallDataCommand()
{
}

std::string DeleteCallDataCommand::GetAndroidBodyData()
{
    // 判断用户是否取消，若取消则立即返回
    ProgressObserver* pObserver = NULL;
    if (!_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

    BinaryBufferWriter writer;
    writer.Clear();
    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);
    writer.WriteInt(_dataList->size());

    IF_CANCEL(pObserver)
        return writer.GetBuffer();

    size_t i = 0;
    for (i = 0; i < _dataList->size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)
        const CallData* pData = _dataList->at(i);
        writer.WriteInt(pData->_rowId);

        IncreaseTaskNumberComplete();
        SendProgress();

        SetCurrentTaskSum(1);
        SetCurrentTaskName(CCodeOperation::UTF_8ToUnicode(pData->_phoneNumber));

        AddTaskProgress(1);
        SendProgress();
    }
    _executeCount = (int)i;
    if (_executeCount != (int)_dataList->size())
        writer.ReplaceInt(_executeCount,8);

    return writer.GetBuffer();
}

void DeleteCallDataCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader reader(data.c_str(), data.length());
    if (!parseAndroidPackageHead(&reader))
    {
        _success = false;
        return;
    }
    reader.ReadBool(_success);
}

void DeleteCallDataCommand::SetDataList( const std::vector<const BaseData*>* DataList )
{
    _dataList = (const std::vector<const CallData*>*)DataList;
}

bool DeleteCallDataCommand::Success()
{
    return _success;
}

void DeleteCallDataCommand::Execute()
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
        DefaultExecute();
    else if ( _pDevice->GetDevInfo()->_deviceType == Ios )
        _success = IosExecute();

    ResetDataStatus();
}

bool DeleteCallDataCommand::IosExecute()
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
        vector<int> vecRowid;
        GetRowIdVector(&vecRowid);
        _executeCount = callDataDB.DeleteCall(vecRowid,(ProgressSubject*)this);
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

void DeleteCallDataCommand::GetRowIdVector(vector<int> *vecRowid)
{
    std::vector<const CallData*>::const_iterator iter;
    for(iter = _dataList->begin(); iter != _dataList->end(); iter++)
    {
        const CallData* pCallData =  *iter;
        int id = pCallData->_rowId;
        vecRowid->push_back(id);
    }
}

void DeleteCallDataCommand::ResetDataStatus()
{
    for (size_t i = _executeCount; i < _dataList->size(); i++)
    {
        const CallData* pCallData = _dataList->at(i);
        BaseData* pBase = (BaseData*)pCallData;
        pBase->SetStatus(BaseData::NO_CHANGE);
    }
}


#endif