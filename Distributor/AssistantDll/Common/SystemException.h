#pragma once

#include "Common/BaseException.h"

// 宏定义：用这个宏来简化SystemException的构造函数
#define SYSTEM_EXCETPION(x) SystemException(SystemException::x, __FILE__ , __LINE__)
//#ifdef _DEBUG
#define THROW_SYSTEM_EXCETPION(x) throw SystemException(SystemException::x, __FILE__ , __LINE__)
// #else
// #define THROW_SYSTEM_EXCETPION(x)
// #endif

namespace ND91Assistant
{
    class SystemException : public BaseException
    {
    public:
        // 系统错误代码
        enum SYSTEM_ERROR_CODE
        {
            MEMORY_OUT,             // 内存不足
            FILE_IO_FORBIDDEN,      // 文件读写权限不足
            DISK_FULL,              // 磁盘空间满
            GET_TCP_TABLE_FAIL,     // call GetTcpTable() fail
        };

        SystemException(SYSTEM_ERROR_CODE code, string filename, int lineNo)
            : BaseException((int) code, filename, lineNo)
        {

        }
    };
}