#ifndef _IMAGE_RENDER_H
#define _IMAGE_RENDER_H

enum ImageAlignHorz
{
	ImageAlignHorzLeft,
	ImageAlignHorzCenter,
	ImageAlignHorzRight,
	ImageAlignHorzStretch
};

enum ImageAlignVert
{
	ImageAlignVertTop,
	ImageAlignVertCenter,
	ImageAlignVertBottom,
	ImageAlignVertStretch
};

class DUI_API ImageObj
{
public:
#ifdef _GDIPLUS_SUPPORT
	ImageObj(Bitmap* bitmap);
	Bitmap*  GetBitmap();
#endif
	
	ImageObj(HBITMAP hBitmap);
	HBITMAP  GetHBitmap();
	
	ImageObj(HICON hIcon);
	HICON  GetHIcon();

public:
	void Release();
	ImageObj* Get();
	virtual ~ImageObj(void);

public:
	int		 GetCellWidth()	    {return m_iCellWidth;}
	int		 GetCellHeight()	{return m_iCellHeight;}
	int		 GetWidth()		    {return m_nWidth;}
	int		 GetHeight()		{return m_nHeight;}
	int		 GetFrameCount()	{return m_nFrameCount;}
	long     GetFrameDelay (int nFrame);
	void     SetActiveFrame(int nFrame);
	bool	 GetAdjustColor()	{return m_bNeedAdjustColor;}
	void     SetAdjustColor(bool bAdjustColor);
	LPCTSTR  GetFilePath();
	void     SetFilePath(LPCTSTR lpszPath);
	bool	 IsAdjustAvgColor(){return m_bAdjustAvgColor;}
	void     SetInset(RECT& rect);
	void     SetCellNum(int nRows, int nCols);
	int		 GetCellRow(){return m_iNumCellRows;}
	int		 GetCellCol(){return m_iNumCellColumns;}
	void     SetKey(LPCTSTR lpszKey);
	RECT     GetInset(){return m_rcInset;};
	void     AdjustHLS(int nHue, int nSaturation, int lightness);
	bool     Reload();
	void     DoGray();  //灰度处理
	bool     IsGray()   {return m_bGary;}  //灰度处理
	void     AdjustAvgColor();
	COLORREF GetAvgColor();
	void     DoAlphaFromMask(ImageObj* pImageObj); //透明处理
	bool     IsDoAlpha()  {return m_bDoAlpha;}
	void     Resize(int nWidth, int nHeight);
	HBITMAP  Detach();
protected:
	void DoAlphaFromMask1(ImageObj* pImageObj); //透明处理
	void DoAlphaFromMask2(ImageObj* pImageObj); //透明处理
protected:
	void    Init();
	long    m_reflong;            //引用计数
	tstring m_strFileName;       //文件名
	tstring m_strKey;            //关键字

	HBITMAP m_hBitmap;	//图像句柄
	HICON   m_hIcon;
	int     m_nWidth, m_nHeight; //宽度高
	int     m_iCellHeight,  m_iCellWidth; //单元格宽度高度
	int     m_iNumCellRows, m_iNumCellColumns; //行数列数
	RECT    m_rcInset; //九宫格设置参数
	bool    m_bNeedAdjustColor; //是否要变色
	UINT	m_nFrameCount;
	bool    m_bGary;
	bool    m_bDoAlpha;
	bool	m_bAdjustAvgColor;
	UINT	m_nActiveFrame;

#ifdef _GDIPLUS_SUPPORT
	Bitmap  * m_pBitmap;
	PropertyItem*	m_pPropertyItem;
#endif
};

#endif //_IMAGE_RENDER_H