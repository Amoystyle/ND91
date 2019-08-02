#pragma once
#include <stdlib.h>
#include <string.h>
#include "d3des.h"

string DesEncryptEBC(string strIn, unsigned char* key)
{
	size_t size = strIn.length();
	unsigned char* pIn = new unsigned char[size + 8];
	memcpy(pIn, strIn.c_str(), size);

	if (size % 8 > 0)
	{
		int nPKCS7 = 8 - size % 8;
		for (int i=0; i<nPKCS7; i++)
			pIn[size++] = nPKCS7;
	}

	unsigned char* pOut= new unsigned char[size];

	memset(pOut, 0, size);

	deskey(key, EN0);
	des(pIn, pOut);

	// 运算模式EBC
	unsigned char pBuffer[8];
	for( size_t i = 8; i < size; i+= 8 )
	{
		memcpy(pBuffer, pIn + i, 8);
		des(pBuffer, pBuffer);
		memcpy(pOut + i, pBuffer, 8);
	}
	string strOut((char*)pOut, size);
	delete[] pIn;
	delete[] pOut;
	return strOut;
}



string DesEncrypt(string strIn, unsigned char* key)
{
	size_t size = strIn.length();
	unsigned char* pIn = new unsigned char[size + 8];
	memcpy(pIn, strIn.c_str(), size);

	if (size % 8 > 0)
	{
		int nPKCS7 = 8 - size % 8;
		for (int i=0; i<nPKCS7; i++)
			pIn[size++] = nPKCS7;
	}
	
	unsigned char* pOut= new unsigned char[size];

	memset(pOut, 0, size);

	deskey(key, EN0);
	des(pIn, pOut);

	// 运算模式默认值为 CBC
	unsigned char pBuffer[8];
	for( size_t i = 8; i < size; i+= 8 )
	{
		memcpy(pBuffer, pIn + i, 8);
		for(int j = 0; j < 8; ++j)
		{
			pBuffer[j] ^= pOut[i - 8 + j];
		}
		des(pBuffer, pBuffer);
		memcpy(pOut + i, pBuffer, 8);
	}
	string strOut((char*)pOut, size);
	delete[] pIn;
	delete[] pOut;
	return strOut;
}

string DesDecrypt(string strIn, unsigned char* key)
{
	size_t size = strIn.length();
	unsigned char* pIn = new unsigned char[size];
	unsigned char* pOut= new unsigned char[size+8];
	memcpy(pIn, strIn.c_str(), size);
	memset(pOut, 0, size+8);

	deskey(key, DE1);
	des(pIn, pOut);

	// 运算模式默认值为 CBC
	unsigned char pBuffer[8];
	for( size_t i = 8; i < size; i+= 8 )
	{
		memcpy(pBuffer, pIn + i, 8);
		des(pBuffer, pBuffer);
		for(int j = 0; j < 8; ++j)
		{
			pBuffer[j] ^= pIn[i - 8 + j];
		}
		memcpy(pOut + i, pBuffer, 8);
	}

	// 填充模式默认值为PKCS7
	int nPlus = 0;
	for(int x = 7; x > 0; --x)
	{
		bool bPadding = true;
		for(int y = 8-x; y < 8; ++y)
		{
			if ( pBuffer[y] != x )
			{
				bPadding = false;
				break;
			}
		}
		if ( bPadding )
		{
			for(int y = 8-x; y < 8; ++y)
				pBuffer[y] = 0;
			memcpy(pOut + size -8, pBuffer, 8);
			nPlus = x;
		}
	}
	string strOut((char*)pOut, size - nPlus);
	delete[] pIn;
	delete[] pOut;
	return strOut;
}
