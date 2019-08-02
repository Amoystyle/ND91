#pragma once

namespace ND91Assistant
{
    class IosContactDB : public IosDBBase
    {
    public:
        IosContactDB(IosDBPath iosDBPath);
        ~IosContactDB(void);

    #pragma region 对群组的操作

        // Summary  : 获取群组列表
        // Returns  : bool 是否成功
        // Parameter: ContactAccount *pAccount 联系人账户
        bool GetAllGroups(ContactAccount *pAccount);

        // Summary  : 根据组名添加群组
        // Returns  : int 返回新增组ID，失败返回-1
        // Parameter: ContactGroupData * group 群组信息
        int AddGroup(ContactGroupData* group);

        // Summary  : 更新群组
        // Returns  : bool 是否成功
        // Parameter: ContactGroupData * group 群组信息
        bool UpdateGroup(const ContactGroupData* group);

        // Summary  : 删除群组
        // Returns  : bool 是否成功
        // Parameter: int groupId 要删除的组ID
        bool DeleteGroup(int groupId);

        // Summary  : 获取组成员
        // Returns  : bool 是否成功
        // Parameter: ContactAccount * pAccount 联系人账户
        bool GetGroupMember(ContactAccount *pAccount);   

        // Summary  : 添加联系人到群组
        // Returns  : int 实际执行数据条数
        // Parameter: int groupId 组ID
        // Parameter: const vector<const ContactData*>* pDataList 成员ID集合
        int AddMember(int groupId, const vector<const ContactData*>* pDataList, ProgressSubject * pProgressSubject);

        // Summary  : 从群组中移除联系人
        // Returns  : int 实际执行数据条数
        // Parameter: int groupId   组ID
        // Parameter: vector<int> memberIdVec 联络人ID数组
        int RemoveMember(int groupId, vector<int> memberIdVec, ProgressSubject * pProgressSubject);

    #pragma endregion

    #pragma region 联系人操作

        // Summary  : 获取所有联系人
        // Returns  : bool 是否成功
        // Parameter: ContactAccount *pAccount 装载到此帐户中
        bool GetAllContact(ContactAccount *pAccount);

        // Summary  : 获取联系人数量
        // Returns  : bool 是否成功
        // Parameter: ContactAccount *pAccount 装载到此帐户中
        bool GetAllContactSize(ContactAccount *pAccount);

        // 添加联系人
        // 返回实际执行数据条数
        int AddContactData(const vector<ContactData*> *contactDataVec, ProgressSubject * pProgressSubject);

        // 更新联系人
        bool UpdateContact(ContactData* person);                                      

        // 删除联系人
        // 返回实际执行数据条数
        int DeleteContact(vector<int> contactIdVec, ProgressSubject * pProgressSubject);                                     

    #pragma endregion
    private:

        // Summary  : 获取组联系人ID
        // Returns  : bool 是否成功
        // Parameter: int groupId 组ID
        // Parameter: ContactAccount *pAccount 联络人账户
        bool GetMember(int groupId,ContactAccount *pAccount);  

        // Summary  : // 获取联系人基本信息
        // Returns  : bool 是否成功
        // Parameter: ContactAccount * pAccount 联络人账户
        bool GetContactBaseInformation(ContactAccount *pAccount);

        // Summary  : // 获取联系人属性(日期,号码,地址,IM)
        // Returns  : bool 是否成功
        // Parameter: ContactAccount * pAccount 联络人账户
        bool GetContactPropertyInformation(ContactAccount *pAccount);

        // Summary  : 检查是否存在组名
        // Returns  : bool true:存在 false:不存在
        // Parameter: int groupId 群组ID
        bool CheckHasGroupName(int groupId);

        // Summary  : 检查联系人是否已存在群组中
        // Returns  : bool true:存在 false: 不存在
        // Parameter: int groupId 群组ID
        // Parameter: int memberId 联系人ID
        bool CheckHasGroupMember(int groupId, int memberId);

        // Summary  : 检查是否存在指定的群组
        // Returns  : bool true:存在 false: 不存在
        // Parameter: wstring groupName 指定的群组名
        bool CheckHasGroup(wstring groupName);

    private:
        // Summary  : 填写复合名称
        // Returns  : void
        // Parameter: ContactData * person 联络人信息
//void MakeCompositeNameFallback(ContactData* person);

        // Summary  : 是否未填写姓名
        // Returns  : bool true:未填写 false: 填写
        // Parameter: ContactData * person 联络人信息
        bool IsNoname(ContactData* person);

        // Summary  : 添加联系人基本信息
        // Returns  : bool
        // Parameter: ContactData * person 联系人
		bool AddContactBaseInformation(ContactData* person);

        // Summary  : 更新联系人基本信息
        // Returns  : void
        // Parameter: ContactData * person 联系人
        bool UpdateContactBaseInformation(ContactData* person);

        // Summary  : 删除联络人
        // Returns  : bool true:成功 false:失败
        // Parameter: int contactId 联络人ID
        bool DeleteABPerson(int contactId);

        // Summary  : 获取联络人头像
        // Returns  : bool
        // Parameter: ContactData * person 联络人
        bool GetPicture(ContactData* person);

        // Summary  : 添加联络人头像
        // Returns  : bool 
        // Parameter: ContactData * person 联络人
        bool AddPicture(ContactData* person);

        // Summary  : 删除联络人头像
        // Returns  : bool
        // Parameter: int contactId 联络人ID
        bool DeletePicture(int contactId);
    };
}
