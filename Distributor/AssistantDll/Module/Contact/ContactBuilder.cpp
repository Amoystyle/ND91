#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/ContactGroupData.h"
#include "Module/Contact/ContactBuilder.h"
#include "Module/Contact/ContactParser.h"
#include "Module/Contact/ContactSynchronizer.h"

#include "Device/CFConverter.h"
#include "Module/Common/Synchronizer/DeviceSyncClient.h"

ContactBuilder::ContactBuilder(ContactSynchronizer* contactSynchronizer)
{
    _contactSynchronizer = contactSynchronizer;
}

ContactBuilder::~ContactBuilder()
{
}

bool ContactBuilder::BuildMainInfos(vector<ContactData*> contacts, vector<ContactGroupData*> groups,bool isHaveRecord, bool linkAllRecordsOfPulledEntityTypeSentKey)
{
    if (contacts.empty() && groups.empty()) 
        return false;

    _contactSynchronizer->ReleaseSendInfo();

    // 开始写数组内容
    _contactSynchronizer->AddSendInfo(CCFConverter::StringtoCFString("SDMessageProcessChanges"));
    _contactSynchronizer->AddSendInfo(CCFConverter::StringtoCFString(_contactSynchronizer->_dataClass));

    // 写实体内容
    map<string, string> dict;
    if (contacts.empty() && groups.empty())
    {
        _contactSynchronizer->AddSendInfo(CCFConverter::DictionarytoCFDictionary(dict));
    }
    else
    {
        map<string, void*> temp;
        for (size_t i = 0; i < contacts.size(); i++)
        {
            ContactData* contact = contacts.at(i);
            if (contact->GetStatus() == BaseData::NO_CHANGE)
                continue;

            map<string, void*> mapInfo;
            contact->ConvertToDictInfos(mapInfo);
            if (mapInfo.empty())// 删除
            {
                temp.insert(pair<string, void*>(contact->_identifier, CCFConverter::StringtoCFString(DeviceSyncClient::EMPTY_PARAMETER_STRING)));
            }
            else // 添加或修改
            {
                temp.insert(pair<string, void*>(contact->_identifier,CCFConverter::DictionarytoCFDictionary(mapInfo)));
            }
        }

        for (size_t i = 0; i < groups.size(); i++)
        {
            ContactGroupData* group = groups.at(i);
            if (group->GetStatus() == BaseData::NO_CHANGE)
                continue;

            map<string, void*> mapInfo;
            group->ConvertToDictInfos(mapInfo);
            if (mapInfo.empty())// 删除
            {
                temp.insert(pair<string, void*>(group->GetKey(), CCFConverter::StringtoCFString(DeviceSyncClient::EMPTY_PARAMETER_STRING)));
            }
            else // 添加或修改
            {
                temp.insert(pair<string, void*>(group->GetKey(),CCFConverter::DictionarytoCFDictionary(mapInfo)));
            }
        }
        RETURN_FALSE_IF(temp.empty());
        _contactSynchronizer->AddSendInfo(CCFConverter::DictionarytoCFDictionary(temp));
    }

    // 是否还有数据
    _contactSynchronizer->AddSendInfo(CCFConverter::BoolToCFBool(!isHaveRecord));

    // sync actions
    vector<void*> linkEntityNamesKey;
    linkEntityNamesKey.push_back(CCFConverter::StringtoCFString(ContactParser::CONTACT_ENTITY_NAME));
    linkEntityNamesKey.push_back(CCFConverter::StringtoCFString(ContactParser::GROUP_ENTITY_NAME));
    vector<void*> linkTypeSentKey;
    linkTypeSentKey.push_back(CCFConverter::BoolToCFBool(linkAllRecordsOfPulledEntityTypeSentKey));

    map< string, vector<void*> > dict1;
    dict1.insert(pair< string,vector<void*> >("SyncDeviceLinkEntityNamesKey",linkEntityNamesKey));
    dict1.insert(pair< string,vector<void*> >("SyncDeviceLinkAllRecordsOfPulledEntityTypeSentKey",linkTypeSentKey));

    _contactSynchronizer->AddSendInfo(CCFConverter::ConvertStringVectorMap(dict1));

    return true;
}

