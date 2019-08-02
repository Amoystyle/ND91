#include "pch_module.h"
#include "Module/BackupAndRestore/ContactBackAndRestoreExtend.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/ContactManager.h"
#include "Module/Contact/ContactSynchronizer.h"
#include "Module/Contact/GetContactCommand.h"
#include "Module/Contact/AddContactDataCommand.h"
#include "Module/Contact/AddContactToGroupCommand.h"
#include "Module/Contact/vCard/vCardEx.h"

#include "Module/Contact/OutlookContactProvider.h"
#include "Module/Contact/outlookContact.h"
#include "Core/DeviceManager.h"

#define PRODUCTVERSION     320    // IOS设备版本号
#define PYVERSION   L"2.0.0"      // 当前最新拼音版本

ContactManager::ContactManager( const DeviceData* pDevice )
: IManager (pDevice)
{
    _pContactSynchronizer = NULL;
    _nSuccessCount        = 0;
	_nRepeatCount		  = 0;
	 InitializeCriticalSection(&_csCommandLock);
}

ContactManager::~ContactManager()
{
	DeleteCriticalSection(&_csCommandLock);
    SAFE_DELETE(_pContactSynchronizer);
}

CommandExecuteResult ND91Assistant::ContactManager::Load(bool bReload, bool bOnlySize)
{
    RETURN_VALUE_IF(!bReload && _bLoad, COMMAND_EXECUTE_SUCCESS);
	SetCSLock(_csCommandLock);
    GetContactCommand cmd(_pDevice);
    cmd.SetOnlySize(bOnlySize);
    cmd.Execute();

    Release();
    const vector<ContactAccount*>* pDataList = cmd.GetAccountList();
    _datas.insert(_datas.end(), pDataList->begin(), pDataList->end());
    _bLoad = true;
    return cmd.Result();
}

void ND91Assistant::ContactManager::Release()
{
	SetCSLock(_csCommandLock);
    for (vector<ContactAccount*>::iterator it = _datas.begin(); it != _datas.end(); it++)
    {
        SAFE_DELETE(*it);
    }
    _datas.clear();
    _bLoad = false;
}

const vector<ContactAccount*>* ND91Assistant::ContactManager::GetDatas()
{
    return &_datas;
}

CommandExecuteResult ND91Assistant::ContactManager::DeleteAll(ProgressObserver* pObserver)
{
	SetCSLock(_csCommandLock);
    if (_pDevice->GetDevInfo()->_deviceType == Android)
    {
        for (vector<ContactAccount*>::iterator it = _datas.begin(); it != _datas.end(); it++)
        {
            if (*it)
            {
                (*it)->DeleteAll(pObserver);
                (*it)->DeleteAllGroup(pObserver);
            }
        }
    }
    else
        GetContactSynchronizer()->RemoveAllData();

    return COMMAND_EXECUTE_SUCCESS;
}

bool ND91Assistant::ContactManager::ImportContactData( const vector<ContactData*>* pDataList, ContactAccount* pAccount,ProgressObserver* pObserver)
{
	vector<ContactGroupData*> groupList = pAccount->GetContactGroupList();
	for (size_t j = 0; j < groupList.size(); j++)
	{
		ContactGroupData* pGroup = groupList.at(j);
		if(pGroup->GetContacts().size() <= 0)
			pAccount->DeleteContactGroup(pGroup);
	}

	SetCSLock(_csCommandLock);

	ContactGroupData* pGroup = pAccount->FindGroupByName(L"System Group: My Contacts");	//我的联系人
	if (pGroup == NULL)
		pGroup = pAccount->FindGroupByName(L"Contacts");		//我的联系人
	if (pGroup == NULL)
		pGroup = pAccount->FindGroupByName(L"我的联系人");		//我的联系人

	if(pGroup == NULL)
	{
		pGroup = new ContactGroupData();
		pGroup->SetName(L"我的联系人");
		pGroup->SetAccountName(pAccount->GetName());
		pGroup->SetAccountType(pAccount->GetType());
		vector<ContactGroupData*> groupver;
		groupver.push_back(pGroup);
		pAccount->AddContactGroup(&groupver);
	}

	for (size_t i=0;i<pDataList->size();i++)
	{
		pDataList->at(i)->_tempGroupNames.push_back(pGroup->GetName());
	}

	AddContactDataCommand command(_pDevice, pAccount);
	command.Attach(pObserver);
	command.SetDataList(pDataList);
	command.Execute();
	_nSuccessCount = command.ExecuteCount();
	RETURN_VALUE_IF(_pDevice->GetDevInfo()->_deviceType == Ios, command.Success());

	RETURN_VALUE_IF(pDataList == NULL 
		|| pDataList->empty()
		|| 0 == _nSuccessCount, command.Success());


    return command.Success();
    // 此接口需要根据导入文件的所含有的信息修改（现在未对账户信息做处理，只能添加到默认账户）
}

