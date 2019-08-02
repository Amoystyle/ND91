#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/ContactData.h"
#include "Module/Contact/ContactParser.h"

#include "Device/CFConverter.h"
#include "Module/Common/Synchronizer/DeviceSyncClient.h"

#define RELEASE_LIST(StuctList, StuctType) 	\
for(vector<StuctType*>::iterator it = StuctList.begin(); it != StuctList.end(); ++it) \
{\
    StuctType* pTemp = *it;\
    SAFE_DELETE(pTemp);\
}\
StuctList.clear();

long ContactData::_idIncrementer  = 10000;
ContactData::ContactData()
{
    char temp[125] = {0};
    sprintf(temp, "ND%ld", _idIncrementer++);
    _identifier = temp;
    SetStatus(NEW);
    _recordEntityName = "com.apple.contacts.Contact";

    _favorite       = false;
    _contactId      = 0;
	_photo_hasLoad  = false;
    _contactType    = Person;

    //_ios_property = new IosContactProperty(this);
}

ContactData::ContactData(const char* iosData[21])
{
    // 获取基本信息
    _contactId                  = atoi(iosData[0]);
    _sName._givenName           = CCodeOperation::UTF_8ToUnicode(iosData[1]); // 姓
    _sName._familyName          = CCodeOperation::UTF_8ToUnicode(iosData[2]);// 名字
    _sName._middleName          = CCodeOperation::UTF_8ToUnicode(iosData[3]);// 中间名 
    _sName._ios_firstPhonetic   = iosData[4];// 名字拼音
    _sName._ios_middlePhonetic  = iosData[5];// 中间名拼音
    _sName._ios_lastPhonetic    = iosData[6];// 姓氏拼音
    Organization * orgaize      = new Organization();
    orgaize->_name              = CCodeOperation::UTF_8ToUnicode(iosData[7]);
    _organizationList.push_back(orgaize);// 组织
    _department                 = CCodeOperation::UTF_8ToUnicode(iosData[8]); // 部门
    _sComment._content          = CCodeOperation::UTF_8ToUnicode(iosData[9]); // 备注
    _sName._ios_kind            = atoi(iosData[10]); // 类别
    //_birthday                   = _ios_property->GetPcTime(iosData[11]);// 生日日期
    _jobTitle                   = CCodeOperation::UTF_8ToUnicode(iosData[12]); // 职称
    _sName._ios_nickName        = CCodeOperation::UTF_8ToUnicode(iosData[13]); // 昵称
    _sName._ios_prefix          = iosData[14]; // 英文称谓
    _sName._ios_suffix          = CCodeOperation::UTF_8ToUnicode(iosData[15]); // 中文称谓
    _photo_hasLoad              = false;

    //_ios_property               = new IosContactProperty(this);
}

ContactData::ContactData(string identifier,map<string,void*> entity,ContactType contactType)
{
    _identifier = identifier;
    _contactId  = atoi(identifier.c_str());
    _photo_hasLoad  = true;
    for (map<string, void*>:: iterator it = entity.begin(); it != entity.end(); it++)
    {
        string str = "";
        if (CCFConverter::NDCFGetTypeID(it->second) == CCFConverter::NDCFStringGetTypeID())
            CCFConverter::GetIosData(it->second, str);

        if (it->first == "first name")
            _sName._familyName = CCodeOperation::GB2312ToUnicode(str);
        else if(it->first == "first name yomi")
            _sName._ios_firstPhonetic = str;
        else if (it->first == "middle name")
            _sName._middleName = CCodeOperation::GB2312ToUnicode(str);
        else if (it->first == "last name")
            _sName._givenName = CCodeOperation::GB2312ToUnicode(str);
        else if (it->first == "last name yomi")
            _sName._ios_lastPhonetic = str;
        else if (it->first == "nickname")
            _sName._ios_nickName = CCodeOperation::GB2312ToUnicode(str);
        else if (it->first == "suffix")
            _sName._ios_suffix = CCodeOperation::GB2312ToUnicode(str);
        else if (it->first == "prefix")
            _sName._ios_prefix = str;
        else if (it->first == "notes")
            _sComment._content = CCodeOperation::GB2312ToUnicode(str);
        else if (it->first == "company name")
            _companyName = CCodeOperation::GB2312ToUnicode(str);
        else if (it->first == "department")
            _department = CCodeOperation::GB2312ToUnicode(str);
        else if (it->first == "job title")
            _jobTitle = CCodeOperation::GB2312ToUnicode(str);
        else if (it->first == "birthday")
            FormatBirthday(it->second);
        else if (it->first == "image")
            _headPhoto._strSavePath/*_thumbnailPath*/ = CCodeOperation::GB2312ToUnicode(str);
            //FormatHeadPhoto(it->second);
        else if (it->first == "com.apple.syncservices.RecordEntityName")
            _recordEntityName = str;
    }
    _sName._displayName = _sName._givenName + _sName._familyName;
    SetStatus(NO_CHANGE);
    _contactType = contactType;
    //_ios_property = new IosContactProperty(this);
}

