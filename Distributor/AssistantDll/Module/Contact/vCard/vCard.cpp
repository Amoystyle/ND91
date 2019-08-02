#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/vCard.h"
#include "Module/Contact/vCard/vCardEx.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Implementation/ItemCollection.h"
#include "Module/Contact/vCard/Implementation/DeliveryAddress.h"
#include "Module/Contact/vCard/Implementation/DeliveryAddressCollection.h"
#include "Module/Contact/vCard/Implementation/PhoneNumber.h"
#include "Module/Contact/vCard/Implementation/PhoneNumberCollection.h"
#include "Module/Contact/vCard/Implementation/EmailAddress.h"
#include "Module/Contact/vCard/Implementation/EmailAddressCollection.h"
#include "Module/Contact/vCard/Implementation/Url.h"
#include "Module/Contact/vCard/Implementation/UrlCollection.h"
#include "Module/Contact/vCard/Implementation/NickName.h"
#include "Module/Contact/vCard/Implementation/NickNameCollection.h"
#include "Module/Contact/vCard/Implementation/Note.h"
#include "Module/Contact/vCard/Implementation/NoteCollection.h"
#include "Module/Contact/vCard/Implementation/vName.h"
#include "Module/Contact/vCard/TypeDefine/ExplanatoryTypes.h"
#include "Module/Contact/vCard/TypeDefine/IdentificationTypes.h"
#include "Module/Contact/vCard/TypeDefine/GeographicalTypes.h"
#include "Module/Contact/vCard/TypeDefine/OrganizationalTypes.h"
#include "Module/Contact/vCard/Util.h"
#include "Common/Path.h"
#include "Device/CFConverter.h"

wstring vCard::BEGIN = L"BEGIN:VCARD";
wstring vCard::END = L"END:VCARD";
wstring vCard::Name_Param_Splitor = L";";
wstring vCard::Name_Value_Splitor = L":";
wstring vCard::BlankSpace = L" ";
wstring vCard::CUSTOM_TAG = L"X-";

vCard::vCard()
{
    _pItems             = new ItemCollection();
    _pAgent             = NULL;
    _pAddresses         = NULL;
    _pPhoneNumbers      = NULL;
    _pEmailAddresses    = NULL;
    _pUrls              = NULL;
    _pNickNames         = NULL;
    _pNotes             = NULL;
}

vCard::~vCard()
{
    UnInitvCard();
}

void vCard::InitvCard()
{
    _pAddresses         = new DeliveryAddressCollection(this);
    _pPhoneNumbers      = new PhoneNumberCollection(this);
    _pEmailAddresses    = new EmailAddressCollection(this);
    _pUrls              = new UrlCollection(this);
    _pNickNames         = new NickNameCollection(this);
    _pNotes             = new NoteCollection(this);

    OnInit();
}

void vCard::UnInitvCard()
{
    SAFE_DELETE(_pAddresses);
    SAFE_DELETE(_pPhoneNumbers);
    SAFE_DELETE(_pEmailAddresses);
    SAFE_DELETE(_pUrls);
    SAFE_DELETE(_pNickNames);
    SAFE_DELETE(_pNotes);

    SAFE_DELETE(_pItems);
}

void vCard::OnInit()
{
}

DeliveryAddressCollection* vCard::GetDeliveryAddressCollection() 
{
    if (_pAddresses == NULL)
        _pAddresses = new DeliveryAddressCollection(this);
    return _pAddresses; 
}

PhoneNumberCollection* vCard::GetPhoneNumberCollection() 
{
    if (_pPhoneNumbers == NULL)
        _pPhoneNumbers = new PhoneNumberCollection(this);
    return _pPhoneNumbers; 
}

EmailAddressCollection* vCard::GetEmailAddressCollection() 
{
    if (_pEmailAddresses == NULL)
        _pEmailAddresses = new EmailAddressCollection(this);
    return _pEmailAddresses; 
}

UrlCollection* vCard::GetUrlCollection() 
{
    if (_pUrls == NULL)
        _pUrls = new UrlCollection(this);
    return _pUrls;
}

NickNameCollection* vCard::GetNickNameCollection() 
{
    if (_pNickNames == NULL)
        _pNickNames = new NickNameCollection(this);
    return _pNickNames;
}

NoteCollection* vCard::GetNoteCollection() 
{
    if (_pNotes == NULL)
        _pNotes = new NoteCollection(this);
    return _pNotes;
}

