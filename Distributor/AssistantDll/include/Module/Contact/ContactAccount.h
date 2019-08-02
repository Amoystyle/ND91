#pragma  once

#include "ContactData.h"
#include "ContactGroupData.h"
#include "Core/DeviceData.h"
#include "Core/International.h"

using namespace std;

#pragma warning(disable : 4251)   // ���ô˾��� 

namespace ND91Assistant
{
    // ��ϵ���˻��ࣨ�������˻��ڵ������˺ͷ���Ĳ�����
    class CLASSINDLL_CLASS_DECL ContactAccount : public BaseData
    {
    friend class ContactBackupAndRestore;
    public:
        ContactAccount(const DeviceData* pDevice);
        virtual ~ContactAccount();

        // Summary  : ��ʼ����ϵ������(��ȡʱ��)
        void InitContactData(ContactData* pContact) { _contactDataList.push_back(pContact); }

        // Summary  : ��ʼ����ϵ�˷�������(��ȡʱ��)
        void InitContactGroupData(ContactGroupData* pGroup) { _contactGroupList.push_back(pGroup); }

		// Summary  : ����������ͷ��
		// Returns	: bool
		// Parameter: ContactData * pcontact
		bool LoadContactPhoto(ContactData* pcontact);

        // Summary  : ɾ��������
        // Returns  : bool �Ƿ�ɾ���ɹ�
        // Parameter: const vector<const ContactData * > * pDataList ��Ҫɾ����������
        // Parameter: ProgressObserver * pObserver ����������
        bool DeleteContact(const vector<ContactData*>* pDataList,ProgressObserver* pObserver = NULL);

        // ɾ��ȫ��������
        bool DeleteAll(ProgressObserver* pObserver = NULL);

        // ɾ�����з���
        bool DeleteAllGroup(ProgressObserver* pObserver = NULL);

        // Summary  : ������������Ϣ��������ı���ϵ�������˻��ͷ�����Ϣ��
        // Returns  : bool �Ƿ���³ɹ�
        // Parameter: ContactData * pContact ��Ҫ���µ�������
        bool UpdateContact(ContactData* pContact);

        // Summary  : ȡ�˻��е����������˷���
        // Returns  : vector<ContactGroupData*> ������ϵ�˷������ָ���vector
        vector<ContactGroupData*> GetContactGroup() const { return _contactGroupList; }

        // Summary  : ɾ���˻��е������˷���
        // Returns  : bool �Ƿ�ɾ���ɹ�
        // Parameter: ContactGroupData* pGroupData ��Ҫɾ���������˷���
        bool DeleteContactGroup(ContactGroupData* pGroupData);

        // Summary  : �����˻��е������˷���
        // Returns  : bool �Ƿ���³ɹ�
        // Parameter: ContactGroupData* pGroupData ��Ҫ���µ������˷���
        bool UpdateContactGroup(ContactGroupData* pGroupData);

        // Summary  : ����˻��е������˷���
        // Returns  : bool �Ƿ���³ɹ�
        // Parameter: ContactGroupData* pGroupData ����������˷���
        bool AddContactGroup(/*ContactGroupData* pGroupData*/const vector<ContactGroupData*>* pDataList);

        // Summary  : ����ϵ����ӵ���Ӧ�ķ���
        // Returns  : bool �Ƿ���ӳɹ�
        // Parameter: const vector<ContactData * > * pDataList ��Ҫ��ӵ�����������˼���
        // Parameter: ContactGroupData * pGroupData Ŀ�ķ���
        // Parameter: ProgressObserver * pObserver ����������
        bool AddContactToGroup(const vector<ContactData*>* pDataList,ContactGroupData* pGroupData,ProgressObserver* pObserver);

        // Summary  : ����ϵ����ӵ���Ӧ�ķ���
        // Returns  : bool �Ƿ���ӳɹ�
        // Parameter: int contactId ��ϵ��ID
        // Parameter: int groupId ����ID
        bool AddContactToGroup(int contactId, int groupId)
        {
            ContactGroupData* pGroup = GetGroup(groupId);
            RETURN_FALSE_IF( ! pGroup);

            pGroup->AddContacts(contactId);
            return true;
        }

        // Summary  : ����ϵ�˴���Ӧ�ķ������Ƴ�
        // Returns  : bool �Ƿ��Ƴ��ɹ�
        // Parameter: const vector<ContactData * > * pDataList ��Ҫ�ӷ������Ƴ������˼���
        // Parameter: ContactGroupData * pGroupData ��Ҫ�Ƴ������˵ķ���
        // Parameter: ProgressObserver * pObserver ����������
        bool RemoveContactFromGroup(const vector<ContactData*>* pDataList,ContactGroupData* pGroupData,ProgressObserver* pObserver);

