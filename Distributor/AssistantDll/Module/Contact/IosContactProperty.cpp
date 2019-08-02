#include "stdafx.h"
#ifdef MODULE_CONTACT

#define USERNAME        L"username"                   // ��ʱͨѶ�˻�����
#define SERVICE         L"service"                    // ��ʱͨѶ�˻���
#define STREET          L"Street"                     // ͨѶ��ַ�ֵ�
#define STATE           L"State"                      // ͨѶ��ַʡ
#define ZIP             L"ZIP"                        // ͨѶ��ַ�ʱ�
#define CITY            L"City"                       // ͨѶ��ַ��
#define COUNTRYCODE     L"CountryCode"                // ͨѶ��ַ���Ҵ���
#define COUNTRY         L"Country"                    // ͨѶ��ַ����
#define PRODUCTVERSIONL            200               // IOS�豸�Ͱ汾��
#define PRODUCTVERSIONH            421               // IOS�豸�߰汾��

IosContactProperty::IosContactProperty(ContactData* person)
{
    _contactDB  = NULL;
    _person     = person;
}

IosContactProperty::~IosContactProperty()
{
    for (vector<Property*>::iterator it = _ios_propertyList.begin(); it != _ios_propertyList.end(); ++it)
    {
        Property *pTemp = *it;
        if (pTemp->_value)
        {
            free(pTemp->_value);
            pTemp->_value = NULL;
        }
        SAFE_DELETE(pTemp);
    }
    _ios_propertyList.clear();

    _contactDB = NULL;
    _person    = NULL;
}

// ��ȡ����������(����,����,��ַ,IM)
void IosContactProperty::GetContactMultiValue(const char* iosData[6])
{
    if (atoi(iosData[0]) == 0)// �ж�����ID
        return ;

    Property * pDto = new Property();
    pDto->_uid = atoi(iosData[0]);
    pDto->_property = (PropertyTypes)atoi(iosData[2]);
    pDto->_identifier = atoi(iosData[3]);
    pDto->_label = atoi(iosData[4]);
    pDto->_labelKey = GetLabelValue(pDto->_label);

    switch (pDto->_property)
    {
    case PROPERTY_PHONENUMBER:
        GetPropertyPhoneNumber(pDto, iosData[5]); // ��ȡ����
        break;
    case PROPERTY_ADDRESS:
        GetPropertyAddress(pDto);// ��ȡ��ַ
        break;
    case PROPERTY_IM:
        GetPropertyIm(pDto);// ��ȡIM
        break;
    case PROPERTY_DATE:
        GetPropertyDate(pDto, iosData[5]);// ��ȡ����
        break;
    case PROPERTY_EMAIL:
        GetPropertyEmail(pDto, iosData[5]);// ��ȡ����
        break;
    case PROPERTY_HOMEPAGE:
        GetPropertyHomepage(pDto, iosData[5]);// ��ȡ��ҳ
        break;
    case PROPERTY_RINGTONE:
        GetPropertyRingtone(pDto, iosData[5]);// ��ȡ����
        break;
    default:
        //pDto->_value = malloc(strlen(iosData[5]));
        //::memcpy(pDto->_value,iosData[5], strlen(iosData[5]));
        break;
    }
    _ios_propertyList.push_back(pDto);
}

void IosContactProperty::AddPropertyList()
{
    if (_person == NULL)
        return ;

    RemovePropertyByType(PROPERTY_PHONENUMBER);
	RemovePropertyByType(PROPERTY_ADDRESS);
	RemovePropertyByType(PROPERTY_IM);
	RemovePropertyByType(PROPERTY_DATE);
    RemovePropertyByType(PROPERTY_EMAIL);
    RemovePropertyByType(PROPERTY_HOMEPAGE);
    RemovePropertyByType(PROPERTY_RINGTONE);

    SetPropertyPhoneNumber();
	SetPropertyAddress();
	SetPropertyIm();
	SetPropertyDate();
    SetPropertyEmail();
    SetPropertyHomePage();
    SetPropertyRing();
}

