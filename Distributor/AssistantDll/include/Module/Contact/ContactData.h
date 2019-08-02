#pragma once

#pragma warning(disable : 4251)

#include "Module/BaseData.h"

namespace ND91Assistant
{
    // 与守护交互的结构标识
    enum MethodTypes
    {
        METHOD_EMAIL            = 1,    // 邮件信息
        METHOD_ADDRESS          = 2,    // 地址信息
        METHOD_IM               = 3,    // 即时通信息
        METHOD_NICKNAME         = 4,    // 昵称
        METHOD_WEBSITE          = 5,    // 网页
        METHOD_COMMENT          = 6,    // 备注
        METHOD_PHONE            = 7,    // 电话
        METHOD_ORGANIZATION     = 8,    // 组织
        METHOD_NAME             = 9,    // 姓名
        METHOD_PHOTO            = 10,   // 图片
        METHOD_GROUPMEMBERSHIP  = 11,   // 分组关联
        METHOD_PEOPLE           = 12,   // 联络基本信息
        METHOD_GROUP            = 13,   // 联络人分组
        METHOD_ACCOUNT          = 14,   // 联络人账户
        METHOD_RAWCONTACT       = 15,
        METHOD_ENDFLAG          = -1,   // 结束标记
        METHOD_REPEAT           = -2    // 继续读取
    };

    // 数据状态（下面几个结构体使用）
    enum ActionState    
    {
        STATE_NOCHANGE,         // 无改变
        STATE_ADD,              // 新增
        STATE_UPDATE,           // 更新
        STATE_DEL               // 删除
    };

    // 电话号码类型
    enum PhoneTypes
    {
        PHONE_UNKNOW,       // 未知
        PHONE_CUSTOM,       // 自定义
        PHONE_OTHER,        // 其他
        PHONE_HOME,         // 家庭
        PHONE_WORK,         // 工作

        PHONE_MOBILE,       // 移动电话
        PHONE_IPHONES,      // iPhone
        PHONE_MAIN,         // 主要
        PHONE_HOMEFAX,      // 家庭传真
        PHONE_WORKFAX,      // 工作传真
        PHONE_OTHERFAX,     // 其他传真
        PHONE_PAGER,        // 传呼
    };

    // 电子邮件类型
    enum EmailTypes
    {
        EMAIL_UNKNOW,       // 未知
        EMAIL_CUSTOM,       // 自定义
        EMAIL_OTHER,        // 其它
        EMAIL_HOME,         // 家庭
        EMAIL_WORK,         // 工作

        EMAIL_MOBILE,       // 手机
    };

    // 即时通讯类型名
    enum IMTypesName
    {
        IM_NAMEUNKNOW,      // 未知
        IM_CUSTOMSERVICE,   // 自定服务
        IM_ATM,             // AIM
        IM_WINDOWSLIVE,     // Windows Live
        IM_YAHOO,           // 雅虎
        IM_SKYPE,           // Skype
        IM_QQ,              // QQ
        IM_GOOLETALK,       // GoogleTalk
        IM_ICQ,             // ICQ
        IM_JABBER,          // Jabber
        IM_FETION,          // 飞信
        IM_MSN,             // MSN
        IM_FACEBOOK,        // facebook
        IM_GADUGADU,        // gadu-gadu
    };

    enum IMTypes
    {
        IM_UNKNOW,          // 未知
        IM_CUSTOM,          // 自定义
        IM_OTHER,           // 其它
        IM_HOME,            // 家庭
        IM_WORK,            // 工作
    };

    // 通讯地址类型
    enum AddressTypes
    {
        ADDRESS_UNKNOW,     // 未知
        ADDRESS_CUSTOM,     // 自定义
        ADDRESS_OTHER,      // 其它
        ADDRESS_HOME,       // 家庭
        ADDRESS_WORK,       // 工作
    };

