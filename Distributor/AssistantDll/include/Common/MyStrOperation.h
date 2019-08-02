#pragma once

#include <string>
#include <vector>
#include "stdint.h"
using namespace std;

/// 字符串处理
class CLASSINDLL_CLASS_DECL MyStrOperation
{
public:

    // 获取时间描述, nDouble(倍数)单位，1标识毫秒，1000标识秒
    static wstring FormatTime(int nSecond, int nDouble=1);
    static wstring FormatTime(time_t nTime);

    // 生成随机数
    static int Random( int start, int end );

    // 中文字符判定
    static bool IsChineseLetter(wchar_t ch);
    static bool IsChineseString(wstring str);

    // 中文转拼音
    static string ChineseToPinyin(wstring chinese);
};
