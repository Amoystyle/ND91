#include "stdafx.h"
#include "FeedbackController.h"
#include "UI/FeedbackUI.h"

FeedbackController::FeedbackController()
{
	
}

void FeedbackController::Init(CContainerUI* pTitleToolBar, CContainerUI* pStatusBar)
{
	/*CButtonUI *pButton = new CButtonUI;
	pButton->SetStyle(L"DependenceFuncCenter_FeedBack");
	pButton->SetINotifyUI(this);
	pButton->SetInitPos(0,0,32,16,0);
	pButton->SetText(CommonUIFunc::LoadString(L"DependenceFuncCenter_thefeedBack"));
	pButton->SetId(L"BtnFeedback");
	pButton->SetAttribute(L"cursor",L"hand");
	pTitleToolBar->Add(pButton);*/

	pTitleToolBar->Resize();
	pTitleToolBar->Invalidate();
}

FeedbackController::~FeedbackController()
{
// 	CButtonUI b;
// 	b.SetINotifyUI()

}


std::string FeedbackController::GetUrl()
{
	string osName = GetOSName();
	string url = "http://zj.91.com/Problem/AddEditProblem.aspx?os=" + osName + "&dotnet=";
	return url;
}

typedef void (WINAPI *PGetNativeSystemInfo)(LPSYSTEM_INFO);
string FeedbackController::GetOSName()
{
	PGetNativeSystemInfo pGNSI = NULL;
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	BOOL bOsVersionInfoEx;
	string strSystemName;

	ZeroMemory( &si, sizeof(SYSTEM_INFO) );
	ZeroMemory( &osvi, sizeof(OSVERSIONINFOEX) );
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *)&osvi)) )
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if( !GetVersionEx ( (OSVERSIONINFO *)&osvi ) ) 
			return "";
	}

	pGNSI = (PGetNativeSystemInfo)GetProcAddress( GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo" );
	if(NULL != pGNSI)
		pGNSI(&si);
	else
		GetSystemInfo(&si);

	switch( osvi.dwPlatformId )
	{  
	case VER_PLATFORM_WIN32_NT: // Windows NT family.
		{
			if( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 && osvi.wProductType == VER_NT_WORKSTATION )
				strSystemName = "Microsoft Windows Vista";
			else if( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 && osvi.wProductType != VER_NT_WORKSTATION )
				strSystemName = "Microsoft Windows Server 2008";
			else if( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1 && osvi.wProductType == VER_NT_WORKSTATION )
				strSystemName = "Microsoft Windows7";
			else if( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1 && osvi.wProductType != VER_NT_WORKSTATION )
				strSystemName = "Microsoft Windows Server 2008 R2";
			else if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
			{
				if( osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 )
					strSystemName = "Microsoft Windows XP Professional x64 Edition";
				else
					strSystemName = GetSystemMetrics(SM_SERVERR2) != 0 ? "Microsoft Windows Server 2003 R2" : "Microsoft Windows Server 2003";
			}
			else if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
				strSystemName = "Microsoft Windows XP";
			else if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
				strSystemName = "Microsoft Windows 2000";
			else if( osvi.dwMajorVersion <= 4 )
				strSystemName = "Microsoft Windows NT";
		}
		break;

	case VER_PLATFORM_WIN32_WINDOWS: // Windows 95 family.
		{
			if( osvi.dwMajorVersion == 4 &&  osvi.dwMinorVersion == 0 )
				strSystemName = "Microsoft Windows 95";
			else if( osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10 )
				strSystemName = osvi.szCSDVersion[1] == 'A' ? "Windows 98_SE" : "Windows 98";
			else if( osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90 )
				strSystemName = "Microsoft Windows Millennium Edition";
		}
		break;

	case VER_PLATFORM_WIN32s:
		{
			strSystemName = "Microsoft Win32s";
		}
		break;

	default:
		{
			strSystemName = "Unknown";
		}
	}

	return strSystemName;
}

bool FeedbackController::Handle( const RequestFeedbackMsg* pMsg )
{
	FeedbackDlg pFeedbackDlg;
	pFeedbackDlg.DoModal();

	return true;
}


bool FeedbackController::OnFeedbackBtn( TNotifyUI* pNotify )
{
	FeedbackDlg pFeedbackDlg;
	pFeedbackDlg.DoModal();	
	return true;
}
