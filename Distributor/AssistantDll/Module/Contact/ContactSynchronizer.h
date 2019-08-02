#pragma once

#include "Module/Common/Synchronizer/SynchronizerBase.h"
#include "Module/Contact/ContactData.h"

namespace ND91Assistant
{
    class ContactAccount;
    class ContactParser;
    class ContactBuilder;
    class ContactGroupData;

    class ContactSynchronizer : public SynchronizerBase
    {
        friend class ContactBuilder;
    public:
        ContactSynchronizer(const DeviceData* pDevice);
        ~ContactSynchronizer();

        void AddContacts(ContactData* contact);
        ContactData* FindContact(string contactId);
        void AddGroups(ContactGroupData* group);
        void AddSendMultiValueInfos(Category category, map<string, void*> sendMultiValueInfo);
        void AddSendInfo(void* info);
        void AddContactsToGroup(const vector<const ContactData*>* pDataList, ContactGroupData* pGroupData);
        void RemoveContactsFromGroup(const vector<const ContactData*>* pDataList, ContactGroupData* pGroupData);
        void AddCustomLabels(string customLabel);
        vector<string>* GetCustomLabels() { return &_customLabels; }

        // 将所有的联络人和分组信息添加进默认账户
        void AddIntoDefaultAccount(ContactAccount* defaultAccount);

        //void SetCacheExpired() { _isCacheExpired = true; }

        // Summary  : 是否支持同步功能
        // Returns  : bool
        bool IsSupportSynchronizer();

        // Summary  : 保存数据对外接口
        // Returns  : bool
        bool SaveGroupsNContacts();

		void RemoveGroupFromList(ContactGroupData* pGroup);

    protected:
        virtual void DoSaveChanges();                   // 保存数据
        virtual void ClearCachedData();                 // 清理缓存数据
        virtual void ParsePlistEntities();              // 解析实体数据

    private:
        void ReassociateContactNGroupIDs();             // 重新映射数据(在添加时返回数据)
        void ReassociatMultiValueIDs(Category c);       // 重新映射联系人属性数据(在添加时返回数据)
        void IterateReassociatePhoneMultiValueIDs(vector<Phone*>* phoneNumberList);  // 重新映射联系人电话属性数据(在添加时返回数据)
        void IterateReassociateEmailMultiValueIDs(vector<Email*>* emailList);        // 重新映射联系人邮箱属性数据(在添加时返回数据)
        void IterateReassociateURLMultiValueIDs(vector<HomePage*>* homePageList);    // 重新映射联系人主页属性数据(在添加时返回数据)
        void IterateReassociateAddressMultiValueIDs(vector<Address*>* addressList);  // 重新映射联系人地址属性数据(在添加时返回数据)
        void IterateReassociateDateMultiValueIDs(vector<Day*>* dateList);            // 重新映射联系人纪念日属性数据(在添加时返回数据)
        void IterateReassociateIMMultiValueIDs(vector<Im*>* imList);                 // 重新映射联系人即时通讯属性数据(在添加时返回数据)
        void IterateReassociateRelatesMultiValueIDs(vector<Related*>* relatedList);  // 重新映射联系人关系属性数据(在添加时返回数据)

    private:
        ContactParser*      _cp; // 联系人数据解析类
        ContactBuilder*     _cb; // 构建联系人信息协议类

        vector<ContactData*>                _contacts;                // 联系人数据集合
        vector<ContactGroupData*>           _groups;                  // 组数据集合
        map<string,string>                  _remappedIDs;             // 重新映射的ID集合
        map< Category, map<string, void*> >   _sendMultiValueInfos;     // 要发送的联系人属性集合
        vector<string>                      _customLabels;            // 自定义标签（用于界面显示）
    };
}