bool ContactBuilder::BuildMultiValueInfos(vector<ContactData*> contacts)
{
    map<string, void*> pns;
    map<string, void*> urls;
    map<string, void*> dates;
    map<string, void*> ims;
    map<string, void*> emails;
    map<string, void*> streetAddresss;
    map<string, void*> relates;

    for (size_t i = 0; i < contacts.size(); i++)
    {
        ContactData* p = contacts.at(i);
        if (p->GetStatus() == BaseData::DEL)
            continue;

        BuildMultiValuePhoneNumber(p->_identifier, Category_PhoneNumber, pns, &p->_phoneNumberList);
        BuildMultiValueEmail(p->_identifier, Category_Email, emails, &p->_emailList);
        BuildMultiValueURL(p->_identifier, Category_URL, urls, &p->_homePageList);
        BuildMultiValueRelated(p->_identifier, Category_Relates, relates, &p->_relatedList);
        BuildMultiValueDate(p->_identifier, Category_Date, dates, &p->_dateList);

        BuildMultiValueAddress(p->_identifier, Category_StreetAddress, streetAddresss, &p->_addressList);
        BuildMultiValueIM(p->_identifier, Category_IM, ims, &p->_imList);
    }

    if (!pns.empty())
        _contactSynchronizer->AddSendMultiValueInfos(Category_PhoneNumber,pns);
    if (!emails.empty())
        _contactSynchronizer->AddSendMultiValueInfos(Category_Email,emails);
    if (!ims.empty())
        _contactSynchronizer->AddSendMultiValueInfos(Category_IM,ims);
    if (!streetAddresss.empty())
        _contactSynchronizer->AddSendMultiValueInfos(Category_StreetAddress,streetAddresss);
    if (!urls.empty())
        _contactSynchronizer->AddSendMultiValueInfos(Category_URL,urls);
    if (!dates.empty())
        _contactSynchronizer->AddSendMultiValueInfos(Category_Date,dates);
    if (!relates.empty())
        _contactSynchronizer->AddSendMultiValueInfos(Category_Relates,relates);

    return true;
}

void ContactBuilder::BuildActionsForCategory(Category category,bool linkAllRecordsOfPulledEntityTypeSentKey,map< string, vector<void*> >& actions)
{
    // sync actions
    vector<void*> linkEntityNamesKey;
    switch (category)
    {
    case Category_PhoneNumber:
        linkEntityNamesKey.push_back(CCFConverter::StringtoCFString(ContactParser::PHONENUMBER_ENTITY_NAME));
        break;
    case Category_Email:
        linkEntityNamesKey.push_back(CCFConverter::StringtoCFString(ContactParser::EMAIL_ENTITY_NAME));
        break;
    case Category_IM:
        linkEntityNamesKey.push_back(CCFConverter::StringtoCFString(ContactParser::IM_ENTITY_NAME));
        break;
    case Category_StreetAddress:
        linkEntityNamesKey.push_back(CCFConverter::StringtoCFString(ContactParser::ADDRESS_ENTITY_NAME));
        break;
    case Category_URL:
        linkEntityNamesKey.push_back(CCFConverter::StringtoCFString(ContactParser::URL_ENTITY_NAME));
        break;
    case Category_Date:
        linkEntityNamesKey.push_back(CCFConverter::StringtoCFString(ContactParser::DATE_ENTITY_NAME));
        break;
    case Category_Relates:
        linkEntityNamesKey.push_back(CCFConverter::StringtoCFString(ContactParser::RELATE_ENTITY_NAME));
        break;

    default:
        break;
    }
    
    vector<void*> linkTypeSentKey;
    linkTypeSentKey.push_back(CCFConverter::BoolToCFBool(linkAllRecordsOfPulledEntityTypeSentKey));
    actions.insert(pair< string,vector<void*> >("SyncDeviceLinkEntityNamesKey",linkEntityNamesKey));
    actions.insert(pair< string,vector<void*> >("SyncDeviceLinkAllRecordsOfPulledEntityTypeSentKey",linkTypeSentKey));
}

const char* PhoneNumber_Labels[] = 
{
    "other",
    "custom",
    "other",
    "home",
    "work",
    "mobile",
    "iphone",
    "main",
    "home fax",
    "work fax",
    "_$!<otherfax>!$_",
    "pager"
};

string ContactBuilder::LabelTypeToPredefineType(const char* labels[], int type)
{
    return labels[type];
}

