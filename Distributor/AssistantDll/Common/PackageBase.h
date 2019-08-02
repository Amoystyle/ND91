#pragma once
#include <string>
using namespace std;

class CPackageBase
{
public:
	CPackageBase(void);
	~CPackageBase(void);
public:
	static void writeInt(char* pData, int n);
	static void writeInt(string strData, int n);
	static int  readInt(const char* pData);

	static void  writeShort(char* pData, short n);
	static void  writeShort(string strData, short n);
	static short readShort(const char* pData);

	static void writeBinary(char* pData, const char* pBinary, size_t nSize);
	static void writeBinary(string strData, const char* pBinary, size_t nSize);
	static size_t readBinary(char* pData, char** ppBinary, bool bMalloc=true);
	static void readBinary(char* pData, char* pBinary, int nSize);
};
