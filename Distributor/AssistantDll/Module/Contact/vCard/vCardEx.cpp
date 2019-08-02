#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/vCardEx.h"
#include "Module/Contact/vCard/Extention/ContactRelated.h"
#include "Module/Contact/vCard/Extention/ContactRelatedCollection.h"
#include "Module/Contact/vCard/Extention/IMAddressCollection.h"
#include "Module/Contact/vCard/Extention/AnniversaryCollection.h"
#include "Module/Contact/vCard/Extention/WebPageCollection.h"
#include "Module/Contact/vCard/Extention/OrganizationExCollection.h"
#include "Module/Contact/vCard/Implementation/Date.h"
#include "Module/Contact/vCard/Implementation/DateCollection.h"

#include "Module/Contact/ContactData.h"
#include "Module/Contact/vCard/Implementation/vName.h"
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
#include "Module/Contact/vCard/Extention/IMAddress.h"
#include "Module/Contact/vCard/Extention/IMAddressCollection.h"

vCardEx::vCardEx()
{
    OnInit();
}

vCardEx::~vCardEx()
{
    UnInit();
}

void vCardEx::OnInit()
{
    vCard::OnInit();

    _pContactRelateds = new ContactRelatedCollection(this);
    _pIMAddresses     = new IMAddressCollection(this);
    _pAnniversaries   = new AnniversaryCollection(this);
    _pWebPages        = new WebPageCollection(this);
    _pOrgCollection   = new OrganizationExCollection(this);
    _pDates           = new DateCollection(this);
}

void vCardEx::UnInit()
{
    SAFE_DELETE(_pContactRelateds);
    SAFE_DELETE(_pIMAddresses);
    SAFE_DELETE(_pAnniversaries);
    SAFE_DELETE(_pWebPages);
    SAFE_DELETE(_pOrgCollection);
    SAFE_DELETE(_pDates);
}

