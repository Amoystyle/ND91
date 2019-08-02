#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/ContactCommandHelper.h"
#include "Module/Contact/ContactData.h"
#include "Module/Contact/ContactGroupData.h"
#include "Common/Path.h"

ContactCommandHelper::ContactCommandHelper( ContactData* pContactData, bool bVersion20, bool bOMS )
{
    _pNewContactData = NULL;
    _pContactData = pContactData;
    _bVersion20   = bVersion20;
    _bOMS         = bOMS;
}


ND91Assistant::ContactCommandHelper::ContactCommandHelper()
{
}

ContactCommandHelper::~ContactCommandHelper()
{
    _pContactData = NULL;
}

void ContactCommandHelper::WriteAndroidPeople(BinaryBufferWriter& writer)
{
    if (_bVersion20)
    {
        writer.WriteInt(_pContactData->_contactId);
        writer.WriteStringAsUTF8(_pContactData->_accountName);
        writer.WriteStringAsUTF8(_pContactData->_accountType);
        writer.WriteStringAsUTF8(_pContactData->_ringtone);
        writer.WriteBool(_pContactData->_favorite);
    }
    else if (_bOMS)
    {
        writer.WriteInt(_pContactData->_contactId);
        writer.WriteStringAsUTF8(_pContactData->_sName._displayName);
        writer.WriteStringAsUTF8(_pContactData->_ringtone);
        writer.WriteBool(_pContactData->_favorite);
        writer.WriteStringAsUTF8(_pContactData->_headPhoto._photoUri);
        writer.WriteStringAsUTF8(_pContactData->_birthday);
        writer.WriteStringAsUTF8(_pContactData->_sComment._content);
    }
    else
    {
        writer.WriteInt(_pContactData->_contactId);
        writer.WriteStringAsUTF8(_pContactData->_sName._displayName);
        writer.WriteStringAsUTF8(_pContactData->_ringtone);
        writer.WriteBool(_pContactData->_favorite);
        writer.WriteStringAsUTF8(_pContactData->_sComment._content);
    }
}

void ND91Assistant::ContactCommandHelper::ReadAndroidPeople( BinaryBufferReader *pReader )
{
    pReader->ReadInt32(_pNewContactData->_contactId);
    pReader->ReadStringFromUTF8(_pNewContactData->_accountName);
    pReader->ReadStringFromUTF8(_pNewContactData->_accountType);
    pReader->ReadStringFromUTF8(_pNewContactData->_ringtone);
    pReader->ReadBool(_pNewContactData->_favorite); 
}

void ContactCommandHelper::WriteAndroidName(BinaryBufferWriter& writer)
{
    if(!_bVersion20 || _bOMS)
    {
        return;
    }
    writer.WriteInt(METHOD_NAME);
    writer.WriteInt(_pContactData->_sName._state);
    writer.WriteInt(_pContactData->_sComment._id);
    writer.WriteInt(_pContactData->_contactId);
    writer.WriteStringAsUTF8(_pContactData->_sName._displayName);
    writer.WriteStringAsUTF8(_pContactData->_sName._familyName);
    writer.WriteStringAsUTF8(_pContactData->_sName._givenName);
    writer.WriteStringAsUTF8(_pContactData->_sName._middleName);
}

void ND91Assistant::ContactCommandHelper::ReadAndroidName( BinaryBufferReader *pReader )
{
    int nTemp;
    pReader->ReadInt32(nTemp);
    pReader->ReadInt32((int&)(_pNewContactData->_sName._state));
    pReader->ReadInt32(_pNewContactData->_sComment._id);
    pReader->ReadInt32(_pNewContactData->_contactId);
    pReader->ReadStringFromUTF8(_pNewContactData->_sName._displayName);
    pReader->ReadStringFromUTF8(_pNewContactData->_sName._familyName);
    pReader->ReadStringFromUTF8(_pNewContactData->_sName._givenName);
    pReader->ReadStringFromUTF8(_pNewContactData->_sName._middleName);
}

