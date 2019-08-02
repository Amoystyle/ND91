#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/DeleteContactGroupCommand.h"
#include "Module/Contact/ContactGroupData.h"
#include "Module/Contact/ContactManager.h"
#include "Module/Contact/ContactSynchronizer.h"
#include "Module/AndroidDefine.h"

DeleteContactGroupCommand::DeleteContactGroupCommand(const DeviceData* pDevice, const ContactGroupData* pGroupData)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = PIM;
    _nAndroidCommandID = DEL_CONTACTGROUP;

    _pGroupData        = (ContactGroupData*)pGroupData;
    _success           = false;
}

DeleteContactGroupCommand::~DeleteContactGroupCommand()
{
    _pGroupData        = NULL;
}

std::string DeleteContactGroupCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;

    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);

    // д��Ҫɾ���ķ���ID��ÿ��ɾ��һ�����飩
    writer.WriteInt(_pGroupData->GetId());    

    return writer.GetBuffer();
}

void DeleteContactGroupCommand::SetAndroidReturnData( std::string data )
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
void DeleteContactGroupCommand::Execute()
{
    if (_pGroupData == NULL)
    {
        _success =true;
        return;
    }

    if ( _pDevice->GetDevInfo()->_deviceType == Android )
        DefaultExecute();        
    else if ( _pDevice->GetDevInfo()->_deviceType == Ios )
        _success = IosExecute();
}

bool DeleteContactGroupCommand::Success()
{
    return _success;
}

// ִ��IOS����
// bool DeleteContactGroupCommand::IosExecute()
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
//         contactDB.DeleteGroup(_pGroupData->GetId());
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

bool DeleteContactGroupCommand::IosExecute()
{
    // û�������豸�򷵻�
    if (_pDevice->GetDevInfo() == NULL)
        return false;

    _pGroupData->SetStatus(BaseData::DEL);
    return _pDevice->GetContactManager()->GetContactSynchronizer()->SaveGroupsNContacts();
}


#endif