bool vCardEx::ExportToVCards(const ContactData* pContact, vector<wstring> groupNames, wstring exportFolder)
{
    vCardEx* exportCard = new vCardEx();
    vName* contactName = new vName(pContact->_sName._givenName, 
                                   pContact->_sName._familyName, 
                                   pContact->_sName._middleName, 
                                   CCodeOperation::StringToWstring(pContact->_sName._ios_prefix), 
                                   pContact->_sName._ios_suffix);
    exportCard->SetName(contactName);
    exportCard->SetFormattedName(pContact->_sName._displayName);
    if (!pContact->_sName._ios_nickName.empty())
    {
        vector<wstring> nickNames = CStrOperation::parseStringsW(pContact->_sName._ios_nickName, L",");
        for (size_t i = 0; i < nickNames.size(); i++)
            exportCard->GetNickNameCollection()->Add(nickNames.at(i));
    }

    if (!pContact->_companyName.empty())
        exportCard->SetOrganization(pContact->_companyName);
    if (!pContact->_department.empty())
    {
        wstring org;
        if (exportCard->GetOrganization().empty())
            org = L"";
        else
            org+=exportCard->GetOrganization()+L";"+pContact->_department;

        exportCard->SetOrganization(org);
    }
    if (!pContact->_jobTitle.empty())
        exportCard->SetTitle(pContact->_jobTitle);
    if (!pContact->_sComment._content.empty())
    {
        vector<wstring> notes = CStrOperation::parseStringsW(pContact->_sComment._content, L"\r\n\r\n");
        for (size_t i = 0; i < notes.size(); i++)
            exportCard->GetNoteCollection()->Add(notes.at(i));
    }

    if (!pContact->_birthday.empty())
        exportCard->SetBirthdate(_wtoi(pContact->_birthday.c_str()));

    if (!groupNames.empty())
    {
        for (size_t i = 0; i < groupNames.size(); i++)
            exportCard->SetCategory(groupNames.at(i));
    }

    if (!pContact->_headPhoto._strSavePath/*_thumbnailPath*/.empty())
        exportCard->SetImage(pContact->_headPhoto._strSavePath/*_thumbnailPath*/);

    //三个通用类型：工作(Internet)、家庭(Preferred)、其他(NotSpecified)，若这三种类型已经有了，多余的则填到Custom中
    bool homeFilled = false;
    bool workFilled = false;
    bool otherFilled = false;

#pragma region 电话号码

    for (size_t i = 0; i < pContact->_phoneNumberList.size(); i++)
    {
        Phone* pn = pContact->_phoneNumberList.at(i);
        switch (pn->_type)
        {
        case PHONE_HOME:
            exportCard->GetPhoneNumberCollection()->Add(PhoneNumber_Home, pn->_number);
            break;
        case PHONE_WORK:
            exportCard->GetPhoneNumberCollection()->Add(PhoneNumber_Work, pn->_number);
            break;
        case PHONE_MOBILE:
            exportCard->GetPhoneNumberCollection()->Add(PhoneNumber_Cellular, pn->_number);
            break;
        case PHONE_MAIN:
            exportCard->GetPhoneNumberCollection()->Add(PhoneNumber_Preferred, pn->_number);
            break;
        case PHONE_PAGER:
            exportCard->GetPhoneNumberCollection()->Add(PhoneNumber_Pager, pn->_number);
            break;
        case PHONE_HOMEFAX:
            exportCard->GetPhoneNumberCollection()->Add(PhoneNumberType_enum(PhoneNumber_Fax | PhoneNumber_Home), pn->_number);
            break;
        case PHONE_WORKFAX:
            exportCard->GetPhoneNumberCollection()->Add(PhoneNumberType_enum(PhoneNumber_Fax | PhoneNumber_Work), pn->_number);
            break;
        case PHONE_OTHER:
            exportCard->GetPhoneNumberCollection()->Add(PhoneNumber_Other, pn->_number);
            break;
        default:
            exportCard->GetPhoneNumberCollection()->Add(PhoneNumber_Custom, pn->_label, pn->_number);
            break;
        }
    }

#pragma endregion
#pragma region 电子邮件

    for (size_t i = 0; i < pContact->_emailList.size(); i++)
    {
        Email* email = pContact->_emailList.at(i);
        switch (email->_type)
        {
        case EMAIL_HOME:
            if (homeFilled)
                exportCard->GetEmailAddressCollection()->Add(EmailAddress_Custom, EmailAddress::Home, email->_emailAccount);
            else
            {
                exportCard->GetEmailAddressCollection()->Add(EmailAddress_Preferred, EmailAddress::Home, email->_emailAccount);
                homeFilled = true;
            }
            break;
        case EMAIL_WORK:
            if (workFilled)
                exportCard->GetEmailAddressCollection()->Add(EmailAddress_Custom, EmailAddress::Work, email->_emailAccount);
            else
            {
                exportCard->GetEmailAddressCollection()->Add(EmailAddress_Internet, EmailAddress::Work, email->_emailAccount);
                workFilled = true;
            }
            break;
        case EMAIL_OTHER:
            if (otherFilled)
                exportCard->GetEmailAddressCollection()->Add(EmailAddress_Custom, EmailAddress::Other, email->_emailAccount);
            else
            {
                exportCard->GetEmailAddressCollection()->Add(EmailAddress_NotSpecified, EmailAddress::Other, email->_emailAccount);
                otherFilled = true;
            }
            break;
        default:
            exportCard->GetEmailAddressCollection()->Add(EmailAddress_Custom, email->_label, email->_emailAccount);
            break;
        }
    }

#pragma endregion
#pragma region 网址

    for (size_t i = 0; i < pContact->_homePageList.size(); i++)
    {
        HomePage* page = pContact->_homePageList.at(i);
        switch (page->_type)
        {
        case HOMEPAGE_HOMEPAGE:
            exportCard->GetUrlCollection()->Add(Url_Preferred, page->_name);
            break;
        case HOMEPAGE_HOME:
            exportCard->GetUrlCollection()->Add(Url_Home, page->_name);
            break;
        case HOMEPAGE_WORK:
            exportCard->GetUrlCollection()->Add(Url_Work, page->_name);
            break;
        case HOMEPAGE_OTHER:
            exportCard->GetUrlCollection()->Add(Url_Other, page->_name);
            break;
        default:
            exportCard->GetUrlCollection()->Add(Url_Custom, page->_label, page->_name);
            break;
        }
    }

#pragma endregion
#pragma region IM

    for (size_t i = 0; i < pContact->_imList.size(); i++)
    {
        Im* pIm = pContact->_imList.at(i);
        wstring label = IMAddress::IMLabelConvert(pIm->_typeName);
        if (label == L"Custom")
            label = pIm->_label;

        switch (pIm->_type)
        {
        case IM_HOME:
            exportCard->GetIMAddressConllection()->Add(label, pIm->_imAccount, IMType_HOME, L"");
            break;
        case IM_WORK:
            exportCard->GetIMAddressConllection()->Add(label, pIm->_imAccount, IMType_WORK, L"");
            break;
        case IM_OTHER:
            exportCard->GetIMAddressConllection()->Add(label, pIm->_imAccount, IMType_OTHER, L"");
            break;
        default:
            exportCard->GetIMAddressConllection()->Add(label, pIm->_imAccount, IMType_CUSTOM, label);
            break;
        }
    }

#pragma endregion
#pragma region 地址

    for (size_t i = 0; i < pContact->_addressList.size(); i++)
    {
        Address* pAddress = pContact->_addressList.at(i);
        switch (pAddress->_type)
        {
        case ADDRESS_HOME:
            exportCard->GetDeliveryAddressCollection()->Add(DeliveryAddress_Home, L"", L"", L"", pAddress->_street, pAddress->_city, pAddress->_province, pAddress->_zipCode, pAddress->_countryCode);
            break;
        case ADDRESS_WORK:
            exportCard->GetDeliveryAddressCollection()->Add(DeliveryAddress_Work, L"", L"", L"", pAddress->_street, pAddress->_city, pAddress->_province, pAddress->_zipCode, pAddress->_countryCode);
            break;
        case ADDRESS_OTHER:
            exportCard->GetDeliveryAddressCollection()->Add(DeliveryAddress_NotSpecified, L"", L"", L"", pAddress->_street, pAddress->_city, pAddress->_province, pAddress->_zipCode, pAddress->_countryCode);
            break;
        default:
            exportCard->GetDeliveryAddressCollection()->Add(DeliveryAddress_Custom, pAddress->_label, L"", L"", pAddress->_street, pAddress->_city, pAddress->_province, pAddress->_zipCode, pAddress->_countryCode);
            break;
        }
    }

#pragma endregion
#pragma region 日期

    for (size_t i = 0; i < pContact->_dateList.size(); i++)
    {
        Day* di = pContact->_dateList.at(i);
        switch (di->_type)
        {
        case DAY_DAY:
            exportCard->GetDateConllection()->Add(DateType_Anniversary, _wtoi(di->_name.c_str()));
            break;
        case DAY_OTHER:
            exportCard->GetDateConllection()->Add(DateType_Other, _wtoi(di->_name.c_str()));
            break;
        default:
            exportCard->GetDateConllection()->Add(DateType_Custom, di->_label, _wtoi(di->_name.c_str()));
            break;
        }
    }

#pragma endregion
#pragma region Relates

    for (size_t i = 0; i < pContact->_relatedList.size(); i++)
    {
        Related* r = pContact->_relatedList.at(i);
        switch (r->_type)
        {          
        case RELATED_PARENT:
            exportCard->GetRelatedConllection()->Add(ContactRelated::RelatedLabelConvert(r->_type), r->_name, RelatedKind_Parent, r->_label);
            break;
        case RELATED_BROTHER:
            exportCard->GetRelatedConllection()->Add(ContactRelated::RelatedLabelConvert(r->_type), r->_name, RelatedKind_Brother, r->_label);
            break;
        case RELATED_SISTER:
            exportCard->GetRelatedConllection()->Add(ContactRelated::RelatedLabelConvert(r->_type), r->_name, RelatedKind_Sister, r->_label);
            break;
        case RELATED_MOTHER:
            exportCard->GetRelatedConllection()->Add(ContactRelated::RelatedLabelConvert(r->_type), r->_name, RelatedKind_Mother, r->_label);
            break;
        case RELATED_FATHER:
            exportCard->GetRelatedConllection()->Add(ContactRelated::RelatedLabelConvert(r->_type), r->_name, RelatedKind_Father, r->_label);
            break;
        case RELATED_CHILD:
            exportCard->GetRelatedConllection()->Add(ContactRelated::RelatedLabelConvert(r->_type), r->_name, RelatedKind_Child, r->_label);
            break;
        case RELATED_FRIEND:
            exportCard->GetRelatedConllection()->Add(ContactRelated::RelatedLabelConvert(r->_type), r->_name, RelatedKind_Friend, r->_label);
            break;
        case RELATED_SPOUSE:
            exportCard->GetRelatedConllection()->Add(ContactRelated::RelatedLabelConvert(r->_type), r->_name, RelatedKind_Spouse, r->_label);
            break;
        case RELATED_PARTNER:
            exportCard->GetRelatedConllection()->Add(ContactRelated::RelatedLabelConvert(r->_type), r->_name, RelatedKind_Partner, r->_label);
            break;
        case RELATED_ASSISTANT:
            exportCard->GetRelatedConllection()->Add(ContactRelated::RelatedLabelConvert(r->_type), r->_name, RelatedKind_Assistant, r->_label);
            break;
        case RELATED_MANAGER:
            exportCard->GetRelatedConllection()->Add(ContactRelated::RelatedLabelConvert(r->_type), r->_name, RelatedKind_Manager, r->_label);
            break;
        case RELATED_CUSTOM:
            exportCard->GetRelatedConllection()->Add(ContactRelated::RelatedLabelConvert(r->_type), r->_name, RelatedKind_Custom, r->_label);
            break;
        case RELATED_OTHER:
            exportCard->GetRelatedConllection()->Add(ContactRelated::RelatedLabelConvert(r->_type), r->_name, RelatedKind_Other, r->_label);
            break;
        default:
            break;
        }
    }

#pragma endregion

    wstring fileName = CStrOperation::CheckAndReplaceFileName(pContact->_sName._displayName, L'_');
    wstring path = CFileOperation::RenameFile(exportFolder+fileName+L".vcf");
    exportCard->ToFile(path);
    bool bResutl = CFileOperation::IsFileExist(path.c_str());

    SAFE_DELETE(contactName);
    SAFE_DELETE(exportCard);
    return bResutl;
}

