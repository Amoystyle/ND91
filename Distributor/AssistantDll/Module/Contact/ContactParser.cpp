#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/ContactParser.h"
#include "Module/Contact/ContactGroupData.h"
#include "Module/Contact/ContactSynchronizer.h"

#include "Device/CFConverter.h"
#include "Module/Common/Synchronizer/DeviceSyncClient.h"

string ContactParser::DISPLYA_AS_COMPANY        = "display as company";
string ContactParser::CONTACT_KEY               = "contact";
string ContactParser::GROUP_ENTITY_NAME         = "com.apple.contacts.Group";
string ContactParser::CONTACT_ENTITY_NAME       = "com.apple.contacts.Contact";
string ContactParser::EMAIL_ENTITY_NAME         = "com.apple.contacts.Email Address";
string ContactParser::URL_ENTITY_NAME           = "com.apple.contacts.URL";
string ContactParser::PHONENUMBER_ENTITY_NAME   = "com.apple.contacts.Phone Number";
string ContactParser::ADDRESS_ENTITY_NAME       = "com.apple.contacts.Street Address";
string ContactParser::DATE_ENTITY_NAME          = "com.apple.contacts.Date";
string ContactParser::IM_ENTITY_NAME            = "com.apple.contacts.IM";
string ContactParser::RELATE_ENTITY_NAME        = "com.apple.contacts.Related Name";

ContactParser::ContactParser(ContactSynchronizer* contactSynchronizer)
{
    _contactSynchronizer = contactSynchronizer;
}

ContactParser::~ContactParser()
{
}

bool ContactParser::ParsePlistContact(map< string, map<string,void*> > mapEntities)
{
    if (mapEntities.empty())
    {
        LOG->WriteDebug(L"Invalid entities data recieved");
        return false;//throw new Exception("Invalid entities data recieved");
    }

    for (map< string, map<string,void*> >::iterator it = mapEntities.begin(); it != mapEntities.end(); it++)
    {
        string temp = it->first;
        temp = CStrOperation::toLower(temp);
        if (temp.find("group") != string::npos)
            ParseGroupInfos(it->first, it->second);
        else
            ParseContactInfos(it->first, it->second);
    }

    return true;
}

string ContactParser::IsValidEntity(map<string,void*> entity)
{
    if (entity.empty())
    {
        LOG->WriteDebug(L"Unexpected type of args entity");
        return ""; //throw "Unexpected type of args entity";
    }

    map<string, void*>::iterator it = entity.find(DeviceSyncClient::RECORD_ENTITY_NAME_KEY);
    if (it == entity.end())
    {
        LOG->WriteDebug(L"missing entity name (com.apple.syncservices.RecordEntityName field)");
        return "";//throw "missing entity name (com.apple.syncservices.RecordEntityName field)";
    }

    string entityName;
    CCFConverter::GetIosData(it->second, entityName);
    return entityName;
}

bool ContactParser::ParseGroupInfos(string key, map<string,void*> entity)
{
    // 验证设备返回的实体数据有效性
    string entityName = IsValidEntity(entity);
    if (entityName.empty())
        return false;

    // 有效则添加组数据
    _contactSynchronizer->AddGroups(new ContactGroupData(key,entity));

    return true;
}

