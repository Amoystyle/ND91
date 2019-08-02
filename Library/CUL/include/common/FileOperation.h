#pragma once

#include "../cul.h"

#ifdef WIN32
    #include <Windows.h>
    #include <tchar.h>
#endif

#include <iostream>
#include <vector>
using namespace std;

class CUL_CLASS_DECL CFileOperation  
{
public:

#pragma region 文件相关操作

    // Summary  : 创建空文件
    static bool CreateEmptyFile(const wchar_t* lpszFileName);

    // Summary  : 删除文件
    static bool DeleteFile(wstring strSrc);

    // Summary  : 移动文件
    static bool MoveFile( wstring strSrc, wstring strDes );

    // Summary  : 复制文件
    // Parameter: bool bFailIfExists true：若文件存在则不复制并返回失败
    static bool CopyFile( wstring strSrc, wstring strDes, bool bFailIfExists );

    // Summary  : 若指定文件已存在，则在后面添加_XX（自动递增）
    // Comment  : 不支持多线程
    static wstring RenameFile(const wstring localName);

    // Summary  : 判断文件是否存在
    static bool IsFileExist(const wchar_t* lpszFileName);

	//  Summary  :判断是否是ios系统的plist文件
	static bool IsIosPlist(wstring strPcPath);

    // Summary  : 读取文件大小
    static int GetFileSize(const string& filename);
    static int GetFileSizeW(const wstring& filename);

    // Summary  : 读取文件内容（指定是否二进制方式）
    static string ReadFile(wstring strFilePath, bool bBinary=false);

    // Summary  : 读取文件内容（二进制方式）
    static string GetFileAllContent(string strFullName);

    // Summary  : 写入文件（二进制方式）
    static bool WriteBinFile(wstring strFilePath, const char* pData, size_t nSize);

    //  Summary  : 生成指定文件的MD5校验码
    static string GetFileMD5(string strFilePath);
    static string GetFileMD5(wstring strFilePath);

#pragma endregion

#pragma region 目录相关方法

    // 创建目录（逐级自动创建）
    static bool MakeLocalDir(wstring strDir);

    // Summary  : 删除文件或文件夹
    // Parameter: wstring strSrc  : 欲删除的文件或文件夹名（含路径）
    // Parameter: DWORD nFlags    : 选项，默认为FOF_NOCONFIRMATION | FOF_NOERRORUI
    //                              即不弹出确认提示（统统YES)和错误提示
    //                              如需自定义请查看SHFILEOPSTRUCT的FILEOP_FLAGS
    static bool DeleteDir(wstring strSrc, DWORD nFlags = 0);   

    // Summary  : 拷贝文件或文件夹
    static bool CopyDir(wstring strSrc, wstring strDes);

    // Summary  : 删除文件夹及其下的所有文件
    static bool DeleteDirectory(wstring strDir);

    // Summary  : 判断目录是否存在
    static bool IsDirExist(const wchar_t* lpszFileName);

    // Summary  : 若指定目录已存在，则在后面添加_XX（自动递增）
    // Comment  : 不支持多线程
    static wstring RenameDir(const wstring localName);

    // Summary  : 获取当前进程的EXE文件所在目录
    // Commment : 注意可能不是当前工作目录
    static std::wstring GetCurrWorkingDir();

    // Summary  : 取得目录下的节点总数和文件大小总和
    // Parameter: const wstring & dir       : 目录名（完整路径）
    // Parameter: int & nodeSum             : 输出：节点总数（文件数）
    // Parameter: __int64 & sizeSum         : 输出：文件大小总和
    // Parameter: vector<wstring>* fileExts : 只计算指定后缀的文件
    static void GetDirSizeW(const wstring& dir, int& nodeSum, __int64& sizeSum, vector<wstring>* fileExts = NULL);

    // Summary  : 获取目录下的所有文件，但不包括子目录下的文件
    // Parameter: wstring strDir            : 目录名（完整路径）
    // Parameter: vector<wstring> & files   : 输出：所有文件名（完整路径）
    // Parameter: wstring strExt            : 只计算指定后缀的文件
    static void FindAllFileInDir(wstring strDir, vector<wstring> &files, wstring strExt=L"");

