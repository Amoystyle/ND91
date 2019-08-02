#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/ContactAccount.h"
#include "Module/Contact/AddContactGroupCommand.h"
#include "Module/Contact/DeleteContactGroupCommand.h"
#include "Module/Contact/UpdateContactGroupCommand.h"
#include "Module/Contact/ContactAndGroupCommand.h"
#include "Module/Contact/SetFavoriteCommand.h"
#include "Module/Contact/DeleteContactCommand.h"
#include "Module/Contact/UpdateContactCommand.h"
#include "Module/Contact/GetContactPhotoCommand.h"

#include "Module/Ring/LoadRingsCommand.h"
#include "Common/Path.h"

#define  RINGTONESPATH     L"/Library/Ringtones"    // 系统铃声文件路径

ContactAccount::ContactAccount(const DeviceData* pDevice)
{
    _pDevice     = pDevice;
    _bVersion20  = (atof(CCodeOperation::WstringToString(_pDevice->GetDevInfo()->_deviceProductVersion).c_str()) >= 2.0);
    _bOMS        = _pDevice->GetDevInfo()->_platform == Platform_OMS;
    _size        = 0;

    if (_pDevice->GetDevInfo()->_deviceType == Android)
    {
        IniAndroidSystemRingtoneMap();
        IniAndroidCustomRingtoneMap();
        IniAndroidTextTone();
    }
    else
    {
        int iProductVersion = IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion);
        if (_pDevice->GetDevInfo()->IsPod() && iProductVersion <= 421)
            return;

        IniIosSystemRingtoneMap();
        IniIosCustomRingtoneMap();
        IniIosTextTone();
    }
}

ContactAccount::~ContactAccount()
{
	LOG->WriteDebug(L"~ContactAccount()");
    for( vector<ContactData*>::iterator it = _contactDataList.begin(); it != _contactDataList.end(); it++)
    {
        SAFE_DELETE(*it);
    }
    _contactDataList.clear();
   /* for( vector<ContactGroupData*>::iterator it = _contactGroupList.begin(); it != _contactGroupList.end(); it++)
    {
        SAFE_DELETE(*it);
    }*/
    _contactGroupList.clear();
	LOG->WriteDebug(L"~ContactAccount() Finished!");
}

void ContactAccount::RemoveGroupFromList(ContactGroupData* pGroup)
{
    for ( vector<ContactGroupData*>::iterator it = _contactGroupList.begin(); it != _contactGroupList.end(); it++ )
    {
        if ( (*it) != pGroup )
            continue;

        _contactGroupList.erase(it);
        SAFE_DELETE(pGroup);
        break;
    }
}

void ContactAccount::RemoveContactFromList(ContactData* pContact)
{
    for ( vector<ContactData*>::iterator it = _contactDataList.begin(); it != _contactDataList.end(); it++ )
    {
        if ( (*it) != pContact )
            continue;

        _contactDataList.erase(it);
        SAFE_DELETE(pContact);
        break;
    }
}

bool ContactAccount::DeleteContactGroup(ContactGroupData* pGroupData)
{
    DeleteContactGroupCommand cmd(_pDevice, pGroupData);
    cmd.Execute();
    RETURN_FALSE_IF ( !cmd.Success() );

	return true;
}

bool ContactAccount::UpdateContactGroup( ContactGroupData* pGroupData )
{
    UpdateContactGroupCommand cmd(_pDevice, pGroupData, _bVersion20, _bOMS);
    cmd.Execute();
    return cmd.Success();
}

bool ContactAccount::AddContactGroup( /*ContactGroupData* pGroupData*/const vector<ContactGroupData*>* pDataList )
{
    AddContactGroupCommand cmd(_pDevice, /*pGroupData*/pDataList, _bVersion20, _bOMS);
    cmd.Execute();
    bool bSuccess = cmd.Success();

    if (true == bSuccess)
        for (size_t i = 0; i < pDataList->size(); i++)
            _contactGroupList.push_back(/*pGroupData*/pDataList->at(i));

    return bSuccess;
}

