#pragma once

#include "Module/Contact/ContactData.h"

namespace ND91Assistant
{
    class ContactSynchronizer;
    class ContactGroupData;

    // 构建联系人信息协议
    class ContactBuilder
    {
    public:
        ContactBuilder(ContactSynchronizer* contactSynchronizer);
        ~ContactBuilder();

        // Summary  : 构建主要数据（包括联系人和组数据）
        // Returns  : bool
        // Parameter: vector<ContactData*> contacts 联系人数据集合
        // Parameter: vector<ContactGroupData*> groups 组数据集合
        // Parameter: bool isHaveRecord 是否还有要打包的数据
        // Parameter: bool linkAllRecordsOfPulledEntityTypeSentKey 是否需要返回重新映射的ID集合
        bool BuildMainInfos(vector<ContactData*> contacts,
            vector<ContactGroupData*> groups,
            bool isHaveRecord,
            bool linkAllRecordsOfPulledEntityTypeSentKey);
        
        // Summary  : 构建联系人属性数据
        // Returns  : bool
        // Parameter: vector<ContactData*> contacts 联系人数据集合
        bool BuildMultiValueInfos(vector<ContactData*> contacts);

        // Summary  : 构建的actions数据
        // Returns  : void
        // Parameter: Category category 联系人属性类型
        // Parameter: bool linkAllRecordsOfPulledEntityTypeSentKey 是否需要返回重新映射的ID集合
        // Parameter: map<string,vector<void*>> & actions 构建的actions数据
        void BuildActionsForCategory(Category category,bool linkAllRecordsOfPulledEntityTypeSentKey, map< string, vector<void*> >& actions);
        
    private:
        void BuildMultiValuePhoneNumber(string contactId, Category category, map<string, void*>& main, vector<Phone*>* phoneNumbers);
        void BuildMultiValueEmail(string contactId, Category category, map<string, void*>& main, vector<Email*>* emails);
        void BuildMultiValueIM(string contactId, Category category, map<string, void*>& main, vector<Im*>* ims);
        void BuildMultiValueAddress(string contactId, Category category, map<string, void*>& main, vector<Address*>* addresses);
        void BuildMultiValueURL(string contactId, Category category, map<string, void*>& main, vector<HomePage*>* urls);
        void BuildMultiValueDate(string contactId, Category category, map<string, void*>& main, vector<Day*>* dates);
        void BuildMultiValueRelated(string contactId, Category category, map<string, void*>& main, vector<Related*>* relateds);

        // 类型由int转为str
        string LabelTypeToPredefineType(const char* labels[], int type);
       
    private:
        ContactSynchronizer* _contactSynchronizer;
    };
}
