#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/GetContactCommand.h"
#include "Module/Contact/ContactAccount.h"
#include "Module/Contact/ContactManager.h"
#include "Module/Contact/ContactSynchronizer.h"
#include "Module/AndroidDefine.h"

#define DEFAULT_ACCOUNT_TYPE    L"default"                          // 默认帐户类型
#define DEFAULT_ACCOUNT_NAME    L"com.android.contacts.default"     // 默认账户名

GetContactCommand::GetContactCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = PIM;
    _nAndroidCommandID = GET_CONTACT;
    
    // 获取安卓系统版本
    //if (((pDevice->GetDevInfo())->_deviceProductVersion).compare(L"2.0") == -1)
    if (((pDevice->GetDevInfo())->_deviceProductVersion).compare(L"2.0") < 0)
        _bVersion20 = false;
    else
        _bVersion20 = true;

    if ((_pDevice->GetDevInfo())->_platform == Platform_OMS)
        _bOMS = true;
    else
        _bOMS = false;

    _onlySize          = false;
}

GetContactCommand::~GetContactCommand()
{
}

std::string GetContactCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;

    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);

    return writer.GetBuffer();
}

void GetContactCommand::SetAndroidReturnData( std::string data )
{
    BinaryBufferReader reader(data.c_str(), data.length());

    if (!parseAndroidPackageHead(&reader))
    {
        _result = COMMAND_EXECUTE_FAIL;
        return;
    }

//     CFileOperation::WriteBinFile(L"d:\\test.dat", data.c_str(), data.size());

    FromBuffer(&reader);
 
}

void GetContactCommand::SetOnlySize(bool onlySize)
{
    _onlySize = onlySize;
}

// 执行命令
void GetContactCommand::Execute()
{
    _result = COMMAND_EXECUTE_SUCCESS;
    if ( _pDevice->GetDevInfo()->_deviceType == Android )
        DefaultExecute();
    else if ( _pDevice->GetDevInfo()->_deviceType == Ios )
        IosExecute();
}

const vector<ContactAccount*>* GetContactCommand::GetAccountList()
{
    return &_contactAcconutList;
}

