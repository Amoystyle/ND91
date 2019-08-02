#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/UpdateContactGroupCommand.h"
#include "Module/Contact/ContactGroupData.h"
#include "Module/Contact/ContactManager.h"
#include "Module/Contact/ContactSynchronizer.h"
#include "Module/AndroidDefine.h"

UpdateContactGroupCommand::UpdateContactGroupCommand(const DeviceData* pDevice, const ContactGroupData* pGroupData, bool bVersion20, bool bOMS)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = PIM;
    _nAndroidCommandID = UPDATE_CONTACTGROUP;

    _pGroupData        = (ContactGroupData*)pGroupData;
    _success           = false;

    _bVersion20        = bVersion20;
    _bOMS              = bOMS;
}

UpdateContactGroupCommand::~UpdateContactGroupCommand()
{
    _pGroupData        = NULL;
}

std::string UpdateContactGroupCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;

    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);
    
    // �����ֻ�ϵͳ�汾д��Ҫ���µķ�����Ϣ��ÿ��ɾ��һ�����飩 
    _pGroupData->Write(&writer, true == _bVersion20 && false == _bOMS);
    return writer.GetBuffer();
}

void UpdateContactGroupCommand::SetAndroidReturnData( std::string data )
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
void UpdateContactGroupCommand::Execute()
{
    if ( _pGroupData == NULL )
    {
        _success = true;
        return;
    }

    if ( _pDevice->GetDevInfo()->_deviceType == Android )
        DefaultExecute();        
    else if ( _pDevice->GetDevInfo()->_deviceType == Ios )
        _success = IosExecute();
}

bool UpdateContactGroupCommand::Success()
{
    return _success;
}

// ִ��IOS����
// bool UpdateContactGroupCommand::IosExecute()
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
//         contactDB.UpdateGroup(_pGroupData);
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

bool UpdateContactGroupCommand::IosExecute()
{
    // û�������豸�򷵻�
    if (_pDevice->GetDevInfo() == NULL)
        return false;

    _pGroupData->SetStatus(BaseData::UPDATE);
    return _pDevice->GetContactManager()->GetContactSynchronizer()->SaveGroupsNContacts();
}


#endif