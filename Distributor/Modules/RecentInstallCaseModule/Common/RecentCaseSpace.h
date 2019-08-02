#pragma once

#define RENCENT_INSTALLCASE_BEGIN                     namespace RencentInstallCaseModuleSpace {
#define RENCENT_INSTALLCASE_END                       }
#define USING_RENCENT_NSTALLCASE_Space                using namespace RencentInstallCaseModuleSpace;

RENCENT_INSTALLCASE_BEGIN

#define WM_CASE_ADD						(WM_USER + 400) //400~499预留给方案管理界面update
#define WM_CASE_APP_ADD					(WM_CASE_ADD + 1)
#define WM_CASE_APP_REMOVE				(WM_CASE_ADD + 2)
#define WM_CASE_REMOVE					(WM_CASE_ADD + 3)
#define WM_CASE_FOLDER_ADD				(WM_CASE_ADD + 4)
#define WM_CASE_SET_NUMBER				(WM_CASE_ADD + 5)
#define WM_CASE_APP_LOADED				(WM_CASE_ADD + 6)
#define WM_CASE_APP_EMPTY				(WM_CASE_ADD + 7)
#define WM_CASE_IOSAPPCHECKED			(WM_CASE_ADD + 9)
#define WM_CASE_ANDAPPCHECKED			(WM_CASE_ADD + 10)
#define WM_CASE_FOLDER_REMOVE			(WM_CASE_ADD + 11)
#define WM_CASE_CLEARAPPIST				(WM_CASE_ADD + 12)
#define WM_CASE_APP_UPGRATECANCEL		(WM_CASE_ADD + 13)
#define WM_CASE_TIMECHANGE				(WM_CASE_ADD + 14)
#define WM_CASE_FINISHREFRESH			(WM_CASE_ADD + 15)
#define WM_CASE_IOSLEGALAPPCHECKED		(WM_CASE_ADD + 16)
#define WM_CASE_STARTUPREFRESH			(WM_CASE_ADD + 17)
#define WM_CASE_APP_ADD_RESULT          (WM_CASE_ADD + 18)
#define WM_CASE_APP_ADD_FILTER          (WM_CASE_ADD + 19)
RENCENT_INSTALLCASE_END