void ContactBuilder::BuildMultiValuePhoneNumber(string contactId, Category category, map<string, void*>& main, vector<Phone*>* phoneNumbers)
{
    int count = phoneNumbers->size() + 7;
    for (size_t i = 0; i < phoneNumbers->size(); i++)
    {
        Phone* phoneNumber = phoneNumbers->at(i);
        if (phoneNumber->_state == STATE_NOCHANGE)
            continue;

        map<string, void*> infos;
        infos.insert(pair<string, void*>(DeviceSyncClient::RECORD_ENTITY_NAME_KEY, CCFConverter::StringtoCFString(ContactParser::PHONENUMBER_ENTITY_NAME)));
        if (phoneNumber->_type == PHONE_IPHONES || phoneNumber->_type == PHONE_OTHERFAX)
        {
            infos.insert(pair<string, void*>("type", CCFConverter::StringtoCFString(LabelTypeToPredefineType(PhoneNumber_Labels,PHONE_OTHER))));
            if (phoneNumber->_type == PHONE_IPHONES)
                infos.insert(pair<string, void*>("label", CCFConverter::StringtoCFString("iPhone")));
            else
                infos.insert(pair<string, void*>("label", CCFConverter::StringtoCFString("_$!<OtherFAX>!$_")));
        }
        else if (phoneNumber->_type == PHONE_CUSTOM)
        {
            if (!phoneNumber->_label.empty())
                infos.insert(pair<string, void*>("label", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(phoneNumber->_label))));
            infos.insert(pair<string, void*>("type", CCFConverter::StringtoCFString(LabelTypeToPredefineType(PhoneNumber_Labels,PHONE_OTHER))));
        }
        else
        {
            infos.insert(pair<string, void*>("type", CCFConverter::StringtoCFString(LabelTypeToPredefineType(PhoneNumber_Labels,phoneNumber->_type))));
            if (!phoneNumber->_label.empty())
                infos.insert(pair<string, void*>("label", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(phoneNumber->_label))));
        }

        vector<void*> mainIDS;
        mainIDS.push_back(CCFConverter::StringtoCFString(contactId));
        infos.insert(pair<string, void*>(ContactParser::CONTACT_KEY, CCFConverter::ArrayToCFArray(mainIDS)));
        infos.insert(pair<string, void*>("value", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(phoneNumber->_number))));

        if (phoneNumber->_state == STATE_ADD)
        {
            char temp[125] = {0};
            sprintf(temp, "%d/%s/%d", (int)category, contactId.c_str(), count);
            string uid = temp;
            phoneNumber->_key = uid;
            main.insert(pair<string, void*>(uid,CCFConverter::DictionarytoCFDictionary(infos)));
            count++;
        }
        else if (phoneNumber->_state == STATE_DEL)
            main.insert(pair<string, void*>(phoneNumber->_key,CCFConverter::StringtoCFString(DeviceSyncClient::EMPTY_PARAMETER_STRING)));
        else
            main.insert(pair<string, void*>(phoneNumber->_key,CCFConverter::DictionarytoCFDictionary(infos)));
    }
}

const char* Email_Labels[] = 
{
    "other",
    "custom",
    "other",
    "home",
    "work",
    "mobile"
};

void ContactBuilder::BuildMultiValueEmail(string contactId, Category category, map<string, void*>& main, vector<Email*>* emails)
{
    int count = emails->size() + 7;
    for (size_t i = 0; i < emails->size(); i++)
    {
        Email* email = emails->at(i);
        if (email->_state ==  STATE_NOCHANGE)
            continue;

        map<string, void*> infos;
        infos.insert(pair<string, void*>(DeviceSyncClient::RECORD_ENTITY_NAME_KEY, CCFConverter::StringtoCFString(ContactParser::EMAIL_ENTITY_NAME)));
        if (email->_type == EMAIL_CUSTOM)
        {
            if (!email->_label.empty())
                infos.insert(pair<string, void*>("label", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(email->_label))));
            infos.insert(pair<string, void*>("type", CCFConverter::StringtoCFString(LabelTypeToPredefineType(Email_Labels, EMAIL_OTHER))));
        }
        else
        {
            infos.insert(pair<string, void*>("type", CCFConverter::StringtoCFString(LabelTypeToPredefineType(Email_Labels, email->_type))));
            if (!email->_label.empty())
                infos.insert(pair<string, void*>("label", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(email->_label))));
        }

        vector<void*> mainIDS;
        mainIDS.push_back(CCFConverter::StringtoCFString(contactId));
        infos.insert(pair<string, void*>(ContactParser::CONTACT_KEY, CCFConverter::ArrayToCFArray(mainIDS)));
        infos.insert(pair<string, void*>("value", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(email->_emailAccount))));

        if (email->_state == STATE_ADD)
        {
            char temp[125] = {0};
            sprintf(temp, "%d/%s/%d", (int)category, contactId.c_str(), count);
            string uid = temp;
            email->_key = uid;
            main.insert(pair<string, void*>(uid,CCFConverter::DictionarytoCFDictionary(infos)));
            count++;
        }
        else if (email->_state == STATE_DEL)
            main.insert(pair<string, void*>(email->_key,CCFConverter::StringtoCFString(DeviceSyncClient::EMPTY_PARAMETER_STRING)));
        else
            main.insert(pair<string, void*>(email->_key,CCFConverter::DictionarytoCFDictionary(infos)));
    }
}

