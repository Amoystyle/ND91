#pragma once
#include "Module/Contact/ContactData.h"

namespace ND91Assistant
{
	// outlook�����˵�ַ
	struct outlookContactAddress
	{
		string _city;//����
		string _country;//����
		string _zip;//�ʱ�
		string _state;//ʡ��
		string _street;//�ֵ�
	};
	//outlook������������
	class outlookContact
	{
	public:
		outlookContact(void);
		~outlookContact(void);

		// Summary  :תΪ�ֻ�����������
		// Returns	: void
		// Parameter: ContactData * pContact����������
		// Parameter: bool isVersion20�Ƿ�android 2.0
		void CovertToContactData(ContactData* pContact,bool isVersion20=false);

		// Summary  :���ֻ�������תΪoutlook������
		// Returns	: void
		// Parameter: ContactData * pContact�ֻ�������
		void FromContactData(ContactData* pContact);

		string		_folder;//����
		string		_lastName;
		string		_middleName;
		string		_firstName;
		string		_nickName;
		string		_suffix;
		string		_title;//prefix
		string		_company;//Organization
		string		_department;
		string		_jobTitle;
		string		_body;//Note
		double		_birthday;//����
		double      _creationTime;
		double      _lastModificationTime;

		bool        _hasPicture;
		wstring     _picturePath;//ͼƬ·��
		double      _anniversary;//������
		string		_assistantTelephoneNumber;
		string		_businessFaxNumber;
		string		_business2TelephoneNumber;
		string		_businessTelephoneNumber;
		string		_callbackTelephoneNumber;
		string		_carTelephoneNumber;
		string		_home2TelephoneNumber;
		string		_homeFaxNumber;
		string		_homeTelephoneNumber;
		string		_ISDNNumber;
		string		_mobileTelephoneNumber;
		string		_otherFaxNumber;
		string		_pagerNumber;
		string		_primaryTelephoneNumber;
		string		_radioTelephoneNumber;
		string		_telexNumber;
		string		_TTYTDDTelephoneNumber;
		string		_otherTelephoneNumber;
		string		_email1Address;
		string		_email2Address;
		string		_email3Address;
		string		_internetFreeBusyAddress;
		string		_businessHomePage;//������ҳ
		string		_webPage;//��ҳ

		outlookContactAddress  _businessAddress;//������ַ
		outlookContactAddress  _homeAddress;//��ͥסַ
		outlookContactAddress  _otherAddress;//������ַ
	private:

		// Summary  : ��ַ�����ֻ�������
		// Returns	: void
		// Parameter: ContactData * pContact�ֻ�������
		// Parameter: outlookContactAddress adr   outlook�����˵�ַ
		// Parameter: AddressTypes type �����˵�ַ����
		void AddAddressToContactData(ContactData* pContact, outlookContactAddress adr, AddressTypes type);

	};

}
