#pragma once
#include <windows.h>
#include <CommDlg.h>
#include <shlobj.h>
#include <string>
#include "Monitor/ProgressObserver.h"
#include "Core/DeviceData.h"
using namespace ND91Assistant;

class FileManagerCommonFunc
{
public:
	FileManagerCommonFunc(void);
	~FileManagerCommonFunc(void);
	static std::wstring GetDateTimeString();
	static wstring DirDialog(HWND pParent, wstring strDefaultPath=L"");
	static vector<wstring> MultiFileDialog(HWND pParent = NULL, LPCTSTR strFileType=L"*.*\0*.*\0", DWORD flags=OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_EXPLORER|OFN_ALLOWMULTISELECT, wstring wsTitle=L"");
	static wstring GetPrompt(MESSAGE_CODE msgCode);
	static DeviceData* FindDevice( wstring id );
};
