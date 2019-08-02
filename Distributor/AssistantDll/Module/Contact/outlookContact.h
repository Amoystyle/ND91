#pragma once
#include "Module/Contact/ContactData.h"

namespace ND91Assistant
{
	// outlook联络人地址
	struct outlookContactAddress
	{
		string _city;//城市
		string _country;//国家
		string _zip;//邮编
		string _state;//省市
		string _street;//街道
	};
	//outlook联络人数据类
	class outlookContact
	{
	public:
		outlookContact(void);
		~outlookContact(void);

		// Summary  :转为手机联络人数据
		// Returns	: void
		// Parameter: ContactData * pContact联络人数据
		// Parameter: bool isVersion20是否android 2.0
		void CovertToContactData(ContactData* pContact,bool isVersion20=false);

		// Summary  :从手机联络人转为outlook联络人
		// Returns	: void
		// Parameter: ContactData * pContact手机联络人
		void FromContactData(ContactData* pContact);

		string		_folder;//组名
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
		double		_birthday;//生日
		double      _creationTime;
		double      _lastModificationTime;

		bool        _hasPicture;
		wstring     _picturePath;//图片路径
		double      _anniversary;//纪念日
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
		string		_businessHomePage;//工作主页
		string		_webPage;//首页

		outlookContactAddress  _businessAddress;//工作地址
		outlookContactAddress  _homeAddress;//家庭住址
		outlookContactAddress  _otherAddress;//其他地址
	private:

		// Summary  : 地址塞入手机联络人
		// Returns	: void
		// Parameter: ContactData * pContact手机联络人
		// Parameter: outlookContactAddress adr   outlook联络人地址
		// Parameter: AddressTypes type 联络人地址类型
		void AddAddressToContactData(ContactData* pContact, outlookContactAddress adr, AddressTypes type);

	};

}
