#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/ContactAndGroupCommand.h"
#include "Module/Contact/ContactData.h"
#include "Module/Contact/ContactGroupData.h"
#include "Module/Contact/ContactManager.h"
#include "Module/Contact/ContactSynchronizer.h"
#include "Module/AndroidDefine.h"

ContactAndGroupCommand::ContactAndGroupCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = PIM;
    _nAndroidCommandID = 0;

    _pGroupData        = NULL;    
    _pDataList         = NULL;
    _success           = false;
    _executeCount      = 0;
}

ContactAndGroupCommand::~ContactAndGroupCommand()
{
    _pDataList         = NULL;
}

std::string ContactAndGroupCommand::GetAndroidBodyData()
{
    // 判断用户是否取消，若取消则立即返回
    ProgressObserver* pObserver = NULL;
    if (!_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

    BinaryBufferWriter writer;
    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);
    writer.WriteInt(_pGroupData->GetId());// 写入需要操作的分组ID
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
        wstring name = L"";
        if ( _pDevice->GetDevInfo()->_deviceType == Android )
            name = pData->_sName._displayName;
        else
            name = pData->_sName._givenName + pData->_sName._familyName;
        SetCurrentTaskName(name);

        AddTaskProgress(1);
        SendProgress();
    }
    _executeCount = (int)i;
    if (_executeCount != (int)_pDataList->size())
        writer.ReplaceInt(_executeCount,8);

    return writer.GetBuffer();
}

void ContactAndGroupCommand::SetAndroidReturnData( std::string data )
{
    BinaryBufferReader reader(data.c_str(), data.length());

    if (!parseAndroidPackageHead(&reader))
    {
        _success = false;
        return;
    }
    reader.ReadBool(_success);
}

bool ContactAndGroupCommand::Success()
{
    return _success;
}

int ContactAndGroupCommand::ExecuteRemoveContactFromGroup(const vector<ContactData*>* pDataList, ContactGroupData* pGroupData)
{
	RETURN_VALUE_IF( !pGroupData, 0 );
    _pGroupData        = pGroupData;
    _pDataList         = (const std::vector<const ContactData*>*)pDataList;
    _nAndroidCommandID = REMOVE_FROMGROUP;

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

    if ( _pDevice->GetDevInfo()->_deviceType == Android )
        DefaultExecute();        
    else if ( _pDevice->GetDevInfo()->_deviceType == Ios )
        _success = IosExecuteRemoveContactFromGroup();

    return _executeCount;
}

int ContactAndGroupCommand::ExecuteAddContactToGroup(const vector<ContactData*>* pDataList, ContactGroupData* pGroupData)
{
	RETURN_VALUE_IF( !pGroupData, 0 );
    _pGroupData        = pGroupData;
    _pDataList         = (const std::vector<const ContactData*>*)pDataList;
    _nAndroidCommandID = ADD_TOGROUP;

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

    if ( _pDevice->GetDevInfo()->_deviceType == Android )
        DefaultExecute();        
    else if ( _pDevice->GetDevInfo()->_deviceType == Ios )
        _success = IosExecuteAddContactToGroup();

    return _executeCount;
}

// 执行IOS命令
// bool ContactAndGroupCommand::IosExecuteAddContactToGroup()
// {
//     // 没连接上设备则返回
//     if (_pDevice->GetDevInfo() == NULL || !_pDevice->GetDevInfo()->_ios_bAFCConn)
//         return false;
// 
//     // 修改数据库文件
//     IosDBPath dbPath(_pDevice,ISCONTACTDB);
//     IosContactDB contactDB(dbPath);
//     contactDB.OpenDB();
//     contactDB.BeginTransaction();
//     try
//     {
//         _executeCount = contactDB.AddMember(_pGroupData->GetId(),_pDataList,(ProgressSubject*)this);
//         contactDB.Commit();
//         contactDB.CloseDB();
//         return contactDB.SyncToIos(_pDevice);// 同步到IOS设备
//     }
//     catch (CppSQLite3Exception& e)
//     {
//         int iError = e.ErrorCode();
//         const char* szError = e.ErrorMessage();
//         contactDB.Rollback();
//     }
//     contactDB.CloseDB();
// 
//     return false;
// }

bool ContactAndGroupCommand::IosExecuteAddContactToGroup()
{
    // 没连接上设备则返回
    if (_pDevice->GetDevInfo() == NULL)
        return false;

    _pGroupData->SetStatus(BaseData::UPDATE);
    _pDevice->GetContactManager()->GetContactSynchronizer()->AddContactsToGroup(_pDataList, _pGroupData);
    return _pDevice->GetContactManager()->GetContactSynchronizer()->SaveGroupsNContacts();
}

// 执行IOS命令
// bool ContactAndGroupCommand::IosExecuteRemoveContactFromGroup()
// {
//     // 没连接上设备则返回
//     if (_pDevice->GetDevInfo() == NULL || !_pDevice->GetDevInfo()->_ios_bAFCConn)
//         return false;
// 
//     // 修改数据库文件
//     IosDBPath dbPath(_pDevice,ISCONTACTDB);
//     IosContactDB contactDB(dbPath);
//     contactDB.OpenDB();
//     contactDB.BeginTransaction();
//     try
//     {
//         vector<int> vecRowid;
//         GetRowIdVector(&vecRowid);
//         _executeCount = contactDB.RemoveMember(_pGroupData->GetId(),vecRowid,(ProgressSubject*)this);
//         contactDB.Commit();
//         contactDB.CloseDB();
//         return contactDB.SyncToIos(_pDevice);// 同步到IOS设备
//     }
//     catch (CppSQLite3Exception& e)
//     {
//         int iError = e.ErrorCode();
//         const char* szError = e.ErrorMessage();
//         contactDB.Rollback();
//     }
//     contactDB.CloseDB();
// 
//     return false;
// }

bool ContactAndGroupCommand::IosExecuteRemoveContactFromGroup()
{
    // 没连接上设备则返回
    if (_pDevice->GetDevInfo() == NULL)
        return false;

    _pGroupData->SetStatus(BaseData::UPDATE);
    _pDevice->GetContactManager()->GetContactSynchronizer()->RemoveContactsFromGroup(_pDataList, _pGroupData);
    return _pDevice->GetContactManager()->GetContactSynchronizer()->SaveGroupsNContacts();
}

void ContactAndGroupCommand::GetRowIdVector(vector<int> *vecRowid)
{
    std::vector<const ContactData*>::const_iterator iter;
    for(iter = _pDataList->begin(); iter != _pDataList->end(); iter++)
    {
        const ContactData* pContactData =  *iter;
        int id = pContactData->_contactId;
        vecRowid->push_back(id);
    }
}


#endif