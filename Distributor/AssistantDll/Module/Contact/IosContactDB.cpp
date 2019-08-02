#include "stdafx.h"
#ifdef MODULE_CONTACT

#define PRODUCTVERSIONL            200               // IOS设备低版本号
#define PRODUCTVERSIONH            421               // IOS设备高版本号
#define OUTLOOKTIME     L"4501-01-01 00:00:00"       // outlook默认时间

IosContactDB::IosContactDB(IosDBPath iosDBPath)
: IosDBBase(iosDBPath)
{
}

IosContactDB::~IosContactDB(void)
{
}

// 获取群组列表
bool IosContactDB::GetAllGroups(ContactAccount *pAccount)
{
    CppSQLite3Buffer sql;
    sql.Format("SELECT ROWID,Name from ABGroup;");
    CppSQLite3Query q = ExecQuery((const char*)sql);

    while (!q.Eof())
    {
        const char* iosData[2] = {0};
        for (int i = 0; i < 2; i++)
            iosData[i] = q.GetStringField(i);

        pAccount->InitContactGroupData(new ContactGroupData(iosData));

        q.NextRow();
    }
    return true;
}

// 根据组名添加群组(返回新增组ID，失败返回-1)
int IosContactDB::AddGroup(ContactGroupData *group)
{
    int groupId = -1;

    CppSQLite3Buffer sql;
    if (_iosDBPath.GetProductVersion() >= PRODUCTVERSIONL)
        sql.Format("INSERT INTO ABGroup(Name,StoreID) Values('%s',0);",CCodeOperation::UnicodeToUTF_8SQL(group->GetName()).c_str());
    else   
        sql.Format("INSERT INTO ABGroup(Name) Values('%s');",CCodeOperation::UnicodeToUTF_8SQL(group->GetName()).c_str());

    if (ExecDML((const char*)sql) > 0)
    {
        sql.Format("select rowid from ABGroup Where Name = '%s' order by rowid Desc;",CCodeOperation::UnicodeToUTF_8SQL(group->GetName()).c_str());
        groupId = ExecuteAndGetId((const char*) sql);
    }

    return groupId;
}

// 更新群组
bool IosContactDB::UpdateGroup(const ContactGroupData* group)
{
    CppSQLite3Buffer sql;
    if (_iosDBPath.GetProductVersion() >= PRODUCTVERSIONL)
        sql.Format("UPDATE ABGroup set name = '%s' , StoreID = 0 where ROWID = %d;",CCodeOperation::UnicodeToUTF_8SQL(group->GetName()).c_str(), group->GetId());
    else
        sql.Format("UPDATE ABGroup set name = '%s' where ROWID = %d;",CCodeOperation::UnicodeToUTF_8SQL(group->GetName()).c_str(), group->GetId());

    return ExecDML((const char*)sql) > 0 ? true : false;
}

// 删除群组
bool IosContactDB::DeleteGroup(int groupId)
{
    CppSQLite3Buffer sql;

    // 删关系表
    sql.Format("DELETE FROM ABGroupMembers WHERE group_id = %d;", groupId);
    ExecDML((const char*)sql);

    // 删除主表
    sql.Format("DELETE FROM ABGroup WHERE ROWID = %d;", groupId);
    return ExecDML((const char*)sql) > 0 ? true : false;
}

bool IosContactDB::GetGroupMember(ContactAccount *pAccount)
{
    for (size_t i = 0; i < pAccount->GetContactGroupList().size(); i++)
    {
        ContactGroupData * group = pAccount->GetContactGroupList().at(i);
        GetMember(group->GetId(),pAccount);
    }
    return true;
}

// 获取联系人ID
bool IosContactDB::GetMember(int groupId,ContactAccount *pAccount)
{
    CppSQLite3Buffer sql;
    sql.Format("select p.ROWID from ABGroupMembers m inner join ABGroup g on g.ROWID = m.group_id inner join ABPerson p on p.ROWID = m.member_id WHERE m.group_id = %d;", groupId);
    CppSQLite3Query q = ExecQuery((const char*)sql);

    while (!q.Eof())
    {
        pAccount->AddContactToGroup(q.GetIntField(0),groupId);
        q.NextRow();
    }
    return true;
}

