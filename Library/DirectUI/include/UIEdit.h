/*********************************************************************
*            Copyright (C) 2010, �����������Ӧ�������
**********************************************************************
*   Date        Name        Description
*   9/17/2010   hanzp		Create.
*   9/20/2010   hanzp		Add Class CEditUI.
*   10/27/2010  hanzp		Add MultiLineEdit
*********************************************************************/

#ifndef _UIEDIT_H
#define _UIEDIT_H

#ifndef DIRECTUI_LAYOUT_RTL
#include "UIMenu.h"
#include "UIList.h"
#include "UIToolTip.h"
#include <Imm.h>


///////////////////////////////////////////////////////////
/*
�˶��б༭��ʵ�ֵ��Զ����й��ܣ��Լ�һЩ��Ҫ�Ĳ����縴������϶��ȵȹ���
һ���ڶ��б༭��ؼ��У��κ�һ���ַ���3�ַ�����ʾ������λ��:
1.  ��λ�ã� ��λ�� ��CMultiLineEdit����CharPos�ṹ���ʾ��
2.  ���ַ�����������
3.  ʵ���ڱ༭���е�λ�á�
������Ա����˵��:
1.  CharPos m_cpSelBegin;               
	CharPos m_cpSelEnd;
����������ʾ��ǰѡ���ı��Ŀ�ʼλ�úͽ���λ����CharPos�ṹ���ʾ��Ĭ�ϵ�,��δ�϶��ı�ʱm_cpSelBegin �� m_cpSelEnd����ȵ�
��m_cpSelEndҲ��ʾ��ǰ����λ��, ��Ҫ�޸Ĺ���λ��ֻ��������ֵ����.  �ر�ע��
2. CharPos m_cpPrevSelEnd;
��ʾ�ϴ�ѡ���Ľ���λ�� (����˱�����Ϊ���϶��ı�ѡ��ʱ����ֹ������˸����)
3. CLineInfoArray m_lineInfoArray;
����CLineInfoArray����vector<LPLINEINFO>�����:
typedef vector<LPLINEINFO>  CLineInfoArray;
typedef LINEINFO *LPLINEINFO;
struct LINEINFO   //��¼ÿһ����Ϣ�Ľṹ��
{
	unsigned CharsCount;  //һ�ж��ٸ��ַ�
	MyBool bParaEnd;    //�Ƿ��лس�
	LINEINFO(unsigned nCount = 0, MyBool bFlag = MYFALSE)  //Ĭ�Ϲ��캯��
	{
	CharsCount = nCount;
	bParaEnd = bFlag;
	}
};
(1).�˳�Ա����������Ҫ, ��¼��ÿ���ַ�����,�Ƿ��лس�. ���ж��Ƿ��лس���Ҳ����һ���ж��ٸ��ַ�(�����2���ַ�)��
(2).ֻҪ�ж��ı��Ĳ���ɾ���������ᵼ�´˱�����Ԫ�ص�����
4. int m_nFirstVisibleLine;
�����ı����࣬����Ҳ�������࣬�˱�����ʾ���� ���ı����������ı�����ʱ����ʾ��һ�пɼ��е��кš�
���µ���m_nFirstVisiblePixel��С����ʹ���λ�ÿɼ�
������Ա����˵����������ע�͡�
*/
///////////////////////////////////////////////////////////

typedef	  BYTE	MyBool;
#ifdef    UNICODE
#define	  MYTRUE	2
#define	  MYFALSE	0
#else
#define	  MYBOOL	BYTE
#define	  MYTRUE	1
#define	  MYFALSE	0
#endif

enum
{
	enEditLimitNULL = 0x0000,
	enEditLimitNumber = 0x0001,
	enEditLimitLetter = 0x0002,
	enEditLimitChinese = 0x0008,
	enEditLimitString = 0x0010,
	enEditExceptNumber = 0x0020,
	enEditExceptLetter = 0x0040,
	enEditExceptChinese = 0x0080,
	enEditExceptString = 0x01000,
};

