#pragma once
#include "../cul.h"
#include "../stdint.h"
#include <vector>
#include <string>
#include <fstream>

namespace ND91Assistant
{
    // ������Buffer������: ��Buffer
    class CUL_CLASS_DECL BinaryBufferReader
    {
    public:
        BinaryBufferReader(const char* pBuffer, int size);
        BinaryBufferReader(std::wstring filename);
        ~BinaryBufferReader() { if ( _ifs.is_open() ) _ifs.close(); }

        // ��õ�ǰ��ȡλ��
        int GetOffset();

		// Summary  : ���ö�ȡλ�ã�������Խ��
		// Returns  : �Ƿ����óɹ�
		// Parameter: int n �¶�ȡλ��
		bool SetOffset(int n);

        // Summary  : ����(����)����byte
        // Returns  : �Ƿ����óɹ�
        // Parameter: int n ��ȡλ�����ӵ�����
        bool AddOffset(int n);
    
        // ��ȡλ������
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

        // �������������NULL��������ָ��p��Ԥ�ȷ�����ڴ�
        // size�ǿ�����p���ֽ���
        bool ReadBuffer(char* p, int size, bool bReverse = false);

		//û��ָ����С����ʾ��������4���ֽڻ�����С
		bool ReadString(std::string& s);

        //û��ָ����С����ʾ��������4���ֽڻ�����С, ��ȡ��󽫽����UTF8ת��GB2312
        bool ReadStringFromUTF8(std::string& s);       

        //û��ָ����С����ʾ��������4���ֽڻ�����С, ��ȡ��󽫽����UTF8ת��UNICODE
        bool ReadStringFromUTF8(std::wstring& s);       

		//��õ�ǰ������ָ��
		const char* GetCurBuffer();

		//��ȡ��ǰ��������С
		int GetSize();

		//�ѵ�ĩβ
		bool IsEnd(){return _offset >= _size;}
		
        void SetBigEndian(bool b);
		//Ԥ�������ַ�
		std::string PreView(int len);
		//ƫ�����Ƶ��ؼ���
		bool MoveTo(const char* strvalue);

    private:
        // Summary  : ��ת�ֽ���
        // Parameter: char * p ��ʼָ��
        // Parameter: int size �ֽ���
        void Reverse(char* p, int size);
		//�ڴ��ַ�������
		const char * memstr(const char *hay, size_t n, const char *needle);

    private:
        const char* _pBuffer;   // ������
        std::ifstream _ifs;     // �ļ�
        int _size;              // ��������С(������Խ��)
        int _offset;            // ��ǰ��ȡλ��

        bool _bigEndian;        // �ֽ���
    };
}