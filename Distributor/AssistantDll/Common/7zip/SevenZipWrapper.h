#pragma once

#include <string>

class SevenZipWrapper
{
public:
	// Summary  : 解压
	// Returns  : bool 成功返回TURE
	// Parameter: std::wstring strSrcFile ：要解压的7z文件
	// Parameter: std::wstring strDesDir  ：解压后存放的路径
	static bool Extract(std::wstring strSrcFile, std::wstring strDesDir);

    // Summary  : 压缩
    // Returns  : bool 成功返回TURE
    // Parameter: std::wstring strSrcDir  ：要解压的目录
    // Parameter: std::wstring strDesFile ：压缩文件路径
    static bool Compress(std::wstring strSrcDir, std::wstring strDesFile);
};

