#include "NDCrEnLib.h"

HINSTANCE CNDCrEnLib::m_ndCrEnLib = NULL;
Encrypt CNDCrEnLib::m_Encrypt = NULL;
Decrypt CNDCrEnLib::m_Decrypt = NULL;

CNDCrEnLib::CNDCrEnLib(void)
{
	m_ndCrEnLib = NULL;
}

CNDCrEnLib::~CNDCrEnLib(void)
{
}

bool CNDCrEnLib::MyLoadLibrary()
{
	if (m_ndCrEnLib)
		return true;

	m_ndCrEnLib = ::LoadLibrary(DLLNDCREN);

	if (!m_ndCrEnLib)
	{
		return false;
	}
    return true;
}

void CNDCrEnLib::MyFreeLibrary()
{
	if (m_ndCrEnLib)
	{
        ::FreeLibrary(m_ndCrEnLib);
		m_ndCrEnLib = NULL;
	}
}

bool CNDCrEnLib::Ios_Encrypt(const char* strPlain, unsigned char* key, unsigned char* pCryptBuf)
{
	if (!MyLoadLibrary())
		return false;

    m_Encrypt = (Encrypt)GetProcAddress(m_ndCrEnLib, "Encrypt");

	if (!m_Encrypt)
	{
		return false;
	}
	m_Encrypt(strPlain, key, pCryptBuf);
	return true;
}

bool CNDCrEnLib::Ios_Decrypt(unsigned char* key, unsigned char* pCryptBuf, char* outPtr)
{
	if (!MyLoadLibrary())
		return false;

    m_Decrypt = (Decrypt)GetProcAddress(m_ndCrEnLib, "Decrypt");

	if (!m_Decrypt)
	{
		return false;
	}

	m_Decrypt(key, pCryptBuf, outPtr);
	return true;
}