ContactData* vCardEx::ImportFromVCards(wstring file)
{
    return GetContactsFromVcards(file);
}

ContactData* vCardEx::GetContactsFromVcards(wstring file)
{
    ContactData* person = new ContactData();
    vCardEx* cardTool = new vCardEx();
    try
    {
        cardTool->FromFile(file);

#pragma region MainInfos

        if (cardTool->GetName() != NULL)
        {
            // 解决wm导出的数据把名字存放在."表示为"的问题
            person->_sName._familyName = cardTool->GetName()->GetFirstName();
            person->_sName._givenName = cardTool->GetName()->GetLastName();
            person->_sName._middleName = cardTool->GetName()->GetMiddleName();
            person->_sName._ios_prefix = CCodeOperation::WstringToString(cardTool->GetName()->GetHonorificPrefix());
			person->_sName._ios_suffix = cardTool->GetName()->GetHonorificSuffix();
			person->_sName._ios_lastPhonetic = CCodeOperation::WstringToString(cardTool->GetName()->GetLastPhonetic());
			person->_sName._ios_firstPhonetic = CCodeOperation::WstringToString(cardTool->GetName()->GetFirstPhonetic());
			person->_sName._ios_middlePhonetic = CCodeOperation::WstringToString(cardTool->GetName()->GetMiddlePhonetic());
            if (person->_sName._familyName.empty() && person->_sName._givenName.empty() && person->_sName._middleName.empty())
                person->_sName._familyName = cardTool->GetFormattedName();
            person->_sName._displayName = person->_sName._givenName + person->_sName._familyName;
            person->_sName._state = STATE_ADD;
        }

        if (cardTool->GetNickNameCollection()->GetCount() > 0)
        {
            for (size_t i = 0; i < cardTool->GetNickNameCollection()->GetCount(); i++)
            {
                NickName* nickname = cardTool->GetNickNameCollection()->GetNickNameByIndex(i);
                person->_sName._ios_nickName += nickname->GetData();
                person->_sName._ios_nickName += L",";
            }
            person->_sName._ios_nickName = person->_sName._ios_nickName.substr(0, person->_sName._ios_nickName.length()-1);
        }

        if (!cardTool->GetOrganization().empty())
        {
            if (cardTool->GetOrganization().find(L";") != wstring::npos)
            {
                vector<wstring> array = CStrOperation::parseStringsW(cardTool->GetOrganization(), L";");
                if (array.size() >= 2)
                {
                    person->_companyName = array.at(0);
                    person->_department = array.at(1);
                }
                else
                    person->_companyName = array.at(0);
            }
            else
                person->_companyName = cardTool->GetOrganization();
        }

        if (!cardTool->GetTitle().empty())
            person->_jobTitle = cardTool->GetTitle();

        if (cardTool->GetNoteCollection()->GetCount() > 0)
        {
            for (size_t i = 0; i < cardTool->GetNoteCollection()->GetCount(); i++)
            {
                Note* note = cardTool->GetNoteCollection()->GetNoteByIndex(i);
                person->_sComment._content += note->GetData();
                person->_sComment._content += L"\r\n\r\n";
            }
            person->_sComment._content = person->_sComment._content.substr(0, person->_sComment._content.length()-1);
        }

        if (cardTool->GetBirthdate() != 0)
        {
            wchar_t time[128] = {0};
            wsprintf(time, L"%d", cardTool->GetBirthdate());
            wstring timeStr = time;
            person->_birthday = timeStr;
        }

        person->_headPhoto._strSavePath/*_thumbnailPath*/ = cardTool->GetImage();

#pragma endregion
#pragma region Emails

        for (size_t i = 0; i < cardTool->GetEmailAddressCollection()->GetCount(); i++)
        {
            EmailAddress* email = cardTool->GetEmailAddressCollection()->GetEmailAddressByIndex(i);
            Email* e = new Email();
            e->_state = STATE_ADD;
            e->_emailAccount = email->GetEmailAddress();
            switch (email->GetType())
            {
            case EmailAddress_Preferred:
                e->_type = EMAIL_HOME;
                break;
            case EmailAddress_Internet:
                e->_type = EMAIL_WORK;
                break;
            case EmailAddress_NotSpecified:
                e->_type = EMAIL_OTHER;
                break;
            case EmailAddress_Custom:
                e->_type = EMAIL_CUSTOM;
                e->_label = email->GetCustomType();
                break;
            }
            person->_emailList.push_back(e);
        }
            
#pragma endregion
#pragma region PhoneNumbers

        for (size_t i = 0; i < cardTool->GetPhoneNumberCollection()->GetCount(); i++)
        {
            PhoneNumber* phone = cardTool->GetPhoneNumberCollection()->GetPhoneNumberByIndex(i);
            Phone* pn = new Phone();
            pn->_state = STATE_ADD;
            pn->_number = phone->GetNumber();
            switch (phone->GetType())
            {
            case PhoneNumber_Home:
                pn->_type = PHONE_HOME;
                break;
            case PhoneNumber_Work:
                pn->_type = PHONE_WORK;
                break;
            case PhoneNumber_Cellular:
                pn->_type = PHONE_MOBILE;
                break;
            case PhoneNumber_Preferred:
                pn->_type = PHONE_MAIN;
                break;
            case PhoneNumber_Pager:
                pn->_type = PHONE_PAGER;
                break;
            case PhoneNumber_Fax | PhoneNumber_Home:
                pn->_type = PHONE_HOMEFAX;
                break;
            case PhoneNumber_Fax | PhoneNumber_Work:
                pn->_type = PHONE_WORKFAX;
                break;
            case PhoneNumber_Other:
                pn->_type = PHONE_OTHER;
                break;
            default:
                pn->_type = PHONE_CUSTOM;
                pn->_label = phone->GetCustomType();
                break;
            }
            person->_phoneNumberList.push_back(pn);
        }

#pragma endregion
#pragma region URLS

        for (size_t i = 0; i < cardTool->GetUrlCollection()->GetCount(); i++)
        {
            Url* url = cardTool->GetUrlCollection()->GetUrlByIndex(i);
            HomePage* h = new HomePage();
            h->_state = STATE_ADD;
            h->_name = url->GetData();
            switch (url->GetType())
            {
            case Url_Preferred:
                h->_type = HOMEPAGE_HOMEPAGE;
                break;
            case Url_Home:
                h->_type = HOMEPAGE_HOME;
                break;
            case Url_Work:
                h->_type = HOMEPAGE_WORK;
                break;
            case Url_Other:
                h->_type = HOMEPAGE_OTHER;
                break;
            default:
                h->_type = HOMEPAGE_CUSTOM;
                h->_label = url->GetCustomType();
                break;
            }
            person->_homePageList.push_back(h);
        }

#pragma endregion
#pragma region Addresses

        for (size_t i = 0; i < cardTool->GetDeliveryAddressCollection()->GetCount(); i++)
        {
            DeliveryAddress* address = cardTool->GetDeliveryAddressCollection()->GetDeliveryAddressByIndex(i);
            Address* a = new Address();
            a->_state = STATE_ADD;

            switch (address->GetType())
            {
            case DeliveryAddress_Home:
                a->_type = ADDRESS_HOME;
                break;
            case DeliveryAddress_Work:
                a->_type = ADDRESS_WORK;
                break;
            case DeliveryAddress_NotSpecified:
                a->_type = ADDRESS_OTHER;
                break;
            default:
                a->_type = ADDRESS_CUSTOM;
                a->_label = address->GetCustomType();
                break;
            }

            a->_city = address->GetLocality();
            a->_country = address->GetCountry();
            a->_countryCode = address->GetCountryCode();
            a->_province = address->GetRegion();
            a->_street = address->GetStreet();
            a->_zipCode = address->GetPostalCode();
            if (a->_city.empty() 
                && a->_country.empty() 
                && a->_countryCode.empty()
                && a->_province.empty()
                && a->_street.empty()
                && a->_zipCode.empty())
                a->_city = address->GetPostOfficeAddress();
            
            person->_addressList.push_back(a);
        }

#pragma endregion
#pragma region IMAddress

        for (size_t i = 0; i < cardTool->GetIMAddressConllection()->GetCount(); i++)
        {
            IMAddress* im = cardTool->GetIMAddressConllection()->GetIMAddressByIndex(i);
            Im* ia = new Im();
            ia->_state = STATE_ADD;

            switch (im->GetType())
            {
            case IMType_HOME:
                ia->_type = IM_HOME;
                break;
            case IMType_WORK:
                ia->_type = IM_WORK;
                break;
            case IMType_OTHER:
                ia->_type = IM_OTHER;
                break;
            case IMType_NotSpecified:
                ia->_type = IM_OTHER;
                break;
            default:
                ia->_type = IM_CUSTOM;
                ia->_label = im->GetCustomName();
                break;
            }

            ia->_typeName = IMAddress::IMLabelConvert(im->GetLabel());
            ia->_imAccount = im->GetAddress();
            
            person->_imList.push_back(ia);
        }

#pragma endregion
#pragma region Dates

        for (size_t i = 0; i < cardTool->GetDateConllection()->GetCount(); i++)
        {
            Date* date = cardTool->GetDateConllection()->GetDateByIndex(i);
            Day *d = new Day();
            d->_state = STATE_ADD;
            wchar_t time[128] = {0};
            wsprintf(time, L"%d", date->GetDate());
            wstring timeStr = time;
            d->_name = timeStr;
            
            switch (date->GetType())
            {
            case DateType_Anniversary:
                d->_type = DAY_DAY;
                break;
            case DateType_Other:
                d->_type = DAY_OTHER;
                break;
            default:
                d->_type = DAY_CUSTOM;
                d->_label = date->GetCustomType();
                break;
            }
            person->_dateList.push_back(d);
        }

#pragma endregion
#pragma region related

        for (size_t i = 0; i < cardTool->GetRelatedConllection()->GetCount(); i++)
        {
            ContactRelated* related = cardTool->GetRelatedConllection()->GetContactRelatedByIndex(i);
            Related* r = new Related();
            r->_state = STATE_ADD;
            r->_label = related->GetLabel();
            r->_name = related->GetRelatedName();

            switch (related->GetKind())
            {
            case RelatedKind_Parent:
                r->_type = RELATED_PARENT;
                break;
            case RelatedKind_Brother:
                r->_type = RELATED_BROTHER;
                break;
            case RelatedKind_Sister:
                r->_type = RELATED_SISTER;
                break;
            case RelatedKind_Mother:
                r->_type = RELATED_MOTHER;
                break;
            case RelatedKind_Father:
                r->_type = RELATED_FATHER;
                break;
            case RelatedKind_Child:
                r->_type = RELATED_CHILD;
                break;
            case RelatedKind_Friend:
                r->_type = RELATED_FRIEND;
                break;
            case RelatedKind_Spouse:
                r->_type = RELATED_SPOUSE;
                break;
            case RelatedKind_Partner:
                r->_type = RELATED_PARENT;
                break;
            case RelatedKind_Assistant:
                r->_type = RELATED_ASSISTANT;
                break;
            case RelatedKind_Manager:
                r->_type = RELATED_MANAGER;
                break;
            case RelatedKind_Other:
                r->_type = RELATED_OTHER;
                break;
            case RelatedKind_Custom:
                r->_type = RELATED_CUSTOM;
                r->_label = related->GetCustomName();
                break;
            default:
                break;
            }
            person->_relatedList.push_back(r);
        }

#pragma endregion
#pragma region Groups

        if (!cardTool->GetCategory().empty())
            person->_tempGroupNames = CStrOperation::parseStringsW(cardTool->GetCategory(), L",");

#pragma endregion

    }
    catch(...)
    {
        SAFE_DELETE(person);
        SAFE_DELETE(cardTool);
    }

    SAFE_DELETE(cardTool);
    return person;
}

