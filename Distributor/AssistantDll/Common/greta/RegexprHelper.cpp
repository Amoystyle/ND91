#include "stdafx.h"

#include "RegexprHelper.h"
#include "Module/File/FileData.h"
#include "Device/AdbDirOperateCommand.h"

RegexprHelper::RegexprHelper()
{
    _spacemode = Espacemodenone;
}

RegexprHelper::~RegexprHelper()
{
}

match_results::backref_type RegexprHelper::PatternMatch(rpattern url_pattern, wstring data, match_results results)
{
#ifdef WIN32 
    return url_pattern.match(data, results);
#else
    return url_pattern.match(CCodeOperation::UnicodeToUTF_8(data), results);
#endif
}

void RegexprHelper::PatternInit(rpattern url_pattern, wstring matchstr, REGEX_FLAGS dw)
{
#ifdef WIN32 
    url_pattern.init(matchstr, dw);
#else
    url_pattern.init(CCodeOperation::UnicodeToUTF_8(matchstr), dw);
#endif
}

string RegexprHelper::GetBackrefString(match_results results, size_t pos)
{
#ifdef WIN32    	
    return CCodeOperation::UnicodeToGB2312(results.backref(pos).str());
#else
    return results.backref(pos).str();
#endif
}

wstring RegexprHelper::GetBackrefWstring(match_results results, size_t pos)
{
#ifdef WIN32    	
    return results.backref(pos).str();
#else
    return CCodeOperation::UTF_8ToUnicode(results.backref(pos).str());
#endif
}

#ifdef WIN32
#define CopyFileCommand_PARSE_RESULT_r  L"^(\\d+) [\\w, ]+ ([\\d,\\.]+)[\\w, ]+\\((\\d+)[\\w, ,/]+\\)"
#else
#define CopyFileCommand_PARSE_RESULT_r  "^(\\d+) [\\w, ]+ ([\\d,\\.]+)[\\w, ]+\\((\\d+)[\\w, ,/]+\\)"
#endif
void RegexprHelper::ParseResultCopyFileCommand(const string& result, CopyInfo retCopyInfo)
{
    vector<string>  resultparts = CStrOperation::parseLines(result.c_str(), result.length());
    RETURN_IF( resultparts.empty() );

    string  data = resultparts[resultparts.size()-1];

    match_results results;
    REGEX_FLAGS dw = GLOBAL | ALLBACKREFS;
    match_results::backref_type br ;

#ifdef WIN32
    wstring s = CCodeOperation::UTF_8ToUnicode(data);
#else
    string s = data;	    	
#endif

    rpattern url_pattern(CopyFileCommand_PARSE_RESULT_r, dw);
    br = url_pattern.match(s, results);

    RETURN_IF( ! br.matched );
    for(size_t i = 1; i < results.cbackrefs(); i++)
    {
#ifdef WIN32
        string temp = CCodeOperation::UnicodeToGB2312(results.backref(i).str());
#else
        string temp = results.backref(i).str();
#endif

        switch(i)
        {
        case 1:
            sscanf(temp.c_str(), "%d", (int*)&retCopyInfo._mTotalTransfered);
            break;
        case 2:
            sscanf(temp.c_str(), "%f", (float*)&retCopyInfo._mDuration);
            break;
        case 3:
            sscanf(temp.c_str(), "%d", (int*)&retCopyInfo._mSpeed);
            break;
        default:
            break;
        }
    }
}

