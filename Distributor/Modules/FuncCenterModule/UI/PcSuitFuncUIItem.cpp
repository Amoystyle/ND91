#include "StdAfx.h"
#include "PcSuitFuncUIItem.h"
#include "logic/UnzipAndExecuteThread.h"

PcSuitFuncUIItem::PcSuitFuncUIItem(CControlUI* ControlUI, FunctionItem& FuncItem, EnumPhoneOS CurPlatform)
{
	_pLayoutUI = dynamic_cast<CLayoutUI*>(ControlUI);
assert(_pLayoutUI);
	_FuncItem = FuncItem;

	_CurPlatform = CurPlatform;

	_unTaskID = 0;

	_pProgressUI = NULL;

	_pProgressUIBG = NULL;

	_pBtnUI = NULL;

	this->Init();
}

PcSuitFuncUIItem::~PcSuitFuncUIItem(void)
{
}

void PcSuitFuncUIItem::Init()
{
	GetDownloadFileReportTaskIDMsg ReportTaskIDMsg;
	ReportTaskIDMsg.SendAndGetReturn();
	_unTaskID = ReportTaskIDMsg.unTaskID;

	_pBtnUI = new CButtonUI;
	_pBtnUI->SetId(L"PCSuitFuncItem");
	_pBtnUI->SetStyle(L"FuncCenter_Style_FuncButton");
	_pBtnUI->SetInitPos(0,-1,80,92,9);
	_pBtnUI->SetText(_FuncItem.st_Name.c_str());
	_pBtnUI->SetAttribute(L"cursor", L"hand");
	_pBtnUI->SetAttribute(L"icon.image", _FuncItem.st_IconSavePath.c_str());
	_pBtnUI->SetINotifyUI(this);
	_pBtnUI->SetToolTipShow(true);

	wstring wsTips = CommonUIFunc::LoadString(L"FuncCenter_Text_PCSuit_Item_Name") + _FuncItem.st_Name + L"\n";
	wsTips = wsTips + CommonUIFunc::LoadString(L"FuncCenter_Text_PCSuit_Item_Size") + _FuncItem.st_FileSize + L"\n";
	wsTips = wsTips + CommonUIFunc::LoadString(L"FuncCenter_Text_PCSuit_Item_Version") + _FuncItem.st_FileVersion + L"\n";
	wsTips = wsTips + CommonUIFunc::LoadString(L"FuncCenter_Text_PCSuit_Item_Desc") + _FuncItem.st_Desc;

	_pBtnUI->SetToolTip(wsTips.c_str());

	_pLayoutUI->Add(_pBtnUI);

	_pProgressUI = dynamic_cast<ImageStyle*>(_pBtnUI->GetStyle(L"Progress_Download"));
	_pProgressUIBG = dynamic_cast<ImageStyle*>(_pBtnUI->GetStyle(L"Progressbk"));
}

bool PcSuitFuncUIItem::OnPCSuitFuncItemClick(TNotifyUI* pNotify)
{
	if(NULL == pNotify)
		return false;

	CButtonUI* pButton = dynamic_cast<CButtonUI*>(pNotify->pSender);
	if (NULL == pButton)
	{
		return false;
	}
	//创建目录

	wstring PcSuitFuncAndroidDir = CFileOperation::GetCurrWorkingDir()+L"PcSuitFunc\\"+ L"Android\\";
	wstring PcSuitFuncIOSDir = CFileOperation::GetCurrWorkingDir()+L"PcSuitFunc\\"+ L"IOS\\";
	if (!CFileOperation::IsDirExist(PcSuitFuncAndroidDir.c_str()))
	{
		CFileOperation::MakeLocalDir(PcSuitFuncAndroidDir);
	}
	if (!CFileOperation::IsDirExist(PcSuitFuncIOSDir.c_str()))
	{
		CFileOperation::MakeLocalDir(PcSuitFuncIOSDir);
	}
	//判断是否软件已下载，否则进行下载，运行操作
	if (_CurPlatform == ePhoneOS_Ios)
	{
		_wsSavePath = PcSuitFuncIOSDir + CFileOperation::GetFileName(_FuncItem.st_Name) + _FuncItem.st_FileVersion + L"." + CFileOperation::GetFileExt(_FuncItem.st_InstallPkDownPath);
	}
	else
		_wsSavePath = PcSuitFuncAndroidDir + CFileOperation::GetFileName(_FuncItem.st_Name) + _FuncItem.st_FileVersion + L"." + CFileOperation::GetFileExt(_FuncItem.st_InstallPkDownPath);

	//存在则运行改软件
	if (CFileOperation::IsFileExist(_wsSavePath.c_str()))
	{
		string strPlat = "android";
		if (ePhoneOS_Ios == _CurPlatform)
		{
			strPlat = "ios";
		}
		else if (ePhoneOS_Android == _CurPlatform)
		{
			strPlat = "android";
		}
		UnzipAndExecuteThread* pUnzipAndExecuteThread = new UnzipAndExecuteThread;
		pUnzipAndExecuteThread->SetDownPath(_wsSavePath, _FuncItem.st_wsExeBinPath, strPlat, CCodeOperation::WstringToString(_FuncItem.st_Name));
		pUnzipAndExecuteThread->Run();
	}
	else
	{
		//下载，执行工作
		DownloadFileReportMsg ReportMsg;
		ReportMsg.wsDownPath = _FuncItem.st_InstallPkDownPath;
		ReportMsg.wsSavePath = _wsSavePath;
		ReportMsg.unTaskID   = _unTaskID;
		ReportMsg.Send();
	}
	return true;
}