bool ContactParser::ParseContactInfos(string key, map<string,void*> entity)
{
    // 验证设备返回的实体数据有效性
    string entityName = IsValidEntity(entity);
    if (entityName.empty())
        return false;

    // 有效则添加联系人数据
    ContactData* contact = NULL;
    if (entityName == CONTACT_ENTITY_NAME)
    {
        //create new contact
        contact = CreateContactFromDictInfos(key, entity);
        if (contact == NULL)
        {
            LOG->WriteDebug(L"Can't create Contact from plist infos");
            return false;//throw "Can't create Contact from plist infos";
        }

        _contactSynchronizer->AddContacts(contact);
    }
    else
    {
        //get the exist contcat 
        contact = GetExistContact(entity);
        if (contact == NULL)
        {
            LOG->WriteDebug(L"Unknow error happened in finding exist contact");
            return false;//throw "Unknow error happened in finding exist contact";
        }

        if (entityName == EMAIL_ENTITY_NAME)
            contact->_emailList.push_back(ParseContactEmail(key, entity));
        else if (entityName == URL_ENTITY_NAME)
            contact->_homePageList.push_back(ParseContactURL(key, entity));
        else if (entityName == PHONENUMBER_ENTITY_NAME)
            contact->_phoneNumberList.push_back(ParseContactPhoneNumber(key, entity));
        else if (entityName == ADDRESS_ENTITY_NAME)
            contact->_addressList.push_back(ParseContactAddress(key, entity));
        else if (entityName == DATE_ENTITY_NAME)
            contact->_dateList.push_back(ParseContactDate(key, entity));
        else if (entityName == IM_ENTITY_NAME)
            contact->_imList.push_back(ParseContactIM(key, entity));
        else if (entityName == RELATE_ENTITY_NAME)
            contact->_relatedList.push_back(ParseContactRelated(key, entity));
        else
        {
            LOG->WriteDebug(L"Unknow Entity encountered");
            return false;//throw "Unknow Entity encountered {0}", entityName;
        }
    }

    return true;
}

void ContactParser::ParseContactBaseInfo(map<string,void*> entity, string& type, string& value, string& label)
{
    for (map<string, void*>:: iterator it = entity.begin(); it != entity.end(); it++)
    {
        string temp;
        time_t tempTime = 0;
        if (CCFConverter::NDCFGetTypeID(it->second) == CCFConverter::NDCFStringGetTypeID())
            CCFConverter::GetIosData(it->second, temp);
        else if (CCFConverter::NDCFGetTypeID(it->second) == CCFConverter::NDCFDateGetTypeID())
            CCFConverter::GetIosData(it->second, tempTime);

        if (it->first == "type")
            type = temp;
        else if (it->first == "value")
        {
            if (tempTime != 0)
            {
                char time[128] = {0};
                sprintf(time, "%d", (int)tempTime);
                temp = time;
            }
            value = temp;
        }
        else if (it->first == "label")
            label = temp;
    }
}

Address* ContactParser::ParseContactAddress(string key, map<string,void*> entity)
{
    Address* address    = new Address();
    address->_key       = key;

    string type;
    string value;
    string label;
    ParseContactBaseInfo(entity, type, value, label);

    address->_type = PredefineTypeToLabelTypeAddress(type);
    for (map<string, void*>:: iterator it = entity.begin(); it != entity.end(); it++)
    {
        string str = "";
        if (CCFConverter::NDCFGetTypeID(it->second) == CCFConverter::NDCFStringGetTypeID())
            CCFConverter::GetIosData(it->second, str);

        if (it->first == "state")
            address->_province = CCodeOperation::GB2312ToUnicode(str);
        else if (it->first == "city")
            address->_city = CCodeOperation::GB2312ToUnicode(str);
        else if (it->first == "street")
            address->_street = CCodeOperation::GB2312ToUnicode(str);
        else if (it->first == "postal code")
            address->_zipCode = CCodeOperation::GB2312ToUnicode(str);
        else if (it->first == "country")
            address->_country = CCodeOperation::GB2312ToUnicode(str);
        else if (it->first == "country code")
            address->_countryCode = CCodeOperation::GB2312ToUnicode(str);
    }
    address->_displayAddress = address->_country + address->_province + address->_city + address->_street + L" " + address->_zipCode;

    if (label.empty())
        return address;

    address->_label = CCodeOperation::GB2312ToUnicode(label);
    address->_type = PredefineTypeToLabelTypeAddress(label);

    //由于苹果没有按照'约定'传送一些预定义类型,因此这边多判断当前label是否是预定义类型
    if (address->_type != ADDRESS_UNKNOW)
        return address;

    address->_type = ADDRESS_CUSTOM;
    _contactSynchronizer->AddCustomLabels(label);

    return address;
}

