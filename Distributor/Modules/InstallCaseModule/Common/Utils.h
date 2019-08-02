#pragma once

#include <cstring>
INSTALLCASE_BEGIN
class PublicFunc
{
public:
	static bool CaseSortByData(CControlUI* pControlA, CControlUI* pControlB,wstring key1);
	static bool CaseSortRult(CControlUI* pControlA, CControlUI* pControlB, wstring key1);
	static bool __stdcall CaseAppCompare(CListItemUI* pControlA, CListItemUI* pControlB, int nCol, bool exchange);
	static void SetWindowsRgnEx(HWND winhwnd, int size=5);
	static bool FindVecInt(int nid,vector<int>* SelectList);
	static std::wstring GetDocmentDataPath();
};
class UIStringFormat
{
public:
	
	static wstring fileSizeStr(unsigned long bytes);
	static wstring GetGmtTime();	// Sat,01-Jan-2000 00:00:00 GMT 
	static wstring DateStringYearMonDay(time_t t);
	static BOOL IsNewVersion(CString strOldVersion, CString strNewVersion);
	static std::wstring InstallNeedTime(int count, unsigned long size, int& duration);
	static std::wstring CheckDownLoadFileName(wstring filename);
	
};

INSTALLCASE_END