bool PcSuitFuncUIItem::Handle(const ReponseDownloadReportMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}
	if (_unTaskID != pMsg->st_DoenloadInfo.unTaskid)
	{
		return true;
	}
	//下载失败
	if (!pMsg->bDownloadSuccess)
	{
		string strPlat = "android";
		if (ePhoneOS_Ios == _CurPlatform)
		{
			strPlat = "ios";
		}
		else if (ePhoneOS_Android == _CurPlatform)
		{
			strPlat = "android";
		}
		string wsTab = strPlat + "_" + CCodeOperation::WstringToString(_FuncItem.st_Name) + "_" + "下载失败";
//下载失败统计
		CStaticsticsMessageWithTab StaticsticsMsg("900011", CCodeOperation::GB2312ToUTF_8(wsTab));
		StaticsticsMsg.SendAndGetReturn();

		if (_pProgressUI && _pProgressUI->IsVisible())
		{
			_pProgressUI->SetVisible(false);
			_pProgressUIBG->SetVisible(false);
			_pBtnUI->Invalidate(true);
			_pBtnUI->Resize();
		}
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("FuncCenter_Text_download_fail")), ICON_WARNING);
		return true;
	}

	//下载完成
	if (pMsg->st_DoenloadInfo.unAllSize <= pMsg->st_DoenloadInfo.unCurDownSize )
	{
		string strPlat = "android";
		if (ePhoneOS_Ios == _CurPlatform)
		{
			strPlat = "ios";
		}
		else if (ePhoneOS_Android == _CurPlatform)
		{
			strPlat = "android";
		}
		string wsTab = strPlat + "_" + CCodeOperation::WstringToString(_FuncItem.st_Name) + "_" + "下载成功";
//下载完成统计
		CStaticsticsMessageWithTab StaticsticsMsg("900011", CCodeOperation::GB2312ToUTF_8(wsTab));
		StaticsticsMsg.SendAndGetReturn();

		if (_pProgressUI && _pProgressUI->IsVisible())
		{
			_pProgressUI->SetVisible(false);
			_pProgressUIBG->SetVisible(false);
			_pBtnUI->Invalidate(true);
			_pBtnUI->Resize();
		}

		UnzipAndExecuteThread* pUnzipAndExecuteThread = new UnzipAndExecuteThread;
		pUnzipAndExecuteThread->SetDownPath(_wsSavePath, _FuncItem.st_wsExeBinPath, strPlat,CCodeOperation::WstringToString( _FuncItem.st_Name));
		pUnzipAndExecuteThread->Run();

		return true;
	}

	if (_pProgressUI)
	{
		//pos="6|0,12,0,0"
		int nBarLength = 80;
		if (0 != pMsg->st_DoenloadInfo.unAllSize)
		{
			int nCurLength = ((float)pMsg->st_DoenloadInfo.unCurDownSize/pMsg->st_DoenloadInfo.unAllSize) * nBarLength;
			nCurLength = nCurLength<=5 ? 5 : nCurLength;
			wstring wsPos = _T("6|0,5,");
			wsPos = wsPos + CStrOperation::Int64ToWString(nBarLength-nCurLength) + _T(",0");
			_pProgressUI->SetAttribute(_T("pos"), wsPos.c_str());
			_pBtnUI->Invalidate(true);
			_pBtnUI->Resize();
		}

		if (!_pProgressUI->IsVisible())
		{
			_pProgressUI->SetVisible(true);
			_pProgressUIBG->SetVisible(true);
		}
	}

	return true;
}