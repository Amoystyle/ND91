#include "stdafx.h"
#ifdef MODULE_CONTACT

#define PYDATFILENAME       L"py.dat"     // 拼音码表文件名
#define PYCUSTOMDATFILENAME L"Py_Custom.dat" // 自定义拼音码表文件名
#define PINYINVERSION   L"0.0.0";        // 拼音版本

IosAutoSort::IosAutoSort(IosContactDB *contactDB)
{
    _contactDB      = contactDB;
    _currWorkingDir = CFileOperation::GetCurrWorkingDir();
}

IosAutoSort::~IosAutoSort()
{
    _contactDB = NULL;
}

// 获取拼音库版本
wstring IosAutoSort::GetPinyinVersion()
{
    CppSQLite3Buffer sql;
    sql.Format("Select * from [sqlite_master] where type = 'table' and name = 'PinyinVersion'");
    // 检查是否添加过pinyin补丁
    if (!CheckHasData((const char*)sql))
        return PINYINVERSION;

    sql.Format("Select * from [sqlite_master] where type = 'table' and name = 'PinyinVersion'");
    // 检查是否存在版本信息表
    if (!CheckHasData((const char*)sql))
        return PINYINVERSION;

    // 获取拼音版本
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

// 获取IOS设备版本
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

// 安装一键排序
bool IosAutoSort::Patch_Install(wstring pyVersion)
{
    if (GetIosVersion() == VERSION_1)
        return SortPathInstall_For_1X(pyVersion);
    else if (GetIosVersion() == VERSION_2)
        return SortPathInstall_For_2X_3X(pyVersion,true);
    else if (GetIosVersion() == VERSION_3)
        return SortPathInstall_For_2X_3X(pyVersion,false);
    else if (GetIosVersion() == VERSION_43)//4.x 开始已经不需要这个一键排序了.
        return SortPatchInstall_For_4X_43X(true);
    else
        return SortPatchInstall_For_4X_43X(false);
}

bool IosAutoSort::SortPathInstall_For_1X(wstring pyVersion)
{
    // 拼音码表路径
    wstring pyDatPath = _currWorkingDir + PYDATFILENAME;
    if (!CFileOperation::IsFileExist(pyDatPath.c_str()))
        return false;

    // 如果已经安装过，则先卸载
    if (IsPatched())
        SortPatchUnInstall_For_1X();

    // ================ 准备重新排序 ================
    CppSQLite3Buffer sql;
    sql.Format("update ABPerson set FirstSort = null, LastSort = null;delete from sorting_first_section_list;delete from sorting_last_section_list;");
    _contactDB->ExecDML((const char*)sql);

    // ================ 安装补丁 ================
    string szSql = IosCreateDBSQL::InstallPatchSQL_1X();
    _contactDB->ExecDML((const char*)szSql.c_str());

    // ================ 码表分析 ================
    // 用户自定义码表 
    wstring pyCustomDatPath = _currWorkingDir + PYCUSTOMDATFILENAME;
    if (CFileOperation::IsFileExist(pyCustomDatPath.c_str()))
        InstallCodeTable_1X(pyCustomDatPath);

    // 系统维护的码表读取 
    if (CFileOperation::IsFileExist(pyDatPath.c_str()))
        InstallCodeTable_1X(pyDatPath);

    // ================ 写拼音数据版本号 ================
    sql.Format("DROP TABLE IF EXISTS PinyinVersion;CREATE TABLE PinyinVersion(value TEXT);Insert into PinyinVersion values('%s');",CCodeOperation::UnicodeToUTF_8SQL(pyVersion).c_str());
    _contactDB->ExecDML((const char*)sql);

    // ================ 重新排序 ================
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

// 检查是否安装过一键排序补丁
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

//"啊", "A " -> "'啊','A '" 
wstring IosAutoSort::PaddingPinyin(wstring fileContext)
{
    // 将内容以逗号拆分
    vector<wstring> strArray = CStrOperation::parseStringsW(fileContext, L",");

    if (strArray.size() <= 1)
        return fileContext;

    // word中的双引号用空格代替并去除左右空格
    wstring word = CStrOperation::ReplaceW(strArray.at(0), L"\"", L" ");
    word = CStrOperation::trimRightW(word, L" ");
    word = CStrOperation::trimLeftW(word, L" ");

    // pinyin中的双引号用空格代替并去除左右空格
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
    // 拼音码表路径
    wstring pyDatPath = _currWorkingDir + PYDATFILENAME;
    if (!CFileOperation::IsFileExist(pyDatPath.c_str()))
        return false;

    // 如果已经安装过，则先卸载
    if (IsPatched())
        SortPatchUnInstall_For_2X_3X_4X();

    // 安装一键排序
    // 创建拼音码表
    CppSQLite3Buffer sql;
    sql.Format("Drop TABLE IF EXISTS [Pinyin];CREATE TABLE [Pinyin]([character] text,[phoneticize] text,[LanguageIndex] integer);CREATE INDEX PinyinCharIDX on Pinyin(character);");
    _contactDB->ExecDML((const char*)sql);

    // 安装默认值
    sql.Format("INSERT INTO [Pinyin] VALUES('~',NULL,2147483647);");
    _contactDB->ExecDML((const char*)sql);

    // 初始化排序键对照字典
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

    // 安装用户自定义码表 
    wstring pyCustomDatPath = _currWorkingDir + PYCUSTOMDATFILENAME;
    if (CFileOperation::IsFileExist(pyCustomDatPath.c_str()))
        InstallCodeTable_2X_3X_4X(pyCustomDatPath,iPhone30SortDic,is2Xor3X);

    // 安装系统码表 
    if (CFileOperation::IsFileExist(pyDatPath.c_str()))
        InstallCodeTable_2X_3X_4X(pyDatPath,iPhone30SortDic,is2Xor3X);

    // 重置排序
    sql.Format("update [ABPerson] set FirstSort = null,LastSort = null, FirstSortSection = null,LastSortSection = null,FirstSortLanguageIndex = 2147483647, LastSortLanguageIndex = 2147483647;");
    _contactDB->ExecDML((const char*)sql);

    // 安装补丁
    string szSql = IosCreateDBSQL::InstallPatchSQL_2X_3X();
    _contactDB->ExecDML((const char*)szSql.c_str());

    // 排序
    string szSerial = CCodeOperation::UnicodeToUTF_8SQL(_contactDB->GetSerial());
    sql.Format("delete from [FirstSortSectionCount];delete from [LastSortSectionCount];\
               insert into [abperson](First,Last,StoreId) values('%s','%s',0); \
               update [FirstSortSectionCount] set number = (select count(*) from [ABPerson]) where LanguageIndex = 2147483647;\
               update [LastSortSectionCount] set number = (select count(*) from [ABPerson]) where LanguageIndex = 2147483647;\
               update [ABPerson] set kind = 0;delete from [abperson] where First='%s' and Last='%s';",szSerial.c_str(),szSerial.c_str(),szSerial.c_str(),szSerial.c_str());
    _contactDB->ExecDML((const char*)sql);

    // 记录拼音库版本
    sql.Format("Drop TABLE IF EXISTS [PinyinVersion];CREATE TABLE PinyinVersion(value TEXT);INSERT INTO [PinyinVersion] values('%s');",CCodeOperation::UnicodeToUTF_8SQL(pyVersion).c_str());
    _contactDB->ExecDML((const char*)sql);

    return true;
}

// ================ 卸载补丁 ================ 
bool IosAutoSort::SortPatchUnInstall_For_2X_3X_4X()
{
    string szSql = IosCreateDBSQL::UnInstallPatchSQL_2X_3X_4X();
    return _contactDB->ExecDML((const char*)szSql.c_str()) > 0 ? true : false;
}

// 安装码表
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
        // 将内容以逗号拆分
        vector<wstring> strArray = CStrOperation::parseStringsW(line, L",");
        // character中的双引号用空格代替并去除左右空格
        wstring character = CStrOperation::ReplaceW(strArray.at(0), L"\"", L" ");
        character = CStrOperation::trimRightW(character, L" ");
        character = CStrOperation::trimLeftW(character, L" ");

        // pinyin中的双引号用空格代替并去除左右空格
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
    // 拼音码表路径
    wstring pyDatPath = _currWorkingDir + PYDATFILENAME;
    if (!CFileOperation::IsFileExist(pyDatPath.c_str()))
        return false;

    //默認就先卸載,以免旧的代码影响
    SortPatchUnInstall_For_2X_3X_4X();

    // 安装一键排序
    // 创建拼音码表
    CppSQLite3Buffer sql;
    sql.Format("Drop TABLE IF EXISTS [Pinyin];CREATE TABLE [Pinyin]([character] text,[phoneticize] text,[LanguageIndex] integer);CREATE INDEX PinyinCharIDX on Pinyin(character);");
    _contactDB->ExecDML((const char*)sql);

    // 初始化排序键对照字典
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

    // 安装默认值
    sql.Format("INSERT INTO [Pinyin] VALUES('~',NULL,2147483647);");
    _contactDB->ExecDML((const char*)sql);

    // 安装系统码表 
    if (CFileOperation::IsFileExist(pyDatPath.c_str()))
        InstallCodeTable_2X_3X_4X(pyDatPath,iPhone40SortDic,false);

    // 重置排序
    sql.Format("update [ABPerson] set FirstSort = null,LastSort = null, FirstSortSection = null,LastSortSection = null,FirstSortLanguageIndex = 2147483647, LastSortLanguageIndex = 2147483647 where FirstSort isnull AND LastSort isnull;");
    _contactDB->ExecDML((const char*)sql);

    // 安装补丁
    string szSql = IosCreateDBSQL::InstallPatchSQL_4X();
    _contactDB->ExecDML((const char*)szSql.c_str());

    // 排序
    sql.Format("update [ABPerson] set kind = 0  Where [abperson].firstsort isnull AND [abperson].lastsort isnull;");
    _contactDB->ExecDML((const char*)sql);

    return true;
}

// 卸载一键排序
bool IosAutoSort::Patch_UnInstall()
{
    if (GetIosVersion() == VERSION_1)
        return SortPatchUnInstall_For_1X();
    else 
        return SortPatchUnInstall_For_2X_3X_4X();
}

#endif