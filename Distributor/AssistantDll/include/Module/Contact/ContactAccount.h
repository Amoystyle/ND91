#pragma  once

#include "ContactData.h"
#include "ContactGroupData.h"
#include "Core/DeviceData.h"
#include "Core/International.h"

using namespace std;

#pragma warning(disable : 4251)   // 禁用此警告 

namespace ND91Assistant
{
    // 联系人账户类（包含对账户内的联络人和分组的操作）
    class CLASSINDLL_CLASS_DECL ContactAccount : public BaseData
    {
    friend class ContactBackupAndRestore;
    public:
        ContactAccount(const DeviceData* pDevice);
        virtual ~ContactAccount();

        // Summary  : 初始化联系人数据(读取时用)
        void InitContactData(ContactData* pContact) { _contactDataList.push_back(pContact); }

        // Summary  : 初始化联系人分组数据(读取时用)
        void InitContactGroupData(ContactGroupData* pGroup) { _contactGroupList.push_back(pGroup); }

		// Summary  : 加载联络人头像
		// Returns	: bool
		// Parameter: ContactData * pcontact
		bool LoadContactPhoto(ContactData* pcontact);

        // Summary  : 删除联络人
        // Returns  : bool 是否删除成功
        // Parameter: const vector<const ContactData * > * pDataList 需要删除的联络人
        // Parameter: ProgressObserver * pObserver 进度条设置
        bool DeleteContact(const vector<ContactData*>* pDataList,ProgressObserver* pObserver = NULL);

        // 删除全部联络人
        bool DeleteAll(ProgressObserver* pObserver = NULL);

        // 删除所有分组
        bool DeleteAllGroup(ProgressObserver* pObserver = NULL);

        // Summary  : 更新联络人信息（此命令不改变联系人所属账户和分组信息）
        // Returns  : bool 是否更新成功
        // Parameter: ContactData * pContact 需要更新的联络人
        bool UpdateContact(ContactData* pContact);

        // Summary  : 取账户中的所有联络人分组
        // Returns  : vector<ContactGroupData*> 包含联系人分组对象指针的vector
        vector<ContactGroupData*> GetContactGroup() const { return _contactGroupList; }

        // Summary  : 删除账户中的联络人分组
        // Returns  : bool 是否删除成功
        // Parameter: ContactGroupData* pGroupData 需要删除的联络人分组
        bool DeleteContactGroup(ContactGroupData* pGroupData);

        // Summary  : 更新账户中的联络人分组
        // Returns  : bool 是否更新成功
        // Parameter: ContactGroupData* pGroupData 需要更新的联络人分组
        bool UpdateContactGroup(ContactGroupData* pGroupData);

        // Summary  : 添加账户中的联络人分组
        // Returns  : bool 是否更新成功
        // Parameter: ContactGroupData* pGroupData 待添加联络人分组
        bool AddContactGroup(/*ContactGroupData* pGroupData*/const vector<ContactGroupData*>* pDataList);

        // Summary  : 将联系人添加到相应的分组
        // Returns  : bool 是否添加成功
        // Parameter: const vector<ContactData * > * pDataList 需要添加到分组的联络人集合
        // Parameter: ContactGroupData * pGroupData 目的分组
        // Parameter: ProgressObserver * pObserver 进度条设置
        bool AddContactToGroup(const vector<ContactData*>* pDataList,ContactGroupData* pGroupData,ProgressObserver* pObserver);

        // Summary  : 将联系人添加到相应的分组
        // Returns  : bool 是否添加成功
        // Parameter: int contactId 联系人ID
        // Parameter: int groupId 分组ID
        bool AddContactToGroup(int contactId, int groupId)
        {
            ContactGroupData* pGroup = GetGroup(groupId);
            RETURN_FALSE_IF( ! pGroup);

            pGroup->AddContacts(contactId);
            return true;
        }

        // Summary  : 将联系人从相应的分组中移除
        // Returns  : bool 是否移除成功
        // Parameter: const vector<ContactData * > * pDataList 需要从分组中移除联络人集合
        // Parameter: ContactGroupData * pGroupData 需要移除联络人的分组
        // Parameter: ProgressObserver * pObserver 进度条设置
        bool RemoveContactFromGroup(const vector<ContactData*>* pDataList,ContactGroupData* pGroupData,ProgressObserver* pObserver);

        // Summary  : 将联系人从相应的分组中移除
        // Returns  : bool 是否移除成功
        // Parameter: int contactId 联系人ID
        // Parameter: int groupId 分组ID
        bool RemoveContactFromGroup(int contactId, int groupId)
        {
            ContactGroupData* pGroup = GetGroup(groupId);
            RETURN_FALSE_IF( ! pGroup);

            pGroup->RemoveContacts(contactId);
            return true;
        }

		void AddContact(ContactData* pData) {_contactDataList.push_back(pData);}

		ContactGroupData* FindGroupByName(wstring groupName);

        // Summary  : 从相应的分组中获取联系人
        // Returns  : 联络人ID集合
        // Parameter: int groupId 分组ID
        set<int> GetContactFromGroup(int groupId)
        {
            ContactGroupData* pGroup = GetGroup(groupId);
            return pGroup->GetContacts();
        }

        // Summary  : 设置联系人的收藏属性
        // Returns  : bool 是否设置成功
        // Parameter: const vector<ContactData * > * pDataList 需要设置属性的联络人集合
        // Parameter: bool bFavorite 是否为收藏
        // Parameter: ProgressObserver * pObserver 进度条设置
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

		// 获取联系人个数
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

        wstring _name;          // 账户名
        wstring _type;          // 账户类型

        const DeviceData* _pDevice;  // 设备信息

        bool _bVersion20;       // 安卓的系统的版本是否为2.0以上
        bool _bOMS;             // 是否为OMS系统

        vector<ContactGroupData*>   _contactGroupList;  // 账户中的分组列表
        vector<ContactData*>        _contactDataList;   // 账户中的联络人列表
        int                         _size;              // 联络人数量
 
        map<wstring, STRING_ID>     _ios_systemRingtone;      // ios系统铃声列表
        map<wstring, wstring>       _android_systemRingtone;  // android系统铃声列表
        map<wstring, wstring>       _ios_customRingtone;      // ios自定义铃声列表
        map<wstring, wstring>       _android_customRingtone;  // android自定义铃声列表
        map<wstring, STRING_ID>     _ios_textTone;            // ios短信铃声列表
        map<wstring, wstring>       _android_textTone;        // android短信铃声列表
    };
}
