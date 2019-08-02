#pragma once
#include "../cul.h"
#include <iostream>
#include "zip.h"
#include "unzip.h"
#include <vector>

using namespace std;

#define CASESENSITIVITY (0)
#define WRITEBUFFERSIZE (8192)
#define MAXFILENAME (256)

//zip文件压缩和解压缩类
class CUL_CLASS_DECL NDZlib
{
public:
	NDZlib(void);
	~NDZlib(void);

	// Summary  : 解压ZIP文件或其中的某个文件到指定目录
	// Returns  : bool 成功返回TRUE
	static bool UnZipFile(
        wstring strSrcFileFullName,         // 欲解压的文件名（含路径）
        wstring strDesFullPath,             // 解压后存放的目录
        wstring strSpecificFileName = L"",  // 指定解压的文件名
        bool bMatchPath = true,             // true 匹配路径 false 仅匹配文件名
        wstring strPreDirName = L"",		// wstring strPreDirName 指定解压文件的上层目录名  
		bool IsUtf8=false);					// 指定解压的内部文件名是否为utf8编码  

	// Summary  : 压缩文件夹
	// Returns  : bool 成功返回TRUE
	// Parameter: wstring strSrcFileFullName ：欲压缩的文件夹名（含路径）
	// Parameter: wstring strDesFileFullName ：压缩后的文件名（含路径）
	static bool ZipDirectory(wstring strSrcFileFullName, wstring strDesFileFullName);
    static bool ZipFile(vector<wstring> files, wstring strDesFileFullName);

    static int makedir2(const char* fileFuleName);
    static int makedir (const char *newdir);
    static void change_file_date(const char *filename, uLong dosdate, tm_unz tmu_date);

    // Summary  : 压缩包中是否存在指定文件
    // Returns  : true 存在 false 不存在
    // Parameter: const char * zipFilename  压缩包(gb2312)
    // Parameter: const char * filename     指定文件名(gb2312)
    static bool Exist(const wchar_t* zipFilename, const char* filename);

    // Summary  : 提供压缩包中文件（目录）列表的方法
    // Returns  : true 成功 false 失败
    // Parameter: wstring zipFilename       ZIP文件
    // Parameter: vector<wstring> & files   文件（目录）列表
    static bool GetFilesInZip(wstring zipFilename, vector<wstring> &files);

private:
	static int mymkdir(const char* dirname);
	static int do_extract_currentfile(unzFile uf, const int* popt_extract_without_path, int* popt_overwrite, const char* szSavePath);
	static int do_extract(unzFile uf, int opt_extract_without_path, int opt_overwrite, const char* szSavePath);
	static int do_extract_onefile(unzFile uf, const char* filename, int opt_extract_without_path, int opt_overwrite, const char* szSavePath, bool bMatchPath = true, const char* strPreDirName = NULL);

	static bool AddContenToZip(zipFile zf, const wstring strPath, const wstring parentDir);
	static bool AddFileToZip(zipFile zf, const char* fileNameInZip, const char* srcFile, bool bDir = false);
	static uLong filetime(char *f, tm_zip *tmzip, uLong *dt);
};
