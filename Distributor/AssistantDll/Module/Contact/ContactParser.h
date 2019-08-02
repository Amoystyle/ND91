#pragma once

#include "Module/Contact/ContactData.h"

namespace ND91Assistant
{
    class ContactSynchronizer;

    // �������豸��ȡ����ϵ����Ϣ
    class ContactParser
    {
    public:
        ContactParser(ContactSynchronizer* contactSynchronizer);
        ~ContactParser();

        // Summary  : ������ϵ������
        // Returns  : bool
        // Parameter: map<string,map<string,void * >> mapEntities �豸���ص���ϵ�����ݼ���
        bool ParsePlistContact(map<string, map<string,void*> > mapEntities);

    private:
        // Summary  : ����������
        // Returns  : bool
        // Parameter: string key ��ID
        // Parameter: map<string, void*> entity �豸���ص�������
        bool ParseGroupInfos(string key, map<string,void*> entity);

        // Summary  : ������ϵ������
        // Returns  : bool
        // Parameter: string key ��ϵ��ID
        // Parameter: map<string, void*> entity �豸���ص���ϵ������
        bool ParseContactInfos(string key, map<string,void*> entity);

        // Summary  : ��������ϵ��
        // Returns  : ContactData* ����ϵ��
        // Parameter: string key ��ϵ��ID
        // Parameter: map<string, void*> entity �豸���ص���ϵ������
        ContactData* CreateContactFromDictInfos(string key, map<string,void*> entity);

        // Summary  : ��ȡ�Ѵ��ڵ���ϵ��
        // Returns  : ContactData*
        // Parameter: map<string,void*> entity �豸���ص���ϵ������
        ContactData* GetExistContact(map<string,void*> entity);

        // Summary  : ��֤�豸���ص�ʵ�������Ƿ���Ч
        // Returns  : string ʵ��������(��Ϊ����Ϊ��Ч)
        // Parameter: map<string, void*> entity �豸���ص�ʵ������
        string IsValidEntity(map<string,void*> entity);

        // ������������
        void ParseContactBaseInfo(map<string,void*> entity, string& type, string& value, string& label);

        // ��������
        Address* ParseContactAddress(string key, map<string,void*> entity);
        Im* ParseContactIM(string key, map<string,void*> entity);
        Email* ParseContactEmail(string key, map<string,void*> entity);
        HomePage* ParseContactURL(string key, map<string,void*> entity);
        Phone* ParseContactPhoneNumber(string key, map<string,void*> entity);
        Day* ParseContactDate(string key, map<string,void*> entity);
        Related* ParseContactRelated(string key, map<string,void*> entity);

        // ������strתΪint
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