#ifdef WIN32
// 模式一：dirname  %dK total, %dK used, %dK available (block size 4096)
#define matchmode1 L"(.+): (\\d+)K total, (\\d+)K used, (\\d+)K .+"
// 模式二：%d %d %d dirname
#define matchmode2 L"^[/,\\w]{0,}\\W+(\\d+)\\W+(\\d+)\\W+(\\d+)\\W+[\\d,%]+\\W(.+)"
// 模式三：dirname: %d(k/g/m) %d(k/g/m) %d(k/m/g)
#define matchmode3 L"^(.+) +(\\d+\\w+) +(\\d+\\w+) +(\\d+\\w+) +.+"
#else
// 模式一：dirname  %dK total, %dK used, %dK available (block size 4096)
#define matchmode1 "(.+): (\\d+)K total, (\\d+)K used, (\\d+)K .+"
// 模式二：%d %d %d dirname
#define matchmode2 "^[/,\\w]{0,}\\W+(\\d+)\\W+(\\d+)\\W+(\\d+)\\W+[\\d,%]+\\W(.+)"
// 模式三：dirname: %d(k/g/m) %d(k/g/m) %d(k/m/g)
#define matchmode3 "^(.+) +(\\d+\\w+) +(\\d+\\w+) +(\\d+\\w+) +.+"
#endif
SpaceInfo RegexprHelper::AnalyzeSpaceInfo(const string& result)
{
    SpaceInfo adir;
    adir._path = L"";
    adir._Available = 0; 
    adir._totalsize = 0;
    adir._usedsize = 0;

    match_results results;
    REGEX_FLAGS dw = GLOBAL | ALLBACKREFS;
    rpattern url_pattern(matchmode1, dw);

#ifdef WIN32
    wstring s = CCodeOperation::UTF_8ToUnicode(result);
#else
    string s = result;
#endif

    match_results::backref_type br = url_pattern.match(s, results);
    if (br.matched)
    {
        _spacemode = Espacemode1;
        AnalyzeSpaceMode(results, adir);
    }
    else
    {
        url_pattern.init(matchmode2, dw);
        br = url_pattern.match(s, results);
        if(br.matched)
        {
            _spacemode = Espacemode2;
            AnalyzeSpaceMode(results, adir);
        }
        else
        {
            url_pattern.init(matchmode3, dw);
            br = url_pattern.match(s, results);
            if (br.matched)
            {
                _spacemode = Espacemode3;
                AnalyzeSpaceMode(results, adir);
            }
        }
    }
    adir._Available *= 1024; 
    adir._totalsize *= 1024;
    adir._usedsize  *= 1024;
    return adir;
}

void RegexprHelper::AnalyzeSpaceMode( match_results results, SpaceInfo& adir )
{	
    int num = 0;
#ifdef WIN32
    if ( _spacemode == Espacemode1 || _spacemode == Espacemode3)
        adir._path = results.backref(++num).str();

    if (_spacemode == Espacemode1 || _spacemode == Espacemode2)
    {
        adir._totalsize = _wtol(results.backref(++num).str().c_str());
        adir._usedsize  = _wtol(results.backref(++num).str().c_str());
        adir._Available = _wtol(results.backref(++num).str().c_str());
    }
    else if(_spacemode == Espacemode3)
    {
        adir._totalsize = ConvertToKB(results.backref(++num).str());
        adir._usedsize = ConvertToKB(results.backref(++num).str());
        adir._Available = ConvertToKB(results.backref(++num).str());
    }

    if ( _spacemode == Espacemode2)
        adir._path = results.backref(++num).str();
#else
    if ( _spacemode == Espacemode1 || _spacemode == Espacemode3)
        adir._path = CCodeOperation::UTF_8ToUnicode(results.backref(++num).str());

    if (_spacemode == Espacemode1 || _spacemode == Espacemode2)
    {
        adir._totalsize = atol(results.backref(++num).str().c_str());
        adir._usedsize  = atol(results.backref(++num).str().c_str());
        adir._Available = atol(results.backref(++num).str().c_str());
    }
    else if(_spacemode == Espacemode3)
    {
        adir._totalsize = ConvertToKB(CCodeOperation::UTF_8ToUnicode(results.backref(++num).str()));
        adir._usedsize = ConvertToKB(CCodeOperation::UTF_8ToUnicode(results.backref(++num).str()));
        adir._Available = ConvertToKB(CCodeOperation::UTF_8ToUnicode(results.backref(++num).str()));
    }

    if ( _spacemode == Espacemode2)
        adir._path = CCodeOperation::UTF_8ToUnicode(results.backref(++num).str());
#endif
}

int RegexprHelper::ConvertToKB(wstring str)
{
    int size = atol(CCodeOperation::WstringToString(str.substr(0,str.length()-1)).c_str());
    if (size<0)
        return 0;
    wstring temp = CStrOperation:: toLowerW(str.substr(str.length()-1,1));
    if (temp == L"g")
        size = size*1024*1024;
    else if(temp == L"m")
        size = size*1024;
    return size;
}

