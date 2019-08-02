#pragma once

// 最多记录多少个浏览历史
#define  MAX_HISTORY_COUNT 128

// 文件管理模块 浏览历史处理类
class FileManagerHistory
{
public:
    FileManagerHistory(unsigned int maxCount);

    // 是否暂停增加新导航位置（前进、后退时使用）
    void EnableGoto(bool val) ;

    void    Goto(wstring path); // 跳转到新位置
    wstring Prev();             // 回退
    wstring Next();             // 前进
    wstring GetCurrent();       // 当前
    bool    IsHead();           // 是否已经到第一个
    bool    IsTail();           // 是否已经到最后一个
    void    DeleteCurrent();    // 删除当前路径

private:
    bool            _enableGoto;    // 开关
    unsigned int    _maxCount;  // 最多保存多少个历史记录
    list<wstring>   _history;   // 浏览历史

    list<wstring>::const_iterator     _current;    // 当前位置
};
