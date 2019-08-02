#include "stdafx.h"
#ifdef MODULE_CONTACT

#define PYDATFILENAME       L"py.dat"     // ƴ������ļ���
#define PYCUSTOMDATFILENAME L"Py_Custom.dat" // �Զ���ƴ������ļ���
#define PINYINVERSION   L"0.0.0";        // ƴ���汾

IosAutoSort::IosAutoSort(IosContactDB *contactDB)
{
    _contactDB      = contactDB;
    _currWorkingDir = CFileOperation::GetCurrWorkingDir();
}

IosAutoSort::~IosAutoSort()
{
    _contactDB = NULL;
}

// ��ȡƴ����汾
wstring IosAutoSort::GetPinyinVersion()
{
    CppSQLite3Buffer sql;
    sql.Format("Select * from [sqlite_master] where type = 'table' and name = 'PinyinVersion'");
    // ����Ƿ���ӹ�pinyin����
    if (!CheckHasData((const char*)sql))
        return PINYINVERSION;

    sql.Format("Select * from [sqlite_master] where type = 'table' and name = 'PinyinVersion'");
    // ����Ƿ���ڰ汾��Ϣ��
    if (!CheckHasData((const char*)sql))
        return PINYINVERSION;

    // ��ȡƴ���汾
    sql.Format("Select * from PinyinVersion");
    return SelectPinyinVersion((const char*)sql);
}

bool IosAutoSort::CheckHasData(const char* sql)
{
    CppSQLite3Query q = _contactDB->ExecQuery(sql);
    return !q.FieldIsNull(0);
}

wstring IosAutoSort::SelectPinyinVersion(const char* sql)
{
    CppSQLite3Query q = _contactDB->ExecQuery(sql);

    wstring version = L"";
    while (!q.Eof())
    {
        version = CCodeOperation::UTF_8ToUnicode(q.GetStringField(0));
        q.NextRow();
    }
    return version;
}

// ��ȡIOS�豸�汾
IosVersion IosAutoSort::GetIosVersion()
{
    int iProductVersion = IosPublicFunc::GetVerNum(_contactDB->_iosDBPath.GetDeviceData()->GetDevInfo()->_deviceProductVersion);

    if (iProductVersion >= 100 && iProductVersion < 200)
        return VERSION_1;
    else if (iProductVersion >= 200 && iProductVersion < 300)
        return VERSION_2;
    else if (iProductVersion >= 300 && iProductVersion < 400)
        return VERSION_3;
    else if (iProductVersion >= 400 && iProductVersion < 500)
        return VERSION_4;
    else if (iProductVersion >= 400 && iProductVersion < 430)
        return VERSION_43;
    else
        return VERSION_OTHER;
}

// ��װһ������
bool IosAutoSort::Patch_Install(wstring pyVersion)
{
    if (GetIosVersion() == VERSION_1)
        return SortPathInstall_For_1X(pyVersion);
    else if (GetIosVersion() == VERSION_2)
        return SortPathInstall_For_2X_3X(pyVersion,true);
    else if (GetIosVersion() == VERSION_3)
        return SortPathInstall_For_2X_3X(pyVersion,false);
    else if (GetIosVersion() == VERSION_43)//4.x ��ʼ�Ѿ�����Ҫ���һ��������.
        return SortPatchInstall_For_4X_43X(true);
    else
        return SortPatchInstall_For_4X_43X(false);
}