typedef enum tagDrawTextH
{
	DT_LEFT_H=0,
	DT_CENTER_H,
	DT_RIGHT_H
} EDrawTextH;

typedef enum tagDrawTextV
{
	DT_TOP_V=0,
	DT_CENTER_V,
	DT_BOTTOM_V
} EDrawTextV;

class DUI_API CMultiLineEdit:public CLabelUI, public INotifyUI
{
public:

	//��Ϣӳ��
	UIBEGIN_MSG_MAP
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, IDR_MENU_UIEDIT,  OnMenuItemClick)
	UIEND_MSG_MAP

	bool OnMenuItemClick(TNotifyUI* pNotify);

	typedef struct tagCHARPOS
	{
		unsigned  LineIndex;//�к�
		unsigned  CharIndex;//�к�
	}CharPos;
	struct LINEINFO
	{
		unsigned CharsCount;//һ�ж��ٸ��ַ�
		MyBool bParaEnd;//�Ƿ��лس�
		LINEINFO(unsigned nCount = 0, MyBool bFlag = MYFALSE)
		{
			CharsCount = nCount;
			bParaEnd = bFlag;
		}
	};
	typedef LINEINFO *LPLINEINFO;
	typedef vector<LPLINEINFO>  CLineInfoArray;
	enum ADJUSTMODE
	{
		INSERT_ONLY,//��ɾ����ֱ��׷��
		DEL_AND_INSERT//ɾ����׷��
	};

public:
	CMultiLineEdit();
	virtual ~CMultiLineEdit();

	LPCTSTR GetText();
	void SetText(LPCTSTR lpszText);
	void SetSelEndPoint();
	void SetClientWidth(unsigned width);
	void SetReadOnly(bool bReadOnly);
	bool IsReadOnly(){return m_bReadOnly;};
	void SetDrawTextType(EDrawTextH eDrawTypeH, EDrawTextV eDrawTextV);
	void SetFirstVisibleLine(unsigned uLineNum);
	void ShowToolTip(LPCTSTR lpszContent);
	bool IsKillFocus();

	void SetTextColor(DWORD dwClrText);
	void SetFontName(LPCTSTR lpszFontName);
	void SetFontSize(int nFontSize);
	void SetBold(bool bBold);
	void SetItalic(bool bItalic);
	void SetUnderline(bool bUnderline);
	void SetFont(LOGFONT logFont);

	FontObj* GetFontObj();
public:
	virtual void Render(IRenderDC* pRenderDC, RECT &rcPaint);
	virtual bool Event(TEventUI& event);
	virtual void Init();
	virtual void SetRect(RECT& rectRegion);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	UI_DECLARE_DYNCREATE();

protected:
	void OnKeyDown(UINT nChar, UINT nFlags);
	void OnChar(UINT nChar, UINT nFlags);
	void OnLButtonDown(UINT nFlags, POINT pt);
	void OnRButtonUp(UINT nFlags, POINT pt);
	void OnLButtonUp(UINT nFlags, POINT pt);
	void OnLButtonDblClk(UINT nFlags, POINT pt);
	void OnMouseMove(UINT nFlags, POINT pt);
	virtual void OnPaste(){}

