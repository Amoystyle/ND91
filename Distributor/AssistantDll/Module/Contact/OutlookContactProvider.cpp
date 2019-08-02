#include "StdAfx.h"
#include <WinBase.h>

#include "OutlookContactProvider.h"
#include "outlookContact.h"


OutlookContactProvider::OutlookContactProvider(void)
{
	_olptr=NULL;
	_olspaceprt=NULL;
	_pFolder=NULL;
}

OutlookContactProvider::~OutlookContactProvider(void)
{
}

bool OutlookContactProvider::Init()
{
	HRESULT hr;
	try
	{
		hr=_olptr.CreateInstance(__uuidof(Outlook::Application));
		if(hr<0)
			return false;
	}
	catch(_com_error)
	{
		return false;
	}
	return true;
}

bool ND91Assistant::OutlookContactProvider::CheckExsitOutlook()
{
	struct HKEY__* hkey;
	long ret = 0;
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE,L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\OUTLOOK.EXE", 0, KEY_QUERY_VALUE, &hkey);
	if(hkey&&ret!=ERROR_SUCCESS)
	{
		RegCloseKey(hkey);
		return false;
	}
	return true;
}

long ND91Assistant::OutlookContactProvider::LoadOutLookContacts()
{
	_olspaceprt= _olptr->GetNamespace(_bstr_t("MAPI"));
	_pFolder = _olspaceprt->GetDefaultFolder(Outlook::olFolderContacts);
	Outlook::_ItemsPtr pItems=_pFolder->GetItems();
	long count=pItems->GetCount();
	Outlook::_FoldersPtr folders=_pFolder->GetFolders();
	long foldcount=folders->GetCount();
	for (int i=0;i<foldcount;i++)
	{
		if(folders->Item(i))
			count+=folders->Item(i)->GetItems()->GetCount();
	}

	Load(_pFolder);
	for (int i=0;i<foldcount;i++)
	{
		Load(folders->Item(i));
	}
	return count;
}

void ND91Assistant::OutlookContactProvider::Load( Outlook::MAPIFolderPtr pFolder )
{
	RETURN_IF(!pFolder);
	_outlookContacts.clear();
	Outlook::_ItemsPtr pItems=pFolder->GetItems();
	Outlook::_ContactItemPtr pContact=pItems->GetFirst();
	vector<outlookContact> olcontactVec;
	while(pContact)
	{
		outlookContact olcontact;
		olcontact._folder=(char*)pFolder->GetName();
		PaserContactItem(pContact,olcontact);
		olcontactVec.push_back(olcontact);
		pContact=pItems->GetNext();
	}

	_outlookContacts[(char*)pFolder->GetName()]=olcontactVec;
}