bool ContactAccount::AddContactToGroup(const vector<ContactData*>* pDataList, ContactGroupData* pGroupData,ProgressObserver* pObserver)
{
    ContactAndGroupCommand cmd(_pDevice);
    cmd.Attach(pObserver);
    int executeCount = cmd.ExecuteAddContactToGroup(pDataList, pGroupData);
    bool bSuccess = cmd.Success();

    if (bSuccess != true)
        return false;

    for (int i = 0; i < executeCount; i++)
    {
        ContactData *pContactData = pDataList->at(i);
        AddContactToGroup(pContactData->_contactId, pGroupData->GetId());
    }

    return true;
}

bool ContactAccount::RemoveContactFromGroup(const vector<ContactData*>* pDataList, ContactGroupData* pGroupData,ProgressObserver* pObserver)
{
    ContactAndGroupCommand cmd(_pDevice);
    cmd.Attach(pObserver);
    int executeCount = cmd.ExecuteRemoveContactFromGroup(pDataList, pGroupData);
    bool bSuccess = cmd.Success();

    if (bSuccess != true)
        return false;

    for (int i = 0; i < executeCount; i++)
    {
        ContactData *pContactData = pDataList->at(i);
        RemoveContactFromGroup(pContactData->_contactId, pGroupData->GetId());
    }

    return true;
}

bool ContactAccount::SetFavorite( const vector<ContactData*>* pDataList,bool bFavorite,ProgressObserver* pObserver)
{
    SetFavoriteCommand cmd(_pDevice, bFavorite);
    cmd.Attach(pObserver);
    cmd.SetDataList(pDataList);
    int executeCount = cmd.ExecuteSetFavorite();
    bool bSuccess = cmd.Success();

    if (true == bSuccess)
    {     
//         for ( vector<ContactData*>::const_iterator it = pDataList->begin(); it != pDataList->end(); ++it)
//         {
//             ContactData* pContactData = *it;
//             pContactData->_favorite = bFavorite;
//         }

        for (int i = 0; i < executeCount; i++)
        {
            ContactData* pContactData = pDataList->at(i);
            pContactData->_favorite = bFavorite;
        }
    }
    return bSuccess;
}

bool ContactAccount::DeleteContact( const vector<ContactData*>* pDataList,ProgressObserver* pObserver)
{
    vector<ContactData*>::const_iterator iter;
    for (iter = pDataList->begin(); iter != pDataList->end(); ++iter)
    {
		ContactData* pdata = (*iter);
        pdata->SetStatus(BaseData::DEL);
    }

    DeleteContactCommand cmd(_pDevice);
    cmd.Attach(pObserver);
    cmd.SetDataList(pDataList);
    cmd.Execute();
    bool bSuccess = cmd.Success();

    if (true == bSuccess)
    {
        vector<ContactData*>::iterator it = _contactDataList.begin();
        while ( it != _contactDataList.end() )
        {
            ContactData* pContact = *it;
			if ( pContact->GetStatus() == BaseData::DEL )
			{
				it = _contactDataList.erase(it);
				SAFE_DELETE(pContact);
			}
			else
	            ++it;
        }
    }
    return bSuccess;
}

bool ContactAccount::DeleteAll( ProgressObserver* pObserver)
{
    vector<ContactData*> DataList;
    DataList.insert(DataList.end(), _contactDataList.begin(), _contactDataList.end());
    return DeleteContact(&DataList, pObserver);
}

bool ContactAccount::DeleteAllGroup( ProgressObserver* pObserver)
{
    for (vector<ContactGroupData*>::iterator it = _contactGroupList.begin(); it != _contactGroupList.end();)
	{
        if (DeleteContactGroup((*it)))
			it = _contactGroupList.erase(it);
		else
			it++;
	}

    return true;
}

