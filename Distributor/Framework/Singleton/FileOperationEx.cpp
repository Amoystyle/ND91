#include "StdAfx.h"
#include "FileOperationEx.h"
#include "md5.h"

CFileOperationEx::CFileOperationEx(void)
{
}

CFileOperationEx::~CFileOperationEx(void)
{
}

string CFileOperationEx::GetMD5( wstring filePath )
{
	char* buf = NULL;
	size_t nFileSize = readBinFile(filePath,&buf);
	MD5 md5(buf,nFileSize);
	string strMd5 = md5.toString();
	return strMd5;
}

size_t CFileOperationEx::readBinFile( wstring strFilePath, char** ppData )
{
	string path = CCodeOperation::ws2s(strFilePath);
	FILE *fp;
	if((fp=fopen(path.c_str(),"rb")) == NULL)
	{
		return 0;
	}
	fseek( fp, 0, SEEK_END );
	size_t nFileByteLen = ftell( fp );
	fseek( fp, 0, SEEK_SET );

	*ppData = (char*)malloc(nFileByteLen + 1);
	memset(*ppData, '\0', nFileByteLen + 1);
	fread( *ppData, nFileByteLen, sizeof(char), fp );
	fclose(fp);
	return nFileByteLen;
}