// 添加联系人到群组
int IosContactDB::AddMember(int groupId, const vector<const ContactData*>* pDataList, ProgressSubject * pProgressSubject)
{
    // 检查groupId是否存在
    if (!CheckHasGroupName(groupId))
        return 0;

    // 判断用户是否取消，若取消则立即返回
    ProgressObserver* pObserver = NULL;
    if (!pProgressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(pProgressSubject->_observers[0]);

    size_t i = 0;
    for (i = 0; i < pDataList->size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)
        const ContactData* person = pDataList->at(i);
        int memberId = person->_contactId;

        // 联系人不存在群组中
        if (!CheckHasGroupMember(groupId,memberId))
        {
            // 添加联系人到群组中
            CppSQLite3Buffer sql;
            sql.Format("INSERT INTO ABGroupMembers Values((SELECT max(UID)+1 FROM ABGroupMembers),%d,%d,%d);",groupId,0,memberId);
            ExecDML((const char*)sql);
        }

        pProgressSubject->IncreaseTaskNumberComplete();
        pProgressSubject->SendProgress();

        pProgressSubject->SetCurrentTaskSum(1);
        pProgressSubject->SetCurrentTaskName(person->_sName._givenName + person->_sName._familyName);

        pProgressSubject->AddTaskProgress(1);
        pProgressSubject->SendProgress();
    }
    return (int)i;
}

// 检查是否存在组名
bool IosContactDB::CheckHasGroupName(int groupId)
{
    CppSQLite3Buffer sql;
    sql.Format("SELECT Name from ABGroup Where ROWID = %d",groupId);
    CppSQLite3Query q = ExecQuery((const char*)sql);
    return !q.FieldIsNull(0);
}

// 检查联系人是否已存在群组中
bool IosContactDB::CheckHasGroupMember(int groupId, int memberId)
{
    CppSQLite3Buffer sql;
    sql.Format("SELECT * FROM ABGroupMembers WHERE group_id = %d and member_id = %d;",groupId,memberId);
    CppSQLite3Query q = ExecQuery((const char*)sql);
    return !q.FieldIsNull(0);
}

// 检查是否存在指定的群组
bool IosContactDB::CheckHasGroup(wstring groupName)
{
    CppSQLite3Buffer sql;
    sql.Format("select * from ABGroup where name = '%s';",CCodeOperation::UnicodeToUTF_8(groupName).c_str());
    CppSQLite3Query q = ExecQuery((const char*)sql);
    return !q.FieldIsNull(0);
}

// 从群组中移除联系人
int IosContactDB::RemoveMember(int groupId, vector<int> memberIdVec, ProgressSubject * pProgressSubject)
{
    // 判断用户是否取消，若取消则立即返回
    ProgressObserver* pObserver = NULL;
    if (!pProgressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(pProgressSubject->_observers[0]);

    size_t i = 0;
    for (i = 0; i < memberIdVec.size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)

        string idstr = "";
        CppSQLite3Buffer id;
        id.Format("%d", memberIdVec[i]);
        idstr = id;

        CppSQLite3Buffer sql;
        sql.Format("DELETE FROM ABGroupMembers WHERE group_id = %d and member_id = '%s';",groupId,idstr.c_str());
        ExecDML((const char*)sql);

        pProgressSubject->IncreaseTaskNumberComplete();
        pProgressSubject->SendProgress();

        pProgressSubject->SetCurrentTaskSum(1);
        pProgressSubject->SetCurrentTaskName(CCodeOperation::UTF_8ToUnicode(idstr));

        pProgressSubject->AddTaskProgress(1);
        pProgressSubject->SendProgress();
    }
    return (int)i;

/*
    string idstr = GetRowidStr(memberIdVec);

    CppSQLite3Buffer sql;
    sql.Format("DELETE FROM ABGroupMembers WHERE group_id = %d and member_id in %s;",groupId,idstr.c_str());
    return ExecDML((const char*)sql) > 0 ? true : false;
*/
}

