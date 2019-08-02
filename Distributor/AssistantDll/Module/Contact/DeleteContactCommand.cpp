#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/DeleteContactCommand.h"
#include "Module/Contact/ContactData.h"
#include "Module/Contact/ContactManager.h"
#include "Module/Contact/ContactSynchronizer.h"
#include "Module/AndroidDefine.h"

DeleteContactCommand::DeleteContactCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = PIM;
    _nAndroidCommandID = DEL_CONTACT;

    _pDataList         = NULL;
    _success           = false;
    _executeCount      = 0;
}

DeleteContactCommand::~DeleteContactCommand()
{
    _pDataList         = NULL;
}

std::string DeleteContactCommand::GetAndroidBodyData()
{
    // �ж��û��Ƿ�ȡ������ȡ������������
    ProgressObserver* pObserver = NULL;
    if (!_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

    BinaryBufferWriter writer;
    writer.Clear();
    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);
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

void DeleteContactCommand::SetAndroidReturnData( std::string data )
{
    BinaryBufferReader reader(data.c_str(), data.length());
    if (!parseAndroidPackageHead(&reader))
    {
        _success = false;
        return;
    }
    reader.ReadBool(_success);
}

// ִ������
void DeleteContactCommand::Execute()
{
    if ( _pDataList == NULL )
    {
        _success = true;
        Complete();
        return;
    }

    // ����������������
    SetAllTaskSum(_pDataList->size());
    // ������������
    SetTaskNumberSum(_pDataList->size());

    if ( _pDevice->GetDevInfo()->_deviceType == Android )
        DefaultExecute();
    else if ( _pDevice->GetDevInfo()->_deviceType == Ios )
        _success = IosExecute();

    // ����δִ�����ݵ�״̬
    ResetDataStatus();
}

bool DeleteContactCommand::Success()
{
    return _success;
}

void DeleteContactCommand::SetDataList( const vector<ContactData*>* pDataList )
{
    _pDataList = pDataList;
}

// bool DeleteContactCommand::IosExecute()
// {
//     // û�������豸�򷵻�
//     if (_pDevice->GetDevInfo() == NULL || !_pDevice->GetDevInfo()->_ios_bAFCConn)
//         return false;
// 
//     // �޸����ݿ��ļ�
//     IosDBPath dbPath(_pDevice,ISCONTACTDB);
//     IosContactDB contactDB(dbPath);
//     contactDB.OpenDB();
//     contactDB.BeginTransaction();
//     try
//     {
//         vector<int> vecRowid;
//         GetRowIdVector(&vecRowid);
//         _executeCount = contactDB.DeleteContact(vecRowid,(ProgressSubject*)this);
//         contactDB.Commit();
//         contactDB.CloseDB();
//         return contactDB.SyncToIos(_pDevice);// ͬ����IOS�豸
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

bool DeleteContactCommand::IosExecute()
{
    // û�������豸�򷵻�
    if (_pDevice->GetDevInfo() == NULL)
        return false;

	_executeCount = _pDataList->size(); // iosδ���������

    return _pDevice->GetContactManager()->GetContactSynchronizer()->SaveGroupsNContacts();
}

void DeleteContactCommand::GetRowIdVector(vector<int> *vecRowid)
{
    std::vector<ContactData*>::const_iterator iter;
    for(iter = _pDataList->begin(); iter != _pDataList->end(); iter++)
    {
        ContactData* pContactData =  *iter;
        int id = pContactData->_contactId;
        vecRowid->push_back(id);
    }
}

void DeleteContactCommand::ResetDataStatus()
{
    for (size_t i = _executeCount; i < _pDataList->size(); i++)
    {
        const ContactData* pContactData = _pDataList->at(i);
        BaseData* pBase = (BaseData*)pContactData;
        pBase->SetStatus(BaseData::NO_CHANGE);
    }
}

#endif