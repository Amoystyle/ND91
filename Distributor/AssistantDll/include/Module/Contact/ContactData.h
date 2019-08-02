#pragma once

#pragma warning(disable : 4251)

#include "Module/BaseData.h"

namespace ND91Assistant
{
    // ���ػ������Ľṹ��ʶ
    enum MethodTypes
    {
        METHOD_EMAIL            = 1,    // �ʼ���Ϣ
        METHOD_ADDRESS          = 2,    // ��ַ��Ϣ
        METHOD_IM               = 3,    // ��ʱͨ��Ϣ
        METHOD_NICKNAME         = 4,    // �ǳ�
        METHOD_WEBSITE          = 5,    // ��ҳ
        METHOD_COMMENT          = 6,    // ��ע
        METHOD_PHONE            = 7,    // �绰
        METHOD_ORGANIZATION     = 8,    // ��֯
        METHOD_NAME             = 9,    // ����
        METHOD_PHOTO            = 10,   // ͼƬ
        METHOD_GROUPMEMBERSHIP  = 11,   // �������
        METHOD_PEOPLE           = 12,   // ���������Ϣ
        METHOD_GROUP            = 13,   // �����˷���
        METHOD_ACCOUNT          = 14,   // �������˻�
        METHOD_RAWCONTACT       = 15,
        METHOD_ENDFLAG          = -1,   // �������
        METHOD_REPEAT           = -2    // ������ȡ
    };

    // ����״̬�����漸���ṹ��ʹ�ã�
    enum ActionState    
    {
        STATE_NOCHANGE,         // �޸ı�
        STATE_ADD,              // ����
        STATE_UPDATE,           // ����
        STATE_DEL               // ɾ��
    };

    // �绰��������
    enum PhoneTypes
    {
        PHONE_UNKNOW,       // δ֪
        PHONE_CUSTOM,       // �Զ���
        PHONE_OTHER,        // ����
        PHONE_HOME,         // ��ͥ
        PHONE_WORK,         // ����

        PHONE_MOBILE,       // �ƶ��绰
        PHONE_IPHONES,      // iPhone
        PHONE_MAIN,         // ��Ҫ
        PHONE_HOMEFAX,      // ��ͥ����
        PHONE_WORKFAX,      // ��������
        PHONE_OTHERFAX,     // ��������
        PHONE_PAGER,        // ����
    };

    // �����ʼ�����
    enum EmailTypes
    {
        EMAIL_UNKNOW,       // δ֪
        EMAIL_CUSTOM,       // �Զ���
        EMAIL_OTHER,        // ����
        EMAIL_HOME,         // ��ͥ
        EMAIL_WORK,         // ����

        EMAIL_MOBILE,       // �ֻ�
    };

    // ��ʱͨѶ������
    enum IMTypesName
    {
        IM_NAMEUNKNOW,      // δ֪
        IM_CUSTOMSERVICE,   // �Զ�����
        IM_ATM,             // AIM
        IM_WINDOWSLIVE,     // Windows Live
        IM_YAHOO,           // �Ż�
        IM_SKYPE,           // Skype
        IM_QQ,              // QQ
        IM_GOOLETALK,       // GoogleTalk
        IM_ICQ,             // ICQ
        IM_JABBER,          // Jabber
        IM_FETION,          // ����
        IM_MSN,             // MSN
        IM_FACEBOOK,        // facebook
        IM_GADUGADU,        // gadu-gadu
    };

    enum IMTypes
    {
        IM_UNKNOW,          // δ֪
        IM_CUSTOM,          // �Զ���
        IM_OTHER,           // ����
        IM_HOME,            // ��ͥ
        IM_WORK,            // ����
    };

    // ͨѶ��ַ����
    enum AddressTypes
    {
        ADDRESS_UNKNOW,     // δ֪
        ADDRESS_CUSTOM,     // �Զ���
        ADDRESS_OTHER,      // ����
        ADDRESS_HOME,       // ��ͥ
        ADDRESS_WORK,       // ����
    };

