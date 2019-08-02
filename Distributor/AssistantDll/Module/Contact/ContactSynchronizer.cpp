#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/ContactSynchronizer.h"
#include "Module/Contact/ContactParser.h"
#include "Module/Contact/ContactBuilder.h"
#include "Module/Contact/ContactGroupData.h"
#include "Module/Contact/ContactAccount.h"

#include "Device/IosUsbConnection.h"
#include "Module/Common/Synchronizer/DeviceSyncClient.h"

#define DATACLASSVERSION   106  // 当前pc请求dataClass对应的版本

ContactSynchronizer::ContactSynchronizer(const DeviceData* pDevice) : SynchronizerBase(pDevice)
{
    _shortDataClass     = "Contacts";
    _dataClass          = "com.apple.Contacts";
    _dataClassAnchor    = "Contacts-Device-Anchor";
    _dataClassVersion   = DATACLASSVERSION;

    //_useCache           = true;
    _cp                 = new ContactParser(this);
    _cb                 = new ContactBuilder(this);

    _contacts.clear();
    _groups.clear();
    _remappedIDs.clear();
    _sendMultiValueInfos.clear();
    _customLabels.clear();
}

ContactSynchronizer::~ContactSynchronizer()
{
    //ClearCachedData();
    SAFE_DELETE(_cp);
    SAFE_DELETE(_cb);
}

bool ContactSynchronizer::IsSupportSynchronizer()
{
    if (_device->GetDeviceInfo()->_connectType == WIFI)
    {
        return false;// NoSupportWIFIConnect
    }
    else if (_device->GetDeviceInfo()->_connectType == USB)
    {
        if (IsDisableSync(true))
            return false;// FailToSyncWithDevice
    }

    return true;
}

void ContactSynchronizer::AddContacts(ContactData* contact)
{
    if (find(_contacts.begin(), _contacts.end(), contact) == _contacts.end())
        _contacts.push_back(contact);
}

void ContactSynchronizer::AddGroups(ContactGroupData* group)
{
    _groups.push_back(group);
}

void ContactSynchronizer::AddSendMultiValueInfos(Category category, map<string, void*> sendMultiValueInfo)
{
    _sendMultiValueInfos.insert(pair< Category,map<string, void*> >(category, sendMultiValueInfo));
}

void ContactSynchronizer::AddSendInfo(void* info)
{
    _sendInfo.push_back(info);
}

ContactData* ContactSynchronizer::FindContact(string contactId)
{
    for (size_t i = 0; i < _contacts.size(); i++)
    {
        ContactData* contact = _contacts.at(i);
        if (contact->_identifier != contactId)
            continue;

        return contact;
    }

    return NULL;
}

void ContactSynchronizer::AddIntoDefaultAccount(ContactAccount* defaultAccount)
{
    for (size_t i = 0; i < _contacts.size(); i++)
        defaultAccount->InitContactData(_contacts.at(i));
    for (size_t i = 0; i < _groups.size(); i++)
        defaultAccount->InitContactGroupData(_groups.at(i));
}

void ContactSynchronizer::AddContactsToGroup(const vector<const ContactData*>* pDataList, ContactGroupData* pGroupData)
{
    for (size_t i = 0; i < _groups.size(); i++)
    {
        ContactGroupData* pGroup = _groups.at(i);
        if (pGroup->GetId() != pGroupData->GetId())
            continue;
        
        for (size_t j = 0; j < pDataList->size(); j++)
        {
            const ContactData* pContact = pDataList->at(j);
            pGroup->AddContacts(pContact->_contactId);
        }
        break;
    }
}

void ContactSynchronizer::RemoveContactsFromGroup(const vector<const ContactData*>* pDataList, ContactGroupData* pGroupData)
{
    for (size_t i = 0; i < _groups.size(); i++)
    {
        ContactGroupData* pGroup = _groups.at(i);
        if (pGroup->GetId() != pGroupData->GetId())
            continue;

        for (size_t j = 0; j < pDataList->size(); j++)
        {
            const ContactData* pContact = pDataList->at(j);
            pGroup->RemoveContacts(pContact->_contactId);
        }
        break;
    }
}