bool IosAutoSort::SortPathInstall_For_1X(wstring pyVersion)
{
    // ƴ�����·��
    wstring pyDatPath = _currWorkingDir + PYDATFILENAME;
    if (!CFileOperation::IsFileExist(pyDatPath.c_str()))
        return false;

    // ����Ѿ���װ��������ж��
    if (IsPatched())
        SortPatchUnInstall_For_1X();

    // ================ ׼���������� ================
    CppSQLite3Buffer sql;
    sql.Format("update ABPerson set FirstSort = null, LastSort = null;delete from sorting_first_section_list;delete from sorting_last_section_list;");
    _contactDB->ExecDML((const char*)sql);

    // ================ ��װ���� ================
    string szSql = IosCreateDBSQL::InstallPatchSQL_1X();
    _contactDB->ExecDML((const char*)szSql.c_str());

    // ================ ������ ================
    // �û��Զ������ 
    wstring pyCustomDatPath = _currWorkingDir + PYCUSTOMDATFILENAME;
    if (CFileOperation::IsFileExist(pyCustomDatPath.c_str()))
        InstallCodeTable_1X(pyCustomDatPath);

    // ϵͳά��������ȡ 
    if (CFileOperation::IsFileExist(pyDatPath.c_str()))
        InstallCodeTable_1X(pyDatPath);

    // ================ дƴ�����ݰ汾�� ================
    sql.Format("DROP TABLE IF EXISTS PinyinVersion;CREATE TABLE PinyinVersion(value TEXT);Insert into PinyinVersion values('%s');",CCodeOperation::UnicodeToUTF_8SQL(pyVersion).c_str());
    _contactDB->ExecDML((const char*)sql);

    // ================ �������� ================
    sql.Format("SELECT count(*) as personNums from abperson;");
    int personNums = _contactDB->ExecuteAndGetId((const char*)sql);
    if (personNums != -1)
    {
        sql.Format("INSERT OR REPLACE INTO sorting_first_section_list VALUES('~',%d);\
                   INSERT OR REPLACE INTO sorting_last_section_list VALUES('~',%d);\
                   update [abperson] set kind = kind - 1;update [abperson] set kind = kind + 1;",personNums,personNums);
        _contactDB->ExecDML((const char*)sql);
    }

    return true;
}

// ����Ƿ�װ��һ�����򲹶�
bool IosAutoSort::IsPatched()
{
    CppSQLite3Buffer sql;
    sql.Format("Select * from [sqlite_master] where type = 'table' and name = 'PinyinVersion'");
    return CheckHasData((const char*)sql);
}

bool IosAutoSort::SortPatchUnInstall_For_1X()
{
    string szSql = IosCreateDBSQL::UnInstallPatchSQL_1X();
    return _contactDB->ExecDML((const char*)szSql.c_str()) > 0 ? true : false;
}

void IosAutoSort::InstallCodeTable_1X(wstring filePath)
{
    ifstream ifs(filePath.c_str()/*, ios_base::in | ios_base::binary*/);
    while (!ifs.eof())
    {
        char szTemp[20] = {0};
        ifs.getline(szTemp, 20);
        string strTemp = szTemp;
        wstring strLine = CCodeOperation::GB2312ToUnicode(strTemp);

        if (strLine == L"" || CStrOperation::startsWith(strLine,L"'"))
            continue;

        strLine = CStrOperation::trimRightW(strLine, L" ");
        strLine = CStrOperation::trimLeftW(strLine, L" ");
        if (strLine.length() - CStrOperation::ReplaceW(CStrOperation::ReplaceW(strLine, L"\"", L" "),L",", L"").length() != 5 || !CStrOperation::startsWith(strLine,L"\"") || !CStrOperation::endsWithW(strLine,L"\""))
            continue;      

        strLine = PaddingPinyin(strLine);
        CppSQLite3Buffer sql;
        sql.Format("insert into Pinyin(character, phoneticize) values ('%s');",CCodeOperation::UnicodeToUTF_8SQL(strLine).c_str());
        _contactDB->ExecDML((const char*)sql);
    }
}

