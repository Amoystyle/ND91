#pragma once

namespace ND91Assistant
{
    class IosContactDB;
    class ContactData;
    struct Address;
    struct Im;

    enum PropertyTypes 
    {
        PROPERTY_UNKNOW      = -1,       // δ֪���� 
        PROPERTY_PHONENUMBER = 3,        // �绰���� 
        PROPERTY_EMAIL       = 4,        // �����ʼ�
        PROPERTY_ADDRESS     = 5,        // ��ַADDRESS
        PROPERTY_DATE        = 12,       // ������
        PROPERTY_IM          = 13,       // ��ʱ��Ϣ
        PROPERTY_RINGTONE    = 16,       // ��������,��������Ҳ�����(TextTone)������������ѡ��,һ����ϵͳ��������,һ������(None)
        PROPERTY_HOMEPAGE    = 22        // ������ҳ
    };

    struct Property         // ����������(_value����绰����ַ��IM����Ϣ)
    {
        int           _uid;
        PropertyTypes _property;     // ��������
        int           _label;        // ��ǩID 
        wstring       _labelKey;     // ϵͳĬ�ϱ�ǩ
        void*         _value;        // ֵ
        int           _identifier;   // ��ʶ��
        Property()
        {
            _uid        = -1;
            _property   = PROPERTY_UNKNOW;
            _value      = NULL;
        }
    };

    struct SystemLabelTypes  // ϵͳĬ�ϱ�ǩ
    {
        string     _mobile;           // �ƶ��绰
        string     _home;             // ��ͥ
        string     _work;             // ����
        string     _main;             // ��Ҫ��
        string     _homeFax;          // ��ͥ����
        string     _workFax;          // ��������
        string     _pager;            // ������
        string     _homePage;         // ������ҳ
        string     _anniversary;      // ������
        string     _custom;           // �Զ���
        string     _other;            // ����
        SystemLabelTypes()
        {
            _mobile       =  "_$!<Mobile>!$_";
            _home         =  "_$!<Home>!$_";		
            _work         =  "_$!<Work>!$_";
            _main         =  "_$!<Main>!$_";
            _homeFax      =  "_$!<HomeFAX>!$_";
            _workFax      =  "_$!<WorkFAX>!$_";
            _pager        =  "_$!<Pager>!$_";
            _homePage     =  "_$!<HomePage>!$_";
            _anniversary  =  "_$!<Anniversary>!$_";
            _custom       =  "_$!<Custom>!$_";
            _other        =  "_$!<Other>!$_";
        }
    };

    class IosContactProperty
    {
    public:
        IosContactProperty(ContactData* person);
        ~IosContactProperty();

        void SetContactDB(IosContactDB *contactDB) { _contactDB = contactDB; }
        
        // Summary  : ��ȡ����������(����,����,��ַ,IM)
        // Returns  : void
        // Parameter: const char * iosData[6] ��ABMultiValue����Ӧ��ֵ
        void GetContactMultiValue(const char* iosData[6]);

        // Summary  : �������˵�����(�绰����ַ��IM������)���ص�person->_ios_propertyList��
        void AddPropertyList();

        // Summary  : �������������
        // Returns  : void
        // Parameter: bool addOrUpdate ����������˻��Ǹ��������� true: ��� false: ����
        void AddContactMultiValue(/*bool addOrUpdate*/);

        // Summary  : ��IOS�豸�ϵ�ʱ��ת��ΪPC�ϵ�ʱ��
        // Returns  : std::wstring PC�ϵ�ʱ��(�磺-510739200)
        // Parameter: const char * iosTime IOS�豸�ϵ�ʱ��(�磺510739200)
        wstring GetPcTime(const char* iosTime);

        // Summary  : ��PC�ϵ�ʱ��ת��ΪIOS�豸�ϵ�ʱ��
        // Returns  : string IOS�豸�ϵ�ʱ��
        // Parameter: wstring pcTime PC�ϵ�ʱ��
        string GetIosTime(wstring pcTime);

        // Summary  : �����������ͻ�ȡ�����б�
        // Returns  : void
        // Parameter: PropertyTypes type ��������
        // Parameter: wstring labelKey
        // Parameter: vector<Property * > * propertyVec ��ȡ�������б�
        //void GetPropertyByType(PropertyTypes type, wstring labelKey, vector<Property*> *propertyVec);

    private:
        // Summary  : ��ȡ��ǩ��ֵ
        // Returns  : std::wstring ��ǩֵ
        // Parameter: int labelRowId ��ǩid
        wstring GetLabelValue(int labelRowId);

