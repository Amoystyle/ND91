#include "stdafx.h"
#ifdef MODULE_CONTACT

#define PRODUCTVERSIONL            200               // IOS�豸�Ͱ汾��
#define PRODUCTVERSIONH            421               // IOS�豸�߰汾��
#define OUTLOOKTIME     L"4501-01-01 00:00:00"       // outlookĬ��ʱ��

IosContactDB::IosContactDB(IosDBPath iosDBPath)
: IosDBBase(iosDBPath)
{
}

IosContactDB::~IosContactDB(void)
{
}

// ��ȡȺ���б�
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

// �����������Ⱥ��(����������ID��ʧ�ܷ���-1)
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

// ����Ⱥ��
bool IosContactDB::UpdateGroup(const ContactGroupData* group)
{
    CppSQLite3Buffer sql;
    if (_iosDBPath.GetProductVersion() >= PRODUCTVERSIONL)
        sql.Format("UPDATE ABGroup set name = '%s' , StoreID = 0 where ROWID = %d;",CCodeOperation::UnicodeToUTF_8SQL(group->GetName()).c_str(), group->GetId());
    else
        sql.Format("UPDATE ABGroup set name = '%s' where ROWID = %d;",CCodeOperation::UnicodeToUTF_8SQL(group->GetName()).c_str(), group->GetId());

    return ExecDML((const char*)sql) > 0 ? true : false;
}

