#include "pch_module.h"

#ifdef MODULE_CALL

#include "Module/Call/CallManager.h"
#include "Module/Call/GetCallDataCommand.h"
#include "Module/Call/AddCallDataCommand.h"
#include "Module/Call/DeleteCallDataCommand.h"

CallManager::CallManager( const DeviceData* pDevice )
: IManager (pDevice)
{

}

CallManager::~CallManager()
{
    Release();
}

CommandExecuteResult ND91Assistant::CallManager::Load(bool bReload, bool bOnlySize)
{
    RETURN_VALUE_IF(!bReload && _bLoad, COMMAND_EXECUTE_SUCCESS);

    GetCallDataCommand cmd(_pDevice);
    cmd.SetOnlySize(bOnlySize);
    cmd.Execute();

    //RETURN_VALUE_IF(cmd.Result() != COMMAND_EXECUTE_SUCCESS, cmd.Result());

    Release();
    const vector<CallData*>* pDataList = cmd.GetDataList();
    for (vector<CallData*>::const_iterator it = pDataList->begin(); it != pDataList->end(); it++)
        _datas.push_back(new CallData(*it));
    _bLoad = true;
    return cmd.Result();
}

void ND91Assistant::CallManager::Release()
{
    for (vector<CallData*>::iterator it = _datas.begin(); it != _datas.end(); it++)
    {
        SAFE_DELETE(*it);
    }
    _datas.clear();
    _bLoad = false;
}

const vector<CallData*>* ND91Assistant::CallManager::GetDatas()
{
    return &_datas;
}

bool ND91Assistant::CallManager::ImportCallData( const vector<const CallData*>* pDataList,ProgressObserver* pObserver )
{
    AddCallDataCommand command(_pDevice);
    command.Attach(pObserver);
    command.SetDataList(pDataList);
    command.Execute();
    _nSuccessCount = command.ExecuteCount();
    LOG->WriteDebug(L"CallManager::ImportMessageData:_nSuccessCount:"+CStrOperation::IntToWString(_nSuccessCount));
    LOG->WriteDebug(L"CallManager::ImportMessageData:result:"+CStrOperation::IntToWString(command.Success()));
    return command.Success();
}

CommandExecuteResult ND91Assistant::CallManager::Delete(int id)
{
    for (vector<CallData*>::iterator it = _datas.begin(); it != _datas.end(); it++)
    {
        if (*it && (*it)->_rowId == id)
        {
            (*it)->SetStatus(BaseData::DEL);
            return COMMAND_EXECUTE_SUCCESS;
        }
    }
    return COMMAND_EXECUTE_NOFOUND;
}

CommandExecuteResult ND91Assistant::CallManager::Delete(const vector<const CallData*>* pDataList)
{
    for (vector<const CallData*>::const_iterator it = pDataList->begin(); it != pDataList->end(); it++)
    {
        Delete((*it)->_rowId);
    }
    return COMMAND_EXECUTE_SUCCESS;
}

CommandExecuteResult ND91Assistant::CallManager::DeleteAll()
{
    for (vector<CallData*>::iterator it = _datas.begin(); it != _datas.end(); it++)
    {
        if (*it)
            (*it)->SetStatus(BaseData::DEL);
    }
    return COMMAND_EXECUTE_SUCCESS;
}

CommandExecuteResult ND91Assistant::CallManager::Update(ProgressObserver* pObserver)
{
    vector<const BaseData*> deleteDataList;
    for (vector<CallData*>::iterator it = _datas.begin(); it != _datas.end(); it++)
    {
        if (*it && (*it)->GetStatus() == BaseData::DEL)
            deleteDataList.push_back(*it);
    }
    DeleteCallDataCommand deletecmd(_pDevice);
    deletecmd.Attach(pObserver);
    deletecmd.SetDataList(&deleteDataList);
    deletecmd.Execute();
    if (deletecmd.Result() == COMMAND_EXECUTE_SUCCESS)
    {
        for (vector<CallData*>::iterator it = _datas.begin(); it != _datas.end();)
        {
            if (*it && (*it)->GetStatus() == BaseData::DEL)
                it = _datas.erase(it);
            else
                it++;
        }
    }
    return deletecmd.Result();
}

#endif