ItemCollection* vCard::GetItemCollection() 
{ 
    return _pItems; 
}

vector<wstring> vCard::SplitLine(wstring line, wchar_t splitChar, size_t count)
{
    vector<wstring> lst;
    if (count <= 1)
    {
        lst.push_back(line);
        return lst;
    }

    size_t startIndex = 0;
    for (size_t i = 0; i < line.length(); i++)
    {
        wchar_t singleChar = line.at(i);
        if (singleChar == splitChar)
        {
            if (i != 0)
            {
                if (line.at(i-1) != L'\\')
                {
                    if (line.at(i-1) != splitChar || (line.at(i-1) == splitChar && i >= 2 && line.at(i-2) == L'\\'))
                        lst.push_back(line.substr(startIndex, i-startIndex));
                    else 
                        lst.push_back(L"");

                    startIndex = i + 1;
                }
            }
            else
            {
                lst.push_back(L"");
                startIndex = i + 1;
            }
        }

        if (lst.size() >= count - 1)
            break;
    }

    if (startIndex > line.length() - 1)
        lst.push_back(L"");
    else
        lst.push_back(line.substr(startIndex, line.length()-startIndex));

    return lst;
}

void vCard::ToFile(wstring file)
{
    string text = CCodeOperation::WstringToString(ToString());
    CFileOperation::WriteBinFile(file, text.c_str(), text.length());
}

wstring vCard::ToString()
{
    return ToString(L"");
}

// void vCard::ToFile(wstring file, wstring pKey)
// {
//     string text = CCodeOperation::WstringToString(ToString(pKey));
//     CFileOperation::WriteBinFile(file, text.c_str(), text.length());
// }

wstring vCard::ToString(wstring pKey)
{
    if (GetVersion().empty())
        SetVersion(L"3.0");
    
    if (GetUid().empty())
        SetUid(CStrOperation::ReplaceW(CFileOperation::GetGuidW(), L"-", L""));

    if (0 == GetLastModifyTime())
    {
        time_t nTime = 0;
        time(&nTime);
        SetLastModifyTime(nTime);
    }

    wstring retVal;
    retVal+=BEGIN+L"\r\n";
    vector<Item*>* pTemp = _pItems->GetItemVec();
    for (size_t i = 0; i < pTemp->size(); i++)
    {
        retVal+=pTemp->at(i)->ToString()+L"\r\n";
    }
    retVal+=END+L"\r\n";

    wstring vcardString;
    if (pKey.empty())
        vcardString = retVal;
    else
        vcardString = CCodeOperation::StringToWstring(
                        CStrOperation::EncryptDes(CCodeOperation::WstringToString(retVal), 
                        (unsigned char*)CCodeOperation::WstringToString(pKey).c_str()));
    
    return vcardString;
}

void vCard::FromFile(wstring file)
{
    wstring text = CCodeOperation::StringToWstring(CFileOperation::ReadFile(file, true));
    FromString(text);
}

void vCard::FromString(wstring source)
{
    Parse(source);
}

void vCard::Parse(wstring source)
{
    RETURN_IF(source.empty());
    _pItems->Clear();

    vector<wstring> text = CStrOperation::parseStringsW(source, L"\r\n");
    int i = 0;
    wstring line = text.at(i++);
    // Find row BEGIN:VCARD
    while (!line.empty() && CStrOperation::toUpperW(line) != vCard::BEGIN)
    {
        line = text.at(i++);
    }

    // Read frist vCard line after BEGIN:VCARD
    line = text.at(i++);
    while (!line.empty() && CStrOperation::toUpperW(line) != vCard::END)
    {
        wstring item;
        item+=line;
        
        // Get next line, see if item continues (folded line).
        line = text.at(i++);

        // fix WM's BUG!
        wstring temp = L"CATEGORY";
        if (CStrOperation::startsWith(CStrOperation::toUpperW(line), temp))
            line = ExplanatoryTypes::CATEGORIES+line.substr(temp.length());

        while (!line.empty() && !Util::StartLine(line))
        {
            if (line.find(L"NOTE;ENCODING=QUOTED-PRINTABLE:") == wstring::npos
                || CStrOperation::startsWith(line, L"ADR"))
                item+=L"\r\n"+line;
            else
                item+=line;

            line = text.at(i++);
        }

        vector<wstring> name_value = vCard::SplitLine(item, L':', 2);
        vector<wstring> name_params = vCard::SplitLine(name_value.at(0), L';', 2);

        wstring name = name_params.at(0);
        wstring parameters;
        if (name_params.size() == 2)
            parameters = name_params.at(1);

        wstring value;
        if (name_value.size() == 2)
            value = name_value.at(1);

        _pItems->Add(name, parameters, value);
    }

    InitvCard();
}