wstring vCardEx::ContactToVCardString(const ContactData* pContact)
{
	vCardEx* exportCard = new vCardEx();
	vName* contactName = new vName(
		pContact->_sName._givenName,
		pContact->_sName._familyName,
		pContact->_sName._middleName,
		CCodeOperation::s2ws(pContact->_sName._ios_prefix),
		pContact->_sName._ios_suffix,
		CCodeOperation::s2ws(pContact->_sName._ios_lastPhonetic),
		CCodeOperation::s2ws(pContact->_sName._ios_firstPhonetic),
		CCodeOperation::s2ws(pContact->_sName._ios_middlePhonetic));

	exportCard->SetName(contactName);
	exportCard->SetFormattedName(pContact->_sName._displayName);
	if (!pContact->_sName._ios_nickName.empty())
	{
		vector<wstring> nickNames = CStrOperation::parseStringsW(pContact->_sName._ios_nickName, L",");
		for (size_t i = 0; i < nickNames.size(); i++)
			exportCard->GetNickNameCollection()->Add(nickNames.at(i));
	}

	if (!pContact->_companyName.empty())
		exportCard->SetOrganization(pContact->_companyName);
	if (!pContact->_department.empty())
	{
		wstring org;
		if (exportCard->GetOrganization().empty())
			org = L"";
		else
			org+=exportCard->GetOrganization()+L";"+pContact->_department;

		exportCard->SetOrganization(org);
	}

	for (size_t i = 0; i < pContact->_phoneNumberList.size(); i++)
	{
		Phone* pn = pContact->_phoneNumberList.at(i);
		switch (pn->_type)
		{
		case PHONE_HOME:
			exportCard->GetPhoneNumberCollection()->Add(PhoneNumber_Home, pn->_number);
			break;
		case PHONE_WORK:
			exportCard->GetPhoneNumberCollection()->Add(PhoneNumber_Work, pn->_number);
			break;
		case PHONE_MOBILE:
			exportCard->GetPhoneNumberCollection()->Add(PhoneNumber_Cellular, pn->_number);
			break;
		case PHONE_MAIN:
			exportCard->GetPhoneNumberCollection()->Add(PhoneNumber_Preferred, pn->_number);
			break;
		case PHONE_PAGER:
			exportCard->GetPhoneNumberCollection()->Add(PhoneNumber_Pager, pn->_number);
			break;
		case PHONE_HOMEFAX:
			exportCard->GetPhoneNumberCollection()->Add(PhoneNumberType_enum(PhoneNumber_Fax | PhoneNumber_Home), pn->_number);
			break;
		case PHONE_WORKFAX:
			exportCard->GetPhoneNumberCollection()->Add(PhoneNumberType_enum(PhoneNumber_Fax | PhoneNumber_Work), pn->_number);
			break;
		case PHONE_OTHER:
			exportCard->GetPhoneNumberCollection()->Add(PhoneNumber_Other, pn->_number);
			break;
		default:
			exportCard->GetPhoneNumberCollection()->Add(PhoneNumber_Custom, pn->_label, pn->_number);
			break;
		}
	}

	return exportCard->ToString();
}

