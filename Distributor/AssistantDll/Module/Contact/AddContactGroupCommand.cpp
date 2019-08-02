#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/AddContactGroupCommand.h"
#include "Module/Contact/ContactGroupData.h"
#include "Module/Contact/ContactManager.h"
#include "Module/Contact/ContactSynchronizer.h"
#include "Module/AndroidDefine.h"

AddContactGroupCommand::AddContactGroupCommand(const DeviceData* pDevice, /*ContactGroupData* pGroupData*/const vector<ContactGroupData*>* pDataList, bool bVersion20, bool bOMS)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = PIM;
    _nAndroidCommandID = ADD_CONTACTGROUP;

    //_pGroupData        = pGroupData;
    _pDataList         = pDataList;
    _success           = false;

    _bVersion20        = bVersion20;
    _bOMS              = bOMS;
	_pData=NULL;
}

AddContactGroupCommand::~AddContactGroupCommand()
{
    //_pGroupData        = NULL;
    _pDataList         = NULL;
}

std::string AddContactGroupCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;

    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);

    // �����ֻ�ϵͳ�汾д��Ҫ���µķ�����Ϣ��ÿ��ɾ��һ�����飩 
    //_pGroupData->Write(&writer, true == _bVersion20 && false == _bOMS);
   // for (size_t i = 0; i < _pDataList->size(); i++)
	{
		if(_pData)
        _pData->Write(&writer, true == _bVersion20 && false == _bOMS);
	}

    return writer.GetBuffer();
}

void AddContactGroupCommand::SetAndroidReturnData( std::string data )
{
    BinaryBufferReader reader(data.c_str(), data.length());

    if (!parseAndroidPackageHead(&reader))
    {
        _success = false;
        return;
    }

    reader.ReadBool(_success);
    
    // ִ�гɹ���Ϊ����ӵķ���д��ID
    if (true == _success)
    {
        int id;
        reader.ReadInt32(id);
		if(_pData)
			_pData->SetId(id);
    }
}

// ִ������
void AddContactGroupCommand::Execute()
{
    //if ( _pGroupData == NULL )
    if ( _pDataList == NULL )
    {
        _success = true;
        return;
    }

    if ( _pDevice->GetDevInfo()->_deviceType == Android )
	{

		for (size_t i=0;i<_pDataList->size();i++)
		{
			_pData=_pDataList->at(i);
			DefaultExecute();  
		}      
	}
    else if ( _pDevice->GetDevInfo()->_deviceType == Ios )
        _success = IosExecute();
}

bool AddContactGroupCommand::Success()
{
    return _success;
}

// ִ��IOS����
// bool AddContactGroupCommand::IosExecute()
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
//         _pGroupData->SetId(contactDB.AddGroup(_pGroupData));
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

bool AddContactGroupCommand::IosExecute()
{
    // û�������豸�򷵻�
    if (_pDevice->GetDevInfo() == NULL)
        return false;

    for (size_t i = 0; i < _pDataList->size(); i++)
        _pDevice->GetContactManager()->GetContactSynchronizer()->AddGroups(/*_pGroupData*/_pDataList->at(i));
    return _pDevice->GetContactManager()->GetContactSynchronizer()->SaveGroupsNContacts();
}

#endif