private:
	void GetSel(tstring &str);
	void Copy();
	void Paste();
	void Clear();

	BOOL CreateCaret(int nWidth, int nHeight);   
	BOOL SetCaretPos(int x, int y);
	BOOL ShowCaret();
	BOOL HideCaret();
	BOOL DestroyCaret();

	void CharPos2Point(const CharPos &pos, POINT &pt);
	void Point2CharPos(const POINT &pt, CharPos &pos);   
	void CharPos2Index(const CharPos &pos, unsigned &nIndex);
	void Index2CharPos(const unsigned &nIndex, CharPos &pos);
	
	void LeftSelEnd();
	void RightSelEnd();
	void UpSelEnd();
	void DownSelEnd();
	bool DelSelRange();
	void Insert(LPCTSTR lpszText);
	bool EnsureSelEndVisible();
	
	void CalcTextMetrics();
	int GetTextHeight();
	void SetStyleObjRect(const RECT& rc);

	RECT GetSelectReDrawRect();
	RECT GetInsertReDrawRect(const CharPos &beforeChangePos, const CharPos &afterChangePos, const bool bAddLine);
	RECT GetDeleteReDrawRect(const CharPos &beforeChangePos, const CharPos &afterChangePos, const bool bSubLine);
	void RemoveLineInfo(unsigned nStartLine, unsigned nCount);
	void RemoveLineInfo(unsigned nStartLine = 0);
	void AdjustFromLineToParaEnd(const unsigned LineNum, const ADJUSTMODE mode = DEL_AND_INSERT);
	void AdjustAllLineInfo();//���ã�����m_lineInfoArrayԪ����Ϣ

	int FindPrevCrlfLine(unsigned nStartLine);
	unsigned FindNextCrlfLine(unsigned nStartLine);
	int CutStringForOneLine(HDC hDC, const TCHAR *lpszText);

	BOOL TextOut(HDC hDC, int x, int y, LPCTSTR lpszText, int c);
	void DrawRange(HDC hDC, const CharPos &pos1, const CharPos &pos2);
	void InvalidateRect(LPRECT lpRect, BOOL bFlag = FALSE);
	
private:
	HCURSOR m_hCursor;
	HWND m_hParentWnd;
	HFONT m_hFont;
	LOGFONT m_logFont;

	CharPos	m_cpSelBegin;
	CharPos	m_cpSelEnd;
	CharPos	m_cpPrevSelEnd;
	CLineInfoArray m_lineInfoArray;
	
	int	m_nFirstVisibleLine;
	int	m_nLineHeight;  //ÿ�еĸ߶�
	int	m_nAveCharWidth; //�ַ���ƽ����Ȩ���
	int	m_nMaxchar;   //������������ַ�����
	int	m_nMaxUTF8Char;
	int	m_nCurUTF8Char;

	unsigned m_uClientWidth;//��ǰ�༭��Ŀ��
	unsigned m_uClientHeight;//��ǰ�༭��ĸ߶�

	COLORREF m_clrInputBk;
	COLORREF m_clrText;
	COLORREF m_clrSelBk;
	COLORREF m_clrSelText;

	bool m_bReadOnly;
	bool m_bKillFocus;
	bool m_bTransparent;  //�Ƿ񱳾�͸��
	bool m_bAutoSize;  //�Ƿ��Ǵ�С�ɱ�ı༭��
	bool m_bMenuEnable;
	bool m_bhasCaret;
	bool m_bCaretShow;
	BOOL m_bMaxSize;
	SIZE m_cxyMaxSize;
	RECT m_rcPrevItem;

	EDrawTextH  m_eDrawTextH;
	EDrawTextV  m_eDrawTextV;
 	//bool m_bLimit;
 	//UINT m_uLimitType;
 	//tstring m_strLimitString;
 	//tstring m_strToolTipAlign;
	bool m_bInit;
	CScrollbarUI *m_pVerticalScrollbar;
	uint8 m_nFontIndex;               //������ʽ���������òμ�xml
};