// �������������
void IosContactProperty::AddContactMultiValue()
{
    int index = -1;
    for (size_t i = 0; i < _ios_propertyList.size(); i++)
    {
        Property* pDto = _ios_propertyList.at(i);

        // ����ֵΪ������
        if (pDto->_value == NULL)
            continue;

        // ����������labelId
        if (pDto->_property != PROPERTY_RINGTONE)
            // ��ȡ���Ա�ǩID
            GetABMultiValueLable(pDto);

        // ��ȡ����identifier
        GetPropertyIdentifier(pDto,_person->_contactId,&index);

        // �������������(����ABMultiValue�������)
		AddPropertyInformation(pDto,index);

        // ����ABPhoneLastFour��
        if (pDto->_property == PROPERTY_PHONENUMBER)
            UpdateABPhoneLastFour(pDto);

		// ���IM�͵�ַ����(����ABMultiValueEntry�������)
        if (pDto->_property == PROPERTY_ADDRESS || pDto->_property == PROPERTY_IM)
		    AddImAndAddressProperty(pDto);
    }
}

// ��ȡ��ǩ��ֵ
wstring IosContactProperty::GetLabelValue(int labelRowId)
{
    CppSQLite3Buffer sql;
    sql.Format("Select value from ABMultiValueLabel where ROWID = %d;", labelRowId);
    CppSQLite3Query q = _contactDB->ExecQuery((const char*)sql);

    wstring lableValue = L"";
    while (!q.Eof())
    {
        lableValue = CCodeOperation::UTF_8ToUnicode(q.GetStringField(0));
        q.NextRow();
    }
    return lableValue;
}

// ��ȡ�����еĵ绰����ֵ
void IosContactProperty::GetPropertyPhoneNumber(Property * pDto, const char* value)
{
    string num = value;
    num = CStrOperation::Replace(num, "[^\\d]*", "");

    Phone * fDto = new Phone();
    fDto->_number = CCodeOperation::UTF_8ToUnicode(value);
    if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._mobile))
        fDto->_type = PHONE_MOBILE;
    else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._home))
        fDto->_type = PHONE_HOME;
    else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._work))
        fDto->_type = PHONE_WORK;
    else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._main))
        fDto->_type = PHONE_MAIN;
    else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._custom))
        fDto->_type = PHONE_CUSTOM;
	else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._workFax))
		fDto->_type = PHONE_WORKFAX;
	else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._homeFax))
		fDto->_type = PHONE_HOMEFAX;
	else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._pager))
		fDto->_type = PHONE_PAGER;
    else
        fDto->_type = PHONE_OTHER;
    _person->_phoneNumberList.push_back(fDto);
}

// ��ȡ�����е�����
void IosContactProperty::GetPropertyEmail(Property * pDto, const char* value)
{
    Email *eDto = new Email();
    eDto->_emailAccount = CCodeOperation::UTF_8ToUnicode(value);
    if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._mobile))
        eDto->_type = EMAIL_MOBILE;
    else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._home))
        eDto->_type = EMAIL_HOME;
    else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._work))
        eDto->_type = EMAIL_WORK;
    else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._custom))
        eDto->_type = EMAIL_CUSTOM;
    else
        eDto->_type = EMAIL_OTHER;

    _person->_emailList.push_back(eDto);
}

// ��ȡ�����е���ҳ
void IosContactProperty::GetPropertyHomepage(Property * pDto, const char* value)
{
    HomePage *hDto = new HomePage();
    hDto->_name = CCodeOperation::UTF_8ToUnicode(value);

	if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._homePage))
		hDto->_type = HOMEPAGE_HOMEPAGE;
    else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._home))
        hDto->_type = HOMEPAGE_HOME;
    else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._work))
        hDto->_type = HOMEPAGE_WORK;
    else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._custom))
        hDto->_type = HOMEPAGE_CUSTOM;
    else
        hDto->_type = HOMEPAGE_OTHER;

    _person->_homePageList.push_back(hDto);
}

// ��ȡ�����е�����
void IosContactProperty::GetPropertyRingtone(Property * pDto, const char* value)
{
    if (pDto->_identifier == -2)// ��������
        _person->_textTone = CCodeOperation::UTF_8ToUnicode(value);
    else
        _person->_ringtone = CCodeOperation::UTF_8ToUnicode(value);
}

