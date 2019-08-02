#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/AddContactDataCommand.h"
#include "Module/Contact/ContactCommandHelper.h"
#include "Module/Contact/ContactGroupData.h"
#include "Module/Contact/ContactAccount.h"
#include "Module/Contact/ContactManager.h"
#include "Module/Contact/ContactSynchronizer.h"
#include "Module/Contact/ContactData.h"
#include "Module/Contact/OMSAddContactPhotoCommand.h"

#include "Module/AndroidDefine.h"

// �����ֵ����Option�����ã�option���Ҳ�����ʹ�ô�Ĭ��ֵ
#define ANDROID_DEFAULT_IMPORT_CONTACT_COUNT  10

AddContactDataCommand::AddContactDataCommand(const DeviceData* pDevice,  ContactAccount* pAccount)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = PIM;
    _nAndroidCommandID = ADD_CONTACT;
    
    _pDataList         = NULL;
    _pAccount          = pAccount;
    _success           = false;
    _executeCount      = 0;
    _executeCountEveryTime = 0;
    
    // ��ȡ��׿ϵͳ�汾
    if (((pDevice->GetDevInfo())->_deviceProductVersion).compare(L"2.0") == -1)
        _bVersion20 = false;
    else
        _bVersion20 = true;

    _bOMS = (_pDevice->GetDevInfo()->_platform == Platform_OMS);

    // ��ȡ��׿����������ϵ�������������������
    if ( ! OPTION->Get(GlobalOption::INT_ANDROID_IMPORT_CONTACT_COUNT, _maxCount) )
        _maxCount = ANDROID_DEFAULT_IMPORT_CONTACT_COUNT;
}

AddContactDataCommand::~AddContactDataCommand()
{
    _executeDataList.clear();
    _pDataList         = NULL;
    _pAccount          = NULL;
}

std::string AddContactDataCommand::GetAndroidBodyData()
{
    // �ж��û��Ƿ�ȡ������ȡ������������
    ProgressObserver* pObserver = NULL;
    if (!_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

    BinaryBufferWriter writer;
    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);
    writer.WriteInt(_executeDataList.size());

    IF_CANCEL(pObserver)
    {
        writer.ReplaceInt(0,8);
        return writer.GetBuffer();
    }

    size_t i = 0;
    wstring name;
    for (i = 0; i < _executeDataList.size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)
        ContactData* pData = _executeDataList.at(i);
		pData->_accountName = _pAccount->GetName();
        pData->_accountType = _pAccount->GetType();

        if(_bOMS)
        {   // OMS���������ͷ��
            OMSAddContactPhotoCommand omscmd(_pDevice);
            omscmd.SetParam(pData->_headPhoto._strSavePath);
            omscmd.Execute();
            Photo* pic = omscmd.GetReturnPictureData();
            if(pic)
            {
                pData->_headPhoto._photoId = pic->_photoId;
                pData->_headPhoto._photoUri = omscmd.GetUri();
                pData->_headPhoto._photoPath = pic->_photoPath;
            }
        }
        ContactCommandHelper CommandHelper(pData, _bVersion20, _bOMS);

        CommandHelper.WriteAndroidPeople(writer);

        //��OMSд��������ͼƬ��Ϣ
        CommandHelper.WriteAndroidPhoto(writer);
        CommandHelper.WriteAndroidName(writer);        
        CommandHelper.WriteAndroidPhone(writer);
        CommandHelper.WriteAndroidOrganization(writer);
        CommandHelper.WriteAndroidEmail(writer);
        CommandHelper.WriteAndroidAddress(writer);
        CommandHelper.WriteAndroidIm(writer);
        CommandHelper.WriteAndroidComment(writer);
        WriteAndroidGroupMemberShip(writer, pData);

        //д�������ʶ
        writer.WriteInt(METHOD_ENDFLAG);

        IncreaseTaskNumberComplete();
        SendProgress();
        name += pData->_sName._displayName+L",";
    }
    _executeCountEveryTime = (int)i;
    if (_executeCountEveryTime != (int)_executeDataList.size())
        writer.ReplaceInt(_executeCountEveryTime,8);

    // ������ʾ
    SetCurrentTaskSum(_executeCountEveryTime);
    SetCurrentTaskName(CStrOperation::trimRightW(name, L","));
    AddTaskProgress(_executeCountEveryTime);
    SendProgress();

    return writer.GetBuffer();
}