bool ContactSynchronizer::SaveGroupsNContacts()
{
    if (_contacts.empty() && _groups.empty())
        return true;

    return SaveChanges();
}

void ContactSynchronizer::DoSaveChanges()
{
    if (!_cb->BuildMainInfos(_contacts,_groups,false,true))
        return ;

    if (!_syncClient->SendProcessChangesToDevice(_sendInfo))
        return ;

     if (_syncClient->GetRemapIdentifiersFromDevice(_remappedIDs))
         ReassociateContactNGroupIDs();

    _sendMultiValueInfos.clear();
    if (!_cb->BuildMultiValueInfos(_contacts))
        return ;

    int i = 0;
    for (map< Category, map<string, void*> >::iterator it = _sendMultiValueInfos.begin(); it != _sendMultiValueInfos.end(); it++)
    {
        map< string, vector<void*> > action;
        _cb->BuildActionsForCategory(it->first,true,action);

        ReleaseSendInfo();
        if (!_syncClient->SendProcessChangesToDevice(it->second,(i == _sendMultiValueInfos.size()-1),action,_sendInfo))
            continue ;

        if (_syncClient->GetRemapIdentifiersFromDevice(_remappedIDs))
            ReassociatMultiValueIDs(it->first);

        i++;
    }
}

void ContactSynchronizer::ClearCachedData()
{
//     for (size_t i = 0; i < _contacts.size(); i++)
//         SAFE_DELETE(_contacts.at(i));
    _contacts.clear();

//     for (size_t i = 0; i < _groups.size(); i++)
//         SAFE_DELETE(_groups.at(i));
    _groups.clear();
}

void ContactSynchronizer::ParsePlistEntities()
{
    _cp->ParsePlistContact(_mapEntities);
}

void ContactSynchronizer::AddCustomLabels(string customLabel)
{
    if (find(_customLabels.begin(), _customLabels.end(), customLabel) == _customLabels.end())
        _customLabels.push_back(customLabel);
}

void ContactSynchronizer::RemoveGroupFromList(ContactGroupData* pGroup)
{
	for ( vector<ContactGroupData*>::iterator it=_groups.begin();it!=_groups.end();++it )
	{
		if ( (*it) == pGroup )
		{
			_groups.erase(it);
			break;
		}
	}
}