// ��ȡ�����е�����ֵ
void IosContactProperty::GetPropertyDate(Property * pDto, const char* value)
{
    string dataStr = value;
    if (dataStr == "")
        return ;

    int i = dataStr.find(".");
    if (i > -1)
        dataStr = dataStr.substr(0,i);

    wstring strDaytime = GetPcTime(dataStr.c_str());

    Day *dDto = new Day();
    dDto->_name = strDaytime;

	if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._anniversary))
		dDto->_type = DAY_DAY;
    else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._home))
        dDto->_type = DAY_HOME;
    else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._work))
        dDto->_type = DAY_WORK;
    else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._custom))
        dDto->_type = DAY_CUSTOM;
    else
        dDto->_type = DAY_OTHER;

    _person->_dateList.push_back(dDto);
}

// ��ȡ�����еĵ�ֵַ
void IosContactProperty::GetPropertyAddress(Property * pDto)
{
    CppSQLite3Buffer sql;
    sql.Format("SELECT A.*,B.value as keyvalue FROM ABMultiValueEntry as A LEFT join [ABMultiValueEntryKey] as B On A.[key] = B.rowid where A.parent_id = %d;", pDto->_uid);
    CppSQLite3Query q = _contactDB->ExecQuery(sql);

    Address *aDto = new Address();
    while (!q.Eof())
    {
        wstring keyVal = CCodeOperation::UTF_8ToUnicode(q.GetStringField(3));
        wstring val = CCodeOperation::UTF_8ToUnicode(q.GetStringField(2));
        if (keyVal == STREET)
            aDto->_street = val;// ȡvalue��ֵ
        else if (keyVal == STATE)
            aDto->_province = val;
        else if (keyVal == ZIP)
            aDto->_zipCode = val;
        else if (keyVal == CITY)
            aDto->_city = val;
        else if (keyVal == COUNTRYCODE)
            aDto->_countryCode = val;
        else if (keyVal == COUNTRY)
            aDto->_country = val;

        q.NextRow();
    }

    if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._home))
        aDto->_type = ADDRESS_HOME;
    else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._work))
        aDto->_type = ADDRESS_WORK;
    else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._custom))
        aDto->_type = ADDRESS_CUSTOM;
    else
        aDto->_type = ADDRESS_OTHER;

    _person->_addressList.push_back(aDto); 
}

// ��ȡ�����еļ�ʱͨѶ�˻�ֵ
void IosContactProperty::GetPropertyIm(Property * pDto)
{
    CppSQLite3Buffer sql;
    sql.Format("SELECT A.*,B.value as keyvalue FROM ABMultiValueEntry as A LEFT join [ABMultiValueEntryKey] as B On A.[key] = B.rowid where A.parent_id = %d;", pDto->_uid);
    CppSQLite3Query q = _contactDB->ExecQuery(sql);

    Im *imDto = new Im();
    while (!q.Eof())
    {
        wstring keyVal = CCodeOperation::UTF_8ToUnicode(q.GetStringField(3));
        wstring val = CCodeOperation::UTF_8ToUnicode(q.GetStringField(2));
        if (keyVal == USERNAME)
            imDto->_imAccount = val;// ȡvalue��ֵ
        else if (keyVal == SERVICE)
            imDto->_service = val;

        q.NextRow();
    }

    if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._home))
        imDto->_type = IM_HOME;
    else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._work))
        imDto->_type = IM_WORK;
    else if (pDto->_labelKey == CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._custom))
        imDto->_type = IM_CUSTOM;
    else
        imDto->_type = IM_OTHER;

    _person->_imList.push_back(imDto);
}

void IosContactProperty::RemovePropertyByType(PropertyTypes type)
{
    vector<Property*>::iterator it;
    for( it = _ios_propertyList.begin(); it != _ios_propertyList.end();)
    {
        Property *pDto = *it;
        if (pDto->_property != type || pDto->_identifier == -2)// pDto._identifier==-2 ��ʾ��������
        {
            ++it;
            continue ;
        }
        it = _ios_propertyList.erase(it);
        SAFE_DELETE(pDto);
    }
}