    // ��֯����
    enum OrganizationTypes
    {
        ORGANIZATION_UNKNOW,        // δ֪
        ORGANIZATION_CUSTOM,        // �Զ���
        ORGANIZATION_OTHER,         // ����
        ORGANIZATION_HOME,          // ��ͥ
        ORGANIZATION_WORK,          // ����
    };

    enum HomePageTypes
    {
        HOMEPAGE_UNKNOW,        // δ֪
        HOMEPAGE_CUSTOM,        // �Զ���
        HOMEPAGE_OTHER,         // ����
        HOMEPAGE_HOME,          // ��ͥ
        HOMEPAGE_WORK,          // ����

		HOMEPAGE_HOMEPAGE,      // ��ҳ
    };

    enum DayTypes
    {
        DAY_UNKNOW,         // δ֪
        DAY_CUSTOM,         // �Զ���
        DAY_OTHER,          // ����
        DAY_HOME,           // ��ͥ
        DAY_WORK,           // ����
        
		DAY_DAY,	        // ������
    };

    enum RelatedTypes
    {
        RELATED_UNKNOW,         // δ֪
        RELATED_CUSTOM,         // �Զ���
        RELATED_OTHER,          // ����

        RELATED_FATHER,         // ����
        RELATED_MOTHER,         // ĸ��
        RELATED_PARENT,         // ��/ĸ��
        RELATED_BROTHER,        // �ֵ�
        RELATED_SISTER,         // ����
        RELATED_CHILD,          // ��Ů
        RELATED_FRIEND,         // ����
        RELATED_SPOUSE,         // ��ż
        RELATED_PARTNER,        // �
        RELATED_ASSISTANT,      // ����
        RELATED_MANAGER,        // ��˾
    };

    enum Category
    {
        Category_Unknow          = -1,      // δ֪���� 
        Category_PhoneNumber     = 3,       // �绰���� 
        Category_Email           = 4,       // �����ʼ�
        Category_IM              = 13,      // ��ʱ��Ϣ
        Category_StreetAddress   = 5,       // ��ַADDRESS
        Category_URL             = 22,      // ������ҳ
        Category_Date            = 12,      // ������
        Category_Relates         = 0x12     // �����ϵ��
    };

    // ios4.0�̼���ʼ.������ͷ��ֳ���������,������ͼ��ABThumbnailImage��
    // format�ֶ���ֵ��0,1,4,����0��Ӧ320*320,1��Ӧ320*480,4 ĿǰΪ��
    // iPhone4 �ֲ�һ����.����0 ��Ӧ����640*640,4��Ӧ����320*480,1ĿǰΪ�� 
    enum PhotoFormats            // ͼƬ��ʽ����
    {
        THUMBNAIL_PHOTO   = 0,            // ����ͼ(320*320),iPhone4 ��Ӧ640*640(2x)
        MEDIUM_PHOTO      = 1,            // �е�����ͼ(320*480),iPhone4 ��320*426��ֵ
        ORIGINAL_PHOTO    = 2             // Դͼ
    };

    // ��ϵ����Person,����Company ��������,��plistЭ�������display as �ֶζ�Ӧ��ֵ,
    // Ӧ�ö�Ӧ���ݿ������ABPerson���kind
    enum ContactType
    {
        Person,
        Company,
    };

    struct CLASSINDLL_CLASS_DECL Name    // ����
    {
        int           _id;
        ActionState   _state;           // �ṹ������״̬
        wstring       _displayName;     
        wstring       _givenName;
        wstring       _familyName;
        wstring       _middleName;
        string        _ios_firstPhonetic;          // ����ƴ��
        string        _ios_middlePhonetic;         // �м���ƴ��
        string        _ios_lastPhonetic;           // ����ƴ��
        int           _ios_kind;                   // ���
        wstring       _ios_nickName;               // �ǳ�
        string        _ios_prefix;                 // Ӣ�ĳ�ν
        wstring       _ios_suffix;                 // ���ĳ�ν
        Name()
        {
            _id     = 0;
            _state  = STATE_NOCHANGE;
        }
    };