// 无法匹配者丢弃
#ifdef WIN32
#define dr L"^([\\w,-]+) (\\w+)\\W+(\\w+)\\W+([\\d,-]+) ([\\d,:]+) (.+)"
#define fr L"^([\\w,-]+) (\\w+)\\W+(\\w+)\\W+([\\d,\\W,\\,]+?)\\W+([\\d,-]+) ([\\d,:]+) (.+)"
#define matchstr2 L"^([\\w,-]+)\\W+(\\d+)\\W+(\\d+)\\W+(\\d+)\\W+([\\d,\\W,\\,]+?) (\\w+\\W+\\d+\\W+[\\d,:]+) (.+)" 
#define matchstr3 L"^([\\w,-]+) +(\\d+) +(\\w+) +(\\w+) +(\\d+) +(\\w+\\W+\\d+\\W+[\\d,:]+) (.+)" 
#else
#define dr "^([\\w,-]+) (\\w+)\\W+(\\w+)\\W+([\\d,-]+) ([\\d,:]+) (.+)"
#define fr "^([\\w,-]+) (\\w+)\\W+(\\w+)\\W+([\\d,\\W,\\,]+?)\\W+([\\d,-]+) ([\\d,:]+) (.+)"
#define matchstr2 "^([\\w,-]+)\\W+(\\d+)\\W+(\\d+)\\W+(\\d+)\\W+([\\d,\\W,\\,]+?) (\\w+\\W+\\d+\\W+[\\d,:]+) (.+)" 
#define matchstr3 "^([\\w,-]+) +(\\d+) +(\\w+) +(\\w+) +(\\d+) +(\\w+\\W+\\d+\\W+[\\d,:]+) (.+)" 
#endif

bool RegexprHelper::ParseResultListDirectoryCommand(const string& result)
{
    if (   result.find("No such file or directory") != string::npos  
        || result.find("Permission denied") != string::npos 
        || result.find("Not a directory") != string::npos )
    {
        return false;
    }

    // 结果为空的情况是空目录，应该算是成功
    RETURN_TRUE_IF(result.empty());

    vector<string> lines = CStrOperation::parseLines(result);
    RETURN_FALSE_IF ( lines.empty() );

#ifdef WIN32
    // 判断返回结果的格式
    wstring regular;    // 正则表达式，由GetMatchMode方法确定
    TMatchmode matchmode = GetMatchMode(CCodeOperation::UTF_8ToUnicode(lines[0]), regular);  
#else
    // 判断返回结果的格式
    string regular;    // 正则表达式，由GetMatchMode方法确定
    TMatchmode matchmode = GetMatchMode(lines[0], regular); 
#endif

    // 如果是查目录，转换出目录指针
    DirData* pDir = NULL;       
    if(NodeData::DIR == _pNode->_type)
    {
        pDir = dynamic_cast<DirData*>(_pNode);
        RETURN_FALSE_IF( ! pDir );
        pDir->ClearChilds();
    }

    for(size_t i =0 ; i < lines.size() ; ++i )
    {
        if(ENonemode == matchmode)
        {
            if (i+1 < lines.size())
            {
#ifdef WIN32
                matchmode = GetMatchMode(CCodeOperation::UTF_8ToUnicode(lines[i+1]), regular);
#else
                matchmode = GetMatchMode(lines[i+1], regular);
#endif
                continue;
            }
        }

        regex::match_results results;
        if(EMathmode1 == matchmode)// 每行都得判断
        {
            regular = lines[i][0]=='-'?fr:dr;
        }

        regex::rpattern url_pattern(regular, GLOBAL | ALLBACKREFS);
        wstring line = CCodeOperation::UTF_8ToUnicode(lines[i]);

#ifdef WIN32
        if (url_pattern.match(line, results).matched)    
#else
        if (url_pattern.match(lines[i], results).matched) 
#endif
        {
            if ( pDir )
            {
                // 目录操作: 每行生成一个节点，放到目录中              
                CreateNodeInDir(pDir, line, results, matchmode);               
            }
            else
            {
                // 文件或链接：读一行就返回
                SetNodeValue(results, _pNode, matchmode);
                return true;
            }
        }
    } 

    return true;
}