bool ContactAccount::UpdateContact( ContactData* pContact )
{
    UpdateContactCommand cmd(_pDevice, pContact, _bVersion20, _bOMS);
    cmd.Execute();  
    return cmd.Success();
}

bool ND91Assistant::ContactAccount::LoadContactPhoto( ContactData* pcontact )
{
	pcontact->_photo_hasLoad = true;
    bool isoms = (_pDevice->GetDevInfo()->_platform == Platform_OMS);
	if(isoms)
	{
		RETURN_FALSE_IF(pcontact->_headPhoto._photoPath.empty());

		wstring localpath = _pDevice->GetPath()->GetLocalPimPhotoPath();
		localpath += CCodeOperation::GB2312ToUnicode(CFileOperation::GetGuid())+L".jpg";

        if( DownloadFileCommand(_pDevice).Download(localpath, pcontact->_headPhoto._photoPath) )
		{
			pcontact->_headPhoto._strSavePath = localpath;
			return true;
		}
		return false;
	}
	else
	{
		GetContactPhotoCommand getphotoCmd(_pDevice,pcontact->_contactId);
		getphotoCmd.Execute();
		pcontact->_headPhoto._strSavePath = getphotoCmd.GetPhotoPath();
		return !pcontact->_headPhoto._strSavePath.empty();
	}
}

ContactGroupData* ND91Assistant::ContactAccount::FindGroupByName( wstring groupName )
{
	for(size_t i = 0; i < _contactGroupList.size(); ++i)
	{
		if ( _contactGroupList[i]->GetName() == groupName)
			return _contactGroupList[i];
	}
	return NULL;
}

void ContactAccount::IniIosSystemRingtoneMap()
{
    map<wstring, STRING_ID> ios_systemRingtone;
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"",                    system_Null));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Boing",        system_Boing));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Trill",        system_Trill));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Pinball",      system_Pinball));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Piano Riff",   system_PianoRiff));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Bark",         system_Bark));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Robot",        system_Robot));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Strum",        system_Strum));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Ascending",    system_Ascending));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Alarm",        system_Alarm));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Sci-Fi",       system_SciFi));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Blues",        system_Blues));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Old Phone",    system_OldPhone));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Old Car Horn", system_OldCarHorn));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"<default>",           system_default));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Doorbell",     system_Doorbell));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Motorcycle",   system_Motorcycle));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Xylophone",    system_Xylophone));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Sonar",        system_Sonar));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Time Passing", system_TimePassing));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Harp",         system_Harp));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Digital",      system_Digital));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Timba",        system_Timba));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Crickets",     system_Crickets));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Duck",         system_Duck));
    ios_systemRingtone.insert(pair<wstring, STRING_ID>(L"system:Bell Tower",   system_BellTower));

    IIosFileSystem* pFileStream = NULL;
    if ( _pDevice->GetDevInfo()->_connectType == WIFI)
        pFileStream =  new IosWiFiFileSystem(_pDevice->GetDevInfo()->_ios_wifi_handle);
    else if (_pDevice->GetDevInfo()->_connectType == USB)
        pFileStream =  new AFCFileSysemBase(_pDevice->GetDevInfo());

    DirData pxlDir(_pDevice);
    pxlDir._name = RINGTONESPATH;
    // 获取该目录下的所有文件
    pFileStream->Ios_GetFileInfos(&pxlDir, eFileGetType_Files);
    deque<NodeData*> childs = pxlDir.GetChildrens();
    for(size_t i = 0; i < childs.size(); i++)
    {
        NodeData* pNode = childs.at(i);
        wstring fileName = pNode->_name;
        if (!CStrOperation::endsWithW(fileName, L".m4r") && !CStrOperation::endsWithW(fileName, L".m4a") && !CStrOperation::endsWithW(fileName, L".mp3"))
            continue;

        fileName.erase(fileName.length()-4, 4);
        wstring guid = L"system:" + fileName;

        map<wstring, STRING_ID>::const_iterator iter = ios_systemRingtone.find(guid);
        if (iter != ios_systemRingtone.end())
            _ios_systemRingtone.insert(pair<wstring, STRING_ID>(iter->first,iter->second));
    }

    SAFE_DELETE(pFileStream);
}