void IosContactProperty::SetPropertyPhoneNumber()
{
    for (size_t i = 0; i < _person->_phoneNumberList.size(); i++)
    {
        Phone* phone = _person->_phoneNumberList.at(i);
        if (phone->_type == PHONE_MOBILE)
            SetProperty(PROPERTY_PHONENUMBER, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._mobile), phone->_number);
        else if (phone->_type == PHONE_HOME)
            SetProperty(PROPERTY_PHONENUMBER, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._home), phone->_number);
        else if (phone->_type == PHONE_WORK)
            SetProperty(PROPERTY_PHONENUMBER, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._work), phone->_number);
        else if (phone->_type == PHONE_MAIN)
            SetProperty(PROPERTY_PHONENUMBER, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._main), phone->_number);
        else if (phone->_type == PHONE_WORKFAX)
            SetProperty(PROPERTY_PHONENUMBER, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._workFax), phone->_number);
        else if (phone->_type == PHONE_HOMEFAX)
            SetProperty(PROPERTY_PHONENUMBER, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._homeFax), phone->_number);
        else if (phone->_type == PHONE_PAGER)
            SetProperty(PROPERTY_PHONENUMBER, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._pager), phone->_number);
		else if (phone->_type == PHONE_CUSTOM)
			SetProperty(PROPERTY_PHONENUMBER,CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._custom), phone->_number);
        else
            SetProperty(PROPERTY_PHONENUMBER, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._other), phone->_number);
    }
}

void IosContactProperty::SetPropertyEmail()
{
    for (size_t i = 0; i < _person->_emailList.size(); i++)
    {
        Email* email = _person->_emailList.at(i);
        if (email->_type == EMAIL_MOBILE)
            SetProperty(PROPERTY_EMAIL, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._mobile), email->_emailAccount);
        else if (email->_type == EMAIL_HOME)
            SetProperty(PROPERTY_EMAIL, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._home), email->_emailAccount);
        else if (email->_type == EMAIL_WORK)
            SetProperty(PROPERTY_EMAIL, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._work), email->_emailAccount);
		else if (email->_type == EMAIL_CUSTOM)
			SetProperty(PROPERTY_EMAIL, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._custom), email->_emailAccount);
        else 
            SetProperty(PROPERTY_EMAIL, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._other), email->_emailAccount);
    }
}

void IosContactProperty::SetPropertyHomePage()
{
    for (size_t i = 0; i < _person->_homePageList.size(); i++)
    {
        HomePage* homePage = _person->_homePageList.at(i);
		if (homePage->_type == HOMEPAGE_HOMEPAGE)
			SetProperty(PROPERTY_HOMEPAGE, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._homePage), homePage->_name);
        else if (homePage->_type == HOMEPAGE_HOME)
            SetProperty(PROPERTY_HOMEPAGE, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._home), homePage->_name);
        else if (homePage->_type == HOMEPAGE_WORK)
            SetProperty(PROPERTY_HOMEPAGE, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._work), homePage->_name);
		else if (homePage->_type == HOMEPAGE_CUSTOM)
			SetProperty(PROPERTY_HOMEPAGE, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._custom), homePage->_name);
        else
            SetProperty(PROPERTY_HOMEPAGE, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._other), homePage->_name);
    }
}

void IosContactProperty::SetPropertyDate()
{
    for (size_t i = 0; i < _person->_dateList.size(); i++)
    {
		Day* day = _person->_dateList.at(i);
		if (day->_type == DAY_DAY)
			SetProperty(PROPERTY_DATE, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._anniversary), day->_name);
		else if (day->_type == DAY_HOME)
			SetProperty(PROPERTY_DATE, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._home), day->_name);
		else if (day->_type == DAY_WORK)
			SetProperty(PROPERTY_DATE, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._work), day->_name);
		else if (day->_type == DAY_CUSTOM)
			SetProperty(PROPERTY_DATE, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._custom), day->_name);
		else
			SetProperty(PROPERTY_DATE, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._other), day->_name);
    }
}

