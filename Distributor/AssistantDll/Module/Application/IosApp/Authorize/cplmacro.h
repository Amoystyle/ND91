//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES.
//
/**
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Abstract: Macros and helper classes & functions of general applicability
**/


#define ARRAYSIZEOF(x)	(sizeof(x) / sizeof((x)[0]))
#define CCHSIZEOF(x)	(sizeof(x) / sizeof(TCHAR))

#define ZEROMEM(p)		memset(p, 0, sizeof(*(p)))

#define LOADSZ(ids, psz)	(LoadString(g_hInst, ids, psz, CCHSIZEOF(psz)))
#define CELOADSZ(ids)		((LPCTSTR)LoadString(g_hInst, ids, NULL, 0))

#define MyAlloc(typ)		((typ*)LocalAlloc(LPTR, sizeof(typ)))
#define MyRgAlloc(typ, n)	((typ*)LocalAlloc(LPTR, (n)*sizeof(typ)))
#define MyRgReAlloc(typ, p, n)	((typ*)LocalReAlloc(p, (n)*sizeof(typ), LMEM_MOVEABLE | LMEM_ZEROINIT))
#define MySzAlloc(n)		((LPTSTR)LocalAlloc(LPTR, (1+(n))*sizeof(TCHAR)))
#define MySzAlloca(n)		((LPTSTR)_alloca((1+(n))*sizeof(TCHAR)))
#define MyFree(p)			{ if(p) LocalFree(p); }

#define DI(x)		GetDlgItem(hDlg, x)

#define MyFreeLib(h)		{ if(h) FreeLibrary(h); }
#define MyCloseHandle(h)	{ if(h) CloseHandle(h); }
#define MyDeleteObject(h)	{ if(h) DeleteObject(h); }

#define abs(x)		( (x) < 0 ? -(x) : (x) )

#ifdef DEBUG
	#define DECLAREWAITCURSOR  HCURSOR hcursor_wait_cursor_save = (HCURSOR)0xDEADBEEF
	#define SetWaitCursor()   { ASSERT(hcursor_wait_cursor_save == (HCURSOR)0xDEADBEEF); hcursor_wait_cursor_save = SetCursor(LoadCursor(NULL, IDC_WAIT)); }
	#define ResetWaitCursor() { ASSERT(hcursor_wait_cursor_save != (HCURSOR)0xDEADBEEF); SetCursor(hcursor_wait_cursor_save); hcursor_wait_cursor_save = (HCURSOR)0xDEADBEEF; }
#else
	#define DECLAREWAITCURSOR  HCURSOR hcursor_wait_cursor_save = NULL
	#define SetWaitCursor()   { hcursor_wait_cursor_save = SetCursor(LoadCursor(NULL, IDC_WAIT)); }
	#define ResetWaitCursor() { SetCursor(hcursor_wait_cursor_save);  hcursor_wait_cursor_save = NULL; }
#endif

#define EMPTY_STRING			TEXT("")

/////////////////////////////////////////////////////////////////////////////
// CReg: Registry helper class
/////////////////////////////////////////////////////////////////////////////
class CReg
{
private:
	HKEY	m_hKey;
	int		m_Index;
	LPBYTE	m_lpbValue; // last value read, if any

public:
	BOOL Create(HKEY hkRoot, LPCTSTR pszKey) {
		DWORD dwDisp;
		return ERROR_SUCCESS==RegCreateKeyEx(hkRoot, pszKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisp);
	}

	BOOL Open(HKEY hkRoot, LPCTSTR pszKey, REGSAM sam=KEY_READ) {
		return ERROR_SUCCESS==RegOpenKeyEx(hkRoot, pszKey, 0, sam, &m_hKey);
	}

	CReg(HKEY hkRoot, LPCTSTR pszKey) {
		m_hKey = NULL;
		m_Index = 0;
		m_lpbValue = NULL;
		Open(hkRoot, pszKey);
	}

	CReg() {
		m_hKey = NULL;
		m_Index = 0;
		m_lpbValue = NULL;
	}

	~CReg() { 
		if(m_hKey) RegCloseKey(m_hKey); 
		MyFree(m_lpbValue);
	}

	void Reset() { 
		if(m_hKey) RegCloseKey(m_hKey); 
		MyFree(m_lpbValue);
		m_hKey = NULL;
		m_Index = 0;
		m_lpbValue = NULL;
	}

	operator HKEY() { return m_hKey; }

