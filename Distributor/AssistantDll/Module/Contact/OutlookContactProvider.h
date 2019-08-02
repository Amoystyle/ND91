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

	//outlook联络人功能类
	class outlookContact;
	class OutlookContactProvider
	{
	public:
		OutlookContactProvider(void);
		~OutlookContactProvider(void);

		// 检查有没装outlook
		bool CheckExsitOutlook();

		//初始化，获得outlook程序实例
		bool Init();

		// 加载outlook联络人
		long LoadOutLookContacts();

		// Summary  : 添加单个联络人
		// Returns	: void
		// Parameter: bool conflict已存在的是否不添加
		// Parameter: outlookContact * olcontact要添加的联络人
		void Save(bool conflict, outlookContact* olcontact);

		// 获取outlook联络人数据
		map<string, vector<outlookContact>>* GetOutLookContacts();

		//生成联络人群组
		Outlook::MAPIFolderPtr GeneratFolder(string groupname);

	private:
		// Summary  :加载指定群组的联络人
		// Returns	: void
		// Parameter: Outlook::MAPIFolderPtr pFolder 联络人群组
		void Load(Outlook::MAPIFolderPtr pFolder);

		// Summary  :解析单个联络人数据
		// Returns	: void
		// Parameter: Outlook::_ContactItemPtr pContact
		// Parameter: outlookContact & olcontact
		void PaserContactItem(Outlook::_ContactItemPtr pContact, outlookContact& olcontact);

		// Summary  :检查是否存在同名的联络人
		// Returns	: bool
		// Parameter: string firstName
		// Parameter: string lastName
		bool FindCanlendarEvent(string firstName, string lastName);

		// Summary  :保存单个联络人信息
		// Returns	: void
		// Parameter: Outlook::_ContactItemPtr pContact
		// Parameter: outlookContact * olcontact
		void SaveContactItem(Outlook::_ContactItemPtr pContact,outlookContact* olcontact);

	private:

		Outlook::_ApplicationPtr	_olptr;//程序实例
		Outlook::_NameSpacePtr		_olspaceprt;//命名空间
		Outlook::MAPIFolderPtr		_pFolder;//默认群组

		map<string, vector<outlookContact>> _outlookContacts;	//获得的联络人数据，key为分组名
	};

}