wstring vCard::GetUid()
{
    Item* pItem = _pItems->GetFirst(ExplanatoryTypes::UID);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetUid(wstring uid)
{
    _pItems->SetValue(ExplanatoryTypes::UID, uid);
}

wstring vCard::GetVersion()
{
    Item* pItem = _pItems->GetFirst(ExplanatoryTypes::VERSION);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetVersion(wstring version)
{
    _pItems->SetValue(ExplanatoryTypes::VERSION, version);
}

wstring vCard::GetCategory()
{
    Item* pItem = _pItems->GetFirst(ExplanatoryTypes::CATEGORIES);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetCategory(wstring category)
{
    _pItems->SetValue(ExplanatoryTypes::CATEGORIES, category);
}

time_t vCard::GetLastModifyTime()
{
    Item* pItem = _pItems->GetFirst(ExplanatoryTypes::REV);
    RETURN_VALUE_IF(pItem == NULL, 0);
    return Util::PareDateTime(pItem->GetValue());
}

void vCard::SetLastModifyTime(time_t lastModifyTime)
{
    if (lastModifyTime != 0)
    {
        CDealTime dTime(lastModifyTime);
        _pItems->SetValue(ExplanatoryTypes::REV, 
                            CCodeOperation::StringToWstring(dTime.TimeFormat()));
    }
    else
        _pItems->SetValue(ExplanatoryTypes::REV, L"");
}

wstring vCard::GetSort()
{
    Item* pItem = _pItems->GetFirst(ExplanatoryTypes::SORT_STRING);
    RETURN_VALUE_IF(pItem == NULL, 0);
    return pItem->GetValue();
}

void vCard::SetSort(wstring sort)
{
    _pItems->SetValue(ExplanatoryTypes::SORT_STRING, sort);
}

wstring vCard::GetSoundURI()
{
    vector<Item*> items = _pItems->Get(ExplanatoryTypes::SOUND);
    for (size_t i = 0; i < items.size(); i++)
    {
        Item* pTemp = items.at(i);
        if (CStrOperation::toUpperW(pTemp->GetParameters()).find(L"VALUE=uri") == wstring::npos)
            continue;

        return pTemp->GetValue();
    }

    return L"";
}

void vCard::SetSoundURI(wstring soundURI)
{
    vector<Item*> items = _pItems->Get(ExplanatoryTypes::SOUND);
    for (size_t i = 0; i < items.size(); i++)
    {
        Item* pTemp = items.at(i);
        if (CStrOperation::toUpperW(pTemp->GetParameters()).find(L"VALUE=URI") == wstring::npos)
            continue;

        if (!soundURI.empty())
            continue;

        _pItems->Remove(pTemp);
        i--;
    }

    if (!soundURI.empty())
        _pItems->SetValue(ExplanatoryTypes::SOUND, L"TYPE=BASIC;VALUE=uri", soundURI);
}

wstring vCard::GetProdID()
{
    Item* pItem = _pItems->GetFirst(ExplanatoryTypes::PRODID);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetProdID(wstring proid)
{
    _pItems->SetValue(ExplanatoryTypes::PRODID, proid);
}

vName* vCard::GetName()
{
    Item* pItem = _pItems->GetFirst(IdentificationTypes::N);
    RETURN_VALUE_IF(pItem == NULL, NULL);
    return vName::Parse(pItem->GetValue());
}

void vCard::SetName(vName* name)
{
    if (name != NULL)
        _pItems->SetValue(IdentificationTypes::N, name->ToValueString());
    else
        _pItems->SetValue(IdentificationTypes::N, L"");
}

wstring vCard::GetFormattedName()
{
    Item* pItem = _pItems->GetFirst(IdentificationTypes::FN);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetFormattedName(wstring formattedName)
{
    _pItems->SetValue(IdentificationTypes::FN, formattedName);
}

wstring vCard::GetImage()
{
    vector<Item*> items = _pItems->Get(IdentificationTypes::PHOTO);
    for (size_t i = 0; i < items.size(); i++)
    {
        Item* pTemp = items.at(i);
        if (CStrOperation::toUpperW(pTemp->GetParameters()).find(L"ENCODING=B") == wstring::npos)
            continue;

        map<string, string> imageMap;
        imageMap["image"] = CCodeOperation::WstringToString(pTemp->GetValue());
        void* pMsgDict = CCFConverter::DictionarytoCFDictionary(imageMap);

        //转换CFDic To CFPropertyList
        void* pMsgAsXML = CCFConverter::NDCFPropertyListCreateData(NULL, pMsgDict, kCFPropertyListBinaryFormat_v1_0, 0, NULL);
        RETURN_VALUE_IF(!pMsgAsXML, L"");
        int nXmlLen = CCFConverter::NDCFDataGetLength(pMsgAsXML);
        char* temp = (char*)CCFConverter::NDCFDataGetBytePtr(pMsgAsXML);

        wstring filePathStr = Path::GlobalTemp()+CFileOperation::GetGuidW()+L"image.xml";
        CFileOperation::WriteBinFile(filePathStr, temp, nXmlLen);
        IosPlistConverter::GetXmlFromPlist(filePathStr, filePathStr);

        string text = CFileOperation::ReadFile(filePathStr, true);
        text = CStrOperation::Replace(text, "string", "data");

        char* temptemp = (char*)text.c_str();
        void* pXmlData = CCFConverter::NDCFDataCreateWithBytesNoCopy(NULL, (unsigned char*)temptemp, (int)text.length(), CCFConverter::NDkCFAllocatorNull()); 
        void* pReplay = CCFConverter::NDCFPropertyListCreateFromXMLData(NULL, pXmlData, kCFPropertyListImmutable, NULL);

        map<string, void*> imageMap1;
        CCFConverter::GetIosData(pReplay, imageMap1);
        for (map<string,void*>::iterator iter = imageMap1.begin(); iter != imageMap1.end(); iter++)
        {
            if (CCFConverter::NDCFGetTypeID(iter->second) != CCFConverter::NDCFDataGetTypeID())
                continue;

            wstring photoPath = Path::GlobalTemp()+CFileOperation::GetGuidW()+L".png";
            int nLen = CCFConverter::NDCFDataGetLength(iter->second);
            void* pValue = CCFConverter::NDCFDataGetBytePtr(iter->second);
            unsigned char* uszValue = new unsigned char[nLen];
            memset(uszValue, 0, nLen);
            memcpy(uszValue, pValue, nLen);

            FILE* f = fopen(CCodeOperation::WstringToString(photoPath).c_str(), "w+b");
            if (f) 
            {
                fwrite(uszValue, nLen, 1, f);
                fflush(f);
                fclose(f);					
            }

            SAFE_DELETE_ARRAY(uszValue);
            return photoPath;
        }
    }

    return L"";
}

wstring vCard::BuildHeadPhoto(wstring imageFilePath)
{
    string srcFilename = CCodeOperation::WstringToString(imageFilePath);
    FILE *pSrcFile = NULL;
    struct stat *filestats = (struct stat *) malloc(sizeof(struct stat));
    pSrcFile = fopen(srcFilename.c_str(), "rb");
    if (pSrcFile == NULL)
        return NULL;

    stat(srcFilename.c_str(), filestats);
    unsigned char* szSrc = (unsigned char *) malloc(sizeof(unsigned char) * (filestats->st_size + 1));
    int read_size = fread(szSrc, sizeof(unsigned char), filestats->st_size, pSrcFile);
    fclose(pSrcFile);
    void* data = CCFConverter::NDCFDataCreate(NULL, szSrc, read_size);

    map<string, void*> imageMap;
    imageMap["image"] = data;

    void* pMsgDict = CCFConverter::DictionarytoCFDictionary(imageMap);

    //转换CFDic To CFPropertyList
    void* pMsgAsXML = CCFConverter::NDCFPropertyListCreateData(NULL, pMsgDict, kCFPropertyListBinaryFormat_v1_0, 0, NULL);
    RETURN_VALUE_IF(!pMsgAsXML, L"");
    int nXmlLen = CCFConverter::NDCFDataGetLength(pMsgAsXML);
    char* temp = (char*)CCFConverter::NDCFDataGetBytePtr(pMsgAsXML);

    wstring filePathStr = Path::GlobalTemp()+CFileOperation::GetGuidW()+L"image.xml";
    CFileOperation::WriteBinFile(filePathStr, temp, nXmlLen);  //写入.plist文件
    IosPlistConverter::GetXmlFromPlist(filePathStr, filePathStr);

    file<> fdoc( CCodeOperation::UnicodeToUTF_8(filePathStr).c_str() );
    xml_document<char> doc;
    doc.parse<0>(fdoc.data());
    RETURN_FALSE_IF(fdoc.data() == NULL);

    rapidxml::xml_node<char> * root = doc.first_node();            //xml文件的根节点
    rapidxml::xml_node<char> * node = root->first_node();
    rapidxml::xml_node<char> * lastnode = node->last_node();
    string value = lastnode->value();
    value = CStrOperation::Replace(value, "\n\t", "");
    CCFConverter::NDCFRelease(pMsgAsXML);
    free(filestats);
    free(szSrc);
    return CCodeOperation::StringToWstring(value);
}

void vCard::SetImage(wstring filePath)
{
    wstring text = BuildHeadPhoto(filePath);

    vector<Item*> items = _pItems->Get(IdentificationTypes::PHOTO);
    for (size_t i = 0; i < items.size(); i++)
    {
        Item* pTemp = items.at(i);
        if (CStrOperation::toUpperW(pTemp->GetParameters()).find(L"ENCODING=B") == wstring::npos)
            continue;

        if (!text.empty())
            continue;

        _pItems->Remove(pTemp);
        i--;
    }

    if (!text.empty())
        _pItems->SetValue(IdentificationTypes::PHOTO, L"ENCODING=b;TYPE=JPEG", text);
}

wstring vCard::GetPhotoURI()
{
    vector<Item*> items = _pItems->Get(IdentificationTypes::PHOTO);
    for (size_t i = 0; i < items.size(); i++)
    {
        Item* pTemp = items.at(i);
        if (CStrOperation::toUpperW(pTemp->GetParameters()).find(L"VALUE=URI") == wstring::npos)
            continue;

        return pTemp->GetValue();
    }

    return L"";
}

void vCard::SetPhotoURI(wstring photoURI)
{
    vector<Item*> items = _pItems->Get(IdentificationTypes::PHOTO);
    for (size_t i = 0; i < items.size(); i++)
    {
        Item* pTemp = items.at(i);
        if (CStrOperation::toUpperW(pTemp->GetParameters()).find(L"VALUE=URI") == wstring::npos)
            continue;

        if (!photoURI.empty())
            continue;

        _pItems->Remove(pTemp);
        i--;
    }

    if (!photoURI.empty())
        _pItems->SetValue(IdentificationTypes::PHOTO, L"VALUE=uri", photoURI);
}

time_t vCard::GetBirthdate()
{
    Item* pItem = _pItems->GetFirst(IdentificationTypes::BDAY);
    RETURN_VALUE_IF(pItem == NULL, 0);
    return Util::PareDateTime(pItem->GetValue());
}

void vCard::SetBirthdate(time_t birthdate)
{
    if (birthdate != 0)
    {
        CDealTime dTime(birthdate);
        wstring value = CCodeOperation::StringToWstring(dTime.TimeFormat());
        vector<wstring> temp = CStrOperation::parseStringsW(value, L" ");
        value = temp.at(0);
        value = CStrOperation::ReplaceW(value, L"-", L"");
        _pItems->SetValue(IdentificationTypes::BDAY, value);
    }
    else
        _pItems->Remove(IdentificationTypes::BDAY);
}

wstring vCard::GetCustomRingtone()
{
    Item* pItem = _pItems->GetFirst(CUSTOM_TAG+L"RINGTONE");
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetCustomRingtone(wstring customRingtone)
{
    _pItems->SetValue(CUSTOM_TAG+L"RINGTONE", customRingtone);
}

wstring vCard::GetAssistantName()
{
    Item* pItem = _pItems->GetFirst(IdentificationTypes::ASSISTANTNAME);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetAssistantName(wstring assistantName)
{
    _pItems->SetValue(IdentificationTypes::ASSISTANTNAME, assistantName);
}

wstring vCard::GetAssistantTel()
{
    Item* pItem = _pItems->GetFirst(IdentificationTypes::ASSISTANTTEL);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetAssistantTel(wstring assistantTel)
{
    _pItems->SetValue(IdentificationTypes::ASSISTANTTEL, assistantTel);
}

wstring vCard::GetChildren()
{
    Item* pItem = _pItems->GetFirst(IdentificationTypes::CHILDREN);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetChildren(wstring children)
{
    _pItems->SetValue(IdentificationTypes::CHILDREN, children);
}

wstring vCard::GetSpouse()
{
    Item* pItem = _pItems->GetFirst(IdentificationTypes::SPOUSE);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetSpouse(wstring spouse)
{
    _pItems->SetValue(IdentificationTypes::SPOUSE, spouse);
}

wstring vCard::GetSuffix()
{
    Item* pItem = _pItems->GetFirst(IdentificationTypes::SUFFIX);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetSuffix(wstring suffix)
{
    _pItems->SetValue(IdentificationTypes::SUFFIX, suffix);
}

wstring vCard::GetManager()
{
    Item* pItem = _pItems->GetFirst(IdentificationTypes::MANAGER);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetManager(wstring manager)
{
    _pItems->SetValue(IdentificationTypes::MANAGER, manager);
}

wstring vCard::GetStatusId()
{
    Item* pItem = _pItems->GetFirst(IdentificationTypes::STATUSID);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetStatusId(wstring statusId)
{
    _pItems->SetValue(IdentificationTypes::STATUSID, statusId);
}

wstring vCard::GetAccount()
{
    Item* pItem = _pItems->GetFirst(IdentificationTypes::ACCOUNT);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetAccount(wstring account)
{
    _pItems->SetValue(IdentificationTypes::ACCOUNT, account);
}

wstring vCard::GetCustomerId()
{
    Item* pItem = _pItems->GetFirst(IdentificationTypes::CUSTOMERID);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetCustomerId(wstring customerId)
{
    _pItems->SetValue(IdentificationTypes::CUSTOMERID, customerId);
}

wstring vCard::GetTimezone()
{
    Item* pItem = _pItems->GetFirst(GeographicalTypes::TZ);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetTimezone(wstring timezone)
{
    if (timezone.empty())
        _pItems->Remove(GeographicalTypes::TZ);
    else
        _pItems->SetValue(GeographicalTypes::TZ, timezone);
}

wstring vCard::GetGeo()
{
    Item* pItem = _pItems->GetFirst(GeographicalTypes::GEO);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetGeo(wstring geo)
{
    if (geo.empty())
        _pItems->Remove(GeographicalTypes::GEO);
    else
        _pItems->SetValue(GeographicalTypes::GEO, geo);
}

wstring vCard::GetTitle()
{
    Item* pItem = _pItems->GetFirst(OrganizationalTypes::TITLE);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetTitle(wstring title)
{
    if (title.empty())
        _pItems->Remove(OrganizationalTypes::TITLE);
    else
        _pItems->SetValue(OrganizationalTypes::TITLE, title);
}

wstring vCard::GetRole()
{
    Item* pItem = _pItems->GetFirst(OrganizationalTypes::ROLE);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetRole(wstring role)
{
    if (role.empty())
        _pItems->Remove(OrganizationalTypes::ROLE);
    else
        _pItems->SetValue(OrganizationalTypes::ROLE, role);
}

wstring vCard::GetOrganization()
{
    Item* pItem = _pItems->GetFirst(OrganizationalTypes::ORG);
    RETURN_VALUE_IF(pItem == NULL, L"");
    return pItem->GetValue();
}

void vCard::SetOrganization(wstring organization)
{
    if (organization.empty())
        _pItems->Remove(OrganizationalTypes::ORG);
    else
        _pItems->SetValue(OrganizationalTypes::ORG, organization);
}

wstring vCard::GetLogoURI()
{
    vector<Item*> items = _pItems->Get(OrganizationalTypes::LOGO);
    for (size_t i = 0; i < items.size(); i++)
    {
        Item* pTemp = items.at(i);
        if (CStrOperation::toUpperW(pTemp->GetParameters()).find(L"VALUE=URI") == wstring::npos)
            continue;

        return pTemp->GetValue();
    }

    return L"";
}

void vCard::SetLogoURI(wstring logoURI)
{
    vector<Item*> items = _pItems->Get(OrganizationalTypes::LOGO);
    for (size_t i = 0; i < items.size(); i++)
    {
        Item* pTemp = items.at(i);
        if (CStrOperation::toUpperW(pTemp->GetParameters()).find(L"VALUE=URI") == wstring::npos)
            continue;

        if (!logoURI.empty())
            continue;

        _pItems->Remove(pTemp);
        i--;
    }

    if (!logoURI.empty())
        _pItems->SetValue(OrganizationalTypes::LOGO, L"VALUE=uri", logoURI);
}

//#endif