void ContactData::FormatBirthday(void* birthday)
{
    time_t time = 0;
    string timeStr;
    CCFConverter::GetIosData(birthday, time, timeStr);
    if (time != 0)
    {
        char timeWstr[128] = {0};
        sprintf(timeWstr,"%d",(int)time);
        string temp = timeWstr;
        _birthday = CCodeOperation::StringToWstring(temp);
    }
    else
    {
        CCFConverter::GetIosData(birthday, timeStr);
        _birthday = CCodeOperation::StringToWstring(timeStr);
    }
}

void ContactData::FormatHeadPhoto(void* image)
{
    vector<void*> temp;
    temp.clear();
    CCFConverter::GetIosData(image,temp);
    for (vector<void*>::iterator iter = temp.begin(); iter != temp.end(); iter++)
    {
        int nLen = CCFConverter::NDCFDataGetLength(*iter);
        void* pValue = CCFConverter::NDCFDataGetBytePtr(*iter);
        unsigned char* uszValue = new unsigned char[nLen];
        //ZeroMemory(uszValue, nLen);
        memset(uszValue, 0, nLen);
        memcpy(uszValue, pValue, nLen);

        //FILE* f = _wfopen(L"D://head.png", L"w+b");
				FILE* f = fopen(CCodeOperation::WstringToString(L"D://head.png").c_str(), "w+b");
        if (f) 
        {
            fwrite(uszValue, nLen, 1, f);
            fflush(f);
            fclose(f);					
        }

        SAFE_DELETE_ARRAY(uszValue);
    }
}

ContactData::~ContactData()
{
    RELEASE_LIST(_phoneNumberList, Phone);
    RELEASE_LIST(_emailList, Email);
    RELEASE_LIST(_imList, Im);
    RELEASE_LIST(_addressList, Address);
    RELEASE_LIST(_organizationList, Organization);
	RELEASE_LIST(_homePageList, HomePage);
	RELEASE_LIST(_dateList, Day);
    RELEASE_LIST(_relatedList, Related);
    //delete _ios_property;
}

std::string ND91Assistant::ContactData::ToBuffer()
{
    BinaryBufferWriter writer; 
    return writer.GetBuffer();
}

wstring ND91Assistant::ContactData::GetPhoneNums()
{
    wstring str = L"";

	for (vector<Phone*>::iterator it = _phoneNumberList.begin(); it != _phoneNumberList.end(); it++)
		str += (*it)->_number;

    return str;
}

void ND91Assistant::ContactData::AddAndroidPhoneNUmber( string telnumber, PhoneTypes phonetype )
{
	RETURN_IF(telnumber.empty());
	Phone* assistantTel=new Phone();
	assistantTel->_type=phonetype;
	assistantTel->_number=CCodeOperation::s2ws(telnumber);
	this->_phoneNumberList.push_back(assistantTel);
}

void ND91Assistant::ContactData::AddAndroidEmail( string emailaccount, EmailTypes emailtype )
{
	RETURN_IF(emailaccount.empty());
	Email* pEmail=new Email();
	pEmail->_emailAccount=CCodeOperation::s2ws(emailaccount);
	pEmail->_type=emailtype;
	this->_emailList.push_back(pEmail);
}