// 获取所有联系人
bool IosContactDB::GetAllContact(ContactAccount *pAccount)
{
    // 获取联络人基本信息
    if (!GetContactBaseInformation(pAccount))
        return false;

    // 获取联系人属性
    if (!GetContactPropertyInformation(pAccount))
        return false;

    return true;
}

bool IosContactDB::GetAllContactSize(ContactAccount *pAccount)
{
    CppSQLite3Buffer sql;
    sql.Format("SELECT count(*) as ContactNums from ABPerson;");
    int count = ExecuteAndGetId((const char*)sql);
    if (count == -1)
        pAccount->SetCount(0);
    else
        pAccount->SetCount(count);

    return true;
}

// 获取联系人基本信息
bool IosContactDB::GetContactBaseInformation(ContactAccount *pAccount)
{
    CppSQLite3Buffer sql;
    sql.Format("select * from ABPerson;");
    CppSQLite3Query q = ExecQuery((const char*)sql);

    while (!q.Eof())
    {
        const char* iosData[21] = {0};
        for (int i = 0; i < 21; i++)
            iosData[i] = q.GetStringField(i);

        ContactData* person = new ContactData(iosData);
        if (!person->_photo_hasLoad)
            GetPicture(person);// 获取头像
        pAccount->InitContactData(person);

        q.NextRow();
    }

    return true;
}

// 获取联系人属性(日期,号码,地址,IM)
bool IosContactDB::GetContactPropertyInformation(ContactAccount *pAccount)
{
    CppSQLite3Buffer sql;
    sql.Format("select * from ABMultiValue;");
    CppSQLite3Query q = ExecQuery((const char*)sql);
    
    while (!q.Eof())
    {
        for (size_t i = 0; i < pAccount->GetContactDataList().size(); i++)
        {
            ContactData* person = pAccount->GetContactDataList().at(i);
            if (person->_contactId != q.GetIntField(1))
                continue ;

            const char* iosData[6] = {0};
            for (int i = 0; i < 6; i++)
                iosData[i] = q.GetStringField(i);

            person->_ios_property->SetContactDB(this);
            person->_ios_property->GetContactMultiValue(iosData);
            break;
        }

        q.NextRow();
    }

    return true;
}

bool IosContactDB::GetPicture(ContactData* person)
{
    IosDBPath dbPath(_iosDBPath.GetDeviceData(),ISCONTACTIMAGEDB);
    IosContactImageDB contactImageDB(dbPath);

    // 打开数据库
    if (!contactImageDB.OpenDB())
        return false;

    contactImageDB.GetPhoto(person,MEDIUM_PHOTO);
    contactImageDB.GetPhoto(person,ORIGINAL_PHOTO);
    contactImageDB.GetPhoto(person,THUMBNAIL_PHOTO);
    person->_photo_hasLoad = true;

    // 关闭数据库
    contactImageDB.Close();

    return true;
}