AddressTypes ContactParser::PredefineTypeToLabelTypeAddress(string type)
{
    type = CStrOperation::toLower(type);
    if (type == "home")
        return ADDRESS_HOME;
    else if (type == "work")
        return ADDRESS_WORK;
    else if (type == "custom")
        return ADDRESS_CUSTOM;
    else if (type == "other")
        return ADDRESS_OTHER;
    else
        return ADDRESS_UNKNOW;
}

Email* ContactParser::ParseContactEmail(string key, map<string,void*> entity)
{
    Email* email    = new Email();
    email->_key     = key;

    string type;
    string value;
    string label;
    ParseContactBaseInfo(entity, type, value, label);

    email->_type = PredefineTypeToLabelTypeEmail(type);
    email->_emailAccount = CCodeOperation::GB2312ToUnicode(value);

    if (label.empty())
        return email;

    email->_label = CCodeOperation::GB2312ToUnicode(label);
    email->_type = PredefineTypeToLabelTypeEmail(label);

    //由于苹果没有按照'约定'传送一些预定义类型,因此这边多判断当前label是否是预定义类型
    if (email->_type != EMAIL_UNKNOW)
        return email;

    email->_type = EMAIL_CUSTOM;
    _contactSynchronizer->AddCustomLabels(label);

    return email;
}

EmailTypes ContactParser::PredefineTypeToLabelTypeEmail(string type)
{
    type = CStrOperation::toLower(type);
    if (type == "mobile")
        return EMAIL_MOBILE;
    else if (type == "home")
        return EMAIL_HOME;
    else if (type == "work")
        return EMAIL_WORK;
    else if (type == "custom")
        return EMAIL_CUSTOM;
    else if (type == "other")
        return EMAIL_OTHER;
    else
        return EMAIL_UNKNOW;
}

HomePage* ContactParser::ParseContactURL(string key, map<string,void*> entity)
{
    HomePage* homePage  = new HomePage();
    homePage->_key      = key;

    string type;
    string value;
    string label;
    ParseContactBaseInfo(entity, type, value, label);

    homePage->_type = PredefineTypeToLabelTypeHomePage(type);
    homePage->_name = CCodeOperation::GB2312ToUnicode(value);

    if (label.empty())
        return homePage;

    homePage->_label = CCodeOperation::GB2312ToUnicode(label);
    homePage->_type = PredefineTypeToLabelTypeHomePage(label);

    //由于苹果没有按照'约定'传送一些预定义类型,因此这边多判断当前label是否是预定义类型
    if (homePage->_type != HOMEPAGE_UNKNOW)
        return homePage;

    homePage->_type = HOMEPAGE_CUSTOM;
    _contactSynchronizer->AddCustomLabels(label);

    return homePage;
}

HomePageTypes ContactParser::PredefineTypeToLabelTypeHomePage(string type)
{
    type = CStrOperation::toLower(type);
    if (type == "home page")
        return HOMEPAGE_HOMEPAGE;
    else if (type == "home")
        return HOMEPAGE_HOME;
    else if (type == "work")
        return HOMEPAGE_WORK;
    else if (type == "custom")
        return HOMEPAGE_CUSTOM;
    else if (type == "other")
        return HOMEPAGE_OTHER;
    else
        return HOMEPAGE_UNKNOW;
}

Phone* ContactParser::ParseContactPhoneNumber(string key, map<string,void*> entity)
{
    Phone* phone     = new Phone();
    phone->_key      = key;

    string type;
    string value;
    string label;
    ParseContactBaseInfo(entity, type, value, label);

    phone->_type = PredefineTypeToLabelTypePhoneNumber(type);
    phone->_number = CCodeOperation::GB2312ToUnicode(value);

    if (label.empty())
        return phone;

    phone->_label = CCodeOperation::GB2312ToUnicode(label);
    phone->_type = PredefineTypeToLabelTypePhoneNumber(label);

    //由于苹果没有按照'约定'传送一些预定义类型,因此这边多判断当前label是否是预定义类型
    if (phone->_type != PHONE_UNKNOW)
        return phone;

    phone->_type = PHONE_CUSTOM;
    _contactSynchronizer->AddCustomLabels(label);

    return phone;
}