#ifdef WIN32
RegexprHelper::TMatchmode RegexprHelper::GetMatchMode(const wstring line, wstring& regular)
#else
RegexprHelper::TMatchmode RegexprHelper::GetMatchMode(const string line, string& regular)
#endif
{
    regex::match_results results;
    regex::rpattern url_pattern;

    bool bIsFile =  '-' == line[0];

    url_pattern.init(bIsFile ? fr : dr, GLOBAL | ALLBACKREFS);
    if (url_pattern.match(line, results).matched)
    {
        regular = bIsFile ? fr : dr;
        return EMathmode1;
    } 	

    url_pattern.init(matchstr2, GLOBAL | ALLBACKREFS);
    if (url_pattern.match(line, results).matched)
    {
        regular = matchstr2;
        return EMathmode2;
    } 

    url_pattern.init(matchstr3, GLOBAL | ALLBACKREFS);
    if (url_pattern.match(line, results).matched)
    {
        regular = matchstr3;
        return EMathmode3;
    }	

    // 糟糕，格式匹配不上
    wostringstream woss;
#ifdef WIN32    
    woss << "ListDirectoryCommand::ParseResult Fail: " << line;
#else
		woss << "ListDirectoryCommand::ParseResult Fail: " << CCodeOperation::UTF_8ToUnicode(line);
#endif    
    LOG->WriteAlert(woss.str());
    //  THROW_ASSISTANT_EXCEPTION(LS_RESULT_PARSE_FAIL);

    return ENonemode;
}

NodeData* RegexprHelper::CreateNodeInDir(DirData* pDir, wstring line, regex::match_results results, TMatchmode mode)
{
    NodeData* pNode;
    switch ( line[0] )
    {
    case 'd':
        pNode = pDir->AddDir();
        break;

    case 'l':
        pNode = pDir->AddLink();
        break;
    case '-':
        pNode = pDir->AddFile();
        break;
    default:
        return NULL;
    }

    SetNodeValue(results, pNode, mode);
    return pNode;
}

void RegexprHelper::SetNodeValue(match_results results, NodeData* pNode, TMatchmode matchMode)
{
    int year, month, day, hour, minut;

    int no = 1;     // 匹配性列表：第一个匹配项是原文，跳过

    // 第一列：权限
#ifdef WIN32
    wstring temp = results.backref(no).str();
    wstring mode = temp.substr(1, temp.length()-1);
    string ansiMode = CCodeOperation::UnicodeToGB2312(mode);
    pNode->_mode  = Mode(ansiMode).GetMode();
#else
    string temp = results.backref(no).str();
    string mode = temp.substr(1, temp.length()-1);
    pNode->_mode  = Mode(mode).GetMode();
#endif

    if (EMathmode3 == matchMode)   // 模式3：跳过第二列
    {
        ++no;
    }

    // 用户、组
#ifdef WIN32
    pNode->_owner = CCodeOperation::UnicodeToGB2312(results.backref(++no).str());
    pNode->_group = CCodeOperation::UnicodeToGB2312(results.backref(++no).str());
#else
    pNode->_owner = results.backref(++no).str();
    pNode->_group = results.backref(++no).str();
#endif

    if ( EMathmode1 == matchMode )
    {
        // 匹配模式1：文件有size，目录和链接没有size
        if ( dynamic_cast<FileData*>(pNode) )
        {      	
#ifdef WIN32
            pNode->_size = _wtol(results.backref(++no).str().c_str());
#else
            pNode->_size = atol(results.backref(++no).str().c_str());
#endif
        } 
#ifdef WIN32
        swscanf_s(results.backref(++no).str().c_str(), L"%d-%d-%d", &year, &month, &day);
        swscanf_s(results.backref(++no).str().c_str(), L"%d:%d", &hour, &minut);
#else
        sscanf(results.backref(++no).str().c_str(), "%d-%d-%d", &year, &month, &day);
        sscanf(results.backref(++no).str().c_str(), "%d:%d", &hour, &minut);
#endif
        pNode->_modifyTime = CDealTime(year, month, day, hour, minut, 0).GetTime();
    }
    else // EMathmode2 or EMathmode3
    {
        if  ( EMathmode2 == matchMode )       // 模式二：跳过第4列
        {
            ++no;
        }
#ifdef WIN32
        pNode->_size = _wtol(results.backref(++no).str().c_str());      // 模式2、3，目录也有大小
        pNode->_modifyTime = ParseDate(CCodeOperation::UnicodeToGB2312(results.backref(++no).str()));// 时间
#else
        pNode->_size = atol( results.backref(++no).str().c_str() );      // 模式2、3，目录也有大小
        pNode->_modifyTime = ParseDate( results.backref(++no).str() );// 时间
#endif
    }

    // 最后一列：名字，如果是link 则是 etc -> /system/etc 格式 
#ifdef WIN32    
    wstring name = results.backref(++no).str();
#else 
    wstring name = CCodeOperation::UTF_8ToUnicode(results.backref(++no).str());
#endif    

    LinkData* pLink = dynamic_cast<LinkData*>(pNode);
    if ( pLink )
    {
        int pos = name.find(L" -> ");
        pLink->_name = name.substr(0, pos);
        pLink->_linkTarget = name.substr(pos+4, name.length()-pos-4);
    }
    else
        pNode->_name = name;

    pNode->_displayName = pNode->_name;

    // 过滤 . 和 .. 二项
    if ( pNode->_name == L"." || pNode->_name == L"..")
        pNode->GetParent()->RemoveFromChildrens(pNode);
}

