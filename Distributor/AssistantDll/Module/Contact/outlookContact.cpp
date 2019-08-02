#include "StdAfx.h"
#include "outlookContact.h"

using namespace ND91Assistant;
outlookContact::outlookContact(void)
{
	_birthday=0;
	_creationTime=0;
	_lastModificationTime=0;
}

outlookContact::~outlookContact(void)
{
}

void ND91Assistant::outlookContact::CovertToContactData( ContactData* pContact,bool isVersion20/*=false*/ )
{
	if(!pContact)
		pContact=new ContactData();

	pContact->_headPhoto._strSavePath=this->_picturePath;
	pContact->_sName._familyName=CCodeOperation::s2ws(this->_lastName);
	pContact->_sName._givenName=CCodeOperation::s2ws(this->_middleName+this->_firstName);
	pContact->_sName._displayName=pContact->_sName._familyName+pContact->_sName._givenName;
	if(pContact->_sName._displayName.empty())
		pContact->_sName._displayName=pContact->_sName._familyName=L"<未知>";

	pContact->_sName._ios_firstPhonetic=this->_firstName;          // 名字拼音
	pContact->_sName._ios_middlePhonetic=this->_middleName;         // 中间名拼音
	pContact->_sName._ios_lastPhonetic=this->_lastName;           // 姓氏拼音
	pContact->_sName._ios_kind;                   // 类别
	pContact->_sName._ios_nickName=CCodeOperation::s2ws(this->_nickName);               // 昵称
	pContact->_sName._ios_prefix=this->_title;                 // 英文称谓
	pContact->_sName._ios_suffix=CCodeOperation::s2ws(this->_suffix);                 // 中文称谓

	pContact->_sComment._content=CCodeOperation::s2ws(this->_body);
	if(this->_birthday<949998.0)
		pContact->_birthday=CStrOperation::Int64ToWString(CDealTime::FromDATE(this->_birthday));
	if(this->_anniversary>0)
	{
		Day* pday=new Day();
		if(this->_anniversary<949998.0)
			pday->_name=CStrOperation::Int64ToWString(CDealTime::FromDATE(this->_anniversary));
		pday->_type=DAY_DAY;
		pContact->_dateList.push_back(pday);
	}

	if(!this->_company.empty())
	{
		Organization* org=new Organization();
		org->_type=ORGANIZATION_WORK;
		org->_name=CCodeOperation::s2ws(this->_company);
		org->_label=CCodeOperation::s2ws(this->_jobTitle);
		pContact->_organizationList.push_back(org);
	}

	pContact->AddAndroidPhoneNUmber(this->_assistantTelephoneNumber,PHONE_OTHER);
	pContact->AddAndroidPhoneNUmber(this->_businessFaxNumber,PHONE_WORKFAX);
	pContact->AddAndroidPhoneNUmber(this->_business2TelephoneNumber,PHONE_WORK);
	pContact->AddAndroidPhoneNUmber(this->_businessTelephoneNumber,PHONE_WORK);
	pContact->AddAndroidPhoneNUmber(this->_callbackTelephoneNumber,PHONE_OTHER);
	pContact->AddAndroidPhoneNUmber(this->_carTelephoneNumber,PHONE_OTHER);
	pContact->AddAndroidPhoneNUmber(this->_home2TelephoneNumber,PHONE_HOME);
	pContact->AddAndroidPhoneNUmber(this->_homeFaxNumber,PHONE_HOMEFAX);
	pContact->AddAndroidPhoneNUmber(this->_homeTelephoneNumber,PHONE_HOME);
	pContact->AddAndroidPhoneNUmber(this->_ISDNNumber,PHONE_OTHER);
	pContact->AddAndroidPhoneNUmber(this->_mobileTelephoneNumber,PHONE_MOBILE);
	pContact->AddAndroidPhoneNUmber(this->_otherFaxNumber,PHONE_OTHERFAX);
	pContact->AddAndroidPhoneNUmber(this->_pagerNumber,PHONE_PAGER);
	pContact->AddAndroidPhoneNUmber(this->_primaryTelephoneNumber,PHONE_MAIN);
	pContact->AddAndroidPhoneNUmber(this->_radioTelephoneNumber,PHONE_OTHER);
	pContact->AddAndroidPhoneNUmber(this->_telexNumber,PHONE_OTHER);
	pContact->AddAndroidPhoneNUmber(this->_TTYTDDTelephoneNumber,PHONE_OTHER);
	pContact->AddAndroidPhoneNUmber(this->_otherTelephoneNumber,PHONE_OTHER);


	pContact->AddAndroidEmail(this->_email1Address,EMAIL_HOME);
	pContact->AddAndroidEmail(this->_email2Address,EMAIL_WORK);
	pContact->AddAndroidEmail(this->_email3Address,EMAIL_OTHER);

	pContact->AddHomepage(this->_internetFreeBusyAddress,HOMEPAGE_OTHER);
	pContact->AddHomepage(this->_businessHomePage,HOMEPAGE_WORK);
	pContact->AddHomepage(this->_webPage,HOMEPAGE_HOMEPAGE);

	AddAddressToContactData(pContact, this->_businessAddress, ADDRESS_WORK);
	AddAddressToContactData(pContact,this->_homeAddress, ADDRESS_HOME);
	AddAddressToContactData(pContact,this->_otherAddress, ADDRESS_OTHER);

	vector<wstring> groupnames=CStrOperation::parseStringsW(CCodeOperation::s2ws(this->_folder),L",");
	for(size_t i=0;i<groupnames.size();i++)
		pContact->_tempGroupNames.push_back(groupnames[i]);
}

