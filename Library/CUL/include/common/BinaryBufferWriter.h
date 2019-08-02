#pragma once
#include "../cul.h"
#include "../stdint.h"
#include <vector>

namespace ND91Assistant
{
    // ������Buffer������: дBuffer
    class CUL_CLASS_DECL BinaryBufferWriter
    {
    public:
        // ���buffer
        void    Clear();

        // ����Bufferָ�룬size�д��buffer����
        const char* GetBuffer(int& size);

        // ���ر�׼string
        std::string GetBuffer();

        void WriteInt(int i);
		void WriteInt32(int32_t i);
		void WriteShort(short i);
        void WriteChar(char c);
        void WriteBool(bool b);
        void WriteInt64(int64_t l);

        void WriteTime(time_t t);
        void WriteString(std::wstring s);

        // �ظ�д��ĳ���ַ����ɸ�
        void WriteRepeatChar(char c, int count);

        //��д��GB2312֮ǰ��д���ַ����ĳ���
        void WriteString(std::string s);

        //��GB2312תΪUTF8��д�룬д��֮ǰ��д���ַ����ĳ���
        void WriteStringAsUTF8(std::string s);

        void WriteStringAsUTF8(std::wstring s);

        void WriteBuffer(const char* pBuffer, int size);

        void ReplaceInt(int i,int pos);
        void ReplaceBuffer(const char* pBuffer, int size,int pos);

    private:
        // ʹ��vector����Ϊ���ڴ��������ģ��ɽ�(&vect[0])��Ϊָ��ʹ��
        // ȱ�����ڴ�����·��䣬����size���Ǻܴ��������ǿ��Խ��ܵ�
#pragma warning (disable:4251)
        std::vector<char> _buffer;
#pragma warning (default:4251)
    };
}