    // Summary  : 获取目录下的所有文件夹，但不包括子目录下的目录
    // Parameter: wstring strDir            : 目录名（完整路径）
    // Parameter: vector<wstring> & files   : 输出：所有文件夹名（完整路径）
    static void FindAllFolderInDir(wstring strDir, vector<wstring> &folders);

    // Summary  : 遍历文件夹，返回与制定文件名匹配的文件
    // Parameter: wstring strDir            : 欲遍历的文件夹
    // Parameter: wstring strFileName       : 匹配的文件名
    // Parameter: vector<wstring> & vtGet   : 返回的匹配文件列表
    // Parameter: bool bAbsolutelyMatch     : 是否完全匹配，为FALSE则不考虑大小写
    static void GetMatchFileFullName(wstring strDir, wstring strFileName, vector<wstring>& vtGet, bool bAbsolutelyMatch = true);

    // Summary  : 遍历文件夹，返回第一个文件夹名以strEndName结尾的文件夹
    // Parameter: wstring strDir            : 欲遍历的文件夹
    // Parameter: wstring strEndName        : 结尾文件夹名
    // Parameter: wstring& strDirFullName   : 输出：返回第一个匹配文件夹全名
    static void GetMatchDirFullNameByEndName(wstring strDir, wstring strEndName, wstring& strDirFullName);

#pragma endregion

#pragma region 路径相关方法

    // Summary  : 取文件名
    // Returns  : 最后一个/或\之后的字符串
    static wstring GetFileName(const wstring strFilePath);

    // Summary  : 取文件扩展名
    // Returns  : 最后一个/或\之后的字符串，其中最后一个.之后的内容
    static wstring GetFileExt(const wstring strFilePath);

    // Summary  : 取文件名
    // Returns  : 最后一个/或\之后的字符串，并去掉最后一个.之后的内容
    static wstring GetFileNameNoExt(const wstring strFilePath);

    // Summary  : 取文件路径
    // Returns  : 最后一个/或\之前的字符串
    static wstring GetFileDir(const wstring strFilePath);

    // Summary  : 取指定文件或目录的父目录
    static wstring GetParentDir(const wstring strPath);

    // Summary  : 拼接目录和文件名，生成完整文件路径
    // Comment  :  ● 没有做目录和文件是否存在的校验
    //             ● 若任一输入参数为空则返回空
    // Returns  : 完整文件路径
    // Parameter: const wstring dir         目录名
    // Parameter: const wstring filename    文件名
    static wstring MakeFilePath(const wstring& dir, const wstring& filename);

#pragma endregion

#pragma region 压缩解压操作

    // Summary  : 解压zip文件到指定路径
    // wstring strPreDirName 指定解压文件的上层目录名,根目录为 “/”
    static bool UnZipFile(wstring strSrcFileFullName, wstring strDesFullPath, wstring strSpecificFileName = L"", bool bMatchPath = true, wstring strPreDirName = L"",bool IsUtf8=false);

    // Summary  : 压缩文件夹
    // Returns  : 成功返回TRUE
    // Parameter: wstring strSrcFileFullName ：欲压缩的文件夹名（含路径）
    // Parameter: wstring strDesFileFullName ：压缩后的文件名（含路径）
    static bool ZipDirectory(wstring strSrcFileFullName, wstring strDesFileFullName);

    // Summary  : 压缩文件
    // Returns  : 成功返回TRUE
    // Parameter: vector<wstring> files         欲压缩的文件夹名（含路径）
    // Parameter: wstring strDesFileFullName    压缩后的文件名（含路径）
    static bool ZipFile(wstring file, wstring strDesFileFullName);
    static bool ZipFile(vector<wstring> files, wstring strDesFileFullName);

#pragma endregion

#pragma region 其他操作

    // Summary  : 取得GUID
    static string GetGuid();
    static wstring GetGuidW();

    // Summary  : 读取ini配置文件
    // Returns  : 指定key的值
    // Parameter: wstring strPath   ini文件路径
    // Parameter: wstring strGroup  配置项的Group
    // Parameter: wstring strName   配置项的key
    static wstring GetProfileString(wstring strPath, wstring strGroup, wstring strName);

#pragma endregion
};
