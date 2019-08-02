#include "stdafx.h"
#include "InputDlg.h"

InputDlg::InputDlg(wstring strMsg,  wstring* pDefault)
{
	_strMsg = strMsg;
	_pDefault = pDefault;
	m_nRoundRgn = 0;
}

void InputDlg::OnCreate()
{
	CLabelUI* labInfo =(CLabelUI*)GetItem(_T("centerlayout.info"));
	CEditUI* inputEdit = (CEditUI*)GetItem(_T("centerlayout.input"));

	if (labInfo) labInfo->SetText(_strMsg.c_str());
	if (inputEdit)
    {
        inputEdit->SetText((*_pDefault).c_str());
        inputEdit->SetFocus();
    }
}

bool InputDlg::OnOK()
{
    EndDialog(1);

    CEditUI* inputEdit = (CEditUI*)GetItem(_T("centerlayout.input"));
    if ( inputEdit ) _pDefault->assign(inputEdit->GetText());

    return true;
}

bool InputDlg::OnbtnOk(TNotifyUI* pNotify)
{
    return OnOK();
}

bool InputDlg::OnbtnCancel(TNotifyUI* pNotify)
{
	EndDialog(0);
	return true;
}

bool InputDlg::OnKeyDown( TNotifyUI* pNotify )
{
    return OnOK();
}

LRESULT InputDlg::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
    CButtonUI* pBtnOK = dynamic_cast<CButtonUI*>(GetItem(_T("buttom_Layout.ok")));    
    CButtonUI* pBtnCancel = dynamic_cast<CButtonUI*>(GetItem(_T("buttom_Layout.cancel")));    
    if (message == WM_KEYDOWN)
    {
        switch (wParam)
        {
        case VK_RETURN:
            OnOK();
            break;
        case VK_ESCAPE:
            EndDialog(0);
            break;
        case VK_LEFT:
            if (pBtnOK && pBtnCancel)
            {
                bool bOK = pBtnOK->IsFocused();
                bool bCancel = pBtnCancel->IsFocused();
                if (!bOK && !bCancel)
                    pBtnCancel->SetFocus();                    
                else if (bCancel)
                    pBtnOK->SetFocus();                
            }
            break;
        case VK_RIGHT:
            if (pBtnOK && pBtnCancel)
            {
                bool bOK = pBtnOK->IsFocused();
                bool bCancel = pBtnCancel->IsFocused();
                if (!bOK && !bCancel)
                    pBtnOK->SetFocus();                    
                else if (bOK)
                    pBtnCancel->SetFocus();                
            }
            break;
        }
    }
    return CWindowUI::WindowProc(message,wParam,lParam);
}

void InputDlg::SetMaxChar(int maxChar)
{
	CEditUI* inputEdit = (CEditUI*)GetItem(_T("centerlayout.input"));
	inputEdit->SetAttribute(L"maxchar", CStrOperation::IntToWString(maxChar).c_str());
}