void ContactCommandHelper::WriteAndroidPhone(BinaryBufferWriter& writer)
{
    for (vector<Phone*>::iterator it = _pContactData->_phoneNumberList.begin(); it != _pContactData->_phoneNumberList.end();)
    {
        Phone* psPhone = *it;
		if(psPhone->_number.empty())
		{
			if(psPhone->_state == STATE_ADD)
			{
				it = _pContactData->_phoneNumberList.erase(it);
                SAFE_DELETE(psPhone);
				continue;
			}
			else
            {
				psPhone->_state = STATE_DEL;
            }
		}
		
        writer.WriteInt(METHOD_PHONE);
        writer.WriteInt(psPhone->_state);
        //writer.WriteInt(psPhone->_id);
        writer.WriteInt(atoi(psPhone->_key.c_str()));

        writer.WriteInt(_pContactData->_contactId);
        if (_bVersion20)
        {			
			char buf[10] = {0};
            string sType;
			sprintf(buf,"%d",psPhone->_type);
			sType = buf;
            writer.WriteString(sType);
        }
        else
        {
            writer.WriteInt(int(psPhone->_type));
        }
        writer.WriteStringAsUTF8(psPhone->_number);
        writer.WriteStringAsUTF8(psPhone->_label);
        ++it;
    }
}

void ND91Assistant::ContactCommandHelper::ReadAndroidPhone( BinaryBufferReader *pReader )
{
    int nTemp;
    int nCount = 0;
    pReader->ReadInt32(nCount);
    for (int i=0; i<nCount; i++)
    {
        Phone* psPhone = new Phone();
        pReader->ReadInt32(nTemp); //METHOD_PHONE
        pReader->ReadInt32((int&)(psPhone->_state));
        //pReader->ReadInt32(psPhone->_id);
        pReader->ReadString(psPhone->_key, 4);

        pReader->ReadInt32(_pContactData->_contactId);
        string sType;
        pReader->ReadStringFromUTF8(sType);
        psPhone->_type = (PhoneTypes)(atoi(sType.c_str()));
        pReader->ReadStringFromUTF8(psPhone->_number);
        pReader->ReadStringFromUTF8(psPhone->_label);
        _pNewContactData->_phoneNumberList.push_back(psPhone);
    }
}

void ContactCommandHelper::WriteAndroidOrganization(BinaryBufferWriter& writer)
{
    for (vector<Organization*>::iterator it = _pContactData->_organizationList.begin(); it != _pContactData->_organizationList.end();)
    {
        Organization* psOrganization = *it;
        if((psOrganization->_name+psOrganization->_address).empty())
        {
            if(psOrganization->_state == STATE_ADD)
            {
                it = _pContactData->_organizationList.erase(it);
                SAFE_DELETE(psOrganization);
                continue;
            }
            else
            {
                psOrganization->_state = STATE_DEL;
            }
        }

        writer.WriteInt(METHOD_ORGANIZATION);
        writer.WriteInt(psOrganization->_state);
        writer.WriteInt(psOrganization->_id);
        writer.WriteInt(_pContactData->_contactId);
        if (_bVersion20)
        {
			char buf[10] = {0};
			string sType;
			sprintf(buf,"%d",psOrganization->_type);
			sType = buf;
            writer.WriteString(sType);
        }
        else
        {
            writer.WriteInt(int(psOrganization->_type));
        }
        writer.WriteStringAsUTF8(psOrganization->_name);
        writer.WriteStringAsUTF8(psOrganization->_address);
        writer.WriteStringAsUTF8(psOrganization->_label);
        ++it;
    }
}

void ND91Assistant::ContactCommandHelper::ReadAndroidOrganization( BinaryBufferReader *pReader )
{
    int nTemp;
    int nCount = 0;
    pReader->ReadInt32(nCount);
    for (int i=0; i<nCount; i++)
    {
        Organization* psOrganization = new Organization();
        pReader->ReadInt32(nTemp); //METHOD_ORGANIZATION
        pReader->ReadInt32((int&)(psOrganization->_state));
        pReader->ReadInt32(psOrganization->_id);
        pReader->ReadInt32(_pContactData->_contactId);
        string sType;
        pReader->ReadStringFromUTF8(sType);
        psOrganization->_type = (OrganizationTypes)(atoi(sType.c_str()));
        pReader->ReadStringFromUTF8(psOrganization->_name);
        pReader->ReadStringFromUTF8(psOrganization->_address);
        pReader->ReadStringFromUTF8(psOrganization->_label);
        _pNewContactData->_organizationList.push_back(psOrganization);
    }
}