    // 组织类型
    enum OrganizationTypes
    {
        ORGANIZATION_UNKNOW,        // 未知
        ORGANIZATION_CUSTOM,        // 自定义
        ORGANIZATION_OTHER,         // 其它
        ORGANIZATION_HOME,          // 家庭
        ORGANIZATION_WORK,          // 工作
    };

    enum HomePageTypes
    {
        HOMEPAGE_UNKNOW,        // 未知
        HOMEPAGE_CUSTOM,        // 自定义
        HOMEPAGE_OTHER,         // 其它
        HOMEPAGE_HOME,          // 家庭
        HOMEPAGE_WORK,          // 工作

		HOMEPAGE_HOMEPAGE,      // 主页
    };

    enum DayTypes
    {
        DAY_UNKNOW,         // 未知
        DAY_CUSTOM,         // 自定义
        DAY_OTHER,          // 其它
        DAY_HOME,           // 家庭
        DAY_WORK,           // 工作
        
		DAY_DAY,	        // 纪念日
    };

    enum RelatedTypes
    {
        RELATED_UNKNOW,         // 未知
        RELATED_CUSTOM,         // 自定义
        RELATED_OTHER,          // 其他

        RELATED_FATHER,         // 父亲
        RELATED_MOTHER,         // 母亲
        RELATED_PARENT,         // 父/母亲
        RELATED_BROTHER,        // 兄弟
        RELATED_SISTER,         // 姐妹
        RELATED_CHILD,          // 子女
        RELATED_FRIEND,         // 朋友
        RELATED_SPOUSE,         // 配偶
        RELATED_PARTNER,        // 搭档
        RELATED_ASSISTANT,      // 助理
        RELATED_MANAGER,        // 上司
    };

    enum Category
    {
        Category_Unknow          = -1,      // 未知类型 
        Category_PhoneNumber     = 3,       // 电话号码 
        Category_Email           = 4,       // 电子邮件
        Category_IM              = 13,      // 即时信息
        Category_StreetAddress   = 5,       // 地址ADDRESS
        Category_URL             = 22,      // 个人主页
        Category_Date            = 12,      // 纪念日
        Category_Relates         = 0x12     // 相关联系人
    };

    // ios4.0固件开始.联络人头像分成两个表存放,在缩略图表ABThumbnailImage中
    // format字段有值有0,1,4,其中0对应320*320,1对应320*480,4 目前为空
    // iPhone4 又不一样了.其中0 对应的是640*640,4对应的是320*480,1目前为空 
    enum PhotoFormats            // 图片格式类型
    {
        THUMBNAIL_PHOTO   = 0,            // 缩略图(320*320),iPhone4 对应640*640(2x)
        MEDIUM_PHOTO      = 1,            // 中等缩略图(320*480),iPhone4 有320*426的值
        ORIGINAL_PHOTO    = 2             // 源图
    };

    // 联系人有Person,或者Company 两种类型,即plist协议里面的display as 字段对应的值,
    // 应该对应数据库里面的ABPerson表的kind
    enum ContactType
    {
        Person,
        Company,
    };

    struct CLASSINDLL_CLASS_DECL Name    // 姓名
    {
        int           _id;
        ActionState   _state;           // 结构体数据状态
        wstring       _displayName;     
        wstring       _givenName;
        wstring       _familyName;
        wstring       _middleName;
        string        _ios_firstPhonetic;          // 名字拼音
        string        _ios_middlePhonetic;         // 中间名拼音
        string        _ios_lastPhonetic;           // 姓氏拼音
        int           _ios_kind;                   // 类别
        wstring       _ios_nickName;               // 昵称
        string        _ios_prefix;                 // 英文称谓
        wstring       _ios_suffix;                 // 中文称谓
        Name()
        {
            _id     = 0;
            _state  = STATE_NOCHANGE;
        }
    };

    struct CLASSINDLL_CLASS_DECL Phone
    {
        string        _key;             // (ios)设备传送过来的Key,由categoryid/mainid/count组成  (android)表示ID
        ActionState   _state;           // 结构体数据状态
        PhoneTypes    _type;            // 电话号码类型
        wstring       _label;           // 自定义标签名
        wstring       _number;          // 电话号码
        Phone()
        {
            _state  = STATE_NOCHANGE;
            _type   = PHONE_UNKNOW;                   
        }
    };

