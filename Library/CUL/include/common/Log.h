#pragma once
#include "../cul.h"
#ifdef WIN32
    #include <windows.h>
    #define pthread_mutex_t HANDLE
#else
    #include <common/winPort.h>
#endif
#include <stdio.h>
#include <string>

using namespace std;
#pragma warning(disable : 4251)

#define LOG Log::GetInstance()

#define LOG_DEBUG(x)    if (Log::GetInstance()->GetLevel() <= Log::LOG_DEBUG) Log::GetInstance()->WriteDebug(x);
#define LOG_ERROR(x)    if (Log::GetInstance()->GetLevel() <= Log::LOG_ERROR) Log::GetInstance()->WriteError(x);
#define LOG_ALERT(x)    if (Log::GetInstance()->GetLevel() <= Log::LOG_ALERT) Log::GetInstance()->WriteAlert(x);

// 输出二进制数据
#define LOG_BINARY(x, y)    if (Log::GetInstance()->GetLevel() <= x) Log::GetInstance()->WriteBinary(x, y);

#define LOG_FUNTCION_BEGIN  {wstringstream woss;woss << "Begin Function " << __FUNCTION__;LOG_DEBUG(woss.str());}
#define LOG_FUNTCION_END  {wstringstream woss;woss << "End Function " << __FUNCTION__;LOG_DEBUG(woss.str());}

// LOG服务类
// 单例，建议使用LOG宏
class CUL_CLASS_DECL Log
{
public:
    enum LOG_LEVEL { LOG_DEBUG, LOG_ERROR, LOG_ALERT };

    // 自动启动LOG服务：
    // 输出文件路径请设置 STR_LOG_FILE_PATH 选项
    // 输出文件大小请设置 INT_MAX_LOG_FILE_SIZE 选项
    // 若未设置则自动在程序所在目录创建assistant.log文件
    static Log* GetInstance();
    static void Release();

#pragma region 控制方法

    // 重新启动Log（改变输出文件位置或大小时使用）
    void Restart();

    // 停止Log
    void Stop();

    // 设置输出等级: 目前支持等级由低到高为: DEBUG, ERROR, ALERT
    // 默认为最低级别DEBUG, 若设置为高等级，则低等级输出将被忽略
    void SetLevel(LOG_LEVEL level);
    LOG_LEVEL GetLevel() { return _level; }

#pragma endregion

#pragma region 输出LOG的方法


    // 输出调试信息（最低优先级）
    void WriteDebug(wstring msg);

    // 输出错误信息
    // 创建Exception时自动输出此级别信息
    void WriteError(wstring msg);

    // 输出警示信息（最高优先级）
    void WriteAlert(wstring msg);

    // 新增：等同printf的可变参数类型
    void Write(LOG_LEVEL level, const wchar_t* fmt, ... );

    // 新增：输出二进制数据
    void WriteBinary(LOG_LEVEL level, string buffer);

#pragma endregion

private:
    Log(void);
    virtual ~Log(void);

    // 启动Log服务
    // 参数path：文件名（若非完整路径）
    void Start();

    void Write(wstring msg, LOG_LEVEL level);

    void GetOption();

    void CreateFile();

    void CheckFileSize();

private:
    static Log* _pInstance; // 单例实例
    LOG_LEVEL   _level;     // 输出等级
    wstring     _path;      // log文件名（不含时间和扩展名）
    wstring     _filename;  // log文件名（含时间和扩展名）
    int         _maxSize;   // log文件最大长度
    FILE*       _pFile;     // log文件指针

    pthread_mutex_t      _mutex;     // 锁
};