void ND91Assistant::ContactData::AddHomepage( string name, HomePageTypes pagetype )
{
	RETURN_IF(name.empty());
	HomePage* page=new HomePage();
	page->_name=CCodeOperation::s2ws(name);
	page->_type=pagetype;
	this->_homePageList.push_back(page);
}
void ContactData::ConvertToDictInfos(map<string, void*>& infoss)
{
    if (GetStatus() == BaseData::DEL)
        return ;

    infoss.insert(pair<string, void*>(DeviceSyncClient::RECORD_ENTITY_NAME_KEY,CCFConverter::StringtoCFString(ContactParser::CONTACT_ENTITY_NAME)));

    if (_contactType != Person)
        infoss.insert(pair<string, void*>(ContactParser::DISPLYA_AS_COMPANY,CCFConverter::StringtoCFString("company")));
    if (!_sName._familyName.empty())
        infoss.insert(pair<string, void*>("first name",CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(_sName._familyName))));
    if (!_sName._ios_firstPhonetic.empty())
        infoss.insert(pair<string, void*>("first name yomi",CCFConverter::StringtoCFString(_sName._ios_firstPhonetic)));
    if (!_sName._middleName.empty())
        infoss.insert(pair<string, void*>("middle name",CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(_sName._middleName))));
    if (!_sName._givenName.empty())
        infoss.insert(pair<string, void*>("last name",CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(_sName._givenName))));
    if (!_sName._ios_lastPhonetic.empty())
        infoss.insert(pair<string, void*>("last name yomi",CCFConverter::StringtoCFString(_sName._ios_lastPhonetic)));
    if (!_sName._ios_nickName.empty())
        infoss.insert(pair<string, void*>("nickname",CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(_sName._ios_nickName))));
    if (!_sName._ios_suffix.empty())
        infoss.insert(pair<string, void*>("suffix",CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(_sName._ios_suffix))));
    if (!_sName._ios_prefix.empty())
        infoss.insert(pair<string, void*>("prefix",CCFConverter::StringtoCFString(_sName._ios_prefix)));
    if (!_sComment._content.empty())
        infoss.insert(pair<string, void*>("notes",CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(_sComment._content))));
    if (!_companyName.empty())
        infoss.insert(pair<string, void*>("company name",CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(_companyName))));
    if (!_department.empty())
        infoss.insert(pair<string, void*>("department",CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(_department))));
    if (!_jobTitle.empty())
        infoss.insert(pair<string, void*>("job title",CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(_jobTitle))));
    if (!_birthday.empty())
        infoss.insert(pair<string, void*>("birthday", CCFConverter::TimeToCFDate((time_t)atoi(CCodeOperation::UnicodeToGB2312(_birthday).c_str()))));
    if (!_headPhoto._strSavePath/*_thumbnailPath*/.empty())
        infoss.insert(pair<string, void*>("image", BuildHeadPhoto()));
}

void* ContactData::BuildHeadPhoto()
{
    string srcFilename = CCodeOperation::WstringToString(_headPhoto._strSavePath/*_thumbnailPath*/);
    FILE *pSrcFile = NULL;
    struct stat *filestats = (struct stat *) malloc(sizeof(struct stat));
    //if (fopen_s(&pSrcFile, srcFilename.c_str(), "rb") != 0)
    pSrcFile = fopen(srcFilename.c_str(), "rb");
    if (pSrcFile == NULL)
        return NULL;

    stat(srcFilename.c_str(), filestats);
    unsigned char* szSrc = (unsigned char *) malloc(sizeof(unsigned char) * (filestats->st_size + 1));
    int read_size = fread(szSrc, sizeof(unsigned char), filestats->st_size, pSrcFile);
    fclose(pSrcFile);
    void* data = CCFConverter::NDCFDataCreate(NULL, szSrc, read_size);
    free(filestats);
    free(szSrc);
    return data;
}


#endif