void AddContactDataCommand::SetAndroidReturnData( std::string data )
{
    BinaryBufferReader reader(data.c_str(), data.length());
    if (!parseAndroidPackageHead(&reader))
    {
        _success = false;
        return;
    }
    reader.ReadBool(_success);
    RETURN_IF(!_success);

    _executeCount += _executeCountEveryTime;   
}

// ִ������
void AddContactDataCommand::Execute()
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
        _success = AndroidExecute();        
    else if ( _pDevice->GetDevInfo()->_deviceType == Ios )
        _success = IosExecute();
}

bool AddContactDataCommand::AndroidExecute()
{
    int count = 0;
    vector<ContactData*>::const_iterator it = _pDataList->begin();
    while (  it != _pDataList->end() )
    {
        IF_CANCEL_RETURN_FALSE(dynamic_cast<ProgressObserver*>(GetFirstObserver()));
        // �Ӵ˶���Ϣ�����ִ�ж���
        _executeDataList.push_back(*it);
        ++count;
        ++it; 

		if(_pDevice->IsConnectRetry())
		{
			return false;
		}

        // �ж϶���Ϣ�������Ƿ�ﵽ������������ޣ��ﵽ��ִ�����������ر�������
        if ( count == _maxCount || (it == _pDataList->end() && count >0) )
        {
            DefaultExecute();
			count = 0;
            _executeDataList.clear();
            RETURN_FALSE_IF(!_success)
        }
    }
    return true;
}

bool AddContactDataCommand::Success()
{
    return _success;
}

void AddContactDataCommand::SetDataList(const vector<ContactData*>* pDataList)
{
    _pDataList = pDataList;
}

void AddContactDataCommand::WriteAndroidGroupMemberShip(BinaryBufferWriter& writer, ContactData* pContactData)
{
    RETURN_IF (!_pAccount || pContactData) // ���޷�����Ϣֱ�ӷ���

    for (vector<wstring>::iterator it = pContactData->_tempGroupNames.begin(); it != pContactData->_tempGroupNames.end(); ++it)
    {      
        wstring groupName = *it;
		ContactGroupData* existGroup = _pAccount->FindGroupByName(groupName);
		if (existGroup)
		{
			bool found = false;
			for(size_t i =0; i < pContactData->_groupIdList.size(); i++)
			{
				if (pContactData->_groupIdList[i]==existGroup->GetId())
				{
					found = true;
					break;
				}
			}
			if(!found)
				pContactData->_groupIdList.push_back(existGroup->GetId());
			else
			{
				pContactData->_tempGroupNames.erase(it);
				--it;
				continue;
			}
		}
		else
		{
			existGroup = new ContactGroupData();
			existGroup->SetName(groupName);
			existGroup->SetAccountName(_pAccount->GetName());
			existGroup->SetAccountType(_pAccount->GetType());
            vector<ContactGroupData*> dataList;
            dataList.push_back(existGroup);
			if(_pAccount->AddContactGroup(/*existGroup*/&dataList))
				pContactData->_groupIdList.push_back(existGroup->GetId());
			else
			{
				SAFE_DELETE(existGroup);	
				pContactData->_tempGroupNames.erase(it);
				--it;
			}
		}
		if(existGroup)
		{
			writer.WriteInt(int(METHOD_GROUPMEMBERSHIP));
			writer.WriteInt(STATE_ADD);
			writer.WriteInt(1); // 2.0�ڴ˴�д�����MemberShip�ṹ��ID�����������Ѿ�ɾ���˴˽ṹ�˴����д��һ��int����
			writer.WriteInt(pContactData->_contactId);
			writer.WriteInt(existGroup->GetId());
		}
    }
}

// bool AddContactDataCommand::IosExecute()
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
//         _executeCount = contactDB.AddContactData(_pDataList,(ProgressSubject*)this);
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

bool AddContactDataCommand::IosExecute()
{
    // û�������豸�򷵻�
    if (_pDevice->GetDevInfo() == NULL)
        return false;

    // ������ʾ
//     SetCurrentTaskSum(1);
//     AddTaskProgress(1);
//     SendProgress();

    for (size_t i = 0; i < _pDataList->size(); i++)
        _pDevice->GetContactManager()->GetContactSynchronizer()->AddContacts(_pDataList->at(i));

    bool ret = _pDevice->GetContactManager()->GetContactSynchronizer()->SaveGroupsNContacts();
    if (ret)
        _executeCount = (int)_pDataList->size();
	else
		_executeCount = 0;

    // ������ʾ
//     SetCurrentTaskSum(_executeCount-1);
//     AddTaskProgress(_executeCount-1);
//     SendProgress();

    return ret;
}

#endif