void GetContactCommand::FromBuffer(BinaryBufferReader* pReader)
{
    std::map<int,ContactData*>::iterator iter; // 存放联络人的集合，为方便比对使用MAP类型

    int id = 0;
    int contactId = 0;      
    int methodType = METHOD_ENDFLAG;
    pReader->ReadInt32(methodType);
    while (methodType != METHOD_ENDFLAG)
    {
        switch (methodType)
        {
        case METHOD_PEOPLE:
            {
                ContactData* pContactData = new ContactData(); // 只在此处实例化ContactData类对象
                ParseAndroidPeople(pReader, pContactData);
                _contactMap.insert( pair<int, ContactData*>(pContactData->_contactId, pContactData) ); // 加入所有的联系人MAP
                break;
            }
        case METHOD_NAME:
            {
                ParseAndroidPhoneName(pReader);
                break;
            }
        case METHOD_PHONE:
            {
                Phone* psPhone = new Phone();
                //pReader->ReadInt32(psPhone->_id);
                pReader->ReadString(psPhone->_key, 4);

                pReader->ReadInt32(contactId);
                ParseAndroidPhoneTypes(pReader, psPhone);
                pReader->ReadStringFromUTF8(psPhone->_number);
                pReader->ReadStringFromUTF8(psPhone->_label);
                iter = _contactMap.find(contactId);
                if ( iter != _contactMap.end() )
                {
                    ContactData* pTempData = iter->second;
                    pTempData->_phoneNumberList.push_back(psPhone);
                }
                else
                {
                    SAFE_DELETE(psPhone);
                }
                break;
            }
        case METHOD_EMAIL:
            {
	            Email* psEmail = new Email();
	            //pReader->ReadInt32(psEmail->_id);
                pReader->ReadString(psEmail->_key, 4);

	            pReader->ReadInt32(contactId);
	            ParseAndroidEmailTypes(pReader, psEmail);
	            pReader->ReadStringFromUTF8(psEmail->_label);
	            pReader->ReadStringFromUTF8(psEmail->_emailAccount);
	            iter = _contactMap.find(contactId);
	            if ( iter != _contactMap.end() )
	            {
	                ContactData* pTempData = iter->second;
	                pTempData->_emailList.push_back(psEmail);
	            }
	            else
	            {
	                SAFE_DELETE(psEmail);
	            }
	            break;
            }
        case METHOD_IM:
            {
	            Im* psIm = new Im();
	            //pReader->ReadInt32(psIm->_id);
                pReader->ReadString(psIm->_key, 4);

	            pReader->ReadInt32(contactId);
	            ParseAndroidImTypes(pReader, psIm);
	            pReader->ReadStringFromUTF8(psIm->_imAccount);
	            pReader->ReadStringFromUTF8(psIm->_label);
	            iter = _contactMap.find(contactId);
	            if ( iter != _contactMap.end() )
	            {
	                ContactData* pTempData = iter->second;
	                pTempData->_imList.push_back(psIm);
	            }
	            else
	            {
	                SAFE_DELETE(psIm);
	            }
	            break;
            }
        case METHOD_ADDRESS:
            {
	            Address* psAddress = new Address();
	            //pReader->ReadInt32(psAddress->_id);
                pReader->ReadString(psAddress->_key, 4);

	            pReader->ReadInt32(contactId);
	            ParseAndroidAddress(pReader, psAddress);
	            iter = _contactMap.find(contactId);
	            if ( iter != _contactMap.end() )
	            {
	                ContactData* pTempData = iter->second;
	                pTempData->_addressList.push_back(psAddress);
	            }
	            else
	            {
	                SAFE_DELETE(psAddress);
	            }
	            break;
            }
        case METHOD_ORGANIZATION:
            {
	            Organization* psOrganization = new Organization();
	            pReader->ReadInt32(psOrganization->_id);
	            pReader->ReadInt32(contactId);
	            ParseAndroidOrganizationTypes(pReader, psOrganization);
	            pReader->ReadStringFromUTF8(psOrganization->_name);
	            pReader->ReadStringFromUTF8(psOrganization->_address);
	            pReader->ReadStringFromUTF8(psOrganization->_label);
	            iter = _contactMap.find(contactId);
	            if ( iter != _contactMap.end() )
	            {
	                ContactData* pTempData = iter->second;
	                pTempData->_organizationList.push_back(psOrganization);
	            }
	            else
	            {
	                SAFE_DELETE(psOrganization);
	            }
	            break;
            }
        case METHOD_COMMENT:
            {
	            wstring content = L"";
	            pReader->ReadInt32(id);
	            pReader->ReadInt32(contactId);
	            pReader->ReadStringFromUTF8(content);
	            iter = _contactMap.find(contactId);
	            if ( iter != _contactMap.end() )
	            {
	                ContactData* pTempData = iter->second;
	                pTempData->_sComment._id = id;
	                pTempData->_sComment._content = content;
	            }
	            break;
            }
        case METHOD_GROUP:
            {
	            ContactGroupData* pContactGroup = new ContactGroupData(); // 只在此处实例化ContactGroupData类对象
	            ParseAndroidGroup(pReader, pContactGroup);    
	            //_groupMap[pContactGroup->GetId()] = pContactGroup; // 加获取到的群组信息加入群组MAP
				_groupMap.insert(pair<int, ContactGroupData*>(pContactGroup->GetId(),pContactGroup));
	            break;
            }
        case METHOD_GROUPMEMBERSHIP:
            {
	            int groupId;
	            pReader->ReadInt32(id);
	            pReader->ReadInt32(contactId);
	            pReader->ReadInt32(groupId);
	            iter = _contactMap.find(contactId);
	            // 根据MemberShip的信息将联络人与其所在分组关联
	            if ( iter != _contactMap.end() )
	            {
	                ContactData* pTempData = iter->second;
					pTempData->_groupIdList.push_back(groupId);
					// 此处会比群组先获取，so下面无效?
	                map<int, ContactGroupData*>::iterator itGroup;
	                itGroup = _groupMap.find(groupId);
	                if ( itGroup != _groupMap.end() )
	                {
	                    ContactGroupData* pTempGroup = itGroup->second;
                        // 将分组信息放入相应的联络人的分组集合中
                        pTempData->_tempGroupNames.push_back(pTempGroup->GetName());
	                }
	            }
	            break;
            }
        case METHOD_PHOTO:
            {   //此处不会加载图片，图片的加载在另外一个线程中进行
	            int nSize = 0;
	            pReader->ReadInt32(id);
	            pReader->ReadInt32(contactId);
	            pReader->ReadInt32(nSize);
	            iter = _contactMap.find(contactId);
                char* photoData = new char[nSize];
                pReader->ReadBuffer(photoData, nSize);
                
				SAFE_DELETE_ARRAY(photoData);
	            break;
            }
        case METHOD_ACCOUNT:
            {
	            ContactAccount* pContactAccout = new ContactAccount(_pDevice);
				wstring  accountName;
				wstring  accountType;
	            pReader->ReadStringFromUTF8(accountName);
	            pReader->ReadStringFromUTF8(accountType);
                if (accountName.empty()
                    || accountType.empty())
                { 
                    accountName = DEFAULT_ACCOUNT_NAME;
                    accountType = DEFAULT_ACCOUNT_TYPE;
                }

				pContactAccout->SetName(accountName);
				pContactAccout->SetType(accountType);
	            _contactAcconutList.push_back(pContactAccout);
	            break;
            }
        default:
            {
                break;
            }
        }
        if(false == pReader->ReadInt32(methodType))
        {
            break;
        }
    }

	for (std::map<int,ContactData*>::iterator iter = _contactMap.begin(); iter != _contactMap.end(); ++iter)
	{
		ContactData* pTempData = iter->second;
		for (size_t i=0; i< pTempData->_groupIdList.size(); i++)
		{
			map<int, ContactGroupData*>::iterator itGroup = _groupMap.find(pTempData->_groupIdList[i]);
			if ( itGroup != _groupMap.end() )
			{
				ContactGroupData* pTempGroup = itGroup->second;
				pTempGroup->AddContacts(pTempData->_contactId);
			}
		}
	}

    // 若是无账户信息的手机，将所有的联络人和分组信息添加进默认账户
    if (!_bVersion20 || _bOMS || _contactAcconutList.empty())
    {
        AddIntoDefaultAccount();
    }
}

