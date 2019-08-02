#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/ContactGroupData.h"
#include "Module/Contact/ContactParser.h"
#include "Common/BinaryBufferReader.h"
#include "Common/BinaryBufferWriter.h"

#include "Device/CFConverter.h"
#include "Module/Common/Synchronizer/DeviceSyncClient.h"

long ContactGroupData::_idIncrementer  = 1;
ContactGroupData::ContactGroupData()
{
    _id     = 0;

    char temp[125] = {0};
    sprintf(temp, "Group/ND%ld", _idIncrementer++);
    _key = temp;
    SetStatus(NEW);
    _recordEntityName = "com.apple.contacts.Group";
}

ContactGroupData::ContactGroupData( ContactGroupData* pGroupData )
{
    _id          = pGroupData->_id;      
    _name        = pGroupData->_name;         
    _comment     = pGroupData->_comment;      
    _accountName = pGroupData->_accountName;
    _accountType = pGroupData->_accountType;
}

ContactGroupData::ContactGroupData(const char* iosData[2])
{
    _id           = atoi(iosData[0]);
    _name         = CCodeOperation::UTF_8ToUnicode(iosData[1]);
}

ContactGroupData::ContactGroupData(string key,map<string,void*> entity)
{
    _id = atoi(CStrOperation::parseStrings(key,'/').at(CStrOperation::parseStrings(key,'/').size()-1).c_str());
    _key = key;
    for (map<string, void*>:: iterator it = entity.begin(); it != entity.end(); it++)
    {
        string str = "";
        if (CCFConverter::NDCFGetTypeID(it->second) == CCFConverter::NDCFStringGetTypeID())
            CCFConverter::GetIosData(it->second, str);

        if (it->first == "name")
            _name = CCodeOperation::GB2312ToUnicode(str);
        else if (it->first == "members")
            IniMembers(str,it->second);
        else if (it->first == "com.apple.syncservices.RecordEntityName")
            _recordEntityName = str;
    }
    SetStatus(NO_CHANGE);
}

void ContactGroupData::IniMembers(string member,void* members)
{
    vector<string> contacts;
    if (!member.empty())
        contacts.push_back(member);
    else
        CCFConverter::GetIosData(members,contacts);

    for (size_t i = 0; i < contacts.size(); i++)
        _contacts.insert(atoi(contacts.at(i).c_str()));
}

ContactGroupData::~ContactGroupData()
{
}

std::wstring ContactGroupData::GetAccountName() const
{
    return _accountName;
}

void ContactGroupData::SetAccountName( wstring accountName )
{
    _accountName = accountName;
}

std::wstring ContactGroupData::GetAccountType() const
{
    return _accountType;
}

void ContactGroupData::SetAccountType( wstring accountType )
{
    _accountType = accountType;
}

void ContactGroupData::ConvertToDictInfos(map<string, void*>& infoss)
{
    if (GetStatus() == BaseData::DEL)
        return;

    vector<void*> members;
    for (std::set<int>::iterator it = _contacts.begin(); it != _contacts.end(); it++)
    {
        char temp[125] = {0};
        sprintf(temp, "%d", *it);
        members.push_back(CCFConverter::StringtoCFString(temp));
    }

    infoss.insert(pair<string, void*>(DeviceSyncClient::RECORD_ENTITY_NAME_KEY,CCFConverter::StringtoCFString(ContactParser::GROUP_ENTITY_NAME)));
    if (!members.empty())
        infoss.insert(pair<string, void*>("members", CCFConverter::ArrayToCFArray(members)));
    infoss.insert(pair<string, void*>("name", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(_name))));
}

void ND91Assistant::ContactGroupData::Read( BinaryBufferReader* pReader, bool hasAccount )
{
    pReader->ReadInt32(_id);
    pReader->ReadStringFromUTF8(_name);
    pReader->ReadStringFromUTF8(_comment);

    if (hasAccount)
    {
        pReader->ReadStringFromUTF8(_accountName);
        pReader->ReadStringFromUTF8(_accountType);
    }
}

void ND91Assistant::ContactGroupData::Write( BinaryBufferWriter* pWriter, bool hasAccount ) const
{
    pWriter->WriteInt(_id);
    pWriter->WriteStringAsUTF8(_name);
    pWriter->WriteStringAsUTF8(_comment);

    if (hasAccount)
    {
        pWriter->WriteStringAsUTF8(_accountName);
        pWriter->WriteStringAsUTF8(_accountType);
    }
}


#endif