void ContactAccount::IniAndroidSystemRingtoneMap()
{
#ifdef MODULE_RING
    LoadRingsCommand cmd(_pDevice);
    cmd.SetAudioType(SYSTEM_RING);
    cmd.Execute();
    const deque<RingData*>* pList = cmd.GetDataList();
    for (deque<RingData*>::const_iterator it = pList->begin(); it != pList->end(); it++)
    {
        RingData* pData = *it;
        if (pData->_ringType != RingData::Ring && (pData->_ringType&RingData::Ring) != RingData::Ring)
            continue;

        char szId[20] = {0};
        sprintf(szId,"%d",pData->_Id);
        string strId = szId;
        _android_systemRingtone.insert(pair<wstring, wstring>(CCodeOperation::StringToWstring(strId), pData->_title));
    }
#endif
}

void ContactAccount::IniIosCustomRingtoneMap()
{
#ifdef MODULE
    LoadRingsCommand cmd(_pDevice);
    cmd.SetAudioType(CUSTOM_RING);
    cmd.Execute();
    const deque<RingData*>* customDeq = cmd.GetDataList();
    for (deque<RingData*>::const_iterator it = customDeq->begin(); it != customDeq->end(); it++)
    {
        RingData* pData = *it;
        wstring temp = L"itunes:" + CCodeOperation::UTF_8ToUnicode(pData->_Guid);
        _ios_customRingtone.insert(pair<wstring, wstring>(temp, pData->_title));
    }
#endif
}

void ContactAccount::IniAndroidCustomRingtoneMap()
{
#ifdef MODULE_RING
    LoadRingsCommand cmd(_pDevice);
    cmd.SetAudioType(CUSTOM_RING);
    cmd.Execute();
    const deque<RingData*>* pList = cmd.GetDataList();
    for (deque<RingData*>::const_iterator it = pList->begin(); it != pList->end(); it++)
    {
        RingData* pData = *it;
        if (pData->_ringType != RingData::Ring && (pData->_ringType&RingData::Ring) != RingData::Ring)
            continue;

        char szId[20] = {0};
        sprintf(szId,"%d",pData->_Id);
        string strId = szId;
        _android_customRingtone.insert(pair<wstring, wstring>(CCodeOperation::StringToWstring(strId), pData->_title));
    }
#endif
}

void ContactAccount::IniIosTextTone()
{
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"<none>",                      texttone_none));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Tri-tone",           texttone_TriTone));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Chime",              texttone_Chime));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Glass",              texttone_Glass));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Horn",               texttone_Horn));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Bell",               texttone_Bell));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Electronic",         texttone_Electronic));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Anticipate",         texttone_Anticipate));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Bloom",              texttone_Bloom));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Calypso",            texttone_Calypso));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Choo Choo",          texttone_ChooChoo));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Descent",            texttone_Descent));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Fanfare",            texttone_Fanfare));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Ladder",             texttone_Ladder));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Minuet",             texttone_Minuet));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:News Flash",         texttone_NewsFlash));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Noir",               texttone_Noir));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Sherwood Forest",    texttone_SherwoodForest));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Spell",              texttone_Spell));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Suspense",           texttone_Suspense));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Telegraph",          texttone_Telegraph));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Tiptoes",            texttone_Tiptoes));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Typewriters",        texttone_Typewriters));
    _ios_textTone.insert(pair<wstring, STRING_ID>(L"texttone:Update",             texttone_Update));
}

void ContactAccount::IniAndroidTextTone()
{
}



#endif