PhoneTypes ContactParser::PredefineTypeToLabelTypePhoneNumber(string type)
{
    type = CStrOperation::toLower(type);
    if (type == "mobile")
        return PHONE_MOBILE;
    else if (type == "iphone")
        return PHONE_IPHONES;
    else if (type == "home")
        return PHONE_HOME;
    else if (type == "work")
        return PHONE_WORK;
    else if (type == "main")
        return PHONE_MAIN;
    else if (type == "home fax")
        return PHONE_HOMEFAX;
    else if (type == "work fax")
        return PHONE_WORKFAX;
    else if (type == "_$!<otherfax>!$_")//诡异的
        return PHONE_OTHERFAX;
    else if (type == "pager")
        return PHONE_PAGER;
    else if (type == "custom")
        return PHONE_CUSTOM;
    else if (type == "other")
        return PHONE_OTHER;
    else
        return PHONE_UNKNOW;
}

Day* ContactParser::ParseContactDate(string key, map<string,void*> entity)
{
    Day* day    = new Day();
    day->_key   = key;

    string type;
    string value;
    string label;
    ParseContactBaseInfo(entity, type, value, label);

    day->_type = PredefineTypeToLabelTypeDate(type);
    day->_name = CCodeOperation::GB2312ToUnicode(value);

    if (label.empty())
        return day;

    day->_label = CCodeOperation::GB2312ToUnicode(label);
    day->_type = PredefineTypeToLabelTypeDate(label);

    //由于苹果没有按照'约定'传送一些预定义类型,因此这边多判断当前label是否是预定义类型
    if (day->_type != DAY_UNKNOW)
        return day;

    day->_type = DAY_CUSTOM;
    _contactSynchronizer->AddCustomLabels(label);

    return day;
}

DayTypes ContactParser::PredefineTypeToLabelTypeDate(string type)
{
    type = CStrOperation::toLower(type);
    if (type == "anniversary")
        return DAY_DAY;
    else if (type == "home")
        return DAY_HOME;
    else if (type == "work")
        return DAY_WORK;
    else if (type == "custom")
        return DAY_CUSTOM;
    else if (type == "other")
        return DAY_OTHER;
    else
        return DAY_UNKNOW;
}

Related* ContactParser::ParseContactRelated(string key, map<string,void*> entity)
{
    Related* related     = new Related();
    related->_key      = key;

    string type;
    string value;
    string label;
    ParseContactBaseInfo(entity, type, value, label);

    related->_type = PredefineTypeToLabelTypeRelated(type);
    related->_name = CCodeOperation::GB2312ToUnicode(value);

    if (label.empty())
        return related;

    related->_label = CCodeOperation::GB2312ToUnicode(label);
    related->_type = PredefineTypeToLabelTypeRelated(label);

    //由于苹果没有按照'约定'传送一些预定义类型,因此这边多判断当前label是否是预定义类型
    if (related->_type != RELATED_UNKNOW)
        return related;

    related->_type = RELATED_CUSTOM;
    _contactSynchronizer->AddCustomLabels(label);

    return related;
}

RelatedTypes ContactParser::PredefineTypeToLabelTypeRelated(string type)
{
    type = CStrOperation::toLower(type);
    if (type == "father")
        return RELATED_FATHER;
    else if (type == "mother")
        return RELATED_MOTHER;
    else if (type == "parent")
        return RELATED_PARENT;
    else if (type == "brother")
        return RELATED_BROTHER;
    else if (type == "sister")
        return RELATED_SISTER;
    else if (type == "child")
        return RELATED_CHILD;
    else if (type == "friend")
        return RELATED_FRIEND;
    else if (type == "spouse")
        return RELATED_SPOUSE;
    else if (type == "partner")
        return RELATED_PARTNER;
    else if (type == "assistant")
        return RELATED_ASSISTANT;
    else if (type == "manager")
        return RELATED_MANAGER;
    else if (type == "custom")
        return RELATED_CUSTOM;
    else if (type == "other")
        return RELATED_OTHER;
    else
        return RELATED_UNKNOW;
}