    struct CLASSINDLL_CLASS_DECL Phone
    {
        string        _key;             // (ios)�豸���͹�����Key,��categoryid/mainid/count���  (android)��ʾID
        ActionState   _state;           // �ṹ������״̬
        PhoneTypes    _type;            // �绰��������
        wstring       _label;           // �Զ����ǩ��
        wstring       _number;          // �绰����
        Phone()
        {
            _state  = STATE_NOCHANGE;
            _type   = PHONE_UNKNOW;                   
        }
    };

    struct CLASSINDLL_CLASS_DECL Email
    {
        string        _key;             // (ios)�豸���͹�����Key,��categoryid/mainid/count���  (android)��ʾID
        ActionState   _state;           // �ṹ������״̬
        EmailTypes    _type;            // �����ʼ�����
        wstring       _label;           // �Զ����ǩ��
        wstring       _emailAccount;    // �����ʼ��˻���
        Email()
        {
            _state  = STATE_NOCHANGE;
            _type   = EMAIL_UNKNOW;     
        }
    };

    struct CLASSINDLL_CLASS_DECL Im     // ��ʱͨѶ�˻�
    {
        string        _key;             // (ios)�豸���͹�����Key,��categoryid/mainid/count���  (android)��ʾID
        ActionState   _state;           // �ṹ������״̬
        IMTypes       _type;            // ��ʱͨѶ�˻�����
        IMTypesName   _typeName;        // ��ʱͨѶ�˻�������
            wstring       _service;         // ios��ʱͨѶ�˻�������(DB��ʽ��Ҫ)
        wstring       _imAccount;       // ��ʱͨѶ�˻���
        wstring       _label;           // �Զ����ǩ��
        Im()
        {
            _state      = STATE_NOCHANGE;    
            _type       = IM_UNKNOW;
            _typeName   = IM_NAMEUNKNOW;
        }
    };

    struct CLASSINDLL_CLASS_DECL Address         // ͨѶ��ַ
    {
        string        _key;                // (ios)�豸���͹�����Key,��categoryid/mainid/count���  (android)��ʾID
        ActionState   _state;              // �ṹ������״̬
        AddressTypes  _type;               // ͨѶ��������    
        wstring       _label;              // ��ַ���ͱ�ǩ
        wstring       _province;           // ʡ
        wstring       _city;               // ��
        wstring       _street;             // �ֵ�
        wstring       _zipCode;            // �ʱ�
        wstring       _displayAddress;     // ��ַ
        wstring       _countryCode;        // ���Ҵ���
        wstring       _country;            // ����
        Address()
        {
            _state  = STATE_NOCHANGE;
            _type   = ADDRESS_UNKNOW;                
        }
    };

    struct CLASSINDLL_CLASS_DECL Organization    // ��֯ 
    {
        int               _id;       // ��֯ID
        ActionState       _state;    // �ṹ������״̬
        OrganizationTypes _type;     // ��֯����
        wstring           _label;    // ��֯���ͱ�ǩ
        wstring           _name;     // ��֯��
        wstring           _address;  // ��֯��ַ
        Organization()
        {
            _id     = 0;
            _state  = STATE_NOCHANGE;
            _type   = ORGANIZATION_UNKNOW;     
        }
    };

    struct CLASSINDLL_CLASS_DECL Comment      // ��ע
    {
        int          _id;
        ActionState  _state;                // �ṹ������״̬
        wstring      _content;              // ��ע����
        Comment()
        {
            _id     = 0;
            _state  = STATE_NOCHANGE;
        }
    };

    struct CLASSINDLL_CLASS_DECL HomePage       // ��ҳ
    {
        string        _key;             // �豸���͹�����Key���categoryid/mainid/count ���
        ActionState   _state;           // �ṹ������״̬
        HomePageTypes  _type;           // ����
        wstring       _name;            // ����
        wstring       _label;           // �Զ����ǩ��
        HomePage()
        {
            _state  = STATE_NOCHANGE;
            _type   = HOMEPAGE_UNKNOW;
        }
    };

    struct CLASSINDLL_CLASS_DECL Day            // ������
    {
        string          _key;             // �豸���͹�����Key���categoryid/mainid/count ���
        ActionState     _state;           // �ṹ������״̬
        DayTypes        _type;            // ����
        wstring         _name;            // ����
        wstring         _label;           // �Զ����ǩ��
        Day()
        {
            _state  = STATE_NOCHANGE;
            _type   = DAY_UNKNOW;
        }
    };