void ND91Assistant::OutlookContactProvider::PaserContactItem(Outlook::_ContactItemPtr pContact, outlookContact& olcontact)
{
	if((char*)pContact->GetLastName())
		olcontact._lastName=pContact->GetLastName();
	if((char*)pContact->GetMiddleName())
	olcontact._middleName=pContact->GetMiddleName();
	if((char*)pContact->GetFirstName())
	olcontact._firstName=pContact->GetFirstName();
	if((char*)pContact->GetNickName())
	olcontact._nickName=pContact->GetNickName();
	if((char*)pContact->GetSuffix())
		olcontact._suffix=pContact->GetSuffix();

	if((char*)pContact->GetTitle())
	olcontact._title=pContact->GetTitle();//prefix
	if((char*)pContact->GetCompanyName())
	olcontact._company=pContact->GetCompanyName();//Organization
	if((char*)pContact->GetDepartment())
	olcontact._department=pContact->GetDepartment();
	if((char*)pContact->GetJobTitle())
	olcontact._jobTitle=pContact->GetJobTitle();
	if((char*)pContact->GetBody())
	olcontact._body=pContact->GetBody();//Note
	olcontact._birthday=pContact->GetBirthday();
	olcontact._creationTime=pContact->GetCreationTime();
	olcontact._lastModificationTime=pContact->GetLastModificationTime();

	olcontact._hasPicture=pContact->GetHasPicture()==-1;
	if(olcontact._hasPicture)
	{
		wstring filepath=CFileOperation::GetCurrWorkingDir()+L"outlook\\Attachments\\";
		CFileOperation::MakeLocalDir(CFileOperation::GetFileDir(filepath));
		filepath+=CFileOperation::GetGuidW()+L".png";
		pContact->GetAttachments()->Item(1)->SaveAsFile(_bstr_t(CCodeOperation::UnicodeToUTF_8(filepath).c_str()));
		if(CFileOperation::IsFileExist(filepath.c_str()))
		{
			olcontact._picturePath=filepath;//CFileOperation::ReadFile(filepath,true);
		}
	}
	olcontact._anniversary=pContact->GetAnniversary();//¼ÍÄîÈÕ
	if((char*)pContact->GetAssistantTelephoneNumber())
	olcontact._assistantTelephoneNumber=pContact->GetAssistantTelephoneNumber();
	if((char*)pContact->GetBusinessFaxNumber())
	olcontact._businessFaxNumber=pContact->GetBusinessFaxNumber();
	if((char*)pContact->GetBusiness2TelephoneNumber())
	olcontact._business2TelephoneNumber=pContact->GetBusiness2TelephoneNumber();
	if((char*)pContact->GetBusinessTelephoneNumber())
	olcontact._businessTelephoneNumber=pContact->GetBusinessTelephoneNumber();
	if((char*)pContact->GetCallbackTelephoneNumber())
	olcontact._callbackTelephoneNumber=pContact->GetCallbackTelephoneNumber();
	if((char*)pContact->GetCarTelephoneNumber())
	olcontact._carTelephoneNumber=pContact->GetCarTelephoneNumber();
	if((char*)pContact->GetHome2TelephoneNumber())
	olcontact._home2TelephoneNumber=pContact->GetHome2TelephoneNumber();
	if((char*)pContact->GetHomeFaxNumber())
	olcontact._homeFaxNumber=pContact->GetHomeFaxNumber();
	if((char*)pContact->GetHomeTelephoneNumber())
	olcontact._homeTelephoneNumber=pContact->GetHomeTelephoneNumber();
	if ((char*)pContact->GetISDNNumber())
	olcontact._ISDNNumber=pContact->GetISDNNumber();
	if((char*)pContact->GetMobileTelephoneNumber())
	olcontact._mobileTelephoneNumber=pContact->GetMobileTelephoneNumber();
	if((char*)pContact->GetOtherFaxNumber())
	olcontact._otherFaxNumber=pContact->GetOtherFaxNumber();
	if((char*)pContact->GetPagerNumber())
	olcontact._pagerNumber=pContact->GetPagerNumber();
	if((char*)pContact->GetPrimaryTelephoneNumber())
	olcontact._primaryTelephoneNumber=pContact->GetPrimaryTelephoneNumber();
	if((char*)pContact->GetRadioTelephoneNumber())
	olcontact._radioTelephoneNumber=pContact->GetRadioTelephoneNumber();
	if((char*)pContact->GetTelexNumber())
	olcontact._telexNumber=pContact->GetTelexNumber();
	if((char*)pContact->GetTTYTDDTelephoneNumber())
	olcontact._TTYTDDTelephoneNumber=pContact->GetTTYTDDTelephoneNumber();
	if((char*)pContact->GetOtherTelephoneNumber())
	olcontact._otherTelephoneNumber=pContact->GetOtherTelephoneNumber();
	if((char*)pContact->GetEmail1Address())
	olcontact._email1Address=pContact->GetEmail1Address();
	if((char*)pContact->GetEmail2Address())
	olcontact._email2Address=pContact->GetEmail2Address();
	if((char*)pContact->GetEmail3Address())
	olcontact._email3Address=pContact->GetEmail3Address();
	if((char*)pContact->GetInternetFreeBusyAddress())
	olcontact._internetFreeBusyAddress=pContact->GetInternetFreeBusyAddress();
	if((char*)pContact->GetBusinessHomePage())
	olcontact._businessHomePage=pContact->GetBusinessHomePage();
	if((char*)pContact->GetWebPage())
	olcontact._webPage=pContact->GetWebPage();

	if((char*)pContact->GetBusinessAddressCity())
	olcontact._businessAddress._city=pContact->GetBusinessAddressCity();
	if((char*)pContact->GetBusinessAddressCountry())
	olcontact._businessAddress._country=pContact->GetBusinessAddressCountry();
	if((char*)pContact->GetBusinessAddressState())
	olcontact._businessAddress._state=pContact->GetBusinessAddressState();
	if((char*)pContact->GetBusinessAddressStreet())
	olcontact._businessAddress._street=pContact->GetBusinessAddressStreet();
	if((char*)pContact->GetBusinessAddressPostalCode())
	olcontact._businessAddress._zip=pContact->GetBusinessAddressPostalCode();

	if((char*)pContact->GetHomeAddressCity())
	olcontact._homeAddress._city=pContact->GetHomeAddressCity();
	if((char*)pContact->GetHomeAddressCountry())
	olcontact._homeAddress._country=pContact->GetHomeAddressCountry();
	if((char*)pContact->GetHomeAddressState())
	olcontact._homeAddress._state=pContact->GetHomeAddressState();
	if((char*)pContact->GetHomeAddressStreet())
	olcontact._homeAddress._street=pContact->GetHomeAddressStreet();
	if((char*)pContact->GetHomeAddressPostalCode())
	olcontact._homeAddress._zip=pContact->GetHomeAddressPostalCode();

	if((char*)pContact->GetOtherAddressCity())
	olcontact._otherAddress._city=pContact->GetOtherAddressCity();
	if((char*)pContact->GetOtherAddressCountry())
	olcontact._otherAddress._country=pContact->GetOtherAddressCountry();
	if((char*)pContact->GetOtherAddressState())
	olcontact._otherAddress._state=pContact->GetOtherAddressState();
	if((char*)pContact->GetOtherAddressStreet())
	olcontact._otherAddress._street=pContact->GetOtherAddressStreet();
	if((char*)pContact->GetOtherAddressPostalCode())
	olcontact._otherAddress._zip=pContact->GetOtherAddressPostalCode();
}