//"��", "A " -> "'��','A '" 
wstring IosAutoSort::PaddingPinyin(wstring fileContext)
{
    // �������Զ��Ų��
    vector<wstring> strArray = CStrOperation::parseStringsW(fileContext, L",");

    if (strArray.size() <= 1)
        return fileContext;

    // word�е�˫�����ÿո���沢ȥ�����ҿո�
    wstring word = CStrOperation::ReplaceW(strArray.at(0), L"\"", L" ");
    word = CStrOperation::trimRightW(word, L" ");
    word = CStrOperation::trimLeftW(word, L" ");

    // pinyin�е�˫�����ÿո���沢ȥ�����ҿո�
    string pinyin = CCodeOperation::UnicodeToUTF_8(strArray.at(1));
    pinyin = CStrOperation::Replace(pinyin, "\"", " ");
    pinyin = CStrOperation::trimRight(pinyin, " ");
    pinyin = CStrOperation::trimLeft(pinyin, " ");

    if (word == L"" || pinyin == "")
        return fileContext;

    if ((int)word[0] < 255)
        return fileContext;

    pinyin += "      ";
    pinyin = CStrOperation::toUpper(pinyin);

    map<string, int> indexDic;
    std::map<string,int>::iterator iter = indexDic.find(pinyin);
    if (iter != indexDic.end())
        indexDic.insert(pair<string, int>(pinyin, 0));
    else
        indexDic[pinyin]++;

    WCHAR szResult[20] = {0};
    wstring strResult = L"";

    char szTemp[20] = {0};
    string strTemp = "";
    sprintf(szTemp,"%d",indexDic[pinyin]);
    strTemp = szTemp; 

    string aa = pinyin + strTemp;
    wsprintf(szResult, L"'%s','%s'", word, CCodeOperation::UTF_8ToUnicode(pinyin + strTemp));

    strResult = szResult;
    return strResult;
}

bool IosAutoSort::SortPathInstall_For_2X_3X(wstring pyVersion,bool is2Xor3X)
{
    // ƴ�����·��
    wstring pyDatPath = _currWorkingDir + PYDATFILENAME;
    if (!CFileOperation::IsFileExist(pyDatPath.c_str()))
        return false;

    // ����Ѿ���װ��������ж��
    if (IsPatched())
        SortPatchUnInstall_For_2X_3X_4X();

    // ��װһ������
    // ����ƴ�����
    CppSQLite3Buffer sql;
    sql.Format("Drop TABLE IF EXISTS [Pinyin];CREATE TABLE [Pinyin]([character] text,[phoneticize] text,[LanguageIndex] integer);CREATE INDEX PinyinCharIDX on Pinyin(character);");
    _contactDB->ExecDML((const char*)sql);

    // ��װĬ��ֵ
    sql.Format("INSERT INTO [Pinyin] VALUES('~',NULL,2147483647);");
    _contactDB->ExecDML((const char*)sql);

    // ��ʼ������������ֵ�
    int index = 0;
    if (is2Xor3X)
        index = 41;
    else
        index = 44;

    map<char, int> iPhone30SortDic;
    for (int i = (int)'A'; i <= (int)'Z'; i++)
    {
        iPhone30SortDic.insert(pair<char, int>((char)i, index));
        index += 2;
    }

    // ��װ�û��Զ������ 
    wstring pyCustomDatPath = _currWorkingDir + PYCUSTOMDATFILENAME;
    if (CFileOperation::IsFileExist(pyCustomDatPath.c_str()))
        InstallCodeTable_2X_3X_4X(pyCustomDatPath,iPhone30SortDic,is2Xor3X);

    // ��װϵͳ��� 
    if (CFileOperation::IsFileExist(pyDatPath.c_str()))
        InstallCodeTable_2X_3X_4X(pyDatPath,iPhone30SortDic,is2Xor3X);

    // ��������
    sql.Format("update [ABPerson] set FirstSort = null,LastSort = null, FirstSortSection = null,LastSortSection = null,FirstSortLanguageIndex = 2147483647, LastSortLanguageIndex = 2147483647;");
    _contactDB->ExecDML((const char*)sql);

    // ��װ����
    string szSql = IosCreateDBSQL::InstallPatchSQL_2X_3X();
    _contactDB->ExecDML((const char*)szSql.c_str());

    // ����
    string szSerial = CCodeOperation::UnicodeToUTF_8SQL(_contactDB->GetSerial());
    sql.Format("delete from [FirstSortSectionCount];delete from [LastSortSectionCount];\
               insert into [abperson](First,Last,StoreId) values('%s','%s',0); \
               update [FirstSortSectionCount] set number = (select count(*) from [ABPerson]) where LanguageIndex = 2147483647;\
               update [LastSortSectionCount] set number = (select count(*) from [ABPerson]) where LanguageIndex = 2147483647;\
               update [ABPerson] set kind = 0;delete from [abperson] where First='%s' and Last='%s';",szSerial.c_str(),szSerial.c_str(),szSerial.c_str(),szSerial.c_str());
    _contactDB->ExecDML((const char*)sql);

    // ��¼ƴ����汾
    sql.Format("Drop TABLE IF EXISTS [PinyinVersion];CREATE TABLE PinyinVersion(value TEXT);INSERT INTO [PinyinVersion] values('%s');",CCodeOperation::UnicodeToUTF_8SQL(pyVersion).c_str());
    _contactDB->ExecDML((const char*)sql);

    return true;
}

