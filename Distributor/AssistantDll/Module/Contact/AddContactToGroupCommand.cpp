#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/AddContactToGroupCommand.h"
#include "Module/Contact/ContactData.h"
#include "Module/Contact/ContactGroupData.h"
#include "Module/AndroidDefine.h"

AddContactToGroupCommand::AddContactToGroupCommand(const DeviceData* pDevice, ContactGroupData* pGroupData )
: BaseCommand(pDevice)
{
    _nAndroidChannelID = PIM;
    _nAndroidCommandID = ADD_TOGROUP;

    _pGroupData        = pGroupData;
    _pDataList         = NULL;
    _success           = false;

}

AddContactToGroupCommand::~AddContactToGroupCommand()
{
    _pDataList         = NULL;
}

std::string AddContactToGroupCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;

    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);

    // д����Ҫ���������ϵ�˲����ķ���ID
    writer.WriteInt(_pGroupData->GetId());    
    writer.WriteInt(_pDataList->size());  // ��Ҫ��ӵ���ϵ�˸���

    std::vector<const ContactData*>::const_iterator iter;
    for( iter = _pDataList->begin(); iter != _pDataList->end(); ++iter)
    {
        writer.WriteInt((*iter)->_contactId); // д��ÿ����ϵ�˵�ID
    }

    return writer.GetBuffer();
}

void AddContactToGroupCommand::SetAndroidReturnData( std::string data )
{
    BinaryBufferReader reader(data.c_str(), data.length());

    if (!parseAndroidPackageHead(&reader))
    {
        _success = false;
        return;
    }

    reader.ReadBool(_success);
}

// ִ������
void AddContactToGroupCommand::Execute()
{
    if ( _pDataList == NULL )
    {
        _success = true;
        return;
    }

    DefaultExecute();

}



bool AddContactToGroupCommand::Success()
{
    return _success;
}

void AddContactToGroupCommand::SetDataList( const vector<ContactData*>* pDataList )
{
    _pDataList = (const std::vector<const ContactData*>*)pDataList;
}

#endif