    struct CLASSINDLL_CLASS_DECL Email
    {
        string        _key;             // (ios)设备传送过来的Key,由categoryid/mainid/count组成  (android)表示ID
        ActionState   _state;           // 结构体数据状态
        EmailTypes    _type;            // 电子邮件类型
        wstring       _label;           // 自定义标签名
        wstring       _emailAccount;    // 电子邮件账户名
        Email()
        {
            _state  = STATE_NOCHANGE;
            _type   = EMAIL_UNKNOW;     
        }
    };

    struct CLASSINDLL_CLASS_DECL Im     // 即时通讯账户
    {
        string        _key;             // (ios)设备传送过来的Key,由categoryid/mainid/count组成  (android)表示ID
        ActionState   _state;           // 结构体数据状态
        IMTypes       _type;            // 即时通讯账户类型
        IMTypesName   _typeName;        // 即时通讯账户类型名
            wstring       _service;         // ios即时通讯账户类型名(DB方式需要)
        wstring       _imAccount;       // 即时通讯账户名
        wstring       _label;           // 自定义标签名
        Im()
        {
            _state      = STATE_NOCHANGE;    
            _type       = IM_UNKNOW;
            _typeName   = IM_NAMEUNKNOW;
        }
    };

    struct CLASSINDLL_CLASS_DECL Address         // 通讯地址
    {
        string        _key;                // (ios)设备传送过来的Key,由categoryid/mainid/count组成  (android)表示ID
        ActionState   _state;              // 结构体数据状态
        AddressTypes  _type;               // 通讯电子类型    
        wstring       _label;              // 地址类型标签
        wstring       _province;           // 省
        wstring       _city;               // 市
        wstring       _street;             // 街道
        wstring       _zipCode;            // 邮编
        wstring       _displayAddress;     // 地址
        wstring       _countryCode;        // 国家代码
        wstring       _country;            // 国家
        Address()
        {
            _state  = STATE_NOCHANGE;
            _type   = ADDRESS_UNKNOW;                
        }
    };

    struct CLASSINDLL_CLASS_DECL Organization    // 组织 
    {
        int               _id;       // 组织ID
        ActionState       _state;    // 结构体数据状态
        OrganizationTypes _type;     // 组织类型
        wstring           _label;    // 组织类型标签
        wstring           _name;     // 组织名
        wstring           _address;  // 组织地址
        Organization()
        {
            _id     = 0;
            _state  = STATE_NOCHANGE;
            _type   = ORGANIZATION_UNKNOW;     
        }
    };

    struct CLASSINDLL_CLASS_DECL Comment      // 备注
    {
        int          _id;
        ActionState  _state;                // 结构体数据状态
        wstring      _content;              // 备注内容
        Comment()
        {
            _id     = 0;
            _state  = STATE_NOCHANGE;
        }
    };

    struct CLASSINDLL_CLASS_DECL HomePage       // 主页
    {
        string        _key;             // 设备传送过来的Key组成categoryid/mainid/count 组成
        ActionState   _state;           // 结构体数据状态
        HomePageTypes  _type;           // 类型
        wstring       _name;            // 名称
        wstring       _label;           // 自定义标签名
        HomePage()
        {
            _state  = STATE_NOCHANGE;
            _type   = HOMEPAGE_UNKNOW;
        }
    };

    struct CLASSINDLL_CLASS_DECL Day            // 纪念日
    {
        string          _key;             // 设备传送过来的Key组成categoryid/mainid/count 组成
        ActionState     _state;           // 结构体数据状态
        DayTypes        _type;            // 类型
        wstring         _name;            // 名称
        wstring         _label;           // 自定义标签名
        Day()
        {
            _state  = STATE_NOCHANGE;
            _type   = DAY_UNKNOW;
        }
    };

