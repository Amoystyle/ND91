#pragma once

#ifdef WIN32
    #include <Windows.h>
    #include <tchar.h>
#endif

#include "AssistantDll.h"
#include <iostream>
#include "Common/StrOperation.h"

using namespace std;
class CVersion;

struct CLASSINDLL_CLASS_DECL FileVersionInfo
{
    wstring _fileVersion;           // 版本号
    wstring _companyName;           // 公司名称
    wstring _fileDescription;       // 文件描述
    wstring _internalName;          // 内部名称
    wstring _legalCopyright;        // 合法版权
    wstring _originalFilename;      // 原始文件名
    wstring _productName;           // 产品名称
    wstring _productVersion;        // 产品版本
    wstring _legalTrademarks;       // 商标名
    wstring _comments;              // 注释
    wstring _privateBuild;
    wstring _specialBuild;
};

class CLASSINDLL_CLASS_DECL MyFileOperation  
{
public:

    // 判断是否是ios系统的plist文件
	static bool IsIosPlist(wstring strPcPath);

    //读取ini配置文件信息
    static wstring GetProfileString(wstring strPath, wstring strGroup, wstring strName);
	// 根据图片文件后缀，返回对应mime
    static string GetImageFileMime(wstring path);

	// 根据图片文件后缀判断是否为支持的图片类型
    static bool  IsSupportImageFile(wstring filepath);

    static wstring GetRegValue(/*HKEY*/struct HKEY__* hTopkey, /*TCHAR*/wchar_t* szSubKey, /*TCHAR*/wchar_t* szValueName);
    static wstring GetIniFileValue(wstring strIniFile, wstring strGroup, wstring strKey);
    static bool SetIniFileValue(wstring strIniFile, wstring strGroup, wstring strKey, wstring strValue);
    static wstring GetWindowsDir();
    // 获取系统文件夹 CSIDL_PERSONAL 我的文档
    static wstring GetSystemPath(int nFolder);
    static wstring GetMyDocumentPath();

    // Summary  : 获取exe文件信息(调用者负责释放FileVersionInfo*)
    // Returns  : FileVersionInfo* 返回文件信息数据结构
    // Parameter: wstring strPath 文件路径
    static FileVersionInfo* GetExeFileInfo(wstring strPath);

	// Summary  : ios根据彩信的组id和信息id生成其附件在sms文件夹下的路径
	// Returns	: testing::internal::wstring
	// Parameter: int messageId 信息id
	// Parameter: int groupId 组id
	static wstring ConvertMMSItemFolder(int messageId, int groupId);

    // Summary  : 创建空文件
    // Returns  : void* 文件句柄指针 
    // Parameter: wstring dest 文件生成的路径
    static void* MyCreateFile(wstring dest);

    // Summary  : 写文件
    // Returns  : void
    // Parameter: void* hFile 文件句柄指针 
    // Parameter: string buffer 文件内容
    // Parameter: unsigned long nWrite 写入长度
    static void MyWriteFile(void* hFile, string buffer, unsigned long nWrite);

    // Summary  : 关闭文件
    // Returns  : void*
    // Parameter: void* handle 文件句柄指针
    static void MyCloseFile(void* hFile);
    
private:
    static wstring QueryValue(LPVOID pInfoBuf, wstring lang_codepage, wstring name, LPVOID pInfoVal);
};
