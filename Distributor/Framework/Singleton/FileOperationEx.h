#pragma once

class SINGLETON_API CFileOperationEx
{
public:
	static string GetMD5(wstring filePath);
private:
	CFileOperationEx(void);
	~CFileOperationEx(void);
	static size_t readBinFile(wstring strFilePath, char** ppData);
};