Outlook::MAPIFolderPtr ND91Assistant::OutlookContactProvider::GeneratFolder(string groupname)
{
	Outlook::_FoldersPtr folders=_pFolder->GetFolders();
	Outlook::MAPIFolderPtr pnewfolder=NULL;
	if(groupname.empty())
		pnewfolder=_pFolder;
	else
	{
		for (int i=0;i<folders->GetCount();i++)
		{
			if(!folders->Item(i))
				continue;
			string foldername=(char*)folders->Item(i)->GetName();
			if(groupname==foldername)
			{
				pnewfolder=folders->Item(i);
				break;
			}
		}
	}
	if(!pnewfolder)
	{
		pnewfolder=folders->Add(groupname.c_str(),Outlook::olFolderContacts);
		pnewfolder->PutShowItemCount(Outlook::olShowTotalItemCount);
	}
	return pnewfolder;
}


void ND91Assistant::OutlookContactProvider::Save( bool conflict, outlookContact* olcontact)
{
	if(conflict&&FindCanlendarEvent(olcontact->_firstName,olcontact->_lastName))
		return;
	try
	{
		Outlook::_FoldersPtr folders=_pFolder->GetFolders();
		Outlook::MAPIFolderPtr pnewfolder=GeneratFolder(olcontact->_folder);
		if(pnewfolder)
		{
			IDispatchPtr pContactptr=_olptr->CreateItem(Outlook::olContactItem);
			Outlook::_ContactItemPtr pContact;
			Outlook::_ContactItem *contactitem=NULL;
			HRESULT hr=pContactptr.QueryInterface(__uuidof(Outlook::_ContactItem),&contactitem);
			pContact.Attach(contactitem);
			SaveContactItem(pContact,olcontact);
			pContact->Move(pnewfolder);
		}
	}
	catch(_com_error & ce)
	{
		LOG_ERROR(CCodeOperation::s2ws((char*)ce.Description()));
	}
}


bool ND91Assistant::OutlookContactProvider::FindCanlendarEvent( string firstName, string lastName )
{
	for (map<string,vector<outlookContact>>::iterator it=_outlookContacts.begin();it!=_outlookContacts.end();it++)
	{
		vector<outlookContact> olcontactVec=it->second;
		for (size_t i=0;i<olcontactVec.size();i++)
		{
			if (olcontactVec[i]._firstName==firstName &&olcontactVec[i]._lastName==lastName)
				return true;
		}
	}
	return false;
}

