#include "stdafx.h"
#include "WelcomeView.h"
#include <ShellAPI.h>
WelcomeView::WelcomeView()
{
}

WelcomeView::~WelcomeView()
{
}

std::wstring WelcomeView::GetStyleID()
{
	return L"Device_View_Welcome";
}

void WelcomeView::OnCreate()
{
	CLayoutUI* pRightBottom = dynamic_cast<CLayoutUI*>(GetItem(L"RightBottom"));
	if(pRightBottom)
	{
		DeviceModule* pModule = CreateRecentInstallCaseModule(L"", NULL);
		pModule->Init(pRightBottom);
	}
	this->ModifyControlFlags(UIFLAG_ENABLE_DRAG|UIFLAG_ENABLE_DROP, 0);

}

bool WelcomeView::OnGotoInstallCaseClick( TNotifyUI* pNotify )
{
	NotifySourceModule msg;
	msg.type = SourceModuleType_InstallCase;
	msg.SendToUI();

	return true;
}

bool WelcomeView::OnGotoWebResourceClick( TNotifyUI* pNotify )
{
	NotifySourceModule msg;
	msg.type = SourceModuleType_WebResource;
	msg.SendToUI();

	return true;
}


bool WelcomeView::OnDropFiles( TNotifyUI* pNotify )
{
	vector<wstring>* files_case = new vector<wstring>();
	HDROP hDropInfo = (HDROP)pNotify->wParam;
	UINT count;
	wchar_t filePath[512];
	count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if(count)
	{        
		for(UINT i=0; i<count; i++)
		{
			int pathLen = DragQueryFile(hDropInfo, i, filePath, sizeof(filePath));
			//装机方案拖拽逻辑移至此处
			if(CFileOperation::IsDirExist(filePath))
			{
				vector<wstring> subFiles;
				CFileOperation::FindAllFileInDir(filePath, subFiles, _T("ipa"));
				files_case->insert(files_case->end(),subFiles.begin(), subFiles.end());
				subFiles.clear();
				CFileOperation::FindAllFileInDir(filePath, subFiles, _T("pxl"));
				files_case->insert(files_case->end(),subFiles.begin(), subFiles.end());
				subFiles.clear();
				CFileOperation::FindAllFileInDir(filePath, subFiles, _T("apk"));
				files_case->insert(files_case->end(),subFiles.begin(), subFiles.end());
				subFiles.clear();
				CFileOperation::FindAllFileInDir(filePath, subFiles, _T("npk"));
				files_case->insert(files_case->end(),subFiles.begin(), subFiles.end());

			}
			else
			{
				wstring ext = CFileOperation::GetFileExt(filePath);
				ext = CStrOperation::toLowerW(ext);
				if(ext == L"ipa" || ext == L"pxl" || ext == L"apk" || ext == L"npk")
					files_case->push_back(wstring(filePath));
			}
		}
	}

	DragFinish(hDropInfo);

	if (files_case->size()>1)
	{

		AddCaseMsg addCaseMsg;
		addCaseMsg.pPathList = (void*)files_case;
		addCaseMsg.SendToUI();
	}

	return true;
}