// ================ ж�ز��� ================ 
bool IosAutoSort::SortPatchUnInstall_For_2X_3X_4X()
{
    string szSql = IosCreateDBSQL::UnInstallPatchSQL_2X_3X_4X();
    return _contactDB->ExecDML((const char*)szSql.c_str()) > 0 ? true : false;
}

// ��װ���
void IosAutoSort::InstallCodeTable_2X_3X_4X(wstring filePath,map<char, int> iPhone30SortDic,bool is2Xor3X)
{
    ifstream ifs(filePath.c_str()/*, ios_base::in | ios_base::binary*/);
    while (!ifs.eof())
    {
        char szTemp[20] = {0};
        ifs.getline(szTemp, 20);
        string strTemp = szTemp;
        wstring line = CCodeOperation::GB2312ToUnicode(strTemp);

        if (line == L"")
            continue;

        if (line.find(L",") == -1)
            continue;
        // �������Զ��Ų��
        vector<wstring> strArray = CStrOperation::parseStringsW(line, L",");
        // character�е�˫�����ÿո���沢ȥ�����ҿո�
        wstring character = CStrOperation::ReplaceW(strArray.at(0), L"\"", L" ");
        character = CStrOperation::trimRightW(character, L" ");
        character = CStrOperation::trimLeftW(character, L" ");

        // pinyin�е�˫�����ÿո���沢ȥ�����ҿո�
        string pinyin = CCodeOperation::UnicodeToUTF_8(strArray.at(1));
        pinyin = CStrOperation::Replace(pinyin, "\"", " ");
        pinyin = CStrOperation::trimRight(pinyin, " ");
        pinyin = CStrOperation::trimLeft(pinyin, " ");
        int languageIndex = 0;

        string section = MakeSortSection(pinyin, iPhone30SortDic,&languageIndex,is2Xor3X);
        CppSQLite3Buffer sql;
        sql.Format("INSERT INTO [Pinyin] VALUES('%s','%s',%d);",CCodeOperation::UnicodeToUTF_8SQL(character).c_str(),section.c_str(),languageIndex);
        _contactDB->ExecDML((const char*)sql);
    }
}

string IosAutoSort::MakeSortSection(string pystr, map<char, int> iPhone30SortDic, int *languageIndex, bool is2Xor3X)
{
    if (is2Xor3X)
        return MakeSortSection_For_2X(pystr, iPhone30SortDic, languageIndex);
    else
        return MakeSortSection_For_3X(pystr, iPhone30SortDic, languageIndex);
}