// 添加联系人
int IosContactDB::AddContactData(const vector<ContactData*> *contactDataVec, ProgressSubject * pProgressSubject)
{
    // 判断用户是否取消，若取消则立即返回
    ProgressObserver* pObserver = NULL;
    if (!pProgressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(pProgressSubject->_observers[0]);

    size_t i = 0;
    for (i = 0; i < contactDataVec->size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)

        ContactData* person = contactDataVec->at(i);

        // 将联络人的属性(电话，地址，IM等属性)加载到person->_ios_propertyList中
        person->_ios_property->SetContactDB(this);
        person->_ios_property->AddPropertyList();

        // 填写复合名称
        //MakeCompositeNameFallback(person);

        // 添加联系人基本信息
	    if (!AddContactBaseInformation(person))
            return false;
        
        // 获取联络人ID
	    CppSQLite3Buffer sql;
        sql.Format("SELECT seq FROM sqlite_sequence WHERE name = 'ABPerson'");
        person->_contactId = ExecuteAndGetId((const char*)sql);

        // 添加联络人属性
        person->_ios_property->SetContactDB(this);
        person->_ios_property->AddContactMultiValue();

        // 添加联络人头像
        AddPicture(person);

        pProgressSubject->IncreaseTaskNumberComplete();
        pProgressSubject->SendProgress();

        pProgressSubject->SetCurrentTaskSum(1);
        pProgressSubject->SetCurrentTaskName(person->_sName._givenName + person->_sName._familyName);

        pProgressSubject->AddTaskProgress(1);
        pProgressSubject->SendProgress();
    }
    return (int)i;
}

// 添加联络人头像
bool IosContactDB::AddPicture(ContactData* person)
{
    // 修改数据库文件
    IosDBPath dbPath(_iosDBPath.GetDeviceData(),ISCONTACTIMAGEDB);
    IosContactImageDB contactImageDB(dbPath);
    contactImageDB.OpenDB();
    contactImageDB.BeginTransaction();
    try
    {
        contactImageDB.UpdatePhoto(person->_contactId,person->_headPhoto._mediumPath,MEDIUM_PHOTO);
        contactImageDB.UpdatePhoto(person->_contactId,person->_headPhoto._strSavePath,ORIGINAL_PHOTO);
        contactImageDB.UpdatePhoto(person->_contactId,person->_headPhoto._thumbnailPath,THUMBNAIL_PHOTO);

        contactImageDB.Commit();
        contactImageDB.CloseDB();
        return contactImageDB.SyncToIos(_iosDBPath.GetDeviceData());// 同步到IOS设备
    }
    catch (CppSQLite3Exception& e)
    {
        int iError = e.ErrorCode();
        const char* szError = e.ErrorMessage();
        contactImageDB.Rollback();
    }
    contactImageDB.CloseDB();
    return false;
}

// 删除联络人头像
bool IosContactDB::DeletePicture(int contactId)
{
    // 修改数据库文件
    IosDBPath dbPath(_iosDBPath.GetDeviceData(),ISCONTACTIMAGEDB);
    IosContactImageDB contactImageDB(dbPath);
    contactImageDB.OpenDB();
    contactImageDB.BeginTransaction();
    try
    {
        contactImageDB.DeletePhoto(contactId);
        contactImageDB.Commit();
        contactImageDB.CloseDB();
        return contactImageDB.SyncToIos(_iosDBPath.GetDeviceData());// 同步到IOS设备
    }
    catch (CppSQLite3Exception& e)
    {
        int iError = e.ErrorCode();
        const char* szError = e.ErrorMessage();
        contactImageDB.Rollback();
    }
    contactImageDB.CloseDB();
    return false;
}