void ContactSynchronizer::ReassociateContactNGroupIDs()
{
    if (_remappedIDs.empty())
    {
        for (size_t i = 0; i < _groups.size(); i++)
        {
            if (_groups.at(i)->GetStatus() != BaseData::DEL)
            {
                _groups.at(i)->SetStatus(BaseData::NO_CHANGE);
                continue;
            }
            
            for( vector<ContactGroupData*>::iterator it = _groups.begin(); it != _groups.end();)
            {
                ContactGroupData *group = *it;
                if (group != _groups.at(i))
                {
                    ++it;
                    continue ;
                }
                it = _groups.erase(it);
                SAFE_DELETE(group);
            }
        }
    
        for (size_t i = 0; i < _contacts.size(); i++)
        {
            if (_contacts.at(i)->GetStatus() != BaseData::DEL)
            {
                _contacts.at(i)->SetStatus(BaseData::NO_CHANGE);
                continue;
            }
            
            for( vector<ContactData*>::iterator it = _contacts.begin(); it != _contacts.end();)
            {
                ContactData *contact = *it;
                if (contact != _contacts.at(i))
                {
                    ++it;
                    continue ;
                }
                it = _contacts.erase(it);
                SAFE_DELETE(contact);
            }
        }
    }
    else
    {
        // 分组处理
        for (size_t i = 0; i < _groups.size(); i++)
        {
            if (_groups.at(i)->GetStatus() == BaseData::DEL)
            {
                for( vector<ContactGroupData*>::iterator it = _groups.begin(); it != _groups.end();)
                {
                    ContactGroupData *group = *it;
                    if (group != _groups.at(i))
                    {
                        ++it;
                        continue ;
                    }
                    it = _groups.erase(it);
                    SAFE_DELETE(group);
                }
                continue;
            }

            // 重新映射组里面联系人id
            set<int> idSet = _groups.at(i)->GetContacts();
            for (set<int>::iterator it = idSet.begin(); it != idSet.end(); it++)
            {
                char id[256] = {0};
                sprintf(id, "%d", *it);
                string temp = id;
                map<string,string>::iterator iter = _remappedIDs.find(temp);
                if (iter == _remappedIDs.end())
                    continue;

                _groups.at(i)->AddContacts(atoi(iter->second.c_str()));
                _groups.at(i)->RemoveContacts(*it);
            }

            char id[256] = {0};
            sprintf(id, "%d", _groups.at(i)->GetId());
            string temp = id;
            map<string,string>::iterator iter = _remappedIDs.find(temp);
            if (iter != _remappedIDs.end())
            {
                _groups.at(i)->SetId(iter->second);
                if (find(_groups.begin(),_groups.end(),_groups.at(i)) == _groups.end())
                    _groups.push_back(_groups.at(i));
            }

            _groups.at(i)->SetStatus(BaseData::NO_CHANGE);
        }

        // 联系人处理
        for (size_t i = 0; i < _contacts.size(); i++)
        {
            if (_contacts.at(i)->GetStatus() == BaseData::DEL)
            {
                for( vector<ContactData*>::iterator it = _contacts.begin(); it != _contacts.end();)
                {
                    ContactData *contact = *it;
                    if (contact != _contacts.at(i))
                    {
                        ++it;
                        continue ;
                    }
                    it = _contacts.erase(it);
                    SAFE_DELETE(contact);
                }
                continue;
            }
            
            map<string,string>::iterator iter = _remappedIDs.find(_contacts.at(i)->_identifier);
            if (iter != _remappedIDs.end())
            {
                _contacts.at(i)->_identifier = iter->second;
                if (find(_contacts.begin(),_contacts.end(),_contacts.at(i)) == _contacts.end())
                    _contacts.push_back(_contacts.at(i));
            }

            _contacts.at(i)->SetStatus(BaseData::NO_CHANGE);
        }           
    }
}

void ContactSynchronizer::ReassociatMultiValueIDs(Category c)
{
    for (size_t i = 0; i < _contacts.size(); i++)
    {
        ContactData* p = _contacts.at(i);
        switch(c)
        {
        case Category_PhoneNumber:
            IterateReassociatePhoneMultiValueIDs(&p->_phoneNumberList);
            break;
        case Category_Email:
            IterateReassociateEmailMultiValueIDs(&p->_emailList);
            break;
        case Category_URL:
            IterateReassociateURLMultiValueIDs(&p->_homePageList);
            break;
        case Category_StreetAddress:
            IterateReassociateAddressMultiValueIDs(&p->_addressList);
            break;
        case Category_Date:
            IterateReassociateDateMultiValueIDs(&p->_dateList);
            break;
        case Category_IM:
            IterateReassociateIMMultiValueIDs(&p->_imList);
            break;
        case Category_Relates:
            IterateReassociateRelatesMultiValueIDs(&p->_relatedList);
            break;
        default:
            break;
        }
    }
}

void ContactSynchronizer::IterateReassociatePhoneMultiValueIDs(vector<Phone*>* phoneNumberList)
{
    for( vector<Phone*>::iterator it = phoneNumberList->begin(); it != phoneNumberList->end(); )
    {
        Phone* p = *it;
        if (p->_state == STATE_DEL)
        {
            it = phoneNumberList->erase(it);
            SAFE_DELETE(p);
        }
        else
        {
            map<string,string>::iterator iter = _remappedIDs.find(p->_key);
            if (iter != _remappedIDs.end())
            {
                p->_key = iter->second;
            }
            p->_state = STATE_NOCHANGE;

            ++it;
        }
    }
}

