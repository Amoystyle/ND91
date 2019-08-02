#include "stdafx.h"
#include "Utils.h"
#include <Wingdi.h>
#include "Common/DealTime.h"

INSTALLCASE_BEGIN
wstring UIStringFormat::fileSizeStr(unsigned long bytes) 
{INTERFUN;
	CString ret;
	if (bytes == 1 || bytes == 0) {
		ret = _T("1 byte");
	}

	if (bytes < 1024) {
		ret.Format(_T("%d bytes"),bytes);

	}
	if (bytes < 1048576) {
		ret.Format(_T("%.2f KB"),((double)bytes / 1024));

	}
	if (bytes < 1073741824) {
		ret.Format(_T("%.2f MB"),((double)bytes / 1048576));
	}
	if(bytes >= 1073741824)
		ret.Format(_T("%.2f GB"),((double)bytes / 1073741824));
	return ret.GetBuffer();
}
std::wstring UIStringFormat::InstallNeedTime(int count, unsigned long size, int& duration)
{INTERFUN;
	if(count==0)
		return L"-";
	wstring ret = L"约";
	int timesum = 0;//count /10;
	//timesum += size / (1048576 *150);
	//@zjf
	timesum += (size+(1048576 *50)) / (1048576 *50);
	if(timesum==0)
		timesum=1;

	duration = timesum;

	ret += CStrOperation::IntToWString(timesum);
	ret += L"分钟";
	return ret;
}
std::wstring UIStringFormat::DateStringYearMonDay( time_t t )
{INTERFUN;
	if(t==0)
		return L"--.--.--";
	CDealTime dt(t);
	wostringstream oss;
	oss<<dt.GetYear()/1000<<(dt.GetYear()%1000)/100<<((dt.GetYear()%1000)%100)/10<<dt.GetYear()%1000%100%10
		<<L"年"<<dt.GetMonth()/10<<dt.GetMonth()%10<<L"月"<<dt.GetDay()/10<<dt.GetDay()%10<<L"日";
	return oss.str();
}

std::wstring UIStringFormat::CheckDownLoadFileName(wstring filename)
{INTERFUN;
	wstring ret = filename;
	wstring str = L"\\/:*?\"<>|";
	for (size_t i=0; i < ret.size(); i++)
	{		
		if((ret[i] >19968 && ret[i] <19968+20902) || (ret[i] > 32 && ret[i] < 128))
		{
			if((int)(str.find(ret[i])) >= 0)
			{
				ret[i] = L'_';
			}
		}			
		else
		{
			ret[i] = L'_';
		}
	} 
	return ret;
}

wstring UIStringFormat::GetGmtTime()
{INTERFUN;
	struct tm *newtime; 
	__int64 ltime; 

	_time64( &ltime ); 

	/*   Obtain   coordinated   universal   time:   */ 
	newtime = _gmtime64(&ltime); 

	string dayTime = asctime( newtime );
	dayTime += " GMT";
	return  CCodeOperation::GB2312ToUnicode(dayTime);
}

BOOL UIStringFormat::IsNewVersion(CString strOldVersion, CString strNewVersion)
{INTERFUN;
	if (strOldVersion == _T("") && strNewVersion == _T(""))
	{
		return FALSE; // 相等
	}

	int oldVer = 0; // 取旧版本的第一个版本位
	int newVer = 0; // 取新版本的第一个版本位
	int posOldVer = 0;    // 位标,用于旧版本
	int posNewVer = 0;    // 位标,用于新版本
USES_CONVERSION;
	posOldVer = strOldVersion.Find(_T("."), 0);
	oldVer = posOldVer<0 ? _wtoi(strOldVersion.GetBuffer()) : _wtoi(strOldVersion.Left(posOldVer));    

	posNewVer = strNewVersion.Find(_T("."), 0);
	newVer = posNewVer<0 ? _wtoi(strNewVersion.GetBuffer()) : _wtoi(strNewVersion.Left(posNewVer));    
	if (oldVer < newVer)
	{
		return TRUE;
	}
	else if (oldVer == newVer)
	{
		CString strOldVersion1 = _T(""); // 旧版本
		CString strNewVersion1 = _T(""); // 新版本
		if (posOldVer > -1)
		{
			strOldVersion1 = strOldVersion.Right(strOldVersion.GetLength()-posOldVer-1);
		}

		if (posNewVer > -1)
		{
			strNewVersion1 = strNewVersion.Right(strNewVersion.GetLength()-posNewVer-1);
		}
		return IsNewVersion(strOldVersion1, strNewVersion1);
	}

	return FALSE;
} 

bool PublicFunc::CaseSortByData(CControlUI* pControlA, CControlUI* pControlB,wstring key1)
{INTERFUN;
	CLabelUI* pTempLabelA = (CLabelUI*)pControlA->GetItem(key1.c_str());
	CLabelUI* pTempLabelB = (CLabelUI*)pControlB->GetItem(key1.c_str());
	if (!pTempLabelA || !pTempLabelB)
	{
		return true;
	}
	UINT userdataA = pTempLabelA->GetUserData();
	UINT userdataB = pTempLabelB->GetUserData();
	return userdataA > userdataB;
}