void ContactCommandHelper::WriteAndroidEmail(BinaryBufferWriter& writer)
{
    for (vector<Email*>::iterator it = _pContactData->_emailList.begin(); it != _pContactData->_emailList.end();)
    {
        Email* psEmail = *it;
		if(psEmail->_emailAccount.empty())
		{
			if(psEmail->_state == STATE_ADD)
			{
				it = _pContactData->_emailList.erase(it);
                SAFE_DELETE(psEmail);
				continue;
			}
			else
            {
				psEmail->_state = STATE_DEL;
            }
		}

        writer.WriteInt(METHOD_EMAIL);
        writer.WriteInt(psEmail->_state);
        //writer.WriteInt(psEmail->_id);
        writer.WriteInt(atoi(psEmail->_key.c_str()));

        writer.WriteInt(_pContactData->_contactId);
        if (_bVersion20)
        {
			char buf[10] = {0};
			string sType;
			sprintf(buf,"%d",psEmail->_type);
			sType = buf;
            writer.WriteString(sType);
        }
        else
        {
            writer.WriteInt(int(psEmail->_type));
        }
        writer.WriteStringAsUTF8(psEmail->_label);
        writer.WriteStringAsUTF8(psEmail->_emailAccount);
        ++it;
    }
}

void ND91Assistant::ContactCommandHelper::ReadAndroidEmail( BinaryBufferReader *pReader )
{
    int nTemp;
    int nCount = 0;
    pReader->ReadInt32(nCount);
    for (int i=0; i<nCount; i++)
    {
        Email* psEmail = new Email();
        pReader->ReadInt32(nTemp); //METHOD_EMAIL
        pReader->ReadInt32((int&)(psEmail->_state));
        //pReader->ReadInt32(psEmail->_id);
        pReader->ReadString(psEmail->_key, 4);

        pReader->ReadInt32(_pContactData->_contactId);
        string sType;
        pReader->ReadStringFromUTF8(sType);
        psEmail->_type = (EmailTypes)(atoi(sType.c_str()));
        pReader->ReadStringFromUTF8(psEmail->_label);
        pReader->ReadStringFromUTF8(psEmail->_emailAccount);
        _pNewContactData->_emailList.push_back(psEmail);
    } 
}

void ContactCommandHelper::WriteAndroidAddress(BinaryBufferWriter& writer)
{
    for (vector<Address*>::iterator it = _pContactData->_addressList.begin(); it != _pContactData->_addressList.end();)
    {
        Address* psAddress = *it;
		wstring addrStr = _bVersion20?psAddress->_street+psAddress->_city+psAddress->_province+psAddress->_zipCode:psAddress->_displayAddress;
        if(addrStr.empty())
		{
			if(psAddress->_state == STATE_ADD)
			{
				it = _pContactData->_addressList.erase(it);
                SAFE_DELETE(psAddress);
				continue;
			}
			else
            {
				psAddress->_state = STATE_DEL;
            }
		}

		writer.WriteInt(METHOD_ADDRESS);
        writer.WriteInt(psAddress->_state);
        //writer.WriteInt(psAddress->_id);
        writer.WriteInt(atoi(psAddress->_key.c_str()));
        
        writer.WriteInt(_pContactData->_contactId);
        if (_bVersion20)
        {
			char buf[10] = {0};
			string sType;
			sprintf(buf,"%d",psAddress->_type);
			sType = buf;
            writer.WriteString(sType);
            
            writer.WriteStringAsUTF8(psAddress->_label);
            writer.WriteStringAsUTF8(psAddress->_displayAddress);
            writer.WriteStringAsUTF8(psAddress->_street);
            writer.WriteStringAsUTF8(psAddress->_city);
            writer.WriteStringAsUTF8(psAddress->_province);
            writer.WriteStringAsUTF8(psAddress->_zipCode);            
        }
        else
        {
            writer.WriteInt(int(psAddress->_type));
            writer.WriteStringAsUTF8(psAddress->_label);
            writer.WriteStringAsUTF8(psAddress->_displayAddress);
        }

        ++it;
    }
}