// 添加联系人基本信息
bool IosContactDB::AddContactBaseInformation(ContactData* person)
{
    // birthday字段的判断
    wstring wstrBirthday = L"";
    if (person->_birthday != L"" && person->_birthday != OUTLOOKTIME)//outlook默认时间
        wstrBirthday = person->_birthday;

    string strBirthday = "";
    if (wstrBirthday != L"")
        strBirthday = person->_ios_property->GetIosTime(wstrBirthday);

    // storId字段的判断
    string storeId = "";
    if (_iosDBPath.GetProductVersion() >= PRODUCTVERSIONL)
        storeId = "0";
    else
        storeId = "null";

    // 获取系统当前的时间
    time_t now_time;
    time(&now_time);
    CDealTime date_time(now_time);

	CppSQLite3Buffer sql;
    sql.Format("INSERT INTO ABPerson(First,Last,Middle,FirstPhonetic,MiddlePhonetic,LastPhonetic,Organization,Department,Note,JobTitle,\
               NickName,Prefix,Suffix,Birthday,CompositeNameFallback,StoreId,Kind,CreationDate,ModificationDate) \
               VALUES('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%s,%d,%d,%d);",
               CCodeOperation::UnicodeToUTF_8SQL(person->_sName._givenName).c_str(),
               CCodeOperation::UnicodeToUTF_8SQL(person->_sName._familyName).c_str(),
               CCodeOperation::UnicodeToUTF_8SQL(person->_sName._middleName).c_str(),
               person->_sName._ios_firstPhonetic.c_str(),
               person->_sName._ios_middlePhonetic.c_str(),
               person->_sName._ios_lastPhonetic.c_str(),
               CCodeOperation::UnicodeToUTF_8SQL(person->_organizationList.at(0)->_name).c_str(),
               CCodeOperation::UnicodeToUTF_8SQL(person->_department).c_str(),
               CCodeOperation::UnicodeToUTF_8SQL(person->_sComment._content).c_str(),
               CCodeOperation::UnicodeToUTF_8SQL(person->_jobTitle).c_str(),
               CCodeOperation::UnicodeToUTF_8SQL(person->_sName._ios_nickName).c_str(),
               person->_sName._ios_prefix.c_str(),
               CCodeOperation::UnicodeToUTF_8SQL(person->_sName._ios_suffix).c_str(),
               strBirthday.c_str(),
//               CCodeOperation::UnicodeToUTF_8SQL(person->_ios_compositeNameFallback).c_str(),
               "",
               storeId.c_str(),
               0,
               (int)date_time.GetTime(),
               (int)date_time.GetTime());

    return ExecDML((const char*)sql) > 0 ? true : false;
}

// 是否未填写姓名
bool IosContactDB::IsNoname(ContactData* person)
{
    if (person->_sName._givenName == L"" && person->_sName._familyName == L"" && person->_sName._middleName == L"")
        return true;
    else
        return false;
}

// 更新联系人
bool IosContactDB::UpdateContact(ContactData* person)
{
    // 将联络人的属性(电话，地址，IM等属性)加载到person->_ios_propertyList中
    person->_ios_property->SetContactDB(this);
    person->_ios_property->AddPropertyList();

    // 填写复合名称
//MakeCompositeNameFallback(person);

    // 更新联系人基本信息
    if (!UpdateContactBaseInformation(person))
        return false;

    // 删除旧属性
    CppSQLite3Buffer sql;
    sql.Format("delete from ABMultiValueEntry where parent_id in (select UID from ABMultiValue where record_id = %d);",person->_contactId);
    ExecDML((const char*)sql);
    sql.Format("delete from ABMultiValue where record_id = %d;",person->_contactId);
    ExecDML((const char*)sql);

    // 添加联络人属性
    person->_ios_property->SetContactDB(this);
    person->_ios_property->AddContactMultiValue();

    // 更新联络人头像
    // 删除联络人头像
    DeletePicture(person->_contactId);
    // 添加联络人头像
    AddPicture(person);

    return true;
}