        // Summary  : ����ϵ�˴���Ӧ�ķ������Ƴ�
        // Returns  : bool �Ƿ��Ƴ��ɹ�
        // Parameter: int contactId ��ϵ��ID
        // Parameter: int groupId ����ID
        bool RemoveContactFromGroup(int contactId, int groupId)
        {
            ContactGroupData* pGroup = GetGroup(groupId);
            RETURN_FALSE_IF( ! pGroup);

            pGroup->RemoveContacts(contactId);
            return true;
        }

		void AddContact(ContactData* pData) {_contactDataList.push_back(pData);}

		ContactGroupData* FindGroupByName(wstring groupName);

        // Summary  : ����Ӧ�ķ����л�ȡ��ϵ��
        // Returns  : ������ID����
        // Parameter: int groupId ����ID
        set<int> GetContactFromGroup(int groupId)
        {
            ContactGroupData* pGroup = GetGroup(groupId);
            return pGroup->GetContacts();
        }

        // Summary  : ������ϵ�˵��ղ�����
        // Returns  : bool �Ƿ����óɹ�
        // Parameter: const vector<ContactData * > * pDataList ��Ҫ�������Ե������˼���
        // Parameter: bool bFavorite �Ƿ�Ϊ�ղ�
        // Parameter: ProgressObserver * pObserver ����������
        bool SetFavorite(const vector<ContactData*>* pDataList,bool bFavorite,ProgressObserver* pObserver);

        const DeviceData* GetPDevice() const { return _pDevice; }

        std::wstring GetName() const { return _name; }
        void SetName(std::wstring val) { _name = val; }

        std::wstring GetType() const { return _type; }
        void SetType(std::wstring val) { _type = val; }

        bool GetOMS() const { return _bOMS; }
   //     void SetOMS(bool val) { _bOMS = val; }

        bool GetVersion20() const { return _bVersion20; }
  //      void SetVersion20(bool val) { _bVersion20 = val; }

        vector<ContactGroupData*> GetContactGroupList() const { return _contactGroupList; }
        vector<ContactData*> GetContactDataList() const { return _contactDataList; }
        void RemoveGroupFromList(ContactGroupData* pGroup);
        void RemoveContactFromList(ContactData* pContact);

		// ��ȡ��ϵ�˸���
		int Count() 
        { 
            if (_contactDataList.size() > 0)
                return _contactDataList.size();
            else
                return _size;
        }

        void SetCount(int size)
        {
            _size = size;
        }

        ContactGroupData* GetGroup(int id)
        {
            for(size_t i =0; i< _contactGroupList.size(); ++i)
            {
                if ( _contactGroupList[i]->GetId() == id)
                    return _contactGroupList[i];
            }

            return NULL;
        }

        const map<wstring, STRING_ID>* GetIosSystemRingtone() { return &_ios_systemRingtone; }
        const map<wstring, wstring>* GetAndroidSystemRingtone() { return &_android_systemRingtone; }
        const map<wstring, wstring>* GetIosCustomRingtone() { return &_ios_customRingtone; }
        const map<wstring, wstring>* GetAndroidCustomRingtone() { return &_android_customRingtone; }
        const map<wstring, STRING_ID>* GetIosTextTone() { return &_ios_textTone; }
        const map<wstring, wstring>* GetAndroidTextTone() { return &_android_textTone; }

    private:
        void IniIosSystemRingtoneMap();
        void IniAndroidSystemRingtoneMap();
        void IniIosCustomRingtoneMap();
        void IniAndroidCustomRingtoneMap();
        void IniIosTextTone();
        void IniAndroidTextTone();
        
    private:

        wstring _name;          // �˻���
        wstring _type;          // �˻�����

        const DeviceData* _pDevice;  // �豸��Ϣ

        bool _bVersion20;       // ��׿��ϵͳ�İ汾�Ƿ�Ϊ2.0����
        bool _bOMS;             // �Ƿ�ΪOMSϵͳ

        vector<ContactGroupData*>   _contactGroupList;  // �˻��еķ����б�
        vector<ContactData*>        _contactDataList;   // �˻��е��������б�
        int                         _size;              // ����������
 
        map<wstring, STRING_ID>     _ios_systemRingtone;      // iosϵͳ�����б�
        map<wstring, wstring>       _android_systemRingtone;  // androidϵͳ�����б�
        map<wstring, wstring>       _ios_customRingtone;      // ios�Զ��������б�
        map<wstring, wstring>       _android_customRingtone;  // android�Զ��������б�
        map<wstring, STRING_ID>     _ios_textTone;            // ios���������б�
        map<wstring, wstring>       _android_textTone;        // android���������б�
    };
}