void IosContactProperty::SetPropertyRing()
{
    SetProperty(PROPERTY_RINGTONE, L"",_person->_ringtone);
    SetProperty(PROPERTY_RINGTONE, L"",_person->_textTone);
}

void IosContactProperty::SetPropertyAddress()
{
    for (size_t i = 0; i < _person->_addressList.size(); i++)
    {
        Address* address = _person->_addressList.at(i);
        if (address->_type == ADDRESS_HOME)
            SetProperty(PROPERTY_ADDRESS, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._home), address);
        else if (address->_type == ADDRESS_WORK)
            SetProperty(PROPERTY_ADDRESS, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._work), address);
		else if (address->_type == ADDRESS_CUSTOM)
			SetProperty(PROPERTY_ADDRESS, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._custom), address);
        else
            SetProperty(PROPERTY_ADDRESS, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._other), address);
    }
}

void IosContactProperty::SetProperty(PropertyTypes type, wstring labelKey, Address* address)
{
    if (address == NULL)
        return ;

    Property* pDto = new Property();
    pDto->_uid = -1;
    pDto->_labelKey = labelKey;
    pDto->_property = type;

    pDto->_value = malloc(sizeof(Address));
    ::memcpy(pDto->_value,address, sizeof(Address));
    _ios_propertyList.push_back(pDto);
}

void IosContactProperty::SetPropertyIm()
{
    for (size_t i = 0; i < _person->_imList.size(); i++)
    {
        Im* im = _person->_imList.at(i);
        if (im->_type == IM_HOME)
            SetProperty(PROPERTY_IM, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._home), im);
        else if (im->_type == IM_WORK)
            SetProperty(PROPERTY_IM, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._work), im);
        else if (im->_type == IM_CUSTOM)
            SetProperty(PROPERTY_IM, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._custom), im);
        else
            SetProperty(PROPERTY_IM, CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._other), im);
    }
}

void IosContactProperty::SetProperty(PropertyTypes type, wstring labelKey, Im* im)
{
    if (im == NULL)
        return ;

    Property* pDto = new Property();
    pDto->_uid = -1;
    pDto->_labelKey = labelKey;
    pDto->_property = type;

    pDto->_value = malloc(sizeof(Im));
    ::memcpy(pDto->_value,im, sizeof(Im));
    _ios_propertyList.push_back(pDto);
}

void IosContactProperty::SetProperty(PropertyTypes type, wstring labelKey, wstring value)
{
    if (value == L"")
        return ;

    Property* pDto = new Property();
    pDto->_uid = -1;
    pDto->_labelKey = labelKey;
    pDto->_property = type;

    string tmp = CCodeOperation::UnicodeToUTF_8(value).c_str();
    pDto->_value = malloc(sizeof(tmp));
    ::memcpy(pDto->_value,tmp.c_str(), sizeof(tmp));
    _ios_propertyList.push_back(pDto);
}

void IosContactProperty::GetABMultiValueLable(Property* pDto)
{
    wstring labelKey = pDto->_labelKey;
    if (labelKey == L"")
        labelKey = CCodeOperation::UTF_8ToUnicode(_systemLabelTypes._other);

    // ��ȡ��ǩID
    CppSQLite3Buffer sql;
    sql.Format("select ROWID from ABMultiValueLabel where value = '%s';",CCodeOperation::UnicodeToUTF_8SQL(labelKey).c_str());
    int labelId = _contactDB->ExecuteAndGetId((const char*)sql);
    if (labelId == -1)
    {
        sql.Format("Insert into ABMultiValueLabel Values('%s');",CCodeOperation::UnicodeToUTF_8SQL(labelKey).c_str());
        if (_contactDB->ExecDML((const char*)sql) > 0)
        {
            sql.Format("select ROWID from ABMultiValueLabel where value = '%s';",CCodeOperation::UnicodeToUTF_8SQL(labelKey).c_str());
            labelId = _contactDB->ExecuteAndGetId((const char*)sql);
        }
    }
    pDto->_label = labelId;
}