// 生成更新联系人语句
bool IosContactDB::UpdateContactBaseInformation(ContactData* person)
{
    // birthday字段的判断
    string strBirthday = "";
    if (person->_birthday != L"")
        strBirthday = person->_ios_property->GetIosTime(person->_birthday);

    // 获取系统当前的时间
    time_t now_time;
    time(&now_time);
    CDealTime date_time(now_time);

    CppSQLite3Buffer sql;
    sql.Format("Update ABPerson Set  First = '%s', Last = '%s', Middle = '%s', FirstPhonetic = '%s', LastPhonetic = '%s', MiddlePhonetic = '%s', Organization = '%s',\
               Department = '%s', Note = '%s', JobTitle = '%s', NickName = '%s', Prefix = '%s', Suffix = '%s', Birthday = '%s', CompositeNameFallback = '%s', \
               ModificationDate = %d where ROWID = %d;",
               CCodeOperation::UnicodeToUTF_8SQL(person->_sName._givenName).c_str(),
               CCodeOperation::UnicodeToUTF_8SQL(person->_sName._familyName).c_str(),
               CCodeOperation::UnicodeToUTF_8SQL(person->_sName._middleName).c_str(),
               person->_sName._ios_firstPhonetic.c_str(),
               person->_sName._ios_lastPhonetic.c_str(),
               person->_sName._ios_middlePhonetic.c_str(),
               CCodeOperation::UnicodeToUTF_8SQL(person->_organizationList.at(0)->_name).c_str(),
               CCodeOperation::UnicodeToUTF_8SQL(person->_department).c_str(),
               CCodeOperation::UnicodeToUTF_8SQL(person->_sComment._content).c_str(),
               CCodeOperation::UnicodeToUTF_8SQL(person->_jobTitle).c_str(),
               CCodeOperation::UnicodeToUTF_8SQL(person->_sName._ios_nickName).c_str(),
               person->_sName._ios_prefix.c_str(),
               CCodeOperation::UnicodeToUTF_8SQL(person->_sName._ios_suffix).c_str(),
               //CCodeOperation::UnicodeToUTF_8SQL(person->_birthday).c_str(),
               strBirthday.c_str(),
//               CCodeOperation::UnicodeToUTF_8SQL(person->_ios_compositeNameFallback).c_str(),
               "",
               (int)date_time.GetTime(),
               person->_contactId);

    return ExecDML((const char*)sql) > 0 ? true : false;
}

// 删除联系人
int IosContactDB::DeleteContact(vector<int> contactIdVec, ProgressSubject * pProgressSubject)
{
    // 判断用户是否取消，若取消则立即返回
    ProgressObserver* pObserver = NULL;
    if (!pProgressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(pProgressSubject->_observers[0]);

    size_t i = 0;
    for (i = 0; i < contactIdVec.size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)

        string idstr = "";
        CppSQLite3Buffer id;
        id.Format("%d", contactIdVec[i]);
        idstr = id;

        // 删除联络人
        DeleteABPerson(contactIdVec.at(i));

        // 删除联络人头像
        DeletePicture(contactIdVec.at(i));

        pProgressSubject->IncreaseTaskNumberComplete();
        pProgressSubject->SendProgress();

        pProgressSubject->SetCurrentTaskSum(1);
        pProgressSubject->SetCurrentTaskName(CCodeOperation::UTF_8ToUnicode(idstr));

        pProgressSubject->AddTaskProgress(1);
        pProgressSubject->SendProgress();
    }
    return (int)i;
}

bool IosContactDB::DeleteABPerson(int contactId)
{
    CppSQLite3Buffer sql;

    // 删除关系表
    sql.Format("DELETE FROM ABMultiValueEntry WHERE parent_id IN (SELECT UID FROM ABMultiValue WHERE record_id = %d);",contactId);
    ExecDML((const char*)sql);

    // 删除属性表
    sql.Format("DELETE FROM ABMultiValue WHERE record_id = %d;",contactId);
    ExecDML((const char*)sql);

    // 删除主表
    sql.Format("DELETE FROM ABPerson WHERE ROWID = %d;",contactId);
    return ExecDML((const char*)sql) > 0 ? true : false;
}

/*
// 填写复合名称
void IosContactDB::MakeCompositeNameFallback(ContactData* person)
{
    // 假如没填写姓名
    if (IsNoname(person))
    {
        vector<Property*> propertyVec;
        person->_ios_property->SetContactDB(this);
        person->_ios_property->GetPropertyByType(PROPERTY_PHONENUMBER, L"", &propertyVec);
        if (propertyVec.size() > 0 && propertyVec[0]->_value != L"")
        {
            string tmp = (char *)propertyVec.at(0)->_value;
            person->_ios_compositeNameFallback = CCodeOperation::UTF_8ToUnicode(tmp);
        }
    }
    else
    {
        person->_ios_compositeNameFallback = L"";
    }
}
*/

#endif