#pragma once

#include "Module/Contact/ContactData.h"

namespace ND91Assistant
{
    class ContactSynchronizer;
    class ContactGroupData;

    // ������ϵ����ϢЭ��
    class ContactBuilder
    {
    public:
        ContactBuilder(ContactSynchronizer* contactSynchronizer);
        ~ContactBuilder();

        // Summary  : ������Ҫ���ݣ�������ϵ�˺������ݣ�
        // Returns  : bool
        // Parameter: vector<ContactData*> contacts ��ϵ�����ݼ���
        // Parameter: vector<ContactGroupData*> groups �����ݼ���
        // Parameter: bool isHaveRecord �Ƿ���Ҫ���������
        // Parameter: bool linkAllRecordsOfPulledEntityTypeSentKey �Ƿ���Ҫ��������ӳ���ID����
        bool BuildMainInfos(vector<ContactData*> contacts,
            vector<ContactGroupData*> groups,
            bool isHaveRecord,
            bool linkAllRecordsOfPulledEntityTypeSentKey);
        
        // Summary  : ������ϵ����������
        // Returns  : bool
        // Parameter: vector<ContactData*> contacts ��ϵ�����ݼ���
        bool BuildMultiValueInfos(vector<ContactData*> contacts);

        // Summary  : ������actions����
        // Returns  : void
        // Parameter: Category category ��ϵ����������
        // Parameter: bool linkAllRecordsOfPulledEntityTypeSentKey �Ƿ���Ҫ��������ӳ���ID����
        // Parameter: map<string,vector<void*>> & actions ������actions����
        void BuildActionsForCategory(Category category,bool linkAllRecordsOfPulledEntityTypeSentKey, map< string, vector<void*> >& actions);
        
    private:
        void BuildMultiValuePhoneNumber(string contactId, Category category, map<string, void*>& main, vector<Phone*>* phoneNumbers);
        void BuildMultiValueEmail(string contactId, Category category, map<string, void*>& main, vector<Email*>* emails);
        void BuildMultiValueIM(string contactId, Category category, map<string, void*>& main, vector<Im*>* ims);
        void BuildMultiValueAddress(string contactId, Category category, map<string, void*>& main, vector<Address*>* addresses);
        void BuildMultiValueURL(string contactId, Category category, map<string, void*>& main, vector<HomePage*>* urls);
        void BuildMultiValueDate(string contactId, Category category, map<string, void*>& main, vector<Day*>* dates);
        void BuildMultiValueRelated(string contactId, Category category, map<string, void*>& main, vector<Related*>* relateds);

        // ������intתΪstr
        string LabelTypeToPredefineType(const char* labels[], int type);
       
    private:
        ContactSynchronizer* _contactSynchronizer;
    };
}