// ��ȡ����identifier
void IosContactProperty::GetPropertyIdentifier(Property *pDto,int contactId,int *index)
{
    //�̼�4.2.1��ʼ����������identifier ��-1��ʼ���,TextToneΪ-2
    //û��������������,ֻ����TextTone,identifierҲΪ-2;
    if (_contactDB->_iosDBPath.GetProductVersion() >= PRODUCTVERSIONH && pDto->_property == PROPERTY_RINGTONE)
    {
        *index = -1;
        string tone = CStrOperation::toLower((char *)pDto->_value);
        if ((int)(tone.find("texttone")) >= 0 || (int)(tone.find("none")) >= 0)
            *index = -2;//˵���Ƕ�������
    }
    else
    {
        *index = -1;
        CppSQLite3Buffer sql;
        sql.Format("SELECT * FROM ABMultiValue where record_id = %d and property = %d;",contactId,(int)pDto->_property);
        if (_contactDB->ExecuteAndGetId((const char*)sql) != -1)
        {
            sql.Format("SELECT MAX(identifier) FROM ABMultiValue where record_id = %d and property = %d;",contactId,(int)pDto->_property);
            *index = _contactDB->ExecuteAndGetId((const char*)sql);
        }
        //ԭ�ȶ��Ǵ�0��ʼ������
        if (*index == -1)
            *index = 0;
        else
            (*index) ++;
    }
}

// �������������
bool IosContactProperty::AddPropertyInformation(Property *pDto,int index/*,bool addOrUpdate*/)
{
    // laber�ֶε��ж�
    char szLabel[128] = {0};
    string strLabel = "";
    sprintf_s(szLabel,"%d",pDto->_label);
    if (pDto->_property != PROPERTY_RINGTONE)
        strLabel = szLabel;
    else
        strLabel = "null";

    // value�ֶε��ж�
    char szValue[128] = {0};
    string strValue = "";
    sprintf_s(szValue,"%s",(char *)pDto->_value);

    if (pDto->_property == PROPERTY_ADDRESS || pDto->_property == PROPERTY_IM)
        strValue = "";
    else
        strValue = szValue;

    if (pDto->_property == PROPERTY_DATE)
        strValue = GetIosTime(CCodeOperation::UTF_8ToUnicode(strValue));

	CppSQLite3Buffer sql;
    sql.Format("INSERT INTO ABMultiValue(record_id,property,identifier,label,value) VALUES(%d,%d,%d,%s,'%s');",
        _person->_contactId,(int)pDto->_property,index,strLabel.c_str(),strValue.c_str());

    return _contactDB->ExecDML((const char*)sql) > 0 ? true : false;
}

void IosContactProperty::AddImAndAddressProperty(Property* pDto)
{
    // ��ȡ���Ա��ID
    CppSQLite3Buffer sql;
    sql.Format("SELECT MAX(UID) FROM ABMultiValue");
    pDto->_uid = _contactDB->ExecuteAndGetId((const char*)sql);
    if (pDto->_uid == -1)// ��ȡ���Ա��ID���ɹ�
        return ;

    // �������IM
    if (pDto->_property == PROPERTY_IM)
        AddPropertyIm(pDto);

    // �������ADDRESS
    if (pDto->_property == PROPERTY_ADDRESS)
        AddPropertyAddress(pDto);
}

// �������IM
void IosContactProperty::AddPropertyIm(Property* pDto)
{
    Im* imDto = (Im*)(pDto->_value);
    // ����ABMultiValueEntry�������
    InsertIntoABMultiValueEntry(imDto->_service,SERVICE,pDto->_uid);
    InsertIntoABMultiValueEntry(imDto->_imAccount,USERNAME,pDto->_uid);
}

// �������ADDRESS
void IosContactProperty::AddPropertyAddress(Property* pDto)
{
    Address *aDto = (Address*)pDto->_value;
    // ����ABMultiValueEntry�������
    InsertIntoABMultiValueEntry(aDto->_street,STREET,pDto->_uid);
    InsertIntoABMultiValueEntry(aDto->_city,CITY,pDto->_uid);
    InsertIntoABMultiValueEntry(aDto->_zipCode,ZIP,pDto->_uid);
    InsertIntoABMultiValueEntry(aDto->_province,STATE,pDto->_uid);
    InsertIntoABMultiValueEntry(aDto->_countryCode,COUNTRYCODE,pDto->_uid);
    InsertIntoABMultiValueEntry(aDto->_country,COUNTRY,pDto->_uid);
}