const char* Im_Labels[] = 
{
    "other",
    "custom",
    "other",
    "home",
    "work"
};

const char* ImName_Labels[] = 
{
    "custom",
    "custom",
    "aim",
    "windows",
    "yahoo",
    "skype",
    "qq",
    "google",
    "icq",
    "jabber",
    "fetion",
    "msn",
    "facebook",
    "gadugadu"
};

void ContactBuilder::BuildMultiValueIM(string contactId, Category category, map<string, void*>& main, vector<Im*>* ims)
{
    int count = ims->size() + 7;
    for (size_t i = 0; i < ims->size(); i++)
    {
        Im* im = ims->at(i);
        if (im->_state == STATE_NOCHANGE)
            continue;

        map<string, void*> infos;
        infos.insert(pair<string, void*>(DeviceSyncClient::RECORD_ENTITY_NAME_KEY, CCFConverter::StringtoCFString(ContactParser::IM_ENTITY_NAME)));
        if (im->_type == IM_CUSTOM)
        {
            if (!im->_label.empty())
                infos.insert(pair<string, void*>("label", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(im->_label))));
            infos.insert(pair<string, void*>("type", CCFConverter::StringtoCFString(LabelTypeToPredefineType(Im_Labels, IM_OTHER))));
        }
        else
        {
            infos.insert(pair<string, void*>("type", CCFConverter::StringtoCFString(LabelTypeToPredefineType(Im_Labels, im->_type))));
            if (!im->_label.empty())
                infos.insert(pair<string, void*>("label", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(im->_label))));
        }

        vector<void*> mainIDS;
        mainIDS.push_back(CCFConverter::StringtoCFString(contactId));
        infos.insert(pair<string, void*>(ContactParser::CONTACT_KEY, CCFConverter::ArrayToCFArray(mainIDS)));

        infos.insert(pair<string, void*>("service", CCFConverter::StringtoCFString(LabelTypeToPredefineType(ImName_Labels, im->_typeName))));
        infos.insert(pair<string, void*>("user", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(im->_imAccount))));

        if (im->_state == STATE_ADD)
        {
            char temp[125] = {0};
            sprintf(temp, "%d/%s/%d", (int)category, contactId.c_str(), count);
            string uid = temp;
            im->_key = uid;
            main.insert(pair<string, void*>(uid,CCFConverter::DictionarytoCFDictionary(infos)));
            count++;
        }
        else if (im->_state == STATE_DEL)
            main.insert(pair<string, void*>(im->_key,CCFConverter::StringtoCFString(DeviceSyncClient::EMPTY_PARAMETER_STRING)));
        else 
            main.insert(pair<string, void*>(im->_key,CCFConverter::DictionarytoCFDictionary(infos)));
    }
}

const char* Address_Labels[] = 
{
    "other",
    "custom",
    "other",
    "home",
    "work"
};