bool ND91Assistant::ContactManager::ImportContactGroupData( const vector<ContactGroupData*>* pDataList,ContactAccount* pAccount,ProgressObserver* pObserver )
{
	SetCSLock(_csCommandLock);
	RETURN_FALSE_IF(!pAccount);
	return pAccount->AddContactGroup(pDataList);
}

ContactSynchronizer* ND91Assistant::ContactManager::GetContactSynchronizer()
{
	SetCSLock(_csCommandLock);
    if (!_pContactSynchronizer)
        _pContactSynchronizer = new ContactSynchronizer(_pDevice);
    return _pContactSynchronizer;
}

void ND91Assistant::ContactManager::ExportToOutlook(ProgressObserver* pObserver,bool conflict)
{
	OutlookContactProvider* pOl=new OutlookContactProvider();
	if(!pOl->CheckExsitOutlook()||!pOl->Init())
	{
		SAFE_DELETE(pOl);
		return;
	}
	Load();
	long count=pOl->LoadOutLookContacts();
	ProgressSubject progress;
	progress.Attach(pObserver);
	int phonecount=0;
	for (size_t i=0;i<_datas.size();i++)
	{
		ContactAccount* pAccount=_datas[i];
		vector<ContactData*> contactlist=pAccount->GetContactDataList();
		phonecount+=contactlist.size();
	}
	progress.SetTaskNumberSum(phonecount); 
	progress.SetAllTaskSum(phonecount * 100);
	progress.SendPrompt(MESSAGE_BACKUP);
	for (size_t i=0;i<_datas.size();i++)
	{
		ContactAccount* pAccount=_datas[i];
		vector<ContactGroupData*> groupVec =  pAccount->GetContactGroupList();
		/*for (vector<ContactGroupData*>::iterator it=groupVec.begin();it!=groupVec.end();it++)
		{
			pOl->GeneratFolder(CCodeOperation::ws2s((*it)->GetName()));
		}*/
		vector<ContactData*> contactlist=pAccount->GetContactDataList();
		for (size_t j=0;j<contactlist.size();j++)
		{
			outlookContact* pOlcontact=new outlookContact();
			ContactData* pCon=contactlist[j];

			progress.SetCurrentTaskSum(100);
			progress.SetCurrentTaskName(pCon->_sName._displayName);

			if (!pCon->_photo_hasLoad)
				pAccount->LoadContactPhoto(pCon);
			pOlcontact->FromContactData(pCon);
			for (size_t k=0;k<pCon->_groupIdList.size();k++)
			{
				ContactGroupData* pGroup=pAccount->GetGroup(pCon->_groupIdList[k]);
				if(pGroup)
				{
					pOlcontact->_folder=CCodeOperation::UnicodeToUTF_8(pGroup->GetName());
					pOl->Save(true,pOlcontact);
				}
			}
			if((int)pCon->_groupIdList.size()<1)
				pOl->Save(true,pOlcontact);
			SAFE_DELETE(pOlcontact);
			progress.AddTaskProgress(100);
		}
	}
	SAFE_DELETE(pOl);
}