	BOOL IsOK(void) { return m_hKey!=NULL; }


	BOOL EnumKey(LPTSTR psz, DWORD dwLen) {
		if(!m_hKey) return FALSE;
		return ERROR_SUCCESS==RegEnumKeyEx(m_hKey, m_Index++, psz, &dwLen, NULL, NULL, NULL, NULL);
	}

	BOOL EnumValue(LPTSTR pszName, DWORD dwLenName, LPTSTR pszValue, DWORD dwLenValue) {
		DWORD dwType;
		if(!m_hKey) return FALSE;
		dwLenValue *= sizeof(TCHAR); // convert length in chars to bytes
		return ERROR_SUCCESS==RegEnumValue(m_hKey, m_Index++, pszName, &dwLenName, NULL, &dwType, (LPBYTE)pszValue, &dwLenValue);
	}

	BOOL ValueSZ(LPCTSTR szName, LPTSTR szValue, DWORD dwLen) {
		if(!m_hKey) return FALSE;
		dwLen *= sizeof(TCHAR); // convert length in chars to bytes
		return ERROR_SUCCESS==RegQueryValueEx(m_hKey, szName, NULL, NULL, (LPBYTE)szValue, &dwLen);
	}

	DWORD ValueBinary(LPCTSTR szName, LPBYTE lpbValue, DWORD dwLen) {
		if(!m_hKey) return FALSE;
		DWORD dwLenWant = dwLen;
		if(ERROR_SUCCESS==RegQueryValueEx(m_hKey, szName, NULL, NULL, lpbValue, &dwLen))
			return dwLen;
		else
			return 0;
	}

	LPCTSTR ValueSZ(LPCTSTR szName)
	{
		DWORD dwLen = 8000;
		if(ERROR_SUCCESS==RegQueryValueEx(m_hKey, szName, NULL, NULL, m_lpbValue, &dwLen))
			return NULL;
		else
			return (LPCTSTR)m_lpbValue;
	}

	LPBYTE ValueBinary(LPCTSTR szName) {
		return (LPBYTE)ValueSZ(szName);
	}

	DWORD ValueDW(LPCTSTR szName, DWORD dwDefault=0) {
		if(!m_hKey) return FALSE;
		DWORD dwValue = dwDefault;
		DWORD dwLen = sizeof(DWORD);
		RegQueryValueEx(m_hKey, szName, NULL, NULL, (LPBYTE)&dwValue, &dwLen);
		return dwValue;
	}

	BOOL SetSZ(LPCTSTR szName, LPCTSTR szValue, DWORD dwLen) {
		//Prefix
		if(!m_hKey) return FALSE;
		//
		return ERROR_SUCCESS==RegSetValueEx(m_hKey, szName, 0, REG_SZ, (LPBYTE)szValue, sizeof(TCHAR)*dwLen);
	}
	
	BOOL SetSZ(LPCTSTR szName, LPCTSTR szValue) {
		return SetSZ(szName, szValue, 1+lstrlen(szValue));
	}

	BOOL SetDW(LPCTSTR szName, DWORD dwValue) {
		//Prefix
		if(!m_hKey) return FALSE;
		//
		return ERROR_SUCCESS==RegSetValueEx(m_hKey, szName, 0, REG_DWORD, (LPBYTE)&dwValue, sizeof(DWORD));
	}
	
	BOOL SetBinary(LPCTSTR szName, LPBYTE lpbValue, DWORD dwLen) {
		//Prefix
		if(!m_hKey) return FALSE;
		//
		return ERROR_SUCCESS==RegSetValueEx(m_hKey, szName, 0, REG_BINARY, lpbValue, dwLen);
	}

	BOOL SetMultiSZ(LPCTSTR szName, LPCTSTR lpszValue, DWORD dwLen) {
		return ERROR_SUCCESS==RegSetValueEx(m_hKey, szName, 0, REG_MULTI_SZ, (LPBYTE)lpszValue, sizeof(TCHAR)*dwLen);
	}

	BOOL DeleteValue(LPCTSTR szName) {
		//Prefix
		if(!m_hKey) return FALSE;
		//
		return ERROR_SUCCESS==RegDeleteValue(m_hKey, szName);
	}

	BOOL DeleteKey(LPCTSTR szName) {
		if(!m_hKey) return FALSE;
		return ERROR_SUCCESS==RegDeleteKey(m_hKey, szName);
	}

};