void ND91Assistant::OutlookContactProvider::SaveContactItem(Outlook::_ContactItemPtr pContact,outlookContact* olcontact)
{
	pContact->PutLastName(olcontact->_lastName.c_str());
	pContact->PutMiddleName(olcontact->_middleName.c_str());
	pContact->PutFirstName(olcontact->_firstName.c_str());
	pContact->PutNickName(olcontact->_nickName.c_str());
	pContact->PutSuffix(olcontact->_suffix.c_str());

	pContact->PutTitle(olcontact->_title.c_str());
	pContact->PutCompanyName(olcontact->_company.c_str());
	pContact->PutDepartment(olcontact->_department.c_str());
	pContact->PutJobTitle(olcontact->_jobTitle.c_str());
	pContact->PutBody(olcontact->_body.c_str());
	if (olcontact->_birthday>0)
		pContact->PutBirthday(olcontact->_birthday);

	if(CFileOperation::IsFileExist(olcontact->_picturePath.c_str()))
		pContact->AddPicture(CCodeOperation::UnicodeToUTF_8(olcontact->_picturePath).c_str());

	if(olcontact->_anniversary>0)
		pContact->PutAnniversary(olcontact->_anniversary);
	pContact->PutAssistantTelephoneNumber(olcontact->_assistantTelephoneNumber.c_str());
	pContact->PutBusinessFaxNumber(olcontact->_businessFaxNumber.c_str());
	pContact->PutBusinessTelephoneNumber(olcontact->_businessTelephoneNumber.c_str());
	pContact->PutCallbackTelephoneNumber(olcontact->_callbackTelephoneNumber.c_str());
	pContact->PutCarTelephoneNumber(olcontact->_carTelephoneNumber.c_str());
	pContact->PutHome2TelephoneNumber(olcontact->_home2TelephoneNumber.c_str());
	pContact->PutHomeFaxNumber(olcontact->_homeFaxNumber.c_str());
	pContact->PutHomeTelephoneNumber(olcontact->_homeTelephoneNumber.c_str());
	pContact->PutISDNNumber(olcontact->_ISDNNumber.c_str());
	pContact->PutMobileTelephoneNumber(olcontact->_mobileTelephoneNumber.c_str());
	pContact->PutOtherFaxNumber(olcontact->_otherFaxNumber.c_str());
	pContact->PutPagerNumber(olcontact->_pagerNumber.c_str());
	pContact->PutPrimaryTelephoneNumber(olcontact->_primaryTelephoneNumber.c_str());
	pContact->PutRadioTelephoneNumber(olcontact->_radioTelephoneNumber.c_str());
	pContact->PutTelexNumber(olcontact->_telexNumber.c_str());
	pContact->PutTTYTDDTelephoneNumber(olcontact->_TTYTDDTelephoneNumber.c_str());
	pContact->PutOtherTelephoneNumber(olcontact->_otherTelephoneNumber.c_str());
	pContact->PutEmail1Address(olcontact->_email1Address.c_str());
	pContact->PutEmail2Address(olcontact->_email2Address.c_str());
	pContact->PutEmail3Address(olcontact->_email3Address.c_str());
	pContact->PutInternetFreeBusyAddress(olcontact->_internetFreeBusyAddress.c_str());
	pContact->PutBusinessHomePage(olcontact->_businessHomePage.c_str());
	pContact->PutWebPage(olcontact->_webPage.c_str());

	pContact->PutBusinessAddressCity(olcontact->_businessAddress._city.c_str());
	pContact->PutBusinessAddressCountry(olcontact->_businessAddress._country.c_str());
	pContact->PutBusinessAddressState(olcontact->_businessAddress._state.c_str());
	pContact->PutBusinessAddressStreet(olcontact->_businessAddress._street.c_str());
	pContact->PutBusinessAddressPostalCode(olcontact->_businessAddress._zip.c_str());

	pContact->PutHomeAddressCity(olcontact->_homeAddress._city.c_str());
	pContact->PutHomeAddressCountry(olcontact->_homeAddress._country.c_str());
	pContact->PutHomeAddressState(olcontact->_homeAddress._state.c_str());
	pContact->PutHomeAddressStreet(olcontact->_homeAddress._street.c_str());
	pContact->PutHomeAddressPostalCode(olcontact->_homeAddress._zip.c_str());

	pContact->PutOtherAddressCity(olcontact->_otherAddress._city.c_str());
	pContact->PutOtherAddressCountry(olcontact->_otherAddress._country.c_str());
	pContact->PutOtherAddressState(olcontact->_otherAddress._state.c_str());
	pContact->PutOtherAddressStreet(olcontact->_otherAddress._street.c_str());
	pContact->PutOtherAddressPostalCode(olcontact->_otherAddress._zip.c_str());
	pContact->Save();
}

map<string, vector<outlookContact>>* ND91Assistant::OutlookContactProvider::GetOutLookContacts()
{
	return &_outlookContacts;
}