void ND91Assistant::ContactManager::ImportFromOutlook(ProgressObserver* pObserver,bool conflict)
{
	OutlookContactProvider* pOl=new OutlookContactProvider();
	if(!pOl->CheckExsitOutlook()||!pOl->Init())
	{
		SAFE_DELETE(pOl);
		return;
	}
	Load();
	long count=pOl->LoadOutLookContacts();
	//ProgressSubject progress;
	//progress.Attach(pObserver);
	//progress.SetTaskNumberSum(count); 
	//progress.SetAllTaskSum(count * 100);
	//progress.SendPrompt(MESSAGE_BACKUP);

	map<string,vector<outlookContact>>*  olcontactmap=pOl->GetOutLookContacts();
	vector<ContactGroupData*> groupVec;
	vector<ContactData*> contactsVec;
	for (map<string,vector<outlookContact>>::iterator it=olcontactmap->begin();it!=olcontactmap->end();it++)
	{
		string groupname=it->first;
		ContactGroupData* group=new ContactGroupData();
		groupVec.push_back(group);
		group->SetName(CCodeOperation::UTF_8ToUnicode(groupname));
		vector<outlookContact> olcontacts=it->second;
		for (size_t i=0; i<olcontacts.size();i++)
		{
			outlookContact olcon=olcontacts[i];
			ContactData* contact=new ContactData();
			olcon.CovertToContactData(contact);
			contact->_tempGroupNames.push_back(group->GetName());
			contactsVec.push_back(contact);
		}
	}
	ImportContactGroupData(&groupVec,_datas[0],NULL);
	Load(true);
	ImportContactData(&contactsVec,_datas[0],pObserver);

	/*for (size_t i=0;i<groupVec.size();i++)
	{
		SAFE_DELETE(groupVec[i]);
	}*/
	groupVec.clear();
	/*for (size_t i=0;i<contactsVec.size();i++)
	{
		SAFE_DELETE(contactsVec[i]);
	}
	contactsVec.clear();*/

	SAFE_DELETE(pOl);
}


/*
InstallState ND91Assistant::ContactManager::CheckPinyinVersion()
{
    // 判断当前设备版本
    int iProductVersion = IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion);
    if (iProductVersion >= PRODUCTVERSION)
        return STATE_UNNEED;

    // 判断设备当前语言
    //     if (_pDevice->GetDevInfo()->_language != L"zh-Hans")
    //         return STATE_UNNEED;

    IosDBPath dbPath(_pDevice,ISCONTACTDB);
    IosContactDB contactDB(dbPath);
    contactDB.OpenDB();

    IosAutoSort autoSort(&contactDB);
    // 获取拼音库版本
    wstring pinyinVersion = autoSort.GetPinyinVersion();

    contactDB.CloseDB();
    // 如果当前版本低于2.0则安装
    if (IosPublicFunc::GetVerNum(PYVERSION) > IosPublicFunc::GetVerNum(pinyinVersion))
        return STATE_INSTALL;
    else
        return STATE_UNINSTALL;
}

bool ND91Assistant::ContactManager::AutoSort()
{
    // 没连接上设备则返回
    if (_pDevice->GetDevInfo() == NULL || !_pDevice->GetDevInfo()->_ios_bAFCConn)
        return false;

    // 修改数据库文件
    IosDBPath dbPath(_pDevice,ISCONTACTDB);
    IosContactDB contactDB(dbPath);
    contactDB.OpenDB();
    contactDB.BeginTransaction();
    try
    {
        IosAutoSort autoSort(&contactDB);
        autoSort.Patch_Install(PYVERSION);
        contactDB.Commit();
        contactDB.CloseDB();
        return contactDB.SyncToIos(_pDevice);// 同步到IOS设备
    }
    catch (CppSQLite3Exception& e)
    {
        int iError = e.ErrorCode();
        const char* szError = e.ErrorMessage();
        contactDB.Rollback();
    }
    contactDB.CloseDB();
    return true;
}

bool ND91Assistant::ContactManager::UnInstallAutoSort()
{
    // 没连接上设备则返回
    if (_pDevice->GetDevInfo() == NULL || !_pDevice->GetDevInfo()->_ios_bAFCConn)
        return false;

    // 修改数据库文件
    IosDBPath dbPath(_pDevice,ISCONTACTDB);
    IosContactDB contactDB(dbPath);
    contactDB.OpenDB();
    contactDB.BeginTransaction();
    try
    {
        IosAutoSort autoSort(&contactDB);
        autoSort.Patch_UnInstall();
        contactDB.Commit();
        contactDB.CloseDB();
        return contactDB.SyncToIos(_pDevice);// 同步到IOS设备
    }
    catch (CppSQLite3Exception& e)
    {
        int iError = e.ErrorCode();
        const char* szError = e.ErrorMessage();
        contactDB.Rollback();
    }
    contactDB.CloseDB();
    return true;
}
*/

