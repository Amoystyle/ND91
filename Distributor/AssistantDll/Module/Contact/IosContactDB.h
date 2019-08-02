#pragma once

namespace ND91Assistant
{
    class IosContactDB : public IosDBBase
    {
    public:
        IosContactDB(IosDBPath iosDBPath);
        ~IosContactDB(void);

    #pragma region ��Ⱥ��Ĳ���

        // Summary  : ��ȡȺ���б�
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: ContactAccount *pAccount ��ϵ���˻�
        bool GetAllGroups(ContactAccount *pAccount);

        // Summary  : �����������Ⱥ��
        // Returns  : int ����������ID��ʧ�ܷ���-1
        // Parameter: ContactGroupData * group Ⱥ����Ϣ
        int AddGroup(ContactGroupData* group);

        // Summary  : ����Ⱥ��
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: ContactGroupData * group Ⱥ����Ϣ
        bool UpdateGroup(const ContactGroupData* group);

        // Summary  : ɾ��Ⱥ��
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: int groupId Ҫɾ������ID
        bool DeleteGroup(int groupId);

        // Summary  : ��ȡ���Ա
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: ContactAccount * pAccount ��ϵ���˻�
        bool GetGroupMember(ContactAccount *pAccount);   

        // Summary  : �����ϵ�˵�Ⱥ��
        // Returns  : int ʵ��ִ����������
        // Parameter: int groupId ��ID
        // Parameter: const vector<const ContactData*>* pDataList ��ԱID����
        int AddMember(int groupId, const vector<const ContactData*>* pDataList, ProgressSubject * pProgressSubject);

        // Summary  : ��Ⱥ�����Ƴ���ϵ��
        // Returns  : int ʵ��ִ����������
        // Parameter: int groupId   ��ID
        // Parameter: vector<int> memberIdVec ������ID����
        int RemoveMember(int groupId, vector<int> memberIdVec, ProgressSubject * pProgressSubject);

    #pragma endregion

    #pragma region ��ϵ�˲���

        // Summary  : ��ȡ������ϵ��
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: ContactAccount *pAccount װ�ص����ʻ���
        bool GetAllContact(ContactAccount *pAccount);

        // Summary  : ��ȡ��ϵ������
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: ContactAccount *pAccount װ�ص����ʻ���
        bool GetAllContactSize(ContactAccount *pAccount);

        // �����ϵ��
        // ����ʵ��ִ����������
        int AddContactData(const vector<ContactData*> *contactDataVec, ProgressSubject * pProgressSubject);

        // ������ϵ��
        bool UpdateContact(ContactData* person);                                      

        // ɾ����ϵ��
        // ����ʵ��ִ����������
        int DeleteContact(vector<int> contactIdVec, ProgressSubject * pProgressSubject);                                     

    #pragma endregion
    private:

        // Summary  : ��ȡ����ϵ��ID
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: int groupId ��ID
        // Parameter: ContactAccount *pAccount �������˻�
        bool GetMember(int groupId,ContactAccount *pAccount);  

        // Summary  : // ��ȡ��ϵ�˻�����Ϣ
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: ContactAccount * pAccount �������˻�
        bool GetContactBaseInformation(ContactAccount *pAccount);

        // Summary  : // ��ȡ��ϵ������(����,����,��ַ,IM)
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: ContactAccount * pAccount �������˻�
        bool GetContactPropertyInformation(ContactAccount *pAccount);

        // Summary  : ����Ƿ��������
        // Returns  : bool true:���� false:������
        // Parameter: int groupId Ⱥ��ID
        bool CheckHasGroupName(int groupId);

        // Summary  : �����ϵ���Ƿ��Ѵ���Ⱥ����
        // Returns  : bool true:���� false: ������
        // Parameter: int groupId Ⱥ��ID
        // Parameter: int memberId ��ϵ��ID
        bool CheckHasGroupMember(int groupId, int memberId);

        // Summary  : ����Ƿ����ָ����Ⱥ��
        // Returns  : bool true:���� false: ������
        // Parameter: wstring groupName ָ����Ⱥ����
        bool CheckHasGroup(wstring groupName);

    private:
        // Summary  : ��д��������
        // Returns  : void
        // Parameter: ContactData * person ��������Ϣ
//void MakeCompositeNameFallback(ContactData* person);

        // Summary  : �Ƿ�δ��д����
        // Returns  : bool true:δ��д false: ��д
        // Parameter: ContactData * person ��������Ϣ
        bool IsNoname(ContactData* person);

        // Summary  : �����ϵ�˻�����Ϣ
        // Returns  : bool
        // Parameter: ContactData * person ��ϵ��
		bool AddContactBaseInformation(ContactData* person);

        // Summary  : ������ϵ�˻�����Ϣ
        // Returns  : void
        // Parameter: ContactData * person ��ϵ��
        bool UpdateContactBaseInformation(ContactData* person);

        // Summary  : ɾ��������
        // Returns  : bool true:�ɹ� false:ʧ��
        // Parameter: int contactId ������ID
        bool DeleteABPerson(int contactId);

        // Summary  : ��ȡ������ͷ��
        // Returns  : bool
        // Parameter: ContactData * person ������
        bool GetPicture(ContactData* person);

        // Summary  : ���������ͷ��
        // Returns  : bool 
        // Parameter: ContactData * person ������
        bool AddPicture(ContactData* person);

        // Summary  : ɾ��������ͷ��
        // Returns  : bool
        // Parameter: int contactId ������ID
        bool DeletePicture(int contactId);
    };
}
