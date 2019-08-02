/*********************************************************************
*            Copyright (C) 2010, 网龙天晴程序应用软件部
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
此多行编辑框实现的自动换行功能，以及一些必要的操作如复制黏贴拖动等等功能
一、在多行编辑框控件中，任何一个字符有3种方法表示其所在位置:
1.  行位置， 列位置 在CMultiLineEdit定义CharPos结构体表示。
2.  在字符串中索引号
3.  实际在编辑框中的位置。
二、成员变量说明:
1.  CharPos m_cpSelBegin;               
	CharPos m_cpSelEnd;
两个变量表示当前选定文本的开始位置和结束位置用CharPos结构体表示。默认的,你未拖动文本时m_cpSelBegin 与 m_cpSelEnd是相等的
且m_cpSelEnd也表示当前光标的位置, 若要修改光标的位置只需更改这个值即可.  特别注意
2. CharPos m_cpPrevSelEnd;
表示上次选定的结束位置 (定义此变量是为了拖动文本选定时，防止出现闪烁问题)
3. CLineInfoArray m_lineInfoArray;
其中CLineInfoArray是由vector<LPLINEINFO>定义的:
typedef vector<LPLINEINFO>  CLineInfoArray;
typedef LINEINFO *LPLINEINFO;
struct LINEINFO   //记录每一行信息的结构体
{
	unsigned CharsCount;  //一行多少个字符
	MyBool bParaEnd;    //是否有回车
	LINEINFO(unsigned nCount = 0, MyBool bFlag = MYFALSE)  //默认构造函数
	{
	CharsCount = nCount;
	bParaEnd = bFlag;
	}
};
(1).此成员变量及其重要, 记录着每行字符个数,是否有回车. 而判断是否有回车，也计入一行有多少个字符(有则加2个字符)。
(2).只要有对文本的插入删除操作都会导致此变量里元素的重设
4. int m_nFirstVisibleLine;
随着文本增多，行数也随着增多，此变量表示的是 当文本行数超出文本区域时，显示第一行可见行的行号。
重新调整m_nFirstVisiblePixel大小，以使光标位置可见
三、成员函数说明，见类中注释。
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

	//消息映射
	UIBEGIN_MSG_MAP
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, IDR_MENU_UIEDIT,  OnMenuItemClick)
	UIEND_MSG_MAP

	bool OnMenuItemClick(TNotifyUI* pNotify);

	typedef struct tagCHARPOS
	{
		unsigned  LineIndex;//行号
		unsigned  CharIndex;//列号
	}CharPos;
	struct LINEINFO
	{
		unsigned CharsCount;//一行多少个字符
		MyBool bParaEnd;//是否有回车
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
		INSERT_ONLY,//不删除，直接追加
		DEL_AND_INSERT//删除并追加
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
	void AdjustAllLineInfo();//慎用，重设m_lineInfoArray元素信息

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
	int	m_nLineHeight;  //每行的高度
	int	m_nAveCharWidth; //字符的平均加权宽带
	int	m_nMaxchar;   //设置最大容纳字符个数
	int	m_nMaxUTF8Char;
	int	m_nCurUTF8Char;

	unsigned m_uClientWidth;//当前编辑框的宽度
	unsigned m_uClientHeight;//当前编辑框的高度

	COLORREF m_clrInputBk;
	COLORREF m_clrText;
	COLORREF m_clrSelBk;
	COLORREF m_clrSelText;

	bool m_bReadOnly;
	bool m_bKillFocus;
	bool m_bTransparent;  //是否背景透明
	bool m_bAutoSize;  //是否是大小可变的编辑框
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
	uint8 m_nFontIndex;               //文字样式索引，配置参见xml
};

//////////////////////////////////////////////////////////////////////////
/*
此为单行编辑框
一、在单行编辑框控件中，任何一个字符有2种方法表示其所在位置:
1. 实际在编辑框中的位置
2. 该字符在字符串中的索引位置
二、成员变量说明
1. 	unsigned m_uSelBegin;
	unsigned m_uSelEnd;
同多行编辑框一样也定义有选定范围变量。不过其类型含义表示是在字符串中的索引。
m_uSelEnd也有控制光标位置作用
2.	unsigned m_uPrevSelEnd;
	int m_nPrevSelEndPixel;
表示上次选定末尾字符索引位置和上次选定末尾字符的像素位置，设置此2个变量用于对编辑框效率优化:
由于计算每个字符的像素位置通常都是从第一个字符所占像素的大小一直累加到你指定字符的位置。但是
当文本框的内容过多，光标的频繁变换会导致计算的卡死，因此设置2个变量每次计算都是从上次光标所在
的位置进行开始计算。
3. int m_nFirstVisiblePixel;
表示第一个可见像素的位置，注意这与多行表示的第一个可见行号有所不同。
*/
//////////////////////////////////////////////////////////////////////////
class DUI_API CEditUI:public CLabelUI, public INotifyUI
{
public:

	//消息映射
	UIBEGIN_MSG_MAP
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, IDR_MENU_UIEDIT,  OnMenuItemClick)
		UIEND_MSG_MAP

	bool OnMenuItemClick(TNotifyUI* pNotify);

	CEditUI();
	virtual ~CEditUI();

	LPCTSTR GetText();	
	virtual void SetText(LPCTSTR lpszText);
	virtual void SetTextWithoutNotify(LPCTSTR lpszText);
	
	void SelectAllText();  //全部选定文本，并刷新

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
	virtual bool SetSelEnd(int pos = -1);  //若有选定则取消选定，并设置光标的位置

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

	void SetPrevSelEndInfo(int offsetPixels); //当进行插入或者删除时, 对m_uPrevSelEnd; m_nPrevSelEndPixel;设置传递的参数是调用insert和DelSelRange的返回值。
	void SetPrevSelEndInfo();//当对光标进行定位时调用 设置m_uPrevSelEnd; m_nPrevSelEndPixel;变量
	BOOL EnsureSelEndVisible();//重新调整m_nFirstVisiblePixel大小，以使光标位置可见

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
	unsigned GetLeftClientWidthIndex(HDC hDC, unsigned pos);//从当前位置向左获取距离该位置有文本框宽度大小的字符索引。
	unsigned GetRightClientWidthIndex(HDC hDC, unsigned pos);//从当前位置向右获取距离该位置有文本框宽度大小的字符索引。

	FontObj* GetFontObj();

protected:
	HWND m_hParentWnd;
	HFONT m_hFont;

	unsigned m_uSelBegin;
	unsigned m_uSelEnd;
	unsigned m_uPrevSelEnd;
	int m_nPrevSelEndPixel;

	int m_nFirstVisiblePixel;
	int m_nTextPixel;//总的文本字符所占的像素大小之和
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
	tstring m_strPasswdText;//密码掩膜，字符个数与真正的密码一直不过里面存放的是m_tcPassChar字符
	TCHAR m_tcPassChar;//若是密码框，密码框密码的字符
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

	UITYPE_FONT   m_nFontEffect;              //文字样式特效索引，配置参见xml
	UINT    m_uAlignStyle;              //文字显示方式
	uint8 m_nFontIndex;               //文字样式索引，配置参见xml
	EDrawTextH  m_eDrawTextH;
};

#endif // DIRECTUI_LAYOUT_RTL

#endif // _UIEDIT_H