int ND91Assistant::ContactManager::ExportContactsToVCards(const vector<ContactAccount*>* accounts, wstring exportFolder, ProgressObserver* pObserver)
{
    RETURN_VALUE_IF(accounts == NULL, 0);
    ProgressSubject progress;
    progress.Attach(pObserver);
    int count = 0;
    for (size_t i = 0; i < accounts->size(); i++)
    {
        count += accounts->at(i)->GetContactDataList().size();
    }
    progress.SetTaskNumberSum(count); 
    progress.SetAllTaskSum(count);

    progress.SendPrompt(MESSAGE_EXPORT);
    IF_CANCEL(pObserver)
        return 0;

	int ret = ContactBackAndRestoreExtend::Export(*accounts, exportFolder, ContactBackAndRestoreType_vCard_OneFile, true);

	progress.SetCurrentTaskComplete();

    return count;
}
/*
void ND91Assistant::ContactManager::ImportContactsFromVCards(vector<wstring> files, ProgressObserver* pObserver)
{
    RETURN_IF(files.empty());
    ProgressSubject progress;
    progress.Attach(pObserver);
    progress.SetTaskNumberSum(files.size());
    progress.SetAllTaskSum(files.size());
    progress.SendPrompt(MESSAGE_RESTORE);
    IF_CANCEL(pObserver)
        return ;

    for (size_t i = 0; i < files.size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)

        // 保存添加联系人之前的联系人ID，为添加组里联系人使用
        vector<int> contactDataIdVec;
        vector<int> oldContactIDs;
        for (size_t p = 0; p < _datas.at(0)->GetContactDataList().size(); p++)
            oldContactIDs.push_back(_datas.at(0)->GetContactDataList().at(p)->_contactId);

        // 添加联系人
        ContactData* pContact = vCardEx::ImportFromVCards(files.at(i));
        vector<ContactData*> contacts;
        contacts.push_back(pContact);
        ImportContactData(&contacts, _datas.at(0), NULL);
        if (pContact->_tempGroupNames.empty())
            pContact->_tempGroupNames.push_back(L"未分组");
        vector<wstring> tempGroupNames = pContact->_tempGroupNames;

        // 添加组
        vector<wstring> groupNames;
        vector<ContactGroupData*> groups = _datas.at(0)->GetContactGroupList();
        for (size_t k = 0; k < groups.size(); k++)
            groupNames.push_back(groups.at(k)->GetName());

        vector<ContactGroupData*> addGroups;
        for (size_t j = 0; j < tempGroupNames.size(); j++)
        {
            if (find(groupNames.begin(), groupNames.end(), tempGroupNames.at(j)) != groupNames.end())
                continue;

            ContactGroupData* group = new ContactGroupData();
            group->SetName(pContact->_tempGroupNames.at(j));
            group->SetAccountName(_datas.at(0)->GetName());
            group->SetAccountType(_datas.at(0)->GetType());
            addGroups.push_back(group);

        }
        if (!addGroups.empty())
            _datas.at(0)->AddContactGroup(&addGroups);

        // 添加组里联系人
        Load(true);

        vector<int> newContactIDs;
        vector<ContactData*> newContact = _datas.at(0)->GetContactDataList();
        for (size_t n = 0; n < newContact.size(); n++)
            newContactIDs.push_back(newContact.at(n)->_contactId);

        for (size_t n = 0; n < newContactIDs.size(); n++)
        {
            if (find(oldContactIDs.begin(), oldContactIDs.end(), newContactIDs.at(n)) != oldContactIDs.end())
                continue;

            contactDataIdVec.push_back(newContactIDs.at(n));
        }

        vector<ContactGroupData*> newGroups = _datas.at(0)->GetContactGroupList();
        for (size_t l = 0; l < newGroups.size(); l++)
        {
            ContactGroupData* newGroup = newGroups.at(l);
            for (size_t m = 0; m < tempGroupNames.size(); m++)
            {
                if (newGroup->GetName() != tempGroupNames.at(m))
                    continue;

                vector<ContactData*> addContactList;
                for (size_t p = 0; p < newContact.size(); p++)
                {
                    if (contactDataIdVec.at(0) != newContact.at(p)->_contactId)
                        continue;

                    addContactList.push_back(newContact.at(p));
                    break;
                }
                _datas.at(0)->AddContactToGroup(&addContactList, newGroup, NULL);
            }
        }

        progress.IncreaseTaskNumberComplete();
        progress.SendProgress();

        progress.SetCurrentTaskSum(1);
        progress.SetCurrentTaskName(pContact->_sName._displayName);

        progress.AddTaskProgress(1);
        progress.SendProgress();

        ReleaseFromVCardContacts(pContact);
    }
}
*/
int ND91Assistant::ContactManager::ImportContactsFromVCards(vector<wstring> files, ProgressObserver* pObserver)
{
    RETURN_VALUE_IF(files.empty(), 0);

	this->Load(true);
	const vector<ContactAccount*>* pDatas = this->GetDatas();

	vector<ContactAccount*> localDatas;
	ContactAccount* pAccount = new ContactAccount(_pDevice);
	if(pDatas->size() > 0)
	{
		pAccount->SetName(pDatas->at(0)->GetName());
		pAccount->SetType(pDatas->at(0)->GetType());
	}
	localDatas.push_back(pAccount);

	ContactBackAndRestoreExtend::Parse(pAccount, files);
	int resCount = pAccount->GetContactDataList().size();
	ContactBackAndRestoreExtend::RemoveRepeatDatas(*pDatas, localDatas);

	BackupRestoreProgressSubject progress;
	progress.Attach(pObserver);
	progress.SetTaskNumberSum(1); 
	progress.SetAllTaskSum(100);
	progress.SendPrompt(MESSAGE_IMPORT);
	vector<ContactData*> contacts = pAccount->GetContactDataList();
	if(contacts.size() > 0)
	{
		progress.SetCurrentTaskName(L"Contact");
		progress.SetCurrentTaskSum(0);  
		progress.SendProgress();

		size_t count = 0;
		if (_pDevice->GetDevInfo()->_deviceType == Ios)
		{
			count = SumTotalContactNum(contacts);
			progress.SetCurrentTaskSum(count);
			progress.AddTaskProgress(1);
			progress.SendProgress();
		}

		ImportContactData(&contacts, _datas.at(0), pObserver);

		if (_pDevice->GetDevInfo()->_deviceType == Ios)
		{
			progress.SetCurrentTaskSum(_nSuccessCount);
			progress.AddTaskProgress(_nSuccessCount);
			progress.SendProgress();
		}
	}

	progress.SetCurrentTaskComplete();
	progress.IncreaseTaskNumberComplete();
	progress.SendProgress();
	progress.Complete();

	_nRepeatCount = resCount - pAccount->GetContactDataList().size();
    return _nSuccessCount;
}