//#endif

ContactData* ND91Assistant::vCardEx::ParseContact( wstring source )
{
	ContactData* person = new ContactData();
	vCardEx* cardTool = new vCardEx();
	try
	{
		cardTool->FromString(source);

		if (cardTool->GetName() != NULL)
		{
			LOG->WriteDebug(L"vCardEx::ParseContact");
			// 解决wm导出的数据把名字存放在."表示为"的问题
			person->_sName._familyName = cardTool->GetName()->GetFirstName();
			person->_sName._givenName = cardTool->GetName()->GetLastName();
			person->_sName._middleName = cardTool->GetName()->GetMiddleName();
			person->_sName._ios_prefix = CCodeOperation::WstringToString(cardTool->GetName()->GetHonorificPrefix());
			if (person&&cardTool&&cardTool->GetName())
			{		
				LOG->WriteDebug(cardTool->GetName()->GetHonorificSuffix());
				person->_sName._ios_suffix = cardTool->GetName()->GetHonorificSuffix();
			}
			person->_sName._ios_lastPhonetic = CCodeOperation::WstringToString(cardTool->GetName()->GetLastPhonetic());
			person->_sName._ios_firstPhonetic = CCodeOperation::WstringToString(cardTool->GetName()->GetFirstPhonetic());
			person->_sName._ios_middlePhonetic = CCodeOperation::WstringToString(cardTool->GetName()->GetMiddlePhonetic());
			if (person->_sName._familyName.empty() && person->_sName._givenName.empty() && person->_sName._middleName.empty())
				person->_sName._familyName = cardTool->GetFormattedName();
			person->_sName._displayName = person->_sName._givenName + person->_sName._familyName;
			person->_sName._state = STATE_ADD;
		}

		if (cardTool->GetNickNameCollection()->GetCount() > 0)
		{
			for (size_t i = 0; i < cardTool->GetNickNameCollection()->GetCount(); i++)
			{
				NickName* nickname = cardTool->GetNickNameCollection()->GetNickNameByIndex(i);
				person->_sName._ios_nickName += nickname->GetData();
				person->_sName._ios_nickName += L",";
			}
			person->_sName._ios_nickName = person->_sName._ios_nickName.substr(0, person->_sName._ios_nickName.length()-1);
		}
		if (cardTool->GetNickNameCollection()->GetCount() > 0)
		{
			for (size_t i = 0; i < cardTool->GetNickNameCollection()->GetCount(); i++)
			{
				NickName* nickname = cardTool->GetNickNameCollection()->GetNickNameByIndex(i);
				person->_sName._ios_nickName += nickname->GetData();
				person->_sName._ios_nickName += L",";
			}
			person->_sName._ios_nickName = person->_sName._ios_nickName.substr(0, person->_sName._ios_nickName.length()-1);
		}

		if (!cardTool->GetOrganization().empty())
		{
			if (cardTool->GetOrganization().find(L";") != wstring::npos)
			{
				vector<wstring> array = CStrOperation::parseStringsW(cardTool->GetOrganization(), L";");
				if (array.size() >= 2)
				{
					person->_companyName = array.at(0);
					person->_department = array.at(1);
				}
				else
					person->_companyName = array.at(0);
			}
			else
				person->_companyName = cardTool->GetOrganization();
		}

		for (size_t i = 0; i < cardTool->GetPhoneNumberCollection()->GetCount(); i++)
		{
			PhoneNumber* phone = cardTool->GetPhoneNumberCollection()->GetPhoneNumberByIndex(i);
			Phone* pn = new Phone();
			pn->_state = STATE_ADD;
			pn->_number = phone->GetNumber();
			switch (phone->GetType())
			{
			case PhoneNumber_Home:
				pn->_type = PHONE_HOME;
				break;
			case PhoneNumber_Work:
				pn->_type = PHONE_WORK;
				break;
			case PhoneNumber_Cellular:
				pn->_type = PHONE_MOBILE;
				break;
			case PhoneNumber_Preferred:
				pn->_type = PHONE_MAIN;
				break;
			case PhoneNumber_Pager:
				pn->_type = PHONE_PAGER;
				break;
			case PhoneNumber_Fax | PhoneNumber_Home:
				pn->_type = PHONE_HOMEFAX;
				break;
			case PhoneNumber_Fax | PhoneNumber_Work:
				pn->_type = PHONE_WORKFAX;
				break;
			case PhoneNumber_Other:
				pn->_type = PHONE_OTHER;
				break;
			default:
				pn->_type = PHONE_CUSTOM;
				pn->_label = phone->GetCustomType();
				break;
			}
			person->_phoneNumberList.push_back(pn);
		}
	}
	catch(...)
	{
		SAFE_DELETE(person);
		SAFE_DELETE(cardTool);
	}

	SAFE_DELETE(cardTool);
	return person;
}
