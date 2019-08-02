
#include "PackageBase.h"
#include <stdlib.h>
#include <stdio.h>
#include "string.h"

CPackageBase::CPackageBase(void)
{
}

CPackageBase::~CPackageBase(void)
{
}

void CPackageBase::writeInt(char* pData, int n)
{
	pData[0] = (n << 24) >> 24;
	pData[1] = (n << 16) >> 24;
	pData[2] = (n << 8) >> 24;
	pData[3] = n >> 24;
}
void CPackageBase::writeInt(string strData, int n)
{
	char pData[4];
	writeInt(pData, 4);
	strData.append(pData, 4);
}
int CPackageBase::readInt(const char* pData)
{
	return ((unsigned char)pData[0])
		+  (((unsigned char)pData[1]) << 8)
		+  (((unsigned char)pData[2]) << 16)
		+  (((unsigned char)pData[3]) << 24);
}
void  CPackageBase::writeShort(char* pData, short n)
{
	pData[0] = (n << 8) >> 8;
	pData[1] = n >> 8;
}
void  CPackageBase::writeShort(string strData, short n)
{
	char pData[2];
	writeShort(pData, n);
	strData.append(pData, 2);
}
short CPackageBase::readShort(const char* pData)
{
	return ((unsigned char)pData[0])
		+  (((unsigned char)pData[1]) << 8);
}

void CPackageBase::writeBinary(char* pData, const char* pBinary, size_t nSize)
{
	writeInt(pData, nSize);
	memcpy(pData + 4, pBinary, nSize);
}

void CPackageBase::writeBinary(string strData, const char* pBinary, size_t nSize)
{
	writeInt(strData, nSize);
	strData.append(pBinary, nSize);
}

size_t CPackageBase::readBinary(char* pData, char** ppBinary, bool bMalloc)
{
	size_t nSize = readInt(pData);
	if (nSize > 10240000) return 0;	//防止出错导致大内存开辟(待定)
	if (bMalloc)
	{
		*ppBinary = (char*)malloc(nSize);
		memcpy(*ppBinary, pData+4, nSize);
	}
	else
	{
		*ppBinary = pData + 4;
	}
	return nSize;
}
void CPackageBase::readBinary(char* pData, char* pBinary, int nSize)
{
	memcpy(pBinary, pData, nSize);
}