void ND91Assistant::ContactCommandHelper::ReadAndroidAddress( BinaryBufferReader *pReader )
{
    int nTemp;
    int nCount = 0;
    pReader->ReadInt32(nCount);
    for (int i=0; i<nCount; i++)
    {
        Address* psAddress = new Address();
        pReader->ReadInt32(nTemp); //METHOD_ADDRESS
        pReader->ReadInt32((int&)(psAddress->_state));
        //pReader->ReadInt32(psAddress->_id);
        pReader->ReadString(psAddress->_key, 4);

        pReader->ReadInt32(_pContactData->_contactId);
        string sType;
        pReader->ReadStringFromUTF8(sType);
        psAddress->_type = (AddressTypes)(atoi(sType.c_str()));


        pReader->ReadStringFromUTF8(psAddress->_label);
        pReader->ReadStringFromUTF8(psAddress->_displayAddress);
        pReader->ReadStringFromUTF8(psAddress->_street);
        pReader->ReadStringFromUTF8(psAddress->_city);
        pReader->ReadStringFromUTF8(psAddress->_province);
        pReader->ReadStringFromUTF8(psAddress->_zipCode);
        _pNewContactData->_addressList.push_back(psAddress);
    }  
}

void ContactCommandHelper::WriteAndroidIm(BinaryBufferWriter& writer)
{
    for (vector<Im*>::iterator it = _pContactData->_imList.begin(); it != _pContactData->_imList.end();)
    {
        Im* psIm = *it;
		if(psIm->_imAccount.empty())
		{
			if(psIm->_state == STATE_ADD)
			{
				it = _pContactData->_imList.erase(it);
                SAFE_DELETE(psIm);
				continue;
			}
			else
            {
				psIm->_state = STATE_DEL;
            }
		}

        writer.WriteInt(METHOD_IM);
        writer.WriteInt(psIm->_state);
        //writer.WriteInt(psIm->_id);
        writer.WriteInt(atoi(psIm->_key.c_str()));

        writer.WriteInt(_pContactData->_contactId);
        if (_bVersion20)
        {
            string sType;
            if (psIm->_typeName == IM_FETION)
            {
                sType = "-1";
                writer.WriteString(sType);
            }
            else
            {
				char buf[10] = {0};
				sprintf(buf,"%d",psIm->_type);
				sType = buf;
                writer.WriteString(sType);
            }
        }
        else if (_bOMS)
        {

            string sType;
			char buf[10] = {0};
			sprintf(buf,"%d",psIm->_type);
			sType = buf;
            sType = "pre:" + sType;
            writer.WriteString(sType);
        }
        else
        {
			string sType;
			char buf[10] = {0};
			sprintf(buf,"%d",psIm->_type);
			sType = buf;
            sType = "pre:" + sType;
            writer.WriteString(sType);
        }
        writer.WriteStringAsUTF8(psIm->_imAccount);
        writer.WriteStringAsUTF8(psIm->_label);
        ++it;
    }
}
void ND91Assistant::ContactCommandHelper::ReadAndroidIm( BinaryBufferReader *pReader )
{
    int nTemp;
    int nCount = 0;
    pReader->ReadInt32(nCount);
    for (int i=0; i<nCount; i++)
    {
        Im* psIm = new Im();
        pReader->ReadInt32(nTemp); //METHOD_IM
        pReader->ReadInt32((int&)(psIm->_state));
        //pReader->ReadInt32(psIm->_id);
        pReader->ReadString(psIm->_key, 4);

        pReader->ReadInt32(_pContactData->_contactId);
        
        string sType;
        pReader->ReadString(sType);
        if (sType == "-1")
            psIm->_typeName = IM_FETION;
        else
            psIm->_typeName = (IMTypesName)(atoi(sType.c_str()));

        pReader->ReadStringFromUTF8(psIm->_imAccount);
        pReader->ReadStringFromUTF8(psIm->_label);
        _pNewContactData->_imList.push_back(psIm);
    }   
}
void ContactCommandHelper::WriteAndroidComment(BinaryBufferWriter& writer)
{
    if(!_bVersion20 || _bOMS)
    {
        return;
    }
    writer.WriteInt(METHOD_COMMENT);
    writer.WriteInt(_pContactData->_sComment._state);
    writer.WriteInt(_pContactData->_sComment._id);
    writer.WriteInt(_pContactData->_contactId);

    //此处应加入去除"\r\n", "\n"内容的代码（下面一行是2.0的实现方式）
    //string tmp = _newData.Replace("\r\n", "\n"); //"\r\n在手机上会显示乱码"

    writer.WriteStringAsUTF8(_pContactData->_sComment._content);

}
void ND91Assistant::ContactCommandHelper::ReadAndroidComment( BinaryBufferReader *pReader )
{
    int nTemp;
    pReader->ReadInt32(nTemp); //METHOD_COMMENT
    pReader->ReadInt32((int&)(_pNewContactData->_sComment._state));
    pReader->ReadInt32(_pNewContactData->_sComment._id);
    pReader->ReadInt32(_pNewContactData->_contactId);

    pReader->ReadStringFromUTF8(_pNewContactData->_sComment._content);  
}

