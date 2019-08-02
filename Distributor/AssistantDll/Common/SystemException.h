#pragma once

#include "Common/BaseException.h"

// �궨�壺�����������SystemException�Ĺ��캯��
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
        // ϵͳ�������
        enum SYSTEM_ERROR_CODE
        {
            MEMORY_OUT,             // �ڴ治��
            FILE_IO_FORBIDDEN,      // �ļ���дȨ�޲���
            DISK_FULL,              // ���̿ռ���
            GET_TCP_TABLE_FAIL,     // call GetTcpTable() fail
        };

        SystemException(SYSTEM_ERROR_CODE code, string filename, int lineNo)
            : BaseException((int) code, filename, lineNo)
        {

        }
    };
}