#pragma once

#pragma warning(disable:4099)
#pragma warning(disable:4278)

#import "libid:2DF8D04C-5BFA-101B-BDE5-00AA0044DE52" rename("RGB","MSRGB")
//using namespace office;
//#import "libid:00062FFF-0000-0000-C000-000000000046" rename("EOF","EOFile")
//#import "lib/mso.dll" named_guids
#import "lib/msoutl.olb"

#pragma warning(default:4099)
#pragma warning(default:4278)

namespace ND91Assistant
{

	//outlook�����˹�����
	class outlookContact;
	class OutlookContactProvider
	{
	public:
		OutlookContactProvider(void);
		~OutlookContactProvider(void);

		// �����ûװoutlook
		bool CheckExsitOutlook();

		//��ʼ�������outlook����ʵ��
		bool Init();

		// ����outlook������
		long LoadOutLookContacts();

		// Summary  : ��ӵ���������
		// Returns	: void
		// Parameter: bool conflict�Ѵ��ڵ��Ƿ����
		// Parameter: outlookContact * olcontactҪ��ӵ�������
		void Save(bool conflict, outlookContact* olcontact);

		// ��ȡoutlook����������
		map<string, vector<outlookContact>>* GetOutLookContacts();

		//����������Ⱥ��
		Outlook::MAPIFolderPtr GeneratFolder(string groupname);

	private:
		// Summary  :����ָ��Ⱥ���������
		// Returns	: void
		// Parameter: Outlook::MAPIFolderPtr pFolder ������Ⱥ��
		void Load(Outlook::MAPIFolderPtr pFolder);

		// Summary  :������������������
		// Returns	: void
		// Parameter: Outlook::_ContactItemPtr pContact
		// Parameter: outlookContact & olcontact
		void PaserContactItem(Outlook::_ContactItemPtr pContact, outlookContact& olcontact);

		// Summary  :����Ƿ����ͬ����������
		// Returns	: bool
		// Parameter: string firstName
		// Parameter: string lastName
		bool FindCanlendarEvent(string firstName, string lastName);

		// Summary  :���浥����������Ϣ
		// Returns	: void
		// Parameter: Outlook::_ContactItemPtr pContact
		// Parameter: outlookContact * olcontact
		void SaveContactItem(Outlook::_ContactItemPtr pContact,outlookContact* olcontact);

	private:

		Outlook::_ApplicationPtr	_olptr;//����ʵ��
		Outlook::_NameSpacePtr		_olspaceprt;//�����ռ�
		Outlook::MAPIFolderPtr		_pFolder;//Ĭ��Ⱥ��

		map<string, vector<outlookContact>> _outlookContacts;	//��õ����������ݣ�keyΪ������
	};

}