void GetContactCommand::AddIntoDefaultAccount()
{
    ContactAccount* pDefaultContactAccount = CreateDefaultAccount();

    for (std::map<int,ContactData*>::iterator iter = _contactMap.begin(); iter != _contactMap.end(); ++iter)
    {
        ContactData* pTempData = iter->second;
        pDefaultContactAccount->InitContactData(pTempData);
    }
    for (map<int, ContactGroupData*>::iterator itGroup = _groupMap.begin(); itGroup != _groupMap.end(); ++itGroup)
    {
        ContactGroupData* pTempGroup = itGroup->second;
        pDefaultContactAccount->InitContactGroupData(pTempGroup);
    }
}

void GetContactCommand::ParseAndroidPeople(BinaryBufferReader* pReader, ContactData* pContactData)
{
    if (_bVersion20)
    {
        pReader->ReadInt32(pContactData->_contactId);
        pReader->ReadStringFromUTF8(pContactData->_accountName);
        pReader->ReadStringFromUTF8(pContactData->_accountType);
        pReader->ReadStringFromUTF8(pContactData->_ringtone);
        pReader->ReadBool(pContactData->_favorite);
		if (pContactData->_accountType.empty())
			pContactData->_accountType = DEFAULT_ACCOUNT_TYPE;
        if (pContactData->_accountName.empty())
            pContactData->_accountName = DEFAULT_ACCOUNT_NAME;

        bool isAddToAccount = false;

        // 添加进相应账户的联系人集合中
        for ( vector<ContactAccount*>::iterator it = _contactAcconutList.begin(); it != _contactAcconutList.end(); ++it )
        {
            ContactAccount* pContactAccount = *it;
            if ((pContactAccount->GetName() == pContactData->_accountName 
				||pContactData->_accountName.empty())
                &&pContactAccount->GetType() == pContactData->_accountType)
            {
				pContactData->_accountName=pContactAccount->GetName();
                pContactAccount->InitContactData(pContactData);
                isAddToAccount = true;
				break;
            }
        }

        // 联系人不属于任何账户
        if (!isAddToAccount)
        {
            ContactAccount* pContactAccout = new ContactAccount(_pDevice);
            pContactAccout->SetName(pContactData->_accountName);
            pContactAccout->SetType(pContactData->_accountType);
            pContactAccout->InitContactData(pContactData);
            _contactAcconutList.push_back(pContactAccout);
        }
    }
    else if (_bOMS)
    {
        pReader->ReadInt32(pContactData->_contactId);
        pReader->ReadStringFromUTF8(pContactData->_sName._displayName);
        pReader->ReadStringFromUTF8(pContactData->_ringtone);
        pReader->ReadBool(pContactData->_favorite);
        pReader->ReadStringFromUTF8(pContactData->_headPhoto._photoUri);
        pReader->ReadStringFromUTF8(pContactData->_headPhoto._photoPath);
        pReader->ReadStringFromUTF8(pContactData->_birthday);
        pReader->ReadStringFromUTF8(pContactData->_sComment._content);
    }
    else
    {
        pReader->ReadInt32(pContactData->_contactId);
        pReader->ReadStringFromUTF8(pContactData->_sName._displayName);
        pReader->ReadStringFromUTF8(pContactData->_ringtone);
        pReader->ReadBool(pContactData->_favorite);
        pReader->ReadStringFromUTF8(pContactData->_sComment._content);
    }
}

