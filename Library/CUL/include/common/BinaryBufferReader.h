#pragma once
#include "../cul.h"
#include "../stdint.h"
#include <vector>
#include <string>
#include <fstream>

namespace ND91Assistant
{
    // 二进制Buffer辅助类: 读Buffer
    class CUL_CLASS_DECL BinaryBufferReader
    {
    public:
        BinaryBufferReader(const char* pBuffer, int size);
        BinaryBufferReader(std::wstring filename);
        ~BinaryBufferReader() { if ( _ifs.is_open() ) _ifs.close(); }

        // 获得当前读取位置
        int GetOffset();

		// Summary  : 设置读取位置，不允许越界
		// Returns  : 是否设置成功
		// Parameter: int n 新读取位置
		bool SetOffset(int n);

        // Summary  : 增加(跳过)若干byte
        // Returns  : 是否设置成功
        // Parameter: int n 读取位置增加的数量
        bool AddOffset(int n);
    
        // 读取位置清零
        void ResetOffset();

        bool ReadChar(char& c);
        bool ReadBool(bool& b);
        bool ReadShort(short& i);
        bool ReadUInt16(unsigned short& i );

        bool ReadTime32(time_t& t);
		
        bool ReadInt64(/*__int64*/int64_t& l);
		bool ReadInt32(/*__int32*/int32_t& i );
        bool ReadUInt32(/*unsigned __int32*/uint32_t& i );
        bool ReadFloat(float& f );
        bool ReadDouble(double& f );

        bool ReadString(std::string& s, int size);
        bool ReadString(std::wstring& s, int size);
		bool ReadWString(std::wstring& s, int size);

        // 本方法不会添加NULL结束符，指针p需预先分配好内存
        // size是拷贝到p的字节数
        bool ReadBuffer(char* p, int size, bool bReverse = false);

		//没有指明大小，表示接下来的4个字节会代表大小
		bool ReadString(std::string& s);

        //没有指明大小，表示接下来的4个字节会代表大小, 读取完后将结果从UTF8转成GB2312
        bool ReadStringFromUTF8(std::string& s);       

        //没有指明大小，表示接下来的4个字节会代表大小, 读取完后将结果从UTF8转成UNICODE
        bool ReadStringFromUTF8(std::wstring& s);       

		//获得当前数据区指针
		const char* GetCurBuffer();

		//获取当前数据区大小
		int GetSize();

		//已到末尾
		bool IsEnd(){return _offset >= _size;}
		
        void SetBigEndian(bool b);
		//预读几个字符
		std::string PreView(int len);
		//偏移量移到关键字
		bool MoveTo(const char* strvalue);

    private:
        // Summary  : 反转字节序
        // Parameter: char * p 起始指针
        // Parameter: int size 字节数
        void Reverse(char* p, int size);
		//内存字符串查找
		const char * memstr(const char *hay, size_t n, const char *needle);

    private:
        const char* _pBuffer;   // 数据区
        std::ifstream _ifs;     // 文件
        int _size;              // 数据区大小(不允许越界)
        int _offset;            // 当前读取位置

        bool _bigEndian;        // 字节序
    };
}