time_t RegexprHelper::ParseDate(string datestr)
{
    const char* MONTH[] = {
        "JAN",
        "FEB",
        "MAR",
        "APR",
        "MAY",
        "JUN",
        "JUL",
        "AUG",
        "SEP",
        "OCT",
        "NOV",
        "DEC"
    };
    time_t  ret = 0;
    vector<string>  sArray = CStrOperation::parseStrings(datestr.c_str(), datestr.length(),' ');
    if(sArray.size()==3)
    {
        // YEAR 的默认值是当前年份（未考虑时区变化）
        // by Qiuchang 2012.7.12 (Bug 19816)
        tm t; time_t now = time(NULL);
        gmtime_s(&t, &now);
        int year = 1900 + t.tm_year; // tm_year: current year minus 1900


        int month=1, day=1, hour=0, minut=0;
        sscanf(sArray[1].c_str(), "%d", &day);

        if((int)sArray[2].find(":") > 0)
        {
            sscanf(sArray[2].c_str(), "%d:%d", &hour, &minut);
        }
        else
        {
            sscanf(sArray[2].c_str(), "%d:%d", &hour, &minut);
        }

        string temp = CStrOperation::toUpper(sArray[0]);
        month = 1; 
        for(size_t i = 0; i < 12; i++) 
        {
            if ( MONTH[i] == temp )
                month = i+1;
        }

        CDealTime dltime(year, month, day, hour, minut, 0);
        ret = dltime.GetTime();
    }
    return ret;
}

#ifdef WIN32
#define MountSystem_r  L"(\\S+)\\s(\\S*\\s)?/system\\s(type\\s)?(\\S+).*"
#else
#define MountSystem_r  "(\\S+)\\s(\\S*\\s)?/system\\s(type\\s)?(\\S+).*"
#endif
bool RegexprHelper::ParseIsMountSystem(wstring strSerialNumber)
{
    AdbDirOperateCommand pAdbDirOp(strSerialNumber);
    pAdbDirOp.setParam("mount");
    pAdbDirOp.Execute();
    RETURN_FALSE_IF (!pAdbDirOp.IsOk());

    string strResult = pAdbDirOp.GetReturnData();

    string strBlock, strType;
    vector<string> verDatas = CStrOperation::parseLines(strResult);
    for ( int i=0;i<(int)verDatas.size();i++ )
    {
#ifdef WIN32
        wstring strTemp = CCodeOperation::UTF_8ToUnicode(verDatas[i]);
#else
        string strTemp = verDatas[i];
#endif

        match_results results;
        REGEX_FLAGS dw = GLOBAL | ALLBACKREFS;
        match_results::backref_type br ;

        rpattern url_pat(MountSystem_r, dw);
        br = url_pat.match(strTemp, results);

        if( br.matched )
        {
#ifdef WIN32
            strBlock = CCodeOperation::UnicodeToGB2312(results.backref(1).str());
            strType  = CCodeOperation::UnicodeToGB2312(results.backref(4).str());
#else
            strBlock = results.backref(1).str();
            strType  = results.backref(4).str();
#endif
            break;
        }
    }

    if ( !strBlock.empty() && !strType.empty() )
    {
        string cmd = "mount -o remount -w -t" + strType + " " + strBlock + " /system";

        pAdbDirOp.setParam(cmd.c_str());
        pAdbDirOp.Execute();

        RETURN_FALSE_IF (!pAdbDirOp.IsOk());

        strResult = pAdbDirOp.GetReturnData();
        if ( strResult.empty() )
        {
            cmd = "chmod 0777 /system/app";
            pAdbDirOp.setParam(cmd.c_str());
            pAdbDirOp.Execute();

            RETURN_FALSE_IF(!pAdbDirOp.IsOk());
            strResult = pAdbDirOp.GetReturnData();

            return strResult.empty();
        }
    }

    return false;
}