#pragma once
#include <windows.h>
#include <CommDlg.h>
#include <shlobj.h>
#include <string>

using namespace std;
class COMMONUI_API BaseOperation
{
public:
    // 选择一个文件
    static wstring FileDialog(HWND pParent = NULL, LPCTSTR strFileType=L"*.*\0*.*\0", DWORD flags=OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_EXPLORER);
    static vector<wstring> MultiFileDialog(HWND pParent = NULL, LPCTSTR strFileType=L"*.*\0*.*\0", DWORD flags=OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_EXPLORER|OFN_ALLOWMULTISELECT, wstring wsTitle=L"");
    static wstring DirDialog(HWND pParent = NULL, wstring strDefaultPath=L"");
    static HANDLE Execute(wstring strPath, wstring strCmdLine=L"");
    static HANDLE Open(wstring strPath, wstring strCmdLine=L"");    
    static HANDLE ExecuteAsUser(wstring strExe, wstring strWorkPath, wstring strCmdLine, WORD wShowWindow=0);
    static HANDLE ExecuteDir(wstring strDir);
    static HANDLE ExecuteUrl(wstring url);

    static wstring ParseCountry(wstring str);

    static int GetPer(size_t nPer, size_t nRange);
    static int GetPer(int64_t nPer, int64_t nRange);

    static bool CopyToClipboard(HWND hWnd, wstring str);

    static wchar_t* AllocateBuffer(wstring sn);
   
    static wstring GetDateTimeString();
    static time_t ParstTimeFormat(tstring sTime, tstring sFormat=L"%d-%d-%d %d:%d:%d");

	//在资源管理器中打开并选中目标
	static void FindTarget(LPCTSTR    str);
    //写信息
    static void InsertFunctionInfo(LPCWSTR filename, int nLine, LPCWSTR functionname);
    static bool appFunctionInfo;

	static string GetMD5(wstring filePath);
private:
	static HRESULT GetShellFolderViewDual(ITEMIDLIST* pidl, IShellFolderViewDual** ppIShellFolderViewDual);

	static BOOL XZSHOpenFolderAndSelectItems(ITEMIDLIST* pidlFolder);

	static size_t readBinFile(wstring strFilePath, char** ppData);
};