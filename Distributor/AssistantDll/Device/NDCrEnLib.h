///////////////////////////////////////////////////////////
//  CNDCrEnLib
//  ndCrEnDll.dll库的导出函数类
//  用于加密解密
///////////////////////////////////////////////////////////

#pragma once

#ifdef WIN32
    #include <Windows.h>
    #define DLLNDCREN L"ndCrEnDll.dll"
#else
    #include <Common/winPort.h>
    #define DLLNDCREN "ndCrEnDll.dylib"
#endif

typedef void (*Encrypt)(const char* strPlain, unsigned char* key, unsigned char* pCryptBuf);
typedef void (*Decrypt)(unsigned char* key, unsigned char* pCryptBuf, char* outPtr);

class CNDCrEnLib
{
public:
	CNDCrEnLib(void);
	~CNDCrEnLib(void);

	static bool MyLoadLibrary();
	static void MyFreeLibrary();

	static bool Ios_Encrypt(const char* strPlain, unsigned char* key, unsigned char* pCryptBuf);
	static bool Ios_Decrypt(unsigned char* key, unsigned char* pCryptBuf, char* outPtr);

private:
	static HINSTANCE m_ndCrEnLib;
	static Encrypt m_Encrypt;
	static Decrypt m_Decrypt;
};