void ND91Assistant::ContactCommandHelper::WriteAndroidPhoto( BinaryBufferWriter& writer)
{
	RETURN_IF(_bOMS);
	_pContactData->_headPhoto._state = STATE_ADD;

	if(_pContactData->_headPhoto._photoId != 0 )
		_pContactData->_headPhoto._state = STATE_UPDATE;

    if(_pContactData->_headPhoto._strSavePath.empty())
        _pContactData->_headPhoto._state = STATE_DEL;

	string binaryData= CFileOperation::ReadFile(_pContactData->_headPhoto._strSavePath,true);
	if((_pContactData->_headPhoto._state == STATE_ADD && binaryData.length() < 1) 
        || (_pContactData->_headPhoto._state == STATE_DEL && _pContactData->_headPhoto._photoId == 0))
	{
		_pContactData->_headPhoto._state = STATE_NOCHANGE;
		return;
	}

	writer.WriteInt(METHOD_PHOTO);
	writer.WriteInt(_pContactData->_headPhoto._state);
	writer.WriteInt(_pContactData->_headPhoto._photoId);
	writer.WriteInt(_pContactData->_contactId);
	writer.WriteInt(binaryData.length());
	if(binaryData.length() > 0)
		writer.WriteBuffer(binaryData.c_str(),binaryData.length());
}


void ND91Assistant::ContactCommandHelper::ReadAndroidPhoto( BinaryBufferReader *pReader )
{
    int nTemp;
    pReader->ReadInt32(nTemp); //METHOD_PHOTO
    pReader->ReadInt32((int&)(_pNewContactData->_headPhoto._state));
    pReader->ReadInt32(_pNewContactData->_headPhoto._photoId);
    pReader->ReadInt32(_pNewContactData->_contactId);
    pReader->ReadInt32(nTemp);
    char* pChar = NULL;
    pReader->ReadBuffer(pChar, nTemp);
    _pNewContactData->_headPhoto._strSavePath = Path::GlobalTempFile();
    CFileOperation::WriteBinFile(_pNewContactData->_headPhoto._strSavePath, pChar, nTemp);
}

string ND91Assistant::ContactCommandHelper::ToBuffer()
{
    BinaryBufferWriter writer;
    WriteAndroidPeople(writer);
    WriteAndroidPhoto(writer);
    WriteAndroidName(writer);
    writer.WriteInt(_pContactData->_phoneNumberList.size());
    WriteAndroidPhone(writer);
    writer.WriteInt(_pContactData->_organizationList.size());
    WriteAndroidOrganization(writer);
    writer.WriteInt(_pContactData->_emailList.size());
    WriteAndroidEmail(writer);
    writer.WriteInt(_pContactData->_addressList.size());
    WriteAndroidAddress(writer);
    writer.WriteInt(_pContactData->_imList.size());
    WriteAndroidIm(writer);
    WriteAndroidComment(writer);
    return writer.GetBuffer();
}

ContactData* ND91Assistant::ContactCommandHelper::FromBuffer( BinaryBufferReader* pReader )
{
    _pNewContactData = new ContactData();
    ReadAndroidPeople(pReader);
    ReadAndroidPhoto(pReader);
    ReadAndroidName(pReader);
    ReadAndroidPhone(pReader);
    ReadAndroidOrganization(pReader);
    ReadAndroidEmail(pReader);
    ReadAndroidAddress(pReader);
    ReadAndroidIm(pReader);
    ReadAndroidComment(pReader);
    return _pNewContactData;
}



#endif