Im* ContactParser::ParseContactIM(string key, map<string,void*> entity)
{
    Im* im      = new Im();
    im->_key    = key;

    string type;
    string value;
    string label;
    ParseContactBaseInfo(entity, type, value, label);

    im->_type = PredefineTypeToLabelTypeIm(type);
    for (map<string, void*>:: iterator it = entity.begin(); it != entity.end(); it++)
    {
        string str = "";
        if (CCFConverter::NDCFGetTypeID(it->second) == CCFConverter::NDCFStringGetTypeID())
            CCFConverter::GetIosData(it->second, str);

        if (it->first == "service")
            im->_typeName = FormatImTypeName(it->second);
        else if (it->first == "user")
            im->_imAccount = CCodeOperation::GB2312ToUnicode(str);
    }

    if (label.empty())
        return im;

    im->_label = CCodeOperation::GB2312ToUnicode(label);
    im->_type = PredefineTypeToLabelTypeIm(label);

    //由于苹果没有按照'约定'传送一些预定义类型,因此这边多判断当前label是否是预定义类型
    if (im->_type != IM_UNKNOW)
        return im;

    im->_type = IM_CUSTOM;
    _contactSynchronizer->AddCustomLabels(label);

    return im;
}

IMTypes ContactParser::PredefineTypeToLabelTypeIm(string type)
{
    type = CStrOperation::toLower(type);
    if (type == "home")
        return IM_HOME;
    else if (type == "work")
        return IM_WORK;
    else if (type == "custom")
        return IM_CUSTOM;
    else if (type == "other")
        return IM_OTHER;
    else
        return IM_UNKNOW;
}

IMTypesName ContactParser::FormatImTypeName(void* service)
{
    string typeName = "";
    CCFConverter::GetIosData(service,typeName);

    if (typeName == "aim")
        return IM_ATM;
    else if (typeName == "yahoo")
        return IM_YAHOO;
//     else if (typeName == "skype")
//         return IM_SKYPE;
//     else if (typeName == "qq")
//         return IM_QQ;
//     else if (typeName == "google")
//         return IM_GOOLETALK;
    else if (typeName == "icq")
        return IM_ICQ;
    else if (typeName == "jabber")
        return IM_JABBER;
    else if (typeName == "msn")
        return IM_MSN;
//     else if (typeName == "facebook")
//         return IM_FACEBOOK;
//     else if (typeName == "gadugadu")
//         return IM_GADUGADU;
    else
        return IM_CUSTOMSERVICE;
}

ContactData* ContactParser::CreateContactFromDictInfos(string key, map<string,void*> entity)
{
    map<string, void*>::iterator it = entity.find(DISPLYA_AS_COMPANY);
    if (it == entity.end())
        return false;//throw "Missing 'display as company' field in main entity";

    string displayas = "";
    CCFConverter::GetIosData(it->second, displayas);

    ContactData* contact = NULL;
    if (displayas == "person")
        contact = new ContactData(key,entity,Person);
    else if (displayas == "company")
        contact = new ContactData(key,entity,Company);
    else
        return NULL;//throw string.Format("Invalid 'display as company' Value :{0} ", displayas);

    return contact;
}

ContactData* ContactParser::GetExistContact(map<string,void*> entity)
{
    map<string, void*>::iterator it = entity.find("contact");
    if (it == entity.end())
        return NULL;//throw "missing 'contact' field in entity";

    string id = "";
    CCFConverter::GetIosData(it->second, id);

    return _contactSynchronizer->FindContact(id);
}

#endif