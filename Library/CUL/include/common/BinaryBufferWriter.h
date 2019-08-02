#pragma once
#include "../cul.h"
#include "../stdint.h"
#include <vector>

namespace ND91Assistant
{
    // 二进制Buffer辅助类: 写Buffer
    class CUL_CLASS_DECL BinaryBufferWriter
    {
    public:
        // 清除buffer
        void    Clear();

        // 返回Buffer指针，size中存放buffer长度
        const char* GetBuffer(int& size);

        // 返回标准string
        std::string GetBuffer();

        void WriteInt(int i);
		void WriteInt32(int32_t i);
		void WriteShort(short i);
        void WriteChar(char c);
        void WriteBool(bool b);
        void WriteInt64(int64_t l);

        void WriteTime(time_t t);
        void WriteString(std::wstring s);

        // 重复写入某个字符若干个
        void WriteRepeatChar(char c, int count);

        //在写入GB2312之前会写入字符串的长度
        void WriteString(std::string s);

        //将GB2312转为UTF8后写入，写入之前会写入字符串的长度
        void WriteStringAsUTF8(std::string s);

        void WriteStringAsUTF8(std::wstring s);

        void WriteBuffer(const char* pBuffer, int size);

        void ReplaceInt(int i,int pos);
        void ReplaceBuffer(const char* pBuffer, int size,int pos);

    private:
        // 使用vector是因为其内存是连续的，可将(&vect[0])作为指针使用
        // 缺点是内存会重新分配，但在size不是很大的情况下是可以接受的
#pragma warning (disable:4251)
        std::vector<char> _buffer;
#pragma warning (default:4251)
    };
}