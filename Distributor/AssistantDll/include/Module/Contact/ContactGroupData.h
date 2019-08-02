#pragma  once

#include "ContactData.h"

#include <set>
#include <map>

using namespace std;

namespace ND91Assistant
{
    class BinaryBufferReader;
    class BinaryBufferWriter;

    // �����˷�����Ϣ��
    class CLASSINDLL_CLASS_DECL ContactGroupData : public BaseData
    {
    public:
        ContactGroupData();
        ContactGroupData(const char* iosData[2]);
        ContactGroupData(string key,map<string,void*> entity);
        ContactGroupData(ContactGroupData* pGroupData);
        virtual ~ContactGroupData();

        // Summary  : ����������
        // Returns  : void
        // Parameter: map<string, void*>& infoss ������������
        void ConvertToDictInfos(map<string, void*>& infoss);
        
        wstring GetAccountName() const;
        void SetAccountName(wstring accountName);

        wstring GetAccountType() const;
        void SetAccountType(wstring accountType);

        int GetId() const { return _id; }
        void SetId(int val) { _id = val; }

        string GetKey() const { return _key; }
        void SetId(string key) { _key = key; }

        std::wstring GetName() const { return _name; }
        void SetName(std::wstring val) { _name = val; }

        std::wstring GetComment() const { return _comment; }
        void SetComment(std::wstring val) { _comment = val; }

        void Read(BinaryBufferReader* pReader, bool hasAccount);

        void Write(BinaryBufferWriter* pWriter, bool hasAccount) const;

        bool Empty() { return _contacts.empty(); }

        set<int> GetContacts() const { return _contacts; }
        void AddContacts(int id) { _contacts.insert(id); }
        void RemoveContacts(int id) { _contacts.erase(id); }

    private:
        // Summary  : 
        // Returns  : void
        // Parameter: string member ��ֵ��ԱΪһ��
        // Parameter: void* members 
        void IniMembers(string member,void* members);

    private:

        int         _id;            // id
        string      _key;           // id = Group/1
        wstring     _name;          // ����
        wstring     _comment;       // ��ע
        wstring     _accountName;   // ���������˻���
        wstring     _accountType;   // ���������˻�����

        static long     _idIncrementer;     // ��ϵ����PC�ϵ�ID
        string          _recordEntityName;  // "com.apple.contacts.Group"

#pragma warning(disable : 4251)

        set<int>        _contacts;      // ������Щ������, set�ڴ洢��ϵ��id

#pragma warning(default : 4251)

    };
}