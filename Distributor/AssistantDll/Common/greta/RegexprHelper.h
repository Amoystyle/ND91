#pragma once

/*********************************************************************
    用于跨平台的正则表达式辅助类
*********************************************************************/

#include "regexpr2.h"
#include "Module/File/RootData.h"
#include "Module/File/NodeData.h"
#include "Module/File/LinkData.h"
#include "Module/File/DirData.h"
#include <string>
using namespace std;
using namespace regex;

namespace ND91Assistant
{
    struct CopyInfo
    {
        long    _mTotalTransfered;
        double  _mDuration;
        long    _mSpeed;
    };

    class RegexprHelper  
    {
    public:
        RegexprHelper();
        ~RegexprHelper();

        match_results::backref_type PatternMatch(rpattern url_pattern, wstring data, match_results results);
        void PatternInit(rpattern url_pattern, wstring matchstr, REGEX_FLAGS dw);
        string GetBackrefString(match_results results, size_t pos);
        wstring GetBackrefWstring(match_results results, size_t pos);


        NodeData* GetNode() { return _pNode; }
        void SetNode(NodeData* pNode) { _pNode = pNode; }

        void ParseResultCopyFileCommand(const string& result, CopyInfo retCopyInfo);
        SpaceInfo AnalyzeSpaceInfo(const string& result);
        bool ParseResultListDirectoryCommand(const string& result);

        bool ParseIsMountSystem(wstring strSerialNumber);

    private:
        enum TSpaceMatchMode
        {
            Espacemodenone,
            Espacemode1,
            Espacemode2,
            Espacemode3
        };

        enum TMatchmode
        {
            ENonemode,
            EMathmode1,
            EMathmode2,
            EMathmode3
        };

        TSpaceMatchMode    _spacemode;  //种模式，
        NodeData*   _pNode;

    private:

        // 模式一：dirname  %dK total, %dK used, %dK available (block size 4096)
        // 模式二：%d %d %d dirname
        // 模式三：dirname: %d(k/g/m) %d(k/g/m) %d(k/m/g)
        void AnalyzeSpaceMode(match_results results, SpaceInfo& adir);

        int ConvertToKB(wstring str);
#ifdef WIN32
        TMatchmode GetMatchMode(const wstring s, wstring& regular);
#else
        TMatchmode GetMatchMode(const string s, string& regular);
#endif
        NodeData* CreateNodeInDir(DirData* pDir, wstring line, regex::match_results results, TMatchmode mode);
        void SetNodeValue(match_results results,NodeData* pNode,TMatchmode matchMode);
        time_t ParseDate(string  datestr);

    };
}