// ����ABMultiValueEntry�������
void IosContactProperty::InsertIntoABMultiValueEntry(wstring value,wstring key,int propertyUid)
{
    if (value == L"")
        return ;

    CppSQLite3Buffer sql;
    sql.Format("Select ROWID From ABMultiValueEntryKey Where value = '%s';",CCodeOperation::UnicodeToUTF_8SQL(key).c_str());
    int iKey = _contactDB->ExecuteAndGetId((const char*)sql);

    if (iKey == -1)
    {
        sql.Format("Insert into ABMultiValueEntryKey Values('%s');",CCodeOperation::UnicodeToUTF_8SQL(key).c_str());
        if (_contactDB->ExecDML((const char*)sql) > 0)
        {
            sql.Format("Select ROWID From ABMultiValueEntryKey Where value = '%s';",CCodeOperation::UnicodeToUTF_8SQL(key).c_str());
            key = _contactDB->ExecuteAndGetId((const char*)sql);
        }
    }
    sql.Format("INSERT OR REPLACE INTO ABMultiValueEntry VALUES(%d,%d,'%s');",propertyUid,iKey,CCodeOperation::UnicodeToUTF_8SQL(value).c_str());
    _contactDB->ExecDML((const char*)sql);
}

// ����ABPhoneLastFour��
void IosContactProperty::UpdateABPhoneLastFour(Property* pDto)
{
    // ��ȡ���Ա��ID
    CppSQLite3Buffer sql;
    sql.Format("SELECT MAX(UID) FROM ABMultiValue");
    pDto->_uid = _contactDB->ExecuteAndGetId((const char*)sql);
    if (pDto->_uid == -1)// ��ȡ���Ա��ID���ɹ�
        return ;

    string tmp = (char *)pDto->_value;
    wstring value = CCodeOperation::UTF_8ToUnicode(tmp);

    wstring lastFour = value.length() < 5 ? value : value.substr(value.length() - 4, 4);
    sql.Format("INSERT INTO ABPhoneLastFour VALUES(%d,'%s');",pDto->_uid,CCodeOperation::UnicodeToUTF_8SQL(lastFour).c_str());
    _contactDB->ExecDML((const char*)sql);
}

wstring IosContactProperty::GetPcTime(const char* iosTime)
{
    string temp = iosTime;
    if (temp == "")
        return L"";

    time_t time = (time_t)atoi(iosTime) + CDealTime::IosStartTime().GetTime();

    WCHAR szDaytime[20] = {0};
    wsprintf(szDaytime, L"%d", (int)time);
    wstring strDaytime = szDaytime;
    return  strDaytime;
}

string IosContactProperty::GetIosTime(wstring pcTime)
{
    time_t now_time = (time_t)_wtoi(pcTime.c_str());
    time_t time = now_time - CDealTime::IosStartTime().GetTime();

    char szDaytime[20] = {0};
    sprintf_s(szDaytime, "%d", (int)time);
    string strDaytime = szDaytime;
    return  strDaytime;
}

/*
// �����������ͻ�ȡ�����б�
void IosContactProperty::GetPropertyByType(PropertyTypes type, wstring labelKey, vector<Property*> *propertyVec)
{
    // ���������������б�
    for (size_t i = 0; i < _ios_propertyList.size(); i++)
    {
        Property* pDto = _ios_propertyList.at(i);
        if (pDto->_property != type)
            continue;

        if (type == PROPERTY_RINGTONE && pDto->_identifier == -2)
            continue;

        if (labelKey == L"")
            propertyVec->push_back(pDto);
        else
            if (CStrOperation::toLowerW(pDto->_labelKey) == CStrOperation::toLowerW(labelKey))
                propertyVec->push_back(pDto);
    }
}
*/

#endif