// ɾ��Ⱥ��
bool IosContactDB::DeleteGroup(int groupId)
{
    CppSQLite3Buffer sql;

    // ɾ��ϵ��
    sql.Format("DELETE FROM ABGroupMembers WHERE group_id = %d;", groupId);
    ExecDML((const char*)sql);

    // ɾ������
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

// ��ȡ��ϵ��ID
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

// �����ϵ�˵�Ⱥ��
int IosContactDB::AddMember(int groupId, const vector<const ContactData*>* pDataList, ProgressSubject * pProgressSubject)
{
    // ���groupId�Ƿ����
    if (!CheckHasGroupName(groupId))
        return 0;

    // �ж��û��Ƿ�ȡ������ȡ������������
    ProgressObserver* pObserver = NULL;
    if (!pProgressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(pProgressSubject->_observers[0]);

    size_t i = 0;
    for (i = 0; i < pDataList->size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)
        const ContactData* person = pDataList->at(i);
        int memberId = person->_contactId;

        // ��ϵ�˲�����Ⱥ����
        if (!CheckHasGroupMember(groupId,memberId))
        {
            // �����ϵ�˵�Ⱥ����
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

// ����Ƿ��������
bool IosContactDB::CheckHasGroupName(int groupId)
{
    CppSQLite3Buffer sql;
    sql.Format("SELECT Name from ABGroup Where ROWID = %d",groupId);
    CppSQLite3Query q = ExecQuery((const char*)sql);
    return !q.FieldIsNull(0);
}

// �����ϵ���Ƿ��Ѵ���Ⱥ����
bool IosContactDB::CheckHasGroupMember(int groupId, int memberId)
{
    CppSQLite3Buffer sql;
    sql.Format("SELECT * FROM ABGroupMembers WHERE group_id = %d and member_id = %d;",groupId,memberId);
    CppSQLite3Query q = ExecQuery((const char*)sql);
    return !q.FieldIsNull(0);
}

// ����Ƿ����ָ����Ⱥ��
bool IosContactDB::CheckHasGroup(wstring groupName)
{
    CppSQLite3Buffer sql;
    sql.Format("select * from ABGroup where name = '%s';",CCodeOperation::UnicodeToUTF_8(groupName).c_str());
    CppSQLite3Query q = ExecQuery((const char*)sql);
    return !q.FieldIsNull(0);
}

// ��Ⱥ�����Ƴ���ϵ��
int IosContactDB::RemoveMember(int groupId, vector<int> memberIdVec, ProgressSubject * pProgressSubject)
{
    // �ж��û��Ƿ�ȡ������ȡ������������
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

// ��ȡ������ϵ��
bool IosContactDB::GetAllContact(ContactAccount *pAccount)
{
    // ��ȡ�����˻�����Ϣ
    if (!GetContactBaseInformation(pAccount))
        return false;

    // ��ȡ��ϵ������
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

// ��ȡ��ϵ�˻�����Ϣ
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
            GetPicture(person);// ��ȡͷ��
        pAccount->InitContactData(person);

        q.NextRow();
    }

    return true;
}

// ��ȡ��ϵ������(����,����,��ַ,IM)
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

    // �����ݿ�
    if (!contactImageDB.OpenDB())
        return false;

    contactImageDB.GetPhoto(person,MEDIUM_PHOTO);
    contactImageDB.GetPhoto(person,ORIGINAL_PHOTO);
    contactImageDB.GetPhoto(person,THUMBNAIL_PHOTO);
    person->_photo_hasLoad = true;

    // �ر����ݿ�
    contactImageDB.Close();

    return true;
}

// �����ϵ��
int IosContactDB::AddContactData(const vector<ContactData*> *contactDataVec, ProgressSubject * pProgressSubject)
{
    // �ж��û��Ƿ�ȡ������ȡ������������
    ProgressObserver* pObserver = NULL;
    if (!pProgressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(pProgressSubject->_observers[0]);

    size_t i = 0;
    for (i = 0; i < contactDataVec->size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)

        ContactData* person = contactDataVec->at(i);

        // �������˵�����(�绰����ַ��IM������)���ص�person->_ios_propertyList��
        person->_ios_property->SetContactDB(this);
        person->_ios_property->AddPropertyList();

        // ��д��������
        //MakeCompositeNameFallback(person);

        // �����ϵ�˻�����Ϣ
	    if (!AddContactBaseInformation(person))
            return false;
        
        // ��ȡ������ID
	    CppSQLite3Buffer sql;
        sql.Format("SELECT seq FROM sqlite_sequence WHERE name = 'ABPerson'");
        person->_contactId = ExecuteAndGetId((const char*)sql);

        // �������������
        person->_ios_property->SetContactDB(this);
        person->_ios_property->AddContactMultiValue();

        // ���������ͷ��
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

// ���������ͷ��
bool IosContactDB::AddPicture(ContactData* person)
{
    // �޸����ݿ��ļ�
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
        return contactImageDB.SyncToIos(_iosDBPath.GetDeviceData());// ͬ����IOS�豸
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

// ɾ��������ͷ��
bool IosContactDB::DeletePicture(int contactId)
{
    // �޸����ݿ��ļ�
    IosDBPath dbPath(_iosDBPath.GetDeviceData(),ISCONTACTIMAGEDB);
    IosContactImageDB contactImageDB(dbPath);
    contactImageDB.OpenDB();
    contactImageDB.BeginTransaction();
    try
    {
        contactImageDB.DeletePhoto(contactId);
        contactImageDB.Commit();
        contactImageDB.CloseDB();
        return contactImageDB.SyncToIos(_iosDBPath.GetDeviceData());// ͬ����IOS�豸
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

// �����ϵ�˻�����Ϣ
bool IosContactDB::AddContactBaseInformation(ContactData* person)
{
    // birthday�ֶε��ж�
    wstring wstrBirthday = L"";
    if (person->_birthday != L"" && person->_birthday != OUTLOOKTIME)//outlookĬ��ʱ��
        wstrBirthday = person->_birthday;

    string strBirthday = "";
    if (wstrBirthday != L"")
        strBirthday = person->_ios_property->GetIosTime(wstrBirthday);

    // storId�ֶε��ж�
    string storeId = "";
    if (_iosDBPath.GetProductVersion() >= PRODUCTVERSIONL)
        storeId = "0";
    else
        storeId = "null";

    // ��ȡϵͳ��ǰ��ʱ��
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

// �Ƿ�δ��д����
bool IosContactDB::IsNoname(ContactData* person)
{
    if (person->_sName._givenName == L"" && person->_sName._familyName == L"" && person->_sName._middleName == L"")
        return true;
    else
        return false;
}

// ������ϵ��
bool IosContactDB::UpdateContact(ContactData* person)
{
    // �������˵�����(�绰����ַ��IM������)���ص�person->_ios_propertyList��
    person->_ios_property->SetContactDB(this);
    person->_ios_property->AddPropertyList();

    // ��д��������
//MakeCompositeNameFallback(person);

    // ������ϵ�˻�����Ϣ
    if (!UpdateContactBaseInformation(person))
        return false;

    // ɾ��������
    CppSQLite3Buffer sql;
    sql.Format("delete from ABMultiValueEntry where parent_id in (select UID from ABMultiValue where record_id = %d);",person->_contactId);
    ExecDML((const char*)sql);
    sql.Format("delete from ABMultiValue where record_id = %d;",person->_contactId);
    ExecDML((const char*)sql);

    // �������������
    person->_ios_property->SetContactDB(this);
    person->_ios_property->AddContactMultiValue();

    // ����������ͷ��
    // ɾ��������ͷ��
    DeletePicture(person->_contactId);
    // ���������ͷ��
    AddPicture(person);

    return true;
}

// ���ɸ�����ϵ�����
bool IosContactDB::UpdateContactBaseInformation(ContactData* person)
{
    // birthday�ֶε��ж�
    string strBirthday = "";
    if (person->_birthday != L"")
        strBirthday = person->_ios_property->GetIosTime(person->_birthday);

    // ��ȡϵͳ��ǰ��ʱ��
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

// ɾ����ϵ��
int IosContactDB::DeleteContact(vector<int> contactIdVec, ProgressSubject * pProgressSubject)
{
    // �ж��û��Ƿ�ȡ������ȡ������������
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

        // ɾ��������
        DeleteABPerson(contactIdVec.at(i));

        // ɾ��������ͷ��
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

    // ɾ����ϵ��
    sql.Format("DELETE FROM ABMultiValueEntry WHERE parent_id IN (SELECT UID FROM ABMultiValue WHERE record_id = %d);",contactId);
    ExecDML((const char*)sql);

    // ɾ�����Ա�
    sql.Format("DELETE FROM ABMultiValue WHERE record_id = %d;",contactId);
    ExecDML((const char*)sql);

    // ɾ������
    sql.Format("DELETE FROM ABPerson WHERE ROWID = %d;",contactId);
    return ExecDML((const char*)sql) > 0 ? true : false;
}

/*
// ��д��������
void IosContactDB::MakeCompositeNameFallback(ContactData* person)
{
    // ����û��д����
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