void ContactSynchronizer::IterateReassociateEmailMultiValueIDs(vector<Email*>* emailList)
{
    for( vector<Email*>::iterator it = emailList->begin(); it != emailList->end(); )
    {
        Email* e = *it;
        if (e->_state == STATE_DEL)
        {
            it = emailList->erase(it);
            SAFE_DELETE(e);
        }
        else
        {
            map<string,string>::iterator iter = _remappedIDs.find(e->_key);
            if (iter != _remappedIDs.end())
            {
                e->_key = iter->second;
            }
            e->_state = STATE_NOCHANGE;

            ++it;
        }
    }
}

void ContactSynchronizer::IterateReassociateURLMultiValueIDs(vector<HomePage*>* homePageList)
{
    for( vector<HomePage*>::iterator it = homePageList->begin(); it != homePageList->end(); )
    {
        HomePage* h = *it;
        if (h->_state == STATE_DEL)
        {
            it = homePageList->erase(it);
            SAFE_DELETE(h);
        }
        else
        {
            map<string,string>::iterator iter = _remappedIDs.find(h->_key);
            if (iter != _remappedIDs.end())
            {
                h->_key = iter->second;
            }
            h->_state = STATE_NOCHANGE;

            ++it;
        }
    }
}

void ContactSynchronizer::IterateReassociateAddressMultiValueIDs(vector<Address*>* addressList)
{
    for( vector<Address*>::iterator it = addressList->begin(); it != addressList->end(); )
    {
        Address* a = *it;
        if (a->_state == STATE_DEL)
        {
            it = addressList->erase(it);
            SAFE_DELETE(a);
        }
        else
        {
            map<string,string>::iterator iter = _remappedIDs.find(a->_key);
            if (iter != _remappedIDs.end())
            {
                a->_key = iter->second;
            }
            a->_state = STATE_NOCHANGE;

            ++it;
        }
    }
}
void ContactSynchronizer::IterateReassociateDateMultiValueIDs(vector<Day*>* dateList)
{
    for( vector<Day*>::iterator it = dateList->begin(); it != dateList->end(); )
    {
        Day* d = *it;
        if (d->_state == STATE_DEL)
        {
            it = dateList->erase(it);
            SAFE_DELETE(d);
        }
        else
        {
            map<string,string>::iterator iter = _remappedIDs.find(d->_key);
            if (iter != _remappedIDs.end())
            {
                d->_key = iter->second;
            }
            d->_state = STATE_NOCHANGE;

            ++it;
        }
    }
}

void ContactSynchronizer::IterateReassociateIMMultiValueIDs(vector<Im*>* imList)
{
    for( vector<Im*>::iterator it = imList->begin(); it != imList->end(); )
    {
        Im* i = *it;
        if (i->_state == STATE_DEL)
        {
            it = imList->erase(it);
            SAFE_DELETE(i);
        }
        else
        {
            map<string,string>::iterator iter = _remappedIDs.find(i->_key);
            if (iter != _remappedIDs.end())
            {
                i->_key = iter->second;
            }
            i->_state = STATE_NOCHANGE;

            ++it;
        }
    }
}

void ContactSynchronizer::IterateReassociateRelatesMultiValueIDs(vector<Related*>* relatedList)
{
    for( vector<Related*>::iterator it = relatedList->begin(); it != relatedList->end(); )
    {
        Related* r = *it;
        if (r->_state == STATE_DEL)
        {
            it = relatedList->erase(it);
            SAFE_DELETE(r);
        }
        else
        {
            map<string,string>::iterator iter = _remappedIDs.find(r->_key);
            if (iter != _remappedIDs.end())
            {
                r->_key = iter->second;
            }
            r->_state = STATE_NOCHANGE;

            ++it;
        }
    }
}

#endif