void ContactBuilder::BuildMultiValueAddress(string contactId, Category category, map<string, void*>& main, vector<Address*>* addresses)
{
    int count = addresses->size() + 7;
    for (size_t i = 0; i < addresses->size(); i++)
    {
        Address* address = addresses->at(i);
        if (address->_state == STATE_NOCHANGE)
            continue;

        map<string, void*> infos;
        infos.insert(pair<string, void*>(DeviceSyncClient::RECORD_ENTITY_NAME_KEY, CCFConverter::StringtoCFString(ContactParser::ADDRESS_ENTITY_NAME)));
        if (address->_type == ADDRESS_CUSTOM)
        {
            if (!address->_label.empty())
                infos.insert(pair<string, void*>("label", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(address->_label))));
            infos.insert(pair<string, void*>("type", CCFConverter::StringtoCFString(LabelTypeToPredefineType(Address_Labels, ADDRESS_OTHER))));
        }
        else
        {
            infos.insert(pair<string, void*>("type", CCFConverter::StringtoCFString(LabelTypeToPredefineType(Address_Labels, address->_type))));
            if (!address->_label.empty())
                infos.insert(pair<string, void*>("label", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(address->_label))));
        }

        vector<void*> mainIDS;
        mainIDS.push_back(CCFConverter::StringtoCFString(contactId));
        infos.insert(pair<string, void*>(ContactParser::CONTACT_KEY, CCFConverter::ArrayToCFArray(mainIDS)));

        infos.insert(pair<string, void*>("street", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(address->_street))));
        infos.insert(pair<string, void*>("postal code", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(address->_zipCode))));
        infos.insert(pair<string, void*>("city", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(address->_city))));
        infos.insert(pair<string, void*>("country", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(address->_country))));
        infos.insert(pair<string, void*>("country code", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(address->_countryCode))));
        infos.insert(pair<string, void*>("state", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(address->_province))));

        if (address->_state == STATE_ADD)
        {
            char temp[125] = {0};
            sprintf(temp, "%d/%s/%d", (int)category, contactId.c_str(), count);
            string uid = temp;
            address->_key = uid;
            main.insert(pair<string, void*>(uid,CCFConverter::DictionarytoCFDictionary(infos)));
            count++;
        }
        else if (address->_state == STATE_DEL)
            main.insert(pair<string, void*>(address->_key,CCFConverter::StringtoCFString(DeviceSyncClient::EMPTY_PARAMETER_STRING)));
        else 
            main.insert(pair<string, void*>(address->_key,CCFConverter::DictionarytoCFDictionary(infos)));
    }
}

const char* URL_Labels[] = 
{
    "other",
    "custom",
    "other",
    "home",
    "work",
    "home page"
};

void ContactBuilder::BuildMultiValueURL(string contactId, Category category, map<string, void*>& main, vector<HomePage*>* urls)
{
    int count = urls->size() + 7;
    for (size_t i = 0; i < urls->size(); i++)
    {
        HomePage* homePage = urls->at(i);
        if (homePage->_state == STATE_NOCHANGE)
            continue;

        map<string, void*> infos;
        infos.insert(pair<string, void*>(DeviceSyncClient::RECORD_ENTITY_NAME_KEY, CCFConverter::StringtoCFString(ContactParser::URL_ENTITY_NAME)));
        if (homePage->_type == HOMEPAGE_CUSTOM)
        {
            if (!homePage->_label.empty())
                infos.insert(pair<string, void*>("label", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(homePage->_label))));
            infos.insert(pair<string, void*>("type", CCFConverter::StringtoCFString(LabelTypeToPredefineType(URL_Labels, HOMEPAGE_OTHER))));
        }
        else
        {
            infos.insert(pair<string, void*>("type", CCFConverter::StringtoCFString(LabelTypeToPredefineType(URL_Labels, homePage->_type))));
            if (!homePage->_label.empty())
                infos.insert(pair<string, void*>("label", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(homePage->_label))));
        }

        vector<void*> mainIDS;
        mainIDS.push_back(CCFConverter::StringtoCFString(contactId));
        infos.insert(pair<string, void*>(ContactParser::CONTACT_KEY, CCFConverter::ArrayToCFArray(mainIDS)));
        infos.insert(pair<string, void*>("value", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(homePage->_name))));

        if (homePage->_state == STATE_ADD)
        {
            char temp[125] = {0};
            sprintf(temp, "%d/%s/%d", (int)category, contactId.c_str(), count);
            string uid = temp;
            homePage->_key = uid;
            main.insert(pair<string, void*>(uid,CCFConverter::DictionarytoCFDictionary(infos)));
            count++;
        }
        else if (homePage->_state == STATE_DEL)
            main.insert(pair<string, void*>(homePage->_key,CCFConverter::StringtoCFString(DeviceSyncClient::EMPTY_PARAMETER_STRING)));
        else 
            main.insert(pair<string, void*>(homePage->_key,CCFConverter::DictionarytoCFDictionary(infos)));
    }
}

const char* Date_Labels[] = 
{
    "other",
    "custom",
    "other",
    "home",
    "work",
    "anniversary"
};

