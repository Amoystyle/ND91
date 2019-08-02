/*********************************************************************
 *       Copyright (C) 2010,应用软件开发
 *********************************************************************
 *   Date             Name                 Description
 *   2010-10-22       xqb
*********************************************************************/
#ifndef __UIRENDER_H__
#define __UIRENDER_H__

//namespace dui{

///////////////////////////////////////////////////////////////////////
class  DUI_API ClipObj
{
public:
    virtual ~ClipObj();
    RECT rcItem;
    HDC hDC;
    HRGN hRgn;
    HRGN hOldRgn;

    static void GenerateClip(HDC hDC, RECT rcItem, ClipObj& clip);
	static void GenerateClip(HDC hDC, RECT rcItem, ClipObj& clip, int nMode);

    static void GenerateClip(HDC hDC, RECT rcItem, int nRound, ClipObj& clip);
    static void UseOldClipBegin(HDC hDC, ClipObj& clip);
    static void UseOldClipEnd(HDC hDC, ClipObj& clip);
};


//////////////////////////////////////////////////////////////////////
class DUI_API DibObj
{
public:
	DibObj();
	virtual ~DibObj(void);

public:
	BOOL Create(HDC hdc, int cx, int cy);
	BOOL Create(HDC hdc, BITMAPINFOHEADER *pBmpHeader);
	void Clear();
	BOOL IsValid(){return m_hMemDC != NULL;}
	HDC GetSafeHdc(){return m_hMemDC;}
	HBITMAP GetHBitmap(){return m_hMemBmp;}
	BYTE * GetBits(){return m_pBits;}
	int GetSize();
	int GetWidth(){return m_bmpHeader.biWidth;};
	int GetHeight(){return -m_bmpHeader.biHeight;};
	void Flush(DWORD dwColor, DWORD dwAlpha = 255);
	void Flush(RECT& rect, DWORD dwColor, DWORD dwAlpha = 255);
	void DoAlpha();
	void DoAlpha(RECT &rect);
	HBITMAP Detach();
protected:
	HDC m_hMemDC;
	HBITMAP m_hMemBmp;
	HBITMAP m_hOldBmp;
	BITMAPINFOHEADER m_bmpHeader;
	BYTE *m_pBits;
	BYTE * m_pBackup;
};
////////////////////////////////////////////////////////////
class  DUI_API IRenderDC
{
public:
	IRenderDC();
	~IRenderDC();
	void SetDevice(HDC hDC);
	void SetDevice(DibObj* pDibObj);
	bool IsDeviceValid();

	HDC GetDC();
	DibObj* GetDibObj();
	void SetInvalidateRect(RECT& rect);
	RECT GetInvalidateRect(){return m_rectInvalidate;};
	void SetDrawWithAlpha(bool bDrawWithAlpha);
public:
	void DrawText(FontObj* pFontObj, RECT& rc, LPCTSTR lpszText, DWORD dwColor, UINT format, UITYPE_FONT nFontEffect = UIFONT_NORMAL);
	void DrawText(int nFontIndex, RECT& rc, LPCTSTR lpszText, DWORD dwColor, UINT format, UITYPE_FONT nFontEffect = UIFONT_NORMAL);
	void DrawHtmlText(FontObj* pFontObjDefault, RECT& rc, LPCTSTR pstrText, DWORD dwTextColor, RECT* prcLinks, tstring* sLinks, int& nLinkRects, UINT uStyle);

	void DrawImage(ImageObj* pImageObj, int nAlpha, int xDst, int yDst, int nImageIndex = 0);
	void DrawImage(ImageObj* pImageObj, int nAlpha, RECT& rcDst, int nImageIndex = 0, bool bHole =false);
	void DrawImage(ImageObj* pImageObj, int nAlpha, RECT& rcSrc,  RECT& rcDst);
protected:
	void DrawFrame (ImageObj* pImageObj, RECT& rect, RECT& rectImage, int nAlpha = 255);
	void FillInterior (ImageObj* pImageObj, RECT& rect, RECT& rectImage, int nAlpha = 255);
	void DrawFrame(ImageObj* pImageObj, RECT& destRect, ImageAlignHorz horzAlign = ImageAlignHorzLeft, ImageAlignVert vertAlign = ImageAlignVertTop, RECT* srcRect = NULL, int nAlpha = 255, int nFrame = 0);
	FontObj* AddFont(vector<FontObj *> &vecFonts, LPCTSTR lpszFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	FontObj* GetFont(vector<FontObj *> &vecFonts, LPCTSTR lpszFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	TEXTMETRIC GetTextMetric(HDC hdc, FontObj *pFontObj);
protected:
	DibObj* m_pDibObj;
	HDC m_hDC;
	RECT m_rectInvalidate;
	bool m_bDrawWithAlpha;
};

///////////////////////////////////////////////////

//} //dui命名空间
#endif // __UIRENDER_H__
