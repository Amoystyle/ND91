#ifndef _FONTOBJ_H
#define _FONTOBJ_H

class  DUI_API FontObj
{
public:
	FontObj();
	FontObj(FontObj& obj);
	~FontObj();
public:
	//qc 21462 modify by hanzp. ±‡¬Î“≈¬©DT_NOPREFIX
	void  CalcText( HDC hdc, RECT& rc, LPCTSTR lpszText, UINT format = DT_CALCRECT|DT_SINGLELINE| DT_LEFT_EX|DT_NOPREFIX);
	void  DrawText( HDC hdc, RECT& rc, LPCTSTR lpszText, DWORD dwColor = 0, UINT format = DT_SINGLELINE|DT_VCENTER|DT_LEFT_EX|DT_NOPREFIX, UITYPE_FONT nFontEffect = UIFONT_NORMAL);
	void  DrawGrowText( HDC hdc, RECT& rc, LPCTSTR lpszText, DWORD dwColor = 0, UINT format = DT_SINGLELINE|DT_VCENTER|DT_LEFT_EX|DT_NOPREFIX);
	void  DrawGDIPlusText(HDC hdc, RECT& rc, LPCTSTR lpszText, DWORD dwColor = 0, UINT format = DT_SINGLELINE|DT_VCENTER|DT_LEFT_EX|DT_NOPREFIX);
	HFONT GetFont();
	LOGFONT& GetLogFont();
	int   GetHeight();
	void  SetId(LPCTSTR lpszId);
	LPCTSTR GetId();
	void  SetFaceName(LPCTSTR lpszFace);
	LPCTSTR GetFaceName();
	void  SetFontSize(int nSize);
	void  SetBold(bool bBold);
	bool  IsBold();
	void  SetUnderline(bool bUnderline);
	bool  IsUnderline();
	void  SetItalic(bool bItalic);
	bool  IsItalic();

	bool IsStrikeOut();
	void SetStrikeOut(bool bStrikeOut);
private:
	LOGFONT m_lfFont;
	HFONT	m_hFont;
	tstring m_strId;
};

#endif // _FONTOBJ_H