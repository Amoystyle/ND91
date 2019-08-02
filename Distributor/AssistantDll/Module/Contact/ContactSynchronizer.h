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

        // �����е������˺ͷ�����Ϣ��ӽ�Ĭ���˻�
        void AddIntoDefaultAccount(ContactAccount* defaultAccount);

        //void SetCacheExpired() { _isCacheExpired = true; }

        // Summary  : �Ƿ�֧��ͬ������
        // Returns  : bool
        bool IsSupportSynchronizer();

        // Summary  : �������ݶ���ӿ�
        // Returns  : bool
        bool SaveGroupsNContacts();

		void RemoveGroupFromList(ContactGroupData* pGroup);

    protected:
        virtual void DoSaveChanges();                   // ��������
        virtual void ClearCachedData();                 // ����������
        virtual void ParsePlistEntities();              // ����ʵ������

    private:
        void ReassociateContactNGroupIDs();             // ����ӳ������(�����ʱ��������)
        void ReassociatMultiValueIDs(Category c);       // ����ӳ����ϵ����������(�����ʱ��������)
        void IterateReassociatePhoneMultiValueIDs(vector<Phone*>* phoneNumberList);  // ����ӳ����ϵ�˵绰��������(�����ʱ��������)
        void IterateReassociateEmailMultiValueIDs(vector<Email*>* emailList);        // ����ӳ����ϵ��������������(�����ʱ��������)
        void IterateReassociateURLMultiValueIDs(vector<HomePage*>* homePageList);    // ����ӳ����ϵ����ҳ��������(�����ʱ��������)
        void IterateReassociateAddressMultiValueIDs(vector<Address*>* addressList);  // ����ӳ����ϵ�˵�ַ��������(�����ʱ��������)
        void IterateReassociateDateMultiValueIDs(vector<Day*>* dateList);            // ����ӳ����ϵ�˼�������������(�����ʱ��������)
        void IterateReassociateIMMultiValueIDs(vector<Im*>* imList);                 // ����ӳ����ϵ�˼�ʱͨѶ��������(�����ʱ��������)
        void IterateReassociateRelatesMultiValueIDs(vector<Related*>* relatedList);  // ����ӳ����ϵ�˹�ϵ��������(�����ʱ��������)

    private:
        ContactParser*      _cp; // ��ϵ�����ݽ�����
        ContactBuilder*     _cb; // ������ϵ����ϢЭ����

        vector<ContactData*>                _contacts;                // ��ϵ�����ݼ���
        vector<ContactGroupData*>           _groups;                  // �����ݼ���
        map<string,string>                  _remappedIDs;             // ����ӳ���ID����
        map< Category, map<string, void*> >   _sendMultiValueInfos;     // Ҫ���͵���ϵ�����Լ���
        vector<string>                      _customLabels;            // �Զ����ǩ�����ڽ�����ʾ��
    };
}
