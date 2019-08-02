#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/UpdateContactCommand.h"
#include "Module/Contact/ContactData.h"
#include "Module/Contact/OMSAddContactPhotoCommand.h"
#include "Module/Contact/ContactCommandHelper.h"
#include "Module/Contact/ContactManager.h"
#include "Module/Contact/ContactSynchronizer.h"
#include "Module/AndroidDefine.h"

UpdateContactCommand::UpdateContactCommand(const DeviceData* pDevice, ContactData* pContact, bool bVersion20, bool bOMS)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = PIM;
    _nAndroidCommandID = UPDATE_CONTACT;

    _bVersion20        = bVersion20;
    _bOMS              = bOMS;
    _pContactData      = pContact; 
    _success           = false;
}

UpdateContactCommand::~UpdateContactCommand()
{
    _pContactData         = NULL;
}

std::string UpdateContactCommand::GetAndroidBodyData()
{
	if(_bOMS)
	{	// OMS添加联络人头像
		OMSAddContactPhotoCommand omscmd(_pDevice);
		omscmd.SetParam(_pContactData->_headPhoto._strSavePath);
		omscmd.Execute();
		Photo* pic = omscmd.GetReturnPictureData();
		if(pic)
		{
			_pContactData->_headPhoto._photoId = pic->_photoId;
			_pContactData->_headPhoto._photoUri = omscmd.GetUri();
			_pContactData->_headPhoto._photoPath = pic->_photoPath;
		}
	}
    BinaryBufferWriter writer;

    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);

    ContactCommandHelper CommandHelper(_pContactData, _bVersion20, _bOMS);

    CommandHelper.WriteAndroidPeople(writer);

    //非oms写入联络人图片信息
	CommandHelper.WriteAndroidPhoto(writer);

    //写入联络人姓名
    CommandHelper.WriteAndroidName(writer); 

    CommandHelper.WriteAndroidPhone(writer);
    CommandHelper.WriteAndroidOrganization(writer);
    CommandHelper.WriteAndroidEmail(writer);
    CommandHelper.WriteAndroidAddress(writer);
    CommandHelper.WriteAndroidIm(writer);

    //写入联络人备注信息
    CommandHelper.WriteAndroidComment(writer);

    //写入结束标识
    writer.WriteInt(METHOD_ENDFLAG);

    return writer.GetBuffer();
}

void UpdateContactCommand::SetAndroidReturnData( std::string data )
{
    BinaryBufferReader reader(data.c_str(), data.length());

    if (!parseAndroidPackageHead(&reader))
    {
        _success = false;
        return;
    }

    reader.ReadBool(_success);

    if (_success)
    {
        // 此处回写联络人图片ID（待添加）

		if(_pContactData->_headPhoto._state == STATE_ADD && !_bOMS)
		{
			reader.ReadInt32(_pContactData->_headPhoto._photoId);
			_pContactData->_headPhoto._state = STATE_NOCHANGE;
		}
        // 回写更新中添加的结构的ID
        if (_pContactData->_sName._state == STATE_ADD && _bVersion20)
        {
            reader.ReadInt32(_pContactData->_sName._id);
            _pContactData->_sName._state = STATE_NOCHANGE;
        }
        for (vector<Phone*>::iterator it = _pContactData->_phoneNumberList.begin(); it != _pContactData->_phoneNumberList.end(); ++it)
        {
            if (STATE_ADD == (*it)->_state)
            {
                //reader.ReadInt32((*it)->_id);
                reader.ReadString((*it)->_key, 4);
            }
            (*it)->_state = STATE_NOCHANGE;
        }
        for (vector<Organization*>::iterator it = _pContactData->_organizationList.begin(); it != _pContactData->_organizationList.end(); ++it)
        {
            if (STATE_ADD == (*it)->_state)
            {
                reader.ReadInt32((*it)->_id);
            }     
            (*it)->_state = STATE_NOCHANGE;
        }
        for (vector<Email*>::iterator it = _pContactData->_emailList.begin(); it != _pContactData->_emailList.end(); ++it)
        {
            if (STATE_ADD == (*it)->_state)
            {
                //reader.ReadInt32((*it)->_id);
                reader.ReadString((*it)->_key, 4);
            }
            (*it)->_state = STATE_NOCHANGE;
        }
        for (vector<Address*>::iterator it = _pContactData->_addressList.begin(); it != _pContactData->_addressList.end(); ++it)
        {
            if (STATE_ADD == (*it)->_state)
            {
                //reader.ReadInt32((*it)->_id);
                reader.ReadString((*it)->_key, 4);
            }
            (*it)->_state = STATE_NOCHANGE;
        }
        for (vector<Im*>::iterator it = _pContactData->_imList.begin(); it != _pContactData->_imList.end(); ++it)
        {
            if (STATE_ADD == (*it)->_state)
            {
                //reader.ReadInt32((*it)->_id);
                reader.ReadString((*it)->_key, 4);
            }
            (*it)->_state = STATE_NOCHANGE;
        }
        if (_pContactData->_sComment._state == STATE_ADD && _bVersion20)
        {
            reader.ReadInt32(_pContactData->_sComment._id);
            _pContactData->_sComment._state = STATE_NOCHANGE;
        }
    }

}

// 执行命令
void UpdateContactCommand::Execute()
{
    if ( _pContactData == NULL )
    {
        _success = true;
        return;
    }

    if ( _pDevice->GetDevInfo()->_deviceType == Android )
        DefaultExecute();
    else if ( _pDevice->GetDevInfo()->_deviceType == Ios )
        _success = IosExecute();
}

bool UpdateContactCommand::Success()
{
    return _success;
}

// bool UpdateContactCommand::IosExecute()
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
//         contactDB.UpdateContact(_pContactData);
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

bool UpdateContactCommand::IosExecute()
{
    // 没连接上设备则返回
    if (_pDevice->GetDevInfo() == NULL)
        return false;

    _pContactData->SetStatus(BaseData::UPDATE);
    return _pDevice->GetContactManager()->GetContactSynchronizer()->SaveGroupsNContacts();
}


#endif