string IosAutoSort::MakeSortSection_For_2X(string pystr, map<char, int> iPhone30SortDic, int *languageIndex)
{
    string section = "";
    *languageIndex = 2147483647;

    if (pystr == "")
        return section;

    if (pystr.length() == 1 && pystr[0] >= '0' && pystr[0] <= '9')
        return section;

    for (size_t i = 0; i < pystr.length(); i++)
    {
        std::map<char,int>::iterator iter = iPhone30SortDic.find(pystr[i]);
        if (iter != iPhone30SortDic.end())
            section += (char)iter->second;
    }

    //section = section.PadRight(6, ' ').ToUpper();
    section += "      ";
    section = CStrOperation::toUpper(section);

    map<string, int> indexDic;
    std::map<string,int>::iterator iter = indexDic.find(section);
    if (iter != indexDic.end())
        indexDic[section]++;
    else
        indexDic.insert(pair<string, int>(section, 0));

    if (section != "")
        *languageIndex = 0;

    return section;
}

string IosAutoSort::MakeSortSection_For_3X(string pystr, map<char, int> iPhone30SortDic, int *languageIndex)
{
    string section = "";
    *languageIndex = 2147483647;

    if (pystr == "")
        return section;

    if (pystr.length() == 1 && pystr[0] >= '0' && pystr[0] <= '9')
        return section;

    for (size_t i = 0; i < pystr.length(); i++)
    {
        std::map<char,int>::iterator iter = iPhone30SortDic.find(pystr[i]);
        if (iter != iPhone30SortDic.end())
            section += (char)iter->second;
    }

    if (section != "")
        *languageIndex = 0;

    return section;
}

bool IosAutoSort::SortPatchInstall_For_4X_43X(bool is4Xor43X)
{
    // ƴ�����·��
    wstring pyDatPath = _currWorkingDir + PYDATFILENAME;
    if (!CFileOperation::IsFileExist(pyDatPath.c_str()))
        return false;

    //Ĭ�J����ж�d,����ɵĴ���Ӱ��
    SortPatchUnInstall_For_2X_3X_4X();

    // ��װһ������
    // ����ƴ�����
    CppSQLite3Buffer sql;
    sql.Format("Drop TABLE IF EXISTS [Pinyin];CREATE TABLE [Pinyin]([character] text,[phoneticize] text,[LanguageIndex] integer);CREATE INDEX PinyinCharIDX on Pinyin(character);");
    _contactDB->ExecDML((const char*)sql);

    // ��ʼ������������ֵ�
    int index = 0;
    if (is4Xor43X)
        index = 45;
    else
        index = 39;

    map<char, int> iPhone40SortDic;
    for (int i = (int)'A'; i <= (int)'Z'; i++)
    {
        iPhone40SortDic.insert(pair<char, int>((char)i, index));
        index += 2;
    }

    // ��װĬ��ֵ
    sql.Format("INSERT INTO [Pinyin] VALUES('~',NULL,2147483647);");
    _contactDB->ExecDML((const char*)sql);

    // ��װϵͳ��� 
    if (CFileOperation::IsFileExist(pyDatPath.c_str()))
        InstallCodeTable_2X_3X_4X(pyDatPath,iPhone40SortDic,false);

    // ��������
    sql.Format("update [ABPerson] set FirstSort = null,LastSort = null, FirstSortSection = null,LastSortSection = null,FirstSortLanguageIndex = 2147483647, LastSortLanguageIndex = 2147483647 where FirstSort isnull AND LastSort isnull;");
    _contactDB->ExecDML((const char*)sql);

    // ��װ����
    string szSql = IosCreateDBSQL::InstallPatchSQL_4X();
    _contactDB->ExecDML((const char*)szSql.c_str());

    // ����
    sql.Format("update [ABPerson] set kind = 0  Where [abperson].firstsort isnull AND [abperson].lastsort isnull;");
    _contactDB->ExecDML((const char*)sql);

    return true;
}

// ж��һ������
bool IosAutoSort::Patch_UnInstall()
{
    if (GetIosVersion() == VERSION_1)
        return SortPatchUnInstall_For_1X();
    else 
        return SortPatchUnInstall_For_2X_3X_4X();
}

#endif