    struct CLASSINDLL_CLASS_DECL Related        // �����ϵ��
    {
        string          _key;             // �豸���͹�����Key���categoryid/mainid/count ���
        ActionState     _state;           // �ṹ������״̬
        RelatedTypes    _type;            // ����
        wstring         _name;            // ����
        wstring         _label;           // �Զ����ǩ��
        Related()
        {
            _state  = STATE_NOCHANGE;
            _type   = RELATED_UNKNOW;
        }
    };

    struct CLASSINDLL_CLASS_DECL Photo
    {
        int			    _photoId;			// ͷ��id
        ActionState     _state;             // �ṹ������״̬
        wstring		    _strSavePath;		// ������ͼƬҪ���浽��PC�е�·��(Դͼ)
        wstring		    _photoUri;			// ͼƬuri��������OMSϵͳ��
        wstring		    _photoPath;			// ͼƬ·����PhotoUriת�����ã����ػ�ת����������OMSϵͳ��

        //wstring		    _mediumPath;		// ������ͼƬҪ���浽��PC�е�·��(�е�����ͼ)
        //wstring		    _thumbnailPath;		// ������ͼƬҪ���浽��PC�е�·��(����ͼ)
        Photo()
        {
            _photoId        = 0;
            _state          = STATE_NOCHANGE;
        }
    };

    class ContactGroupData;
    //class IosContactProperty;
    
    // ��ϵ��������
    class CLASSINDLL_CLASS_DECL ContactData : public BaseData
    {
        //friend class IosContactProperty;
    public:
        ContactData();
        ContactData(const char* iosData[21]);
        ContactData(string identifier, map<string,void*> entity, ContactType contactType);
        virtual ~ContactData();

        // Summary  : ������ϵ������
        // Returns  : void
        // Parameter: map<string, void*>& infoss ��������ϵ������
        void ConvertToDictInfos(map<string, void*>& infoss);

    public:
        int     _contactId;					// ������ID
        string  _identifier;                // ������ID
        Name    _sName;						// ���ֽṹ��
        Comment _sComment;					// ��ע�ṹ��

        wstring _companyName;               // ��˾
        wstring _department;				// ����
        wstring _birthday;					// ��������
        wstring _jobTitle;					// ְ��

        wstring _accountName;				// �����˻�����
        wstring _accountType;				// �����˻�����
        bool    _favorite;					// �Ƿ��ղ�
        wstring _ringtone;					// ��������
        wstring _textTone;                  // ��������
       
        // ������ͷ��
        bool    _photo_hasLoad;
        Photo   _headPhoto;

        vector<Phone*>            _phoneNumberList;         // �绰�����б�
        vector<Email*>            _emailList;               // �����ʼ��б�
        vector<Address*>          _addressList;             // ͨѶ��ַ�б�
        vector<Im*>               _imList;                  // ��ʱͨͨѶ�˻��б�
        vector<Organization*>     _organizationList;        // ��֯�б�
        
        vector<HomePage*>         _homePageList;            // ��ҳ�б�
        vector<Day*>              _dateList;                // �������б�
        vector<Related*>          _relatedList;             // �����ϵ���б�5.0����У�
        //IosContactProperty*       _ios_property;            // ����������
        vector<int>               _groupIdList;             // ������ID�б�

		vector<wstring>           _tempGroupNames;          // ���������ʱ����ķ�������

        static long     _idIncrementer;     // ��ϵ����PC�ϵ�ID
        string          _recordEntityName;  // "com.apple.contacts.Contact"
        ContactType     _contactType;       // IOS��ϵ������

    public:
        string ToBuffer();
        wstring GetPhoneNums();
		void AddAndroidPhoneNUmber(string telnumber, PhoneTypes phonetype);
		void AddAndroidEmail(string emailaccount, EmailTypes emailtype);
		void AddHomepage(string name,HomePageTypes pagetype);

    private:
        void FormatBirthday(void* birthday);
        void FormatHeadPhoto(void* image);

        // Summary  : ������ϵ��ͷ������
        // Returns  : void*
        void* BuildHeadPhoto(); 
    };
}