void GetContactCommand::ParseAndroidPhoneName(BinaryBufferReader* pReader)
{
    std::map<int,ContactData*>::iterator iter;
    int id = 0;
    int contactId = 0;  
    wstring displayName = L"";
    wstring givenName = L"";
    wstring familyName = L"";
    wstring middleName = L"";
    pReader->ReadInt32(id);
    pReader->ReadInt32(contactId);
    pReader->ReadStringFromUTF8(displayName);
    pReader->ReadStringFromUTF8(familyName);
    pReader->ReadStringFromUTF8(givenName);
    pReader->ReadStringFromUTF8(middleName);
    iter = _contactMap.find(contactId);
    if ( iter != _contactMap.end() )
    {
        ContactData* pTempData = iter->second;
        pTempData->_sName._id = id;
        pTempData->_sName._displayName = displayName;
        pTempData->_sName._familyName = familyName;
        pTempData->_sName._givenName = givenName;
        pTempData->_sName._middleName = middleName;
    }
}

void GetContactCommand::ParseAndroidPhoneTypes(BinaryBufferReader* pReader, Phone* psPhone)
{
    if (_bVersion20)
    {
        string sType = "";
        pReader->ReadString(sType);
        psPhone->_type = (PhoneTypes) (sType[0] - '0');
    }
    else
    {
        int nType = 0;
        pReader->ReadInt32(nType);
        psPhone->_type = (PhoneTypes) nType;
    }
}

void GetContactCommand::ParseAndroidEmailTypes(BinaryBufferReader* pReader, Email* psEmail)
{
    if (_bVersion20)
    {
        string sType = "";
        pReader->ReadString(sType);
        psEmail->_type = (EmailTypes) (sType[0] - '0');
    }
    else
    {
        int nType = 0;
        pReader->ReadInt32(nType);
        psEmail->_type = (EmailTypes) nType;
    }
}
void GetContactCommand::ParseAndroidImTypes(BinaryBufferReader* pReader, Im* psIm)
{
    string sType = "";
    pReader->ReadString(sType);

    if (_bVersion20)
    {
        psIm->_typeName = (IMTypesName) (sType[0] - '0');
    }
    else if (_bOMS)
    {
        if ("pre:0" == sType)
            psIm->_typeName = IM_ATM;
        else if("pre:1" == sType) 
            psIm->_typeName = IM_WINDOWSLIVE;
        else if("pre:2" == sType) 
            psIm->_typeName = IM_YAHOO;
        else if("pre:3" == sType) 
            psIm->_typeName = IM_SKYPE;
        else if("pre:4" == sType) 
            psIm->_typeName = IM_QQ;
        else if("pre:5" == sType) 
            psIm->_typeName = IM_GOOLETALK;
        else if("pre:6" == sType) 
            psIm->_typeName = IM_ICQ;
        else if("pre:7" == sType) 
            psIm->_typeName = IM_JABBER;
        else if("pre:8" == sType) 
            psIm->_typeName = IM_FETION;
    }
    else
    {
        if ("pre:0" == sType)
            psIm->_typeName = IM_ATM;
        else if("pre:1" == sType) 
            psIm->_typeName = IM_WINDOWSLIVE;
        else if("pre:2" == sType) 
            psIm->_typeName = IM_YAHOO;
        else if("pre:3" == sType) 
            psIm->_typeName = IM_SKYPE;
        else if("pre:4" == sType) 
            psIm->_typeName = IM_QQ;
        else if("pre:5" == sType) 
            psIm->_typeName = IM_GOOLETALK;
        else if("pre:6" == sType) 
            psIm->_typeName = IM_ICQ;
        else if("pre:7" == sType)
            psIm->_typeName = IM_JABBER;
    }
}

void GetContactCommand::ParseAndroidAddress(BinaryBufferReader* pReader, Address* psAddress)
{
    if (_bVersion20)
    {
        string sType = "";
        pReader->ReadString(sType);
        psAddress->_type = (AddressTypes) (sType[0] - '0');

        pReader->ReadStringFromUTF8(psAddress->_label);
        pReader->ReadStringFromUTF8(psAddress->_displayAddress);
        pReader->ReadStringFromUTF8(psAddress->_street);
        pReader->ReadStringFromUTF8(psAddress->_city);
        pReader->ReadStringFromUTF8(psAddress->_province);
        pReader->ReadStringFromUTF8(psAddress->_zipCode);
    }
    else
    {
        int nType = 0;
        pReader->ReadInt32(nType);
        psAddress->_type = (AddressTypes) nType;
        
        pReader->ReadStringFromUTF8(psAddress->_label);
        pReader->ReadStringFromUTF8(psAddress->_displayAddress);
    }
}