void ND91Assistant::outlookContact::AddAddressToContactData( ContactData* pContact, outlookContactAddress adr, AddressTypes type)
{
	if(adr._city.empty()&&adr._country.empty()&&adr._state.empty()&&adr._street.empty()&&adr._zip.empty())
		return;
	Address* padr=new Address();
	padr->_city=CCodeOperation::s2ws(adr._city);
	padr->_country=CCodeOperation::s2ws(adr._country);
	padr->_province=CCodeOperation::s2ws(adr._state);
	padr->_street=CCodeOperation::s2ws(adr._street);
	padr->_zipCode=CCodeOperation::s2ws(adr._zip);
	padr->_type=type;
	pContact->_addressList.push_back(padr);
}

 void CopyAdressFromContactData(outlookContactAddress& oladr,Address* pAdr)
 {
	 oladr._city=CCodeOperation::ws2s(pAdr->_city);
	 oladr._country=CCodeOperation::ws2s(pAdr->_country);
	 oladr._state=CCodeOperation::ws2s(pAdr->_province);
	 oladr._street=CCodeOperation::ws2s(pAdr->_street);
	 oladr._zip=CCodeOperation::ws2s(pAdr->_zipCode);
 }

void ND91Assistant::outlookContact::FromContactData( ContactData* pContact )
{
	if(!pContact->_birthday.empty())
		this->_birthday = CDealTime::ToDATE(_wtoi64(pContact->_birthday.c_str()));
	this->_firstName=CCodeOperation::ws2s(pContact->_sName._familyName);
	this->_lastName=CCodeOperation::ws2s(pContact->_sName._givenName);
	this->_body=CCodeOperation::ws2s(pContact->_sComment._content);
	//this->_picturePath = pContact->_headPhoto._strSavePath.empty()?pContact->_headPhoto._thumbnailPath:pContact->_headPhoto._strSavePath;
    this->_picturePath = pContact->_headPhoto._strSavePath;
	if(((int)pContact->_organizationList.size())>0)
	{
		this->_company=CCodeOperation::ws2s(pContact->_organizationList[0]->_name);
		this->_jobTitle=CCodeOperation::ws2s(pContact->_organizationList[0]->_address);
	}
	if(((int)pContact->_dateList.size())>0)
	{
		this->_anniversary=CDealTime::ToDATE(_wtoi64(pContact->_dateList[0]->_name.c_str()));
		for (size_t i=1;i<pContact->_dateList.size();i++)
		{
			this->_body+="\r\nAnniversary : ";
			this->_body+=CDealTime(_wtoi64(pContact->_dateList[i]->_name.c_str())).TimeFormat();
		}
	}

	string unmatchedphone;
	for (size_t i=0;i<pContact->_phoneNumberList.size();i++)
	{
		Phone* pTel= pContact->_phoneNumberList[i];
		bool matched=false;
		switch(pTel->_type)
		{
			case PHONE_OTHER:        // 其他
				if(this->_otherTelephoneNumber.empty())
				{
					this->_otherTelephoneNumber=CCodeOperation::ws2s(pTel->_number);
					matched = true;
				}
				break;
			case PHONE_HOME:         // 家庭
				if(this->_homeTelephoneNumber.empty())
				{
					this->_homeTelephoneNumber=CCodeOperation::ws2s(pTel->_number);
					matched = true;
				}
				break;
			case PHONE_WORK:         // 工作
				if(this->_businessTelephoneNumber.empty())
				{
					this->_businessTelephoneNumber=CCodeOperation::ws2s(pTel->_number);
					matched =true;
				}
				break;
			case PHONE_MOBILE:       // 移动电话
				if(this->_mobileTelephoneNumber.empty())
				{
					this->_mobileTelephoneNumber=CCodeOperation::ws2s(pTel->_number);
					matched=true;
				}
				break;
			case PHONE_MAIN:         // 主要
				if(this->_primaryTelephoneNumber.empty())
				{
					this->_primaryTelephoneNumber=CCodeOperation::ws2s(pTel->_number);
					matched =true;
				}
				break;
			case PHONE_HOMEFAX:      // 家庭传真
				if(this->_homeFaxNumber.empty())
				{
					this->_homeFaxNumber=CCodeOperation::ws2s(pTel->_number);
					matched = true;
				}
				break;
			case PHONE_WORKFAX:      // 工作传真
				if(this->_businessFaxNumber.empty())
				{
					this->_businessFaxNumber=CCodeOperation::ws2s(pTel->_number);
					matched = true;
				}
				break;
			case PHONE_OTHERFAX:     // 其他传真
				if(this->_otherFaxNumber.empty())
				{
					this->_otherFaxNumber=CCodeOperation::ws2s(pTel->_number);
					matched = true;
				}
				break;
			case PHONE_PAGER:        // 传呼
				if(this->_pagerNumber.empty())
				{
					this->_pagerNumber=CCodeOperation::ws2s(pTel->_number);
					matched = true;
				}
				break;
			default:
				break;
		}
		if(!matched)
			unmatchedphone+="\r\nPhoneNumber : "+CCodeOperation::ws2s(pTel->_number);
	}

	string unmatchedEmail;
	for (size_t i=0;i<pContact->_emailList.size();i++)
	{
		Email* pEmail=pContact->_emailList[i];
		bool matched=false;
		switch(pEmail->_type)
		{
			case EMAIL_HOME:
				if(this->_email1Address.empty())
				{
					this->_email1Address=CCodeOperation::ws2s(pEmail->_emailAccount);
					matched=true;
				}
				break;
			case EMAIL_OTHER:
				if(this->_email2Address.empty())
				{
					this->_email2Address=CCodeOperation::ws2s(pEmail->_emailAccount);
					matched=true;
				}
				break;
			case EMAIL_WORK:
				if(this->_email3Address.empty())
				{
					this->_email3Address=CCodeOperation::ws2s(pEmail->_emailAccount);
					matched=true;
				}
				break;
			default:
				break;
		}
		if(!matched)
			unmatchedEmail+="\r\nEmail : "+CCodeOperation::ws2s(pEmail->_emailAccount);
	}

	for (size_t i=0;i<pContact->_homePageList.size();i++)
	{
		HomePage* pPage=pContact->_homePageList[i];
		switch(pPage->_type)
		{
			case HOMEPAGE_HOMEPAGE:
				{
					if(this->_businessHomePage.empty())
						this->_businessHomePage=CCodeOperation::ws2s(pPage->_name);
					if(this->_webPage.empty())
						this->_webPage=CCodeOperation::ws2s(pPage->_name);
				}
				break;
			case HOMEPAGE_HOME:          // 家庭
				this->_webPage=CCodeOperation::ws2s(pPage->_name);
				break;
			case HOMEPAGE_WORK:          // 工作
				this->_businessHomePage=CCodeOperation::ws2s(pPage->_name);
				break;
			case HOMEPAGE_OTHER:         // 其它
			default:
				{
					this->_body+="\r\nHomePage : ";
					this->_body+=CCodeOperation::ws2s(pPage->_name);
				}
				break;
		}
	}

	for (size_t i=0;i<pContact->_addressList.size();i++)
	{
		Address* pAdr=pContact->_addressList[i];
		switch (pAdr->_type)
		{
			case ADDRESS_HOME:
				CopyAdressFromContactData(this->_homeAddress,pAdr);
				break;
			case ADDRESS_WORK:
				CopyAdressFromContactData(this->_businessAddress,pAdr);
				break;
			case ADDRESS_OTHER:
				CopyAdressFromContactData(this->_otherAddress,pAdr);
				break;
			default:
				{
					this->_body+="\r\nAddress : ";
					this->_body+="\r\nCity : "+CCodeOperation::ws2s(pAdr->_city);
					this->_body+="\r\nCountry : "+CCodeOperation::ws2s(pAdr->_country);
					this->_body+="\r\nZip : "+CCodeOperation::ws2s(pAdr->_zipCode);
					this->_body+="\r\nState : "+CCodeOperation::ws2s(pAdr->_province);
					this->_body+="\r\nStreet : "+CCodeOperation::ws2s(pAdr->_street);
				}
				break;
		}
	}

	this->_body+=unmatchedphone;
	this->_body+=unmatchedEmail;
}