size_t ND91Assistant::ContactManager::SumTotalContactNum(vector<ContactData*> contacts)
{
    size_t count = 0;
    count += contacts.size();
    for (vector<ContactData*>::iterator iter = contacts.begin(); iter != contacts.end(); iter++)
    {
        ContactData* pContact = *iter;
        count += pContact->_tempGroupNames.size();
    }

    return count;
}

void ND91Assistant::ContactManager::ReleaseFromVCardContacts(ContactData* pContact)
{
    size_t i = 0;
    for (i = 0; i < pContact->_emailList.size(); i++)
        SAFE_DELETE(pContact->_emailList.at(i));

    for (i = 0; i < pContact->_phoneNumberList.size(); i++)
        SAFE_DELETE(pContact->_phoneNumberList.at(i));
    
    for (i = 0; i < pContact->_homePageList.size(); i++)
        SAFE_DELETE(pContact->_homePageList.at(i));

    for (i = 0; i < pContact->_addressList.size(); i++)
        SAFE_DELETE(pContact->_addressList.at(i));

    for (i = 0; i < pContact->_imList.size(); i++)
        SAFE_DELETE(pContact->_imList.at(i));

    for (i = 0; i < pContact->_dateList.size(); i++)
        SAFE_DELETE(pContact->_dateList.at(i));

    for (i = 0; i < pContact->_relatedList.size(); i++)
        SAFE_DELETE(pContact->_relatedList.at(i));

    SAFE_DELETE(pContact);
}

#endif