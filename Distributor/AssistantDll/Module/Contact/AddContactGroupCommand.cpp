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

    // 根据手机系统版本写入要更新的分组信息（每次删除一个分组） 
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
    
    // 执行成功，为新添加的分组写入ID
    if (true == _success)
    {
        int id;
        reader.ReadInt32(id);
		if(_pData)
			_pData->SetId(id);
    }
}

// 执行命令
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

// 执行IOS命令
// bool AddContactGroupCommand::IosExecute()
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
//         _pGroupData->SetId(contactDB.AddGroup(_pGroupData));
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

bool AddContactGroupCommand::IosExecute()
{
    // 没连接上设备则返回
    if (_pDevice->GetDevInfo() == NULL)
        return false;

    for (size_t i = 0; i < _pDataList->size(); i++)
        _pDevice->GetContactManager()->GetContactSynchronizer()->AddGroups(/*_pGroupData*/_pDataList->at(i));
    return _pDevice->GetContactManager()->GetContactSynchronizer()->SaveGroupsNContacts();
}

#endif