void ContactBuilder::BuildMultiValueDate(string contactId, Category category, map<string, void*>& main, vector<Day*>* dates)
{
    int count = dates->size() + 7;
    for (size_t i = 0; i < dates->size(); i++)
    {
        Day* date = dates->at(i);
        if (date->_state == STATE_NOCHANGE)
            continue;

        map<string, void*> infos;
        infos.insert(pair<string, void*>(DeviceSyncClient::RECORD_ENTITY_NAME_KEY, CCFConverter::StringtoCFString(ContactParser::DATE_ENTITY_NAME)));
        if (date->_type == DAY_CUSTOM)
        {
            if (!date->_label.empty())
                infos.insert(pair<string, void*>("label", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(date->_label))));
            infos.insert(pair<string, void*>("type", CCFConverter::StringtoCFString(LabelTypeToPredefineType(Date_Labels, DAY_OTHER))));
        }
        else
        {
            infos.insert(pair<string, void*>("type", CCFConverter::StringtoCFString(LabelTypeToPredefineType(Date_Labels, date->_type))));
            if (!date->_label.empty())
                infos.insert(pair<string, void*>("label", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(date->_label))));
        }

        vector<void*> mainIDS;
        mainIDS.push_back(CCFConverter::StringtoCFString(contactId));
        infos.insert(pair<string, void*>(ContactParser::CONTACT_KEY, CCFConverter::ArrayToCFArray(mainIDS)));
        infos.insert(pair<string, void*>("value", CCFConverter::TimeToCFDate((time_t)atoi(CCodeOperation::UnicodeToGB2312(date->_name).c_str()))));

        if (date->_state == STATE_ADD)
        {
            char temp[125] = {0};
            sprintf(temp, "%d/%s/%d", (int)category, contactId.c_str(), count);
            string uid = temp;
            date->_key = uid;
            main.insert(pair<string, void*>(uid,CCFConverter::DictionarytoCFDictionary(infos)));
            count++;
        }
        else if (date->_state == STATE_DEL)
            main.insert(pair<string, void*>(date->_key,CCFConverter::StringtoCFString(DeviceSyncClient::EMPTY_PARAMETER_STRING)));
        else 
            main.insert(pair<string, void*>(date->_key,CCFConverter::DictionarytoCFDictionary(infos)));
    }
}

const char* Related_Labels[] = 
{
    "other",
    "custom",
    "other",
    "father",
    "mother",
    "parent",
    "brother",
    "sister",
    "child",
    "friend",
    "spouse",
    "partner",
    "assistant",
    "manager"
};

void ContactBuilder::BuildMultiValueRelated(string contactId, Category category, map<string, void*>& main, vector<Related*>* relateds)
{
    int count = relateds->size() + 7;
    for (size_t i = 0; i < relateds->size(); i++)
    {
        Related* related = relateds->at(i);
        if (related->_state == STATE_NOCHANGE)
            continue;

        map<string, void*> infos;
        infos.insert(pair<string, void*>(DeviceSyncClient::RECORD_ENTITY_NAME_KEY, CCFConverter::StringtoCFString(ContactParser::RELATE_ENTITY_NAME)));
        if (related->_type == RELATED_CUSTOM)
        {
            if (!related->_label.empty())
                infos.insert(pair<string, void*>("label", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(related->_label))));
            infos.insert(pair<string, void*>("type", CCFConverter::StringtoCFString(LabelTypeToPredefineType(Related_Labels, RELATED_OTHER))));
        }
        else
        {
            infos.insert(pair<string, void*>("type", CCFConverter::StringtoCFString(LabelTypeToPredefineType(Related_Labels, related->_type))));
            if (!related->_label.empty())
                infos.insert(pair<string, void*>("label", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(related->_label))));
        }

        vector<void*> mainIDS;
        mainIDS.push_back(CCFConverter::StringtoCFString(contactId));
        infos.insert(pair<string, void*>(ContactParser::CONTACT_KEY, CCFConverter::ArrayToCFArray(mainIDS)));
        infos.insert(pair<string, void*>("value", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(related->_name))));

        if (related->_state == STATE_ADD)
        {
            char temp[125] = {0};
            sprintf(temp, "%d/%s/%d", (int)category, contactId.c_str(), count);
            string uid = temp;
            related->_key = uid;
            main.insert(pair<string, void*>(uid,CCFConverter::DictionarytoCFDictionary(infos)));
            count++;
        }
        else if (related->_state == STATE_DEL)
            main.insert(pair<string, void*>(related->_key,CCFConverter::StringtoCFString(DeviceSyncClient::EMPTY_PARAMETER_STRING)));
        else 
            main.insert(pair<string, void*>(related->_key,CCFConverter::DictionarytoCFDictionary(infos)));
    }
}


#endif