//////////////////////////////////////////////////////////////////////////
/*
��Ϊ���б༭��
һ���ڵ��б༭��ؼ��У��κ�һ���ַ���2�ַ�����ʾ������λ��:
1. ʵ���ڱ༭���е�λ��
2. ���ַ����ַ����е�����λ��
������Ա����˵��
1. 	unsigned m_uSelBegin;
	unsigned m_uSelEnd;
ͬ���б༭��һ��Ҳ������ѡ����Χ���������������ͺ����ʾ�����ַ����е�������
m_uSelEndҲ�п��ƹ��λ������
2.	unsigned m_uPrevSelEnd;
	int m_nPrevSelEndPixel;
��ʾ�ϴ�ѡ��ĩβ�ַ�����λ�ú��ϴ�ѡ��ĩβ�ַ�������λ�ã����ô�2���������ڶԱ༭��Ч���Ż�:
���ڼ���ÿ���ַ�������λ��ͨ�����Ǵӵ�һ���ַ���ռ���صĴ�Сһֱ�ۼӵ���ָ���ַ���λ�á�����
���ı�������ݹ��࣬����Ƶ���任�ᵼ�¼���Ŀ������������2������ÿ�μ��㶼�Ǵ��ϴι������
��λ�ý��п�ʼ���㡣
3. int m_nFirstVisiblePixel;
��ʾ��һ���ɼ����ص�λ�ã�ע��������б�ʾ�ĵ�һ���ɼ��к�������ͬ��
*/
//////////////////////////////////////////////////////////////////////////
class DUI_API CEditUI:public CLabelUI, public INotifyUI
{
public:

	//��Ϣӳ��
	UIBEGIN_MSG_MAP
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, IDR_MENU_UIEDIT,  OnMenuItemClick)
		UIEND_MSG_MAP

	bool OnMenuItemClick(TNotifyUI* pNotify);

	CEditUI();
	virtual ~CEditUI();

	LPCTSTR GetText();	
	virtual void SetText(LPCTSTR lpszText);
	virtual void SetTextWithoutNotify(LPCTSTR lpszText);
	
	void SelectAllText();  //ȫ��ѡ���ı�����ˢ��

	bool IsKillFocus();
	bool IsReadOnly();
	void SetReadOnly(bool bReadOnly = true);
	void SetPassword(bool bPassword);
	void SetDrawTextType(EDrawTextH eDrawTypeH);
	void HideToolTip();
	void ShowToolTip(LPCTSTR lpszContent, CToolTipUI::ENToolTipType type = CToolTipUI::enToolTipTypeInfo);

	void EnableIME();
	void DisableIME();
	bool IsEmpty();
	bool IsMailFormat();
	bool IsNumberFormat();
	bool IsLetterFormat();
	bool IsChineseFormat();

	UINT GetAlign();
	void SetAlign(UINT uStyle); 
public:
	virtual void Render(IRenderDC* pRenderDC, RECT &rcPaint);
	virtual bool Event(TEventUI& event);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	virtual void Init();
	virtual void SetRect(RECT& rectRegion);
	UI_DECLARE_DYNCREATE();

	virtual int DelSelRange();
	virtual bool SetSelEnd(int pos = -1);  //����ѡ����ȡ��ѡ���������ù���λ��

protected:
	void OnSetFocus();
	void OnKillFocus();
	virtual void OnChar(WPARAM wParam, LPARAM lParam, LPPOINT ppt = NULL);
	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam, LPPOINT ppt = NULL);
	virtual void OnRButtonUp(WPARAM wParam, LPARAM lParam, LPPOINT ppt = NULL);
	virtual void OnLButtonUp(WPARAM wParam, LPARAM lParam, LPPOINT ppt = NULL);
	virtual void OnLButtonDown(WPARAM wParam, LPARAM lParam, LPPOINT ppt = NULL);
	virtual void OnLButtonDblick(WPARAM wParam, LPARAM lParam, LPPOINT ppt = NULL);
	virtual void OnMouseMove(WPARAM wParam, LPARAM lParam, LPPOINT ppt = NULL);
	virtual void OnPaste(){}

	void SetPrevSelEndInfo(int offsetPixels); //�����в������ɾ��ʱ, ��m_uPrevSelEnd; m_nPrevSelEndPixel;���ô��ݵĲ����ǵ���insert��DelSelRange�ķ���ֵ��
	void SetPrevSelEndInfo();//���Թ����ж�λʱ���� ����m_uPrevSelEnd; m_nPrevSelEndPixel;����
	BOOL EnsureSelEndVisible();//���µ���m_nFirstVisiblePixel��С����ʹ���λ�ÿɼ�

	void Index2Point(const unsigned index, POINT &pt);
	void Point2Index(LPPOINT ppt, unsigned &index);
	RECT GetInsertReDrawRect(unsigned beforeInsertPos);
	RECT GetDeleteReDrawRect();
	RECT GetSelectReDrawRect(unsigned beforeMovePos);
	void SetSelEndPoint();