    struct CLASSINDLL_CLASS_DECL Related        // 相关联系人
    {
        string          _key;             // 设备传送过来的Key组成categoryid/mainid/count 组成
        ActionState     _state;           // 结构体数据状态
        RelatedTypes    _type;            // 类型
        wstring         _name;            // 姓名
        wstring         _label;           // 自定义标签名
        Related()
        {
            _state  = STATE_NOCHANGE;
            _type   = RELATED_UNKNOW;
        }
    };

    struct CLASSINDLL_CLASS_DECL Photo
    {
        int			    _photoId;			// 头像id
        ActionState     _state;             // 结构体数据状态
        wstring		    _strSavePath;		// 联络人图片要保存到的PC中的路径(源图)
        wstring		    _photoUri;			// 图片uri（仅限于OMS系统）
        wstring		    _photoPath;			// 图片路径（PhotoUri转换而得，由守护转换，仅限于OMS系统）

        //wstring		    _mediumPath;		// 联络人图片要保存到的PC中的路径(中等缩略图)
        //wstring		    _thumbnailPath;		// 联络人图片要保存到的PC中的路径(缩略图)
        Photo()
        {
            _photoId        = 0;
            _state          = STATE_NOCHANGE;
        }
    };

    class ContactGroupData;
    //class IosContactProperty;
    
    // 联系人数据类
    class CLASSINDLL_CLASS_DECL ContactData : public BaseData
    {
        //friend class IosContactProperty;
    public:
        ContactData();
        ContactData(const char* iosData[21]);
        ContactData(string identifier, map<string,void*> entity, ContactType contactType);
        virtual ~ContactData();

        // Summary  : 构建联系人数据
        // Returns  : void
        // Parameter: map<string, void*>& infoss 构建的联系人数据
        void ConvertToDictInfos(map<string, void*>& infoss);

    public:
        int     _contactId;					// 联络人ID
        string  _identifier;                // 联络人ID
        Name    _sName;						// 名字结构体
        Comment _sComment;					// 备注结构体

        wstring _companyName;               // 公司
        wstring _department;				// 部门
        wstring _birthday;					// 生日日期
        wstring _jobTitle;					// 职称

        wstring _accountName;				// 所属账户名称
        wstring _accountType;				// 所属账户类型
        bool    _favorite;					// 是否收藏
        wstring _ringtone;					// 来电铃声
        wstring _textTone;                  // 短信铃声
       
        // 联络人头像
        bool    _photo_hasLoad;
        Photo   _headPhoto;

        vector<Phone*>            _phoneNumberList;         // 电话号码列表
        vector<Email*>            _emailList;               // 电子邮件列表
        vector<Address*>          _addressList;             // 通讯地址列表
        vector<Im*>               _imList;                  // 即时通通讯账户列表
        vector<Organization*>     _organizationList;        // 组织列表
        
        vector<HomePage*>         _homePageList;            // 主页列表
        vector<Day*>              _dateList;                // 纪念日列表
        vector<Related*>          _relatedList;             // 相关联系人列表（5.0后才有）
        //IosContactProperty*       _ios_property;            // 联络人属性
        vector<int>               _groupIdList;             // 所属组ID列表

		vector<wstring>           _tempGroupNames;          // 添加联络人时传入的分组名称

        static long     _idIncrementer;     // 联系人在PC上的ID
        string          _recordEntityName;  // "com.apple.contacts.Contact"
        ContactType     _contactType;       // IOS联系人类型

    public:
        string ToBuffer();
        wstring GetPhoneNums();
		void AddAndroidPhoneNUmber(string telnumber, PhoneTypes phonetype);
		void AddAndroidEmail(string emailaccount, EmailTypes emailtype);
		void AddHomepage(string name,HomePageTypes pagetype);

    private:
        void FormatBirthday(void* birthday);
        void FormatHeadPhoto(void* image);

        // Summary  : 构建联系人头像数据
        // Returns  : void*
        void* BuildHeadPhoto(); 
    };
}
