#pragma  once

#include "ContactData.h"

#include <set>
#include <map>

using namespace std;

namespace ND91Assistant
{
    class BinaryBufferReader;
    class BinaryBufferWriter;

    // 联络人分组信息类
    class CLASSINDLL_CLASS_DECL ContactGroupData : public BaseData
    {
    public:
        ContactGroupData();
        ContactGroupData(const char* iosData[2]);
        ContactGroupData(string key,map<string,void*> entity);
        ContactGroupData(ContactGroupData* pGroupData);
        virtual ~ContactGroupData();

        // Summary  : 构建组数据
        // Returns  : void
        // Parameter: map<string, void*>& infoss 构建的组数据
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
        // Parameter: string member 有值成员为一个
        // Parameter: void* members 
        void IniMembers(string member,void* members);

    private:

        int         _id;            // id
        string      _key;           // id = Group/1
        wstring     _name;          // 名称
        wstring     _comment;       // 备注
        wstring     _accountName;   // 分组所属账户名
        wstring     _accountType;   // 分组所属账户类型

        static long     _idIncrementer;     // 联系人在PC上的ID
        string          _recordEntityName;  // "com.apple.contacts.Group"

#pragma warning(disable : 4251)

        set<int>        _contacts;      // 包含哪些联络人, set内存储联系人id

#pragma warning(default : 4251)

    };
}