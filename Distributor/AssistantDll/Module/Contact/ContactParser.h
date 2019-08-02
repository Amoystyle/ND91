#pragma once

#include "Module/Contact/ContactData.h"

namespace ND91Assistant
{
    class ContactSynchronizer;

    // 解析从设备获取的联系人信息
    class ContactParser
    {
    public:
        ContactParser(ContactSynchronizer* contactSynchronizer);
        ~ContactParser();

        // Summary  : 解析联系人数据
        // Returns  : bool
        // Parameter: map<string,map<string,void * >> mapEntities 设备返回的联系人数据集合
        bool ParsePlistContact(map<string, map<string,void*> > mapEntities);

    private:
        // Summary  : 解析组数据
        // Returns  : bool
        // Parameter: string key 组ID
        // Parameter: map<string, void*> entity 设备传回的组数据
        bool ParseGroupInfos(string key, map<string,void*> entity);

        // Summary  : 解析联系人数据
        // Returns  : bool
        // Parameter: string key 联系人ID
        // Parameter: map<string, void*> entity 设备传回的联系人数据
        bool ParseContactInfos(string key, map<string,void*> entity);

        // Summary  : 创建新联系人
        // Returns  : ContactData* 新联系人
        // Parameter: string key 联系人ID
        // Parameter: map<string, void*> entity 设备传回的联系人数据
        ContactData* CreateContactFromDictInfos(string key, map<string,void*> entity);

        // Summary  : 获取已存在的联系人
        // Returns  : ContactData*
        // Parameter: map<string,void*> entity 设备传回的联系人数据
        ContactData* GetExistContact(map<string,void*> entity);

        // Summary  : 验证设备返回的实体数据是否有效
        // Returns  : string 实体数据名(不为空则为有效)
        // Parameter: map<string, void*> entity 设备返回的实体数据
        string IsValidEntity(map<string,void*> entity);

        // 解析基本数据
        void ParseContactBaseInfo(map<string,void*> entity, string& type, string& value, string& label);

        // 解析数据
        Address* ParseContactAddress(string key, map<string,void*> entity);
        Im* ParseContactIM(string key, map<string,void*> entity);
        Email* ParseContactEmail(string key, map<string,void*> entity);
        HomePage* ParseContactURL(string key, map<string,void*> entity);
        Phone* ParseContactPhoneNumber(string key, map<string,void*> entity);
        Day* ParseContactDate(string key, map<string,void*> entity);
        Related* ParseContactRelated(string key, map<string,void*> entity);

        // 类型由str转为int
        AddressTypes PredefineTypeToLabelTypeAddress(string type);
        IMTypes PredefineTypeToLabelTypeIm(string type);
        IMTypesName FormatImTypeName(void* service);

        EmailTypes PredefineTypeToLabelTypeEmail(string type);
        HomePageTypes PredefineTypeToLabelTypeHomePage(string type);
        PhoneTypes PredefineTypeToLabelTypePhoneNumber(string type);
        DayTypes PredefineTypeToLabelTypeDate(string type);
        RelatedTypes PredefineTypeToLabelTypeRelated(string type);

    public:
        static string DISPLYA_AS_COMPANY;
        static string CONTACT_KEY;
        static string GROUP_ENTITY_NAME;
        static string CONTACT_ENTITY_NAME;
        static string EMAIL_ENTITY_NAME;
        static string URL_ENTITY_NAME;
        static string PHONENUMBER_ENTITY_NAME;
        static string ADDRESS_ENTITY_NAME;
        static string DATE_ENTITY_NAME;
        static string IM_ENTITY_NAME;
        static string RELATE_ENTITY_NAME;

    private:
        ContactSynchronizer*     _contactSynchronizer;
    };
}