private:
	void GetSel(tstring &str);
	void Copy();
	void Paste();

	BOOL CreateCaret(int nWidth, int nHeight);
	BOOL SetCaretPos(int x, int y);
	BOOL ShowCaret();
	BOOL HideCaret();
	BOOL DestroyCaret();

	void LeftSelEnd();
	void RightSelEnd();
	virtual bool OnLimitChar(UINT nChar);
	int Insert(LPCTSTR lpszText);

	void CalcTextMetrics();
	void SetStyleObjRect(const RECT& rc);

	BOOL TextOut(HDC hDC, int x, int y, LPCTSTR lpszText, int c);
	void DrawRange(HDC hDC, const unsigned &index1, const unsigned &index2);

	int GetCharsPixels(HDC hDC, const unsigned nCounts) const;	
	unsigned GetCharsCounts(HDC hDC, const int pixels) const;
	unsigned GetLeftClientWidthIndex(HDC hDC, unsigned pos);//�ӵ�ǰλ�������ȡ�����λ�����ı����ȴ�С���ַ�������
	unsigned GetRightClientWidthIndex(HDC hDC, unsigned pos);//�ӵ�ǰλ�����һ�ȡ�����λ�����ı����ȴ�С���ַ�������

	FontObj* GetFontObj();

protected:
	HWND m_hParentWnd;
	HFONT m_hFont;

	unsigned m_uSelBegin;
	unsigned m_uSelEnd;
	unsigned m_uPrevSelEnd;
	int m_nPrevSelEndPixel;

	int m_nFirstVisiblePixel;
	int m_nTextPixel;//�ܵ��ı��ַ���ռ�����ش�С֮��
	int m_nAveCharWidth;
	int m_nLineHeight;
	int m_cy;

	COLORREF m_clrInputBk;
	COLORREF m_clrText;
	COLORREF m_clrSelBk;
	COLORREF m_clrSelText;
	COLORREF m_clrEmptyTipsText;

	bool m_bCaretShow;
	bool m_bReadOnly;
	bool m_bAutoSize;
	bool m_bMenuEnable;
	bool m_bTransparent;
	bool m_bhasCaret;
	bool m_bKillFocus;

	bool m_bPassword;
	tstring m_strPasswdText;//������Ĥ���ַ�����������������һֱ���������ŵ���m_tcPassChar�ַ�
	TCHAR m_tcPassChar;//��������������������ַ�
	int m_nPasswdCharWidth;
	tstring m_strEmptyTips;

	bool m_bLimit;
	UINT m_uLimitType;
	tstring m_strLimitString;

	bool m_bMaxSize;
	int m_nMaxchar;
	int m_nMaxUTF8Char;
	int m_nCurUTF8Char;
	SIZE m_cxyMaxSize;
	RECT m_rcPrevItem;

	tstring m_strToolTipAlign;
	HWND m_hToolTipWnd;
	bool m_bInit;

	UITYPE_FONT   m_nFontEffect;              //������ʽ��Ч���������òμ�xml
	UINT    m_uAlignStyle;              //������ʾ��ʽ
	uint8 m_nFontIndex;               //������ʽ���������òμ�xml
	EDrawTextH  m_eDrawTextH;
};

#endif // DIRECTUI_LAYOUT_RTL

#endif // _UIEDIT_H