#include "stdafx.h"
#include "WhatsNewDlg.h"

CWhatsNewDlg* CWhatsNewDlg::ms_Instance = NULL;

CWhatsNewDlg::CWhatsNewDlg(void)
{
	m_dwExStyle &= ~WS_EX_APPWINDOW;
	m_dwExStyle |= WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
	m_dwStyle &= ~WS_CAPTION;

	m_bEnableChangeSkin = false;
}

CWhatsNewDlg::~CWhatsNewDlg(void)
{
	ms_Instance = NULL;
}

CWhatsNewDlg* CWhatsNewDlg::GetInstance()
{
	if (ms_Instance==NULL)
	{
		ms_Instance = new CWhatsNewDlg;
		//ms_Instance->Create(HWND_DESKTOP,L"whatsnew");
		ms_Instance->Create(Singleton::GetMainWnd(),L"whatsnew");
		//ms_Instance->SetLayerWindow(true);
		ms_Instance->SetCloseAnimation(new CCloseAnimation);
		ms_Instance->SetTitle(L"");
	}

	return ms_Instance;
}

void CWhatsNewDlg::OnCreate()
{
    CToggleLayoutUI* pToggle = dynamic_cast<CToggleLayoutUI*>(GetItem(L"toggleLayout"));
    if (pToggle)
        pToggle->SetSelectItemCallBack(CWhatsNewDlg::OnSelectItem, this);
	IStaticUI* pStatic = dynamic_cast<IStaticUI*>(GetItem(L"toggleLayout"));
	if (pStatic)
	{
		CControlUI* pCtrl = GetItem(L"btnClose");
		pStatic->AddStaticItem(pCtrl);
		pCtrl = GetItem(L"btnPrev");
		pStatic->AddStaticItem(pCtrl);
		pCtrl = GetItem(L"btnNext");
		pStatic->AddStaticItem(pCtrl);

		pCtrl = GetItem(L"lblNewFun");
		pStatic->AddStaticItem(pCtrl);


//         pCtrl = GetItem(L"toggleBK");
//         pStatic->AddStaticItem(pCtrl);

        SetPrevOrNextVisible();		
	}

    //设置Radio按钮
    
    CToggleLayoutUI* pToggleLayout = dynamic_cast<CToggleLayoutUI*>(GetItem(L"toggleLayout"));    
    CLayoutUI* pRadioLayout = pToggleLayout->GetRadioLayout();
    if (pToggleLayout)
    {
        vector<CRadioUI*> radioList;
        for (int i=0; i<20; i++)
        {
            CRadioUI* pRadio = dynamic_cast<CRadioUI*>(pRadioLayout->GetItem(i));
            if (pRadio)
            {
                //radioList.push_back(pRadio);
                pRadio->SetAttribute(_T("bk.image"), _T("#DependenceFuncCenter_WhatsNew_Radio"));                                
                pRadio->SetAttribute(_T("bk.pos"), _T("9|-1,-1,16,16"));
                pRadio->SetAttribute(_T("bk.imgseq"), _T("0,1,2,-1,-1"));

                pRadio->SetAttribute(_T("ckbk.image"), _T("#DependenceFuncCenter_WhatsNew_Radio"));                
                pRadio->SetAttribute(_T("ckbk.pos"), _T("9|-1,-1,16,16"));
                pRadio->SetAttribute(_T("ckbk.imgseq"), _T("2"));

                pRadio->Resize();
                pRadio->Invalidate();
            }
        }
        
    }
}

void CWhatsNewDlg::SetPrevOrNextVisible(int nCurSel)
{
	CToggleLayoutUI* pToggle = dynamic_cast<CToggleLayoutUI*>(GetItem(L"toggleLayout"));
	CButtonUI* prevbutton = dynamic_cast<CButtonUI*>(GetItem(_T("btnPrev")));
	CButtonUI* nextbutton = dynamic_cast<CButtonUI*>(GetItem(_T("btnNext")));
	if (pToggle && prevbutton && nextbutton)
	{
		prevbutton->SetVisible(true);
		nextbutton->SetVisible(true);		//先显示两个按钮
        if (nCurSel == -1)
            nCurSel = pToggle->GetCurSel();
		if (nCurSel == 0)
		{	//第一页隐藏prev按钮
			prevbutton->SetVisible(false);
		}
		else if (nCurSel == 4)
		{
			nextbutton->SetVisible(false);
		}
	}
}

bool CWhatsNewDlg::OnBtnRadio( TNotifyUI* pNotify )
{
	this->CloseWindow();
	return true;
}

bool CWhatsNewDlg::OnBtnPrev( TNotifyUI* pNotify )
{
	CToggleLayoutUI* pToggle = dynamic_cast<CToggleLayoutUI*>(GetItem(L"toggleLayout"));
	if (pToggle)
	{
		pToggle->SelectIndex(pToggle->GetCurSel()-1);
	}
	return true;
}

bool CWhatsNewDlg::OnBtnNext( TNotifyUI* pNotify )
{
	CToggleLayoutUI* pToggle = dynamic_cast<CToggleLayoutUI*>(GetItem(L"toggleLayout"));
	if (pToggle)
	{
		pToggle->SelectIndex(pToggle->GetCurSel()+1);
	}
	return true;
}

void CWhatsNewDlg::OnSelectItem( int nIndex, void* pParam )
{
    CWhatsNewDlg* pThis = (CWhatsNewDlg*)pParam;
    pThis->SetPrevOrNextVisible(nIndex);
}

void CWhatsNewDlg::OnClose()
{
    //::PostMessage(MainDlg::GetInstance()->GetHWND(), WM_ONLY,0,0);
}