bool PublicFunc::CaseSortRult(CControlUI* pControlA, CControlUI* pControlB, wstring key1)
{INTERFUN;
	CLabelUI* pTempLabelA = (CLabelUI*)pControlA->GetItem(key1.c_str());
	CLabelUI* pTempLabelB = (CLabelUI*)pControlB->GetItem(key1.c_str());
	RETURN_FALSE_IF(!pTempLabelA || !pTempLabelB);

	wstring tempA = pTempLabelA->GetText();
	wstring tempB = pTempLabelB->GetText();

    tempA = CStrOperation::trimLeftW(tempA, L" ");
    tempB = CStrOperation::trimLeftW(tempB, L" ");
	return lstrcmpi(tempA.c_str(), tempB.c_str()) > 0;
}

bool __stdcall PublicFunc::CaseAppCompare(CListItemUI* pControlA, CListItemUI* pControlB, int nCol, bool exchange)
{INTERFUN;
	RETURN_FALSE_IF(!pControlA || !pControlB);

	switch (nCol)
	{
	case 0://名称
		return PublicFunc::CaseSortRult(pControlA, pControlB,L"LabContainer.LabelAppName");
		break;
	case 1:// 
		return PublicFunc::CaseSortRult(pControlA, pControlB, L"LabelAppVer");
		break;
	case 2://
		return PublicFunc::CaseSortByData(pControlA, pControlB, L"LabelAppSize");
		break;
	case 3:// 
		//return CaseSortRult(pControlA, pControlB, L"LabelAppType", L"", L"");
		break;
	case 4://
		return PublicFunc::CaseSortByData(pControlA, pControlB, L"LabelDownloadTime");
		break;
	default:
		break;
	}
	return false;
}

void PublicFunc::SetWindowsRgnEx(HWND winhwnd, int size)
{INTERFUN;
	RECT l_rtWnd;
	::GetWindowRect(winhwnd,&l_rtWnd);

	l_rtWnd.right=l_rtWnd.right-l_rtWnd.left;
	l_rtWnd.bottom=l_rtWnd.bottom-l_rtWnd.top;
	l_rtWnd.left=0;
	l_rtWnd.top=0;
	POINT l_ptLeftTop1, l_ptLeftTop2, l_ptRightTop1, l_ptRightTop2, l_ptLeftBottom1, l_ptLeftBottom2, l_ptRightBottom1, l_ptRightBottom2;
	l_ptLeftTop1.x = l_rtWnd.left,				l_ptLeftTop1.y = l_rtWnd.top + size-1;
	l_ptLeftTop2.x = l_rtWnd.left + size-1,			l_ptLeftTop2.y = l_rtWnd.top;
	l_ptRightTop1.x = l_rtWnd.right- l_rtWnd.left- size-1,		l_ptRightTop1.y = l_rtWnd.top;
	l_ptRightTop2.x = l_rtWnd.right- l_rtWnd.left,			l_ptRightTop2.y = l_rtWnd.top + size;
	l_ptRightBottom1.x = l_rtWnd.right- l_rtWnd.left,		l_ptRightBottom1.y = l_rtWnd.bottom-l_rtWnd.top - (size+1);
	l_ptRightBottom2.x = l_rtWnd.right- l_rtWnd.left - (size+1),	l_ptRightBottom2.y = l_rtWnd.bottom-l_rtWnd.top;
	l_ptLeftBottom1.x = l_rtWnd.left + size+1,		l_ptLeftBottom1.y = l_rtWnd.bottom-l_rtWnd.top;
	l_ptLeftBottom2.x = l_rtWnd.left,			l_ptLeftBottom2.y = l_rtWnd.bottom-l_rtWnd.top - (size+1);

	POINT l_ptArray[8] = 
	{
		l_ptLeftTop1,
		l_ptLeftTop2,
		l_ptRightTop1,
		l_ptRightTop2,
		l_ptRightBottom1,
		l_ptRightBottom2,
		l_ptLeftBottom1,
		l_ptLeftBottom2 
	};
	HRGN l_rgnWnd = CreatePolygonRgn(l_ptArray, 8, ALTERNATE);

	// 2、设置rgn
	::SetWindowRgn(winhwnd,(HRGN)l_rgnWnd, TRUE);
}
bool PublicFunc::FindVecInt(int nid,vector<int>* SelectList)
{INTERFUN;
	for(size_t i=0;i<SelectList->size();i++)
	{
		if(nid == SelectList->at(i))
		{
			SelectList->erase(SelectList->begin()+i);
			return true;
		}
	}
	return false;
}


std::wstring PublicFunc::GetDocmentDataPath()
{INTERFUN;
	TCHAR AppdataPath[MAX_PATH]; 
	::SHGetSpecialFolderPath(NULL, AppdataPath, CSIDL_MYDOCUMENTS, FALSE);
	wstring filePath = AppdataPath;
	return filePath;
}

INSTALLCASE_END