#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/SetFavoriteCommand.h"
#include "Module/Contact/ContactData.h"
#include "Module/AndroidDefine.h"

SetFavoriteCommand::SetFavoriteCommand(const DeviceData* pDevice, bool bFavorite)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = PIM;
    _nAndroidCommandID = SET_FAVORITE;
    
    _bFavorite         = bFavorite;
    _pDataList         = NULL;
    _success           = false;
    _executeCount      = 0;
}

SetFavoriteCommand::~SetFavoriteCommand()
{
    _pDataList         = NULL;
}

std::string SetFavoriteCommand::GetAndroidBodyData()
{
    // 判断用户是否取消，若取消则立即返回
    ProgressObserver* pObserver = NULL;
    if (!_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

    BinaryBufferWriter writer;
    writer.Clear();
    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);
    writer.WriteBool(_bFavorite);
    writer.WriteInt(_pDataList->size());

    IF_CANCEL(pObserver)
        return writer.GetBuffer();

    size_t i = 0;
    for (i = 0; i < _pDataList->size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)
        const ContactData* pData = _pDataList->at(i);
        writer.WriteInt(pData->_contactId);

        IncreaseTaskNumberComplete();
        SendProgress();

        SetCurrentTaskSum(1);
        SetCurrentTaskName(pData->_sName._displayName);

        AddTaskProgress(1);
        SendProgress();
    }
    _executeCount = (int)i;
    if (_executeCount != (int)_pDataList->size())
        writer.ReplaceInt(_executeCount,8);

    return writer.GetBuffer();
}

void SetFavoriteCommand::SetAndroidReturnData( std::string data )
{
    BinaryBufferReader reader(data.c_str(), data.length());
    if (!parseAndroidPackageHead(&reader))
    {
        _success = false;
        return;
    }
    reader.ReadBool(_success);
}

int SetFavoriteCommand::ExecuteSetFavorite()
{
    if ( _pDataList == NULL )
    {
        _success = true;
        Complete();
        return _executeCount;
    }

    // 设置所有任务总量
    SetAllTaskSum(_pDataList->size());
    // 设置任务总数
    SetTaskNumberSum(_pDataList->size());

    DefaultExecute();

    return _executeCount;
}

bool SetFavoriteCommand::Success()
{
    return _success;
}

void SetFavoriteCommand::SetDataList( const vector<ContactData*>* pDataList )
{
    _pDataList = (const std::vector<const ContactData*>*)pDataList;
}


#endif