void GetContactCommand::ParseAndroidOrganizationTypes(BinaryBufferReader* pReader, Organization* psOrganization)
{
    if (_bVersion20)
    {
        string sType = "";
        pReader->ReadString(sType);
        psOrganization->_type = (OrganizationTypes) (sType[0] - '0');
    }
    else
    {
        int nType = 0;
        pReader->ReadInt32(nType);
        psOrganization->_type = (OrganizationTypes) nType;
    }
}

void GetContactCommand::ParseAndroidGroup(BinaryBufferReader* pReader, ContactGroupData* pContactGroup)
{
    pContactGroup->Read(pReader, _bVersion20 && !_bOMS);

    if (_bVersion20 && !_bOMS)
    {
		if(pContactGroup->GetAccountType().empty())
			pContactGroup->SetAccountType(DEFAULT_ACCOUNT_TYPE);
        // 添加进相应账户的联系人集合中
        for ( vector<ContactAccount*>::iterator it = _contactAcconutList.begin();
            it != _contactAcconutList.end(); ++it )
        {
            ContactAccount* pContactAccount = *it;
            if ((pContactAccount->GetName() == pContactGroup->GetAccountName()
				||pContactGroup->GetAccountName().empty()) 
				&&pContactAccount->GetType() == pContactGroup->GetAccountType())
            {
                pContactAccount->InitContactGroupData(pContactGroup);
            }
        }
    }
}

bool GetContactCommand::IosExecute()
{
    if (_pDevice->GetDevInfo() == NULL)
    {
        _result = COMMAND_EXECUTE_FAIL;
        return false;
    }
    
    if (!_pDevice->GetContactManager()->GetContactSynchronizer()->LoadAllData())
    {
        _result = COMMAND_EXECUTE_FAIL;
        return false;
    }
    _pDevice->GetContactManager()->GetContactSynchronizer()->AddIntoDefaultAccount(CreateDefaultAccount());

    return true;
}
/*
bool GetContactCommand::IosExecute()
{
    // 创建默认账户
    CreateDefaultAccount();

    if (_onlySize)
    {
        if (!ReloadContact())
            return false;
    }
    else
    {
        if (!ReloadImage() || !ReloadContact())
            return false;
    }

    // 一键排序
    //AutoSort();
    return true;
}

bool GetContactCommand::ReloadContact()
{
    bool result = false;
    // 没连接上设备则返回
    if (_pDevice->GetDevInfo() == NULL || !_pDevice->GetDevInfo()->_ios_bAFCConn)
        return result;

    IosDBPath dbPath(_pDevice,ISCONTACTDB);
    IosContactDB contactDB(dbPath);
    // 加载数据库
    if (!contactDB.Reload(_pDevice))
        return result;

    // 打开数据库
    if (!contactDB.OpenDB())
        return result;

    if (_onlySize)
    {
        // 获取联系人数量
        if (contactDB.GetAllContactSize(_contactAcconutList[0]))
            result = true;
        else
            result = false;
    }
    else
    {
        // 获取组信息
        // 获取组成员
        // 获取所有联系人
        if (contactDB.GetAllGroups(_contactAcconutList[0]) && 
            contactDB.GetGroupMember(_contactAcconutList[0]) && 
            contactDB.GetAllContact(_contactAcconutList[0]))
            result = true;
        else
            result = false;
    }

    // 关闭数据库
    contactDB.Close();

    return result;
}

bool GetContactCommand::ReloadImage()
{
    // 没连接上设备则返回
    if (_pDevice->GetDevInfo() == NULL || !_pDevice->GetDevInfo()->_ios_bAFCConn)
        return false;

    IosDBPath dbPath(_pDevice,ISCONTACTIMAGEDB);
    // 加载数据库
    IosContactImageDB contactImageDB(dbPath);
    return contactImageDB.Reload(_pDevice);
}
*/
ContactAccount* GetContactCommand::CreateDefaultAccount() 
{
    ContactAccount* pDefaultContactAccount = new ContactAccount(_pDevice);

    pDefaultContactAccount->SetName(DEFAULT_ACCOUNT_NAME);
    pDefaultContactAccount->SetType(DEFAULT_ACCOUNT_TYPE);

    _contactAcconutList.push_back(pDefaultContactAccount);
    return pDefaultContactAccount;
}


#endif