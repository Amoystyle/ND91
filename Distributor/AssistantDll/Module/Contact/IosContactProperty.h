#pragma once

namespace ND91Assistant
{
    class IosContactDB;
    class ContactData;
    struct Address;
    struct Im;

    enum PropertyTypes 
    {
        PROPERTY_UNKNOW      = -1,       // 未知类型 
        PROPERTY_PHONENUMBER = 3,        // 电话号码 
        PROPERTY_EMAIL       = 4,        // 电子邮件
        PROPERTY_ADDRESS     = 5,        // 地址ADDRESS
        PROPERTY_DATE        = 12,       // 纪念日
        PROPERTY_IM          = 13,       // 即时信息
        PROPERTY_RINGTONE    = 16,       // 来电铃声,短信铃声也是这个(TextTone)短信铃声两种选择,一种是系统短信铃声,一种是无(None)
        PROPERTY_HOMEPAGE    = 22        // 个人主页
    };

    struct Property         // 联络人属性(_value保存电话，地址，IM等信息)
    {
        int           _uid;
        PropertyTypes _property;     // 属性类型
        int           _label;        // 标签ID 
        wstring       _labelKey;     // 系统默认标签
        void*         _value;        // 值
        int           _identifier;   // 标识符
        Property()
        {
            _uid        = -1;
            _property   = PROPERTY_UNKNOW;
            _value      = NULL;
        }
    };

    struct SystemLabelTypes  // 系统默认标签
    {
        string     _mobile;           // 移动电话
        string     _home;             // 家庭
        string     _work;             // 工作
        string     _main;             // 主要的
        string     _homeFax;          // 家庭传真
        string     _workFax;          // 工作传真
        string     _pager;            // 传呼机
        string     _homePage;         // 个人主页
        string     _anniversary;      // 纪念日
        string     _custom;           // 自定义
        string     _other;            // 其他
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
        
        // Summary  : 获取联络人属性(日期,号码,地址,IM)
        // Returns  : void
        // Parameter: const char * iosData[6] 表ABMultiValue所对应的值
        void GetContactMultiValue(const char* iosData[6]);

        // Summary  : 将联络人的属性(电话，地址，IM等属性)加载到person->_ios_propertyList中
        void AddPropertyList();

        // Summary  : 添加联络人属性
        // Returns  : void
        // Parameter: bool addOrUpdate 是添加联络人还是更新联络人 true: 添加 false: 更新
        void AddContactMultiValue(/*bool addOrUpdate*/);

        // Summary  : 将IOS设备上的时间转换为PC上的时间
        // Returns  : std::wstring PC上的时间(如：-510739200)
        // Parameter: const char * iosTime IOS设备上的时间(如：510739200)
        wstring GetPcTime(const char* iosTime);

        // Summary  : 将PC上的时间转换为IOS设备上的时间
        // Returns  : string IOS设备上的时间
        // Parameter: wstring pcTime PC上的时间
        string GetIosTime(wstring pcTime);

        // Summary  : 根据属性类型获取属性列表
        // Returns  : void
        // Parameter: PropertyTypes type 属性类型
        // Parameter: wstring labelKey
        // Parameter: vector<Property * > * propertyVec 获取的属性列表
        //void GetPropertyByType(PropertyTypes type, wstring labelKey, vector<Property*> *propertyVec);

    private:
        // Summary  : 获取标签的值
        // Returns  : std::wstring 标签值
        // Parameter: int labelRowId 标签id
        wstring GetLabelValue(int labelRowId);

        // Summary  : 获取属性中的电话号码值
        // Returns  : void
        // Parameter: ContactData::Property * pDto 属性结构
        // Parameter: const char * value 属性值
        void GetPropertyPhoneNumber(Property * pDto, const char* value);

        // Summary  : 获取属性中的邮箱值
        // Returns  : void
        // Parameter: ContactData::Property * pDto 属性结构
        // Parameter: const char * value 属性值
        void GetPropertyEmail(Property * pDto, const char* value);

        // Summary  : 获取属性中的主页
        // Returns  : void
        // Parameter: ContactData::Property * pDto 属性结构
        // Parameter: const char * value 属性值
        void GetPropertyHomepage(Property * pDto, const char* value);

        // Summary  : 获取属性中的铃声
        // Returns  : void
        // Parameter: ContactData::Property * pDto 属性结构
        // Parameter: const char * value 属性值
        void GetPropertyRingtone(Property * pDto, const char* value);

        // Summary  : 获取属性中的日期值
        // Returns  : void
        // Parameter: ContactData::Property * pDto 属性结构
        // Parameter: const char * value 属性值
        void GetPropertyDate(Property * pDto, const char* value);

        // Summary  : 获取属性中的地址值
        // Returns  : void
        // Parameter: ContactData::Property * pDto 属性结构
        void GetPropertyAddress(Property * pDto);

        // Summary  : 获取属性中的即时通讯账户值
        // Returns  : void
        // Parameter: ContactData::Property * pDto 属性结构
        void GetPropertyIm(Property * pDto);

        // Summary  : 将属性从属性列表移除
        // Returns  : void
        // Parameter: PropertyTypes type 要从属性列表移除的属性
        void RemovePropertyByType(PropertyTypes type);

        // Summary  : 设置电话属性
        void SetPropertyPhoneNumber();
        void SetPropertyEmail();
        void SetPropertyHomePage();
        void SetPropertyDate();
        void SetPropertyAddress();
        void SetPropertyIm();
        void SetPropertyRing();

        // Summary  : 设置属性
        // Returns  : void
        // Parameter: wstring labelKey
        // Parameter: wstring value 属性值，value为空表示删除
        void SetProperty(PropertyTypes type, wstring labelKey, wstring value);
        void SetProperty(PropertyTypes type, wstring labelKey, Address* address);
        void SetProperty(PropertyTypes type, wstring labelKey, Im* im);


        // Summary  : 获取属性标签ID(添加联络人使用)
        // Returns  : void
        // Parameter: Property* pDto 联络人属性
        void GetABMultiValueLable(Property* pDto);

        // Summary  : 获取属性identifier
        // Returns  : void 
        // Parameter: Property * pDto 联络人属性
        // Parameter: int contactId 联络人ID
        // Parameter: int * index 属性identifier
        void GetPropertyIdentifier(Property *pDto,int contactId,int *index);

        // Summary  : 添加联络人属性
        // Returns  : bool
        // Parameter: Property * pDto 属性结构
        // Parameter: int index 属性中的identifier值
		bool AddPropertyInformation(Property *pDto,int index);

        // Summary  : 添加联络人IM和address属性语句
        // Returns  : void
        // Parameter: Property * pDto 联络人属性
		void AddImAndAddressProperty(Property* pDto);

        // Summary  : 添加属性IM
        // Returns  : void
        // Parameter: Property * pDto 属性结构
        void AddPropertyIm(Property* pDto);

        // Summary  : 添加属性ADDRESS
        // Returns  : void
        // Parameter: Property * pDto 属性结构
        void AddPropertyAddress(Property* pDto);

        // Summary  : 往表ABMultiValueEntry添加数据
        // Returns  : void
        // Parameter: wstring value 往表ABMultiValueEntry添加的值
        // Parameter: wstring key 往表ABMultiValueEntryKey添加的值
        // Parameter: int propertyUid 属性UID
        void InsertIntoABMultiValueEntry(wstring value,wstring key,int propertyUid);

        // Summary  : 更新ABPhoneLastFour表
        // Returns  : void
        // Parameter: Property * pDto 联络人属性
        void UpdateABPhoneLastFour(Property* pDto);

    private:
        SystemLabelTypes    _systemLabelTypes;      // 系统默认标签
        IosContactDB *      _contactDB;             // 联络人数据库
        vector<Property*>   _ios_propertyList;      // 联络人属性列表
        ContactData*        _person;                // 联络人
    };
}