        // Summary  : ��ȡ�����еĵ绰����ֵ
        // Returns  : void
        // Parameter: ContactData::Property * pDto ���Խṹ
        // Parameter: const char * value ����ֵ
        void GetPropertyPhoneNumber(Property * pDto, const char* value);

        // Summary  : ��ȡ�����е�����ֵ
        // Returns  : void
        // Parameter: ContactData::Property * pDto ���Խṹ
        // Parameter: const char * value ����ֵ
        void GetPropertyEmail(Property * pDto, const char* value);

        // Summary  : ��ȡ�����е���ҳ
        // Returns  : void
        // Parameter: ContactData::Property * pDto ���Խṹ
        // Parameter: const char * value ����ֵ
        void GetPropertyHomepage(Property * pDto, const char* value);

        // Summary  : ��ȡ�����е�����
        // Returns  : void
        // Parameter: ContactData::Property * pDto ���Խṹ
        // Parameter: const char * value ����ֵ
        void GetPropertyRingtone(Property * pDto, const char* value);

        // Summary  : ��ȡ�����е�����ֵ
        // Returns  : void
        // Parameter: ContactData::Property * pDto ���Խṹ
        // Parameter: const char * value ����ֵ
        void GetPropertyDate(Property * pDto, const char* value);

        // Summary  : ��ȡ�����еĵ�ֵַ
        // Returns  : void
        // Parameter: ContactData::Property * pDto ���Խṹ
        void GetPropertyAddress(Property * pDto);

        // Summary  : ��ȡ�����еļ�ʱͨѶ�˻�ֵ
        // Returns  : void
        // Parameter: ContactData::Property * pDto ���Խṹ
        void GetPropertyIm(Property * pDto);

        // Summary  : �����Դ������б��Ƴ�
        // Returns  : void
        // Parameter: PropertyTypes type Ҫ�������б��Ƴ�������
        void RemovePropertyByType(PropertyTypes type);

        // Summary  : ���õ绰����
        void SetPropertyPhoneNumber();
        void SetPropertyEmail();
        void SetPropertyHomePage();
        void SetPropertyDate();
        void SetPropertyAddress();
        void SetPropertyIm();
        void SetPropertyRing();

        // Summary  : ��������
        // Returns  : void
        // Parameter: wstring labelKey
        // Parameter: wstring value ����ֵ��valueΪ�ձ�ʾɾ��
        void SetProperty(PropertyTypes type, wstring labelKey, wstring value);
        void SetProperty(PropertyTypes type, wstring labelKey, Address* address);
        void SetProperty(PropertyTypes type, wstring labelKey, Im* im);


        // Summary  : ��ȡ���Ա�ǩID(���������ʹ��)
        // Returns  : void
        // Parameter: Property* pDto ����������
        void GetABMultiValueLable(Property* pDto);

        // Summary  : ��ȡ����identifier
        // Returns  : void 
        // Parameter: Property * pDto ����������
        // Parameter: int contactId ������ID
        // Parameter: int * index ����identifier
        void GetPropertyIdentifier(Property *pDto,int contactId,int *index);

        // Summary  : �������������
        // Returns  : bool
        // Parameter: Property * pDto ���Խṹ
        // Parameter: int index �����е�identifierֵ
		bool AddPropertyInformation(Property *pDto,int index);

        // Summary  : ���������IM��address�������
        // Returns  : void
        // Parameter: Property * pDto ����������
		void AddImAndAddressProperty(Property* pDto);

        // Summary  : �������IM
        // Returns  : void
        // Parameter: Property * pDto ���Խṹ
        void AddPropertyIm(Property* pDto);

        // Summary  : �������ADDRESS
        // Returns  : void
        // Parameter: Property * pDto ���Խṹ
        void AddPropertyAddress(Property* pDto);

        // Summary  : ����ABMultiValueEntry�������
        // Returns  : void
        // Parameter: wstring value ����ABMultiValueEntry��ӵ�ֵ
        // Parameter: wstring key ����ABMultiValueEntryKey��ӵ�ֵ
        // Parameter: int propertyUid ����UID
        void InsertIntoABMultiValueEntry(wstring value,wstring key,int propertyUid);

        // Summary  : ����ABPhoneLastFour��
        // Returns  : void
        // Parameter: Property * pDto ����������
        void UpdateABPhoneLastFour(Property* pDto);

    private:
        SystemLabelTypes    _systemLabelTypes;      // ϵͳĬ�ϱ�ǩ
        IosContactDB *      _contactDB;             // ���������ݿ�
        vector<Property*>   _ios_propertyList;      // �����������б�
        ContactData*        _person;                // ������
    };
}
