#pragma once

class DUI_API StyleObj
{
public:
	//************************************
	// @brief:    设置样式可见与否
	// @note:     
	// @param: 	  bool bVisible 
	// @return:   void 
	//************************************
	void SetVisible(bool bVisible);

	//************************************
	// @brief:    获取样式可见与否
	// @note:     
	// @return:   bool 
	//************************************
	bool IsVisible();

	//************************************
	// @brief:    依据名称创建样式
	// @note:     
	// @param: 	  LPCTSTR lpszName 样式名称:Image/Text/Fill
	// @param: 	  CControlUI * pOwner 样式的拥有者控件
	// @return:   StyleObj* 目标样式指针
	//************************************
	static StyleObj* CreateStyle(LPCTSTR lpszName, CControlUI* pOwner);

	//************************************
	// @brief:    获取样式ID
	// @note:     
	// @return:   LPCTSTR 
	//************************************
	LPCTSTR GetId();

	//************************************
	// @brief:    获取样式类型
	// @note:     
	// @return:   UITYPE_STYLE 样式类型UISTYLE_IMAGE/UISTYLE_TEXT/UISTYLE_FILL
	//************************************
	UITYPE_STYLE GetType();

	//************************************
	// @brief:    设置相对父容器的描点位置
	// @note:     
	// @param: 	  int l left
	// @param: 	  int t top
	// @param: 	  int r right
	// @param: 	  int b bottom
	// @param: 	  int a 描点方式
	// @return:   void 
	//************************************
	void SetInitPos(int l,int t,int r,int b, int a);

	//************************************
	// @brief:    获取相对父容器的描点位置
	// @note:     
	// @param: 	  int & l 输出left
	// @param: 	  int & t 输出top
	// @param: 	  int & r 输出right
	// @param: 	  int & b 输出bottom
	// @return:   void 
	//************************************
	void GetInitPos(int& l,int& t,int& r,int& b);

	//************************************
	// @brief:    获取相对父容器的描点方式
	// @note:     
	// @return:   int 描点方式
	//************************************
	int GetAnchor();

	//************************************
	// @brief:    设置样式在所属窗口中的位置
	// @note:     
	// @param: 	  RECT & rectRegion 
	// @return:   void 
	//************************************
	void SetRect(RECT& rectRegion);

	//************************************
	// @brief:    获取样式在所属窗口中的位置
	// @note:     
	// @return:   RECT 
	//************************************
	RECT GetRect();

	//************************************
	// @brief:    该样式是否涵盖该点位置
	// @note:     
	// @param: 	  POINT point 
	// @return:   bool 
	//************************************
	bool HitTest(POINT point);

	//************************************
	// @brief:    设置样式参数
	// @note:     
	// @param: 	  LPCTSTR lpszName 
	// @param: 	  LPCTSTR lpszValue 
	// @return:   void 
	//************************************
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR  lpszValue); //重载基类

public:
	virtual bool Render(IRenderDC* pRenderDC, RECT& rcPaint, UINT uState);
	virtual void OnSize(const RECT& rect);
	virtual void ReleaseResource();
	virtual void OnLanguageChange();
	virtual ~StyleObj(void);
public:
	RECT m_rcItem;

protected:
	StyleObj(CControlUI* pOwner);
	UINT m_uState;
	RECT m_rcCfg;
	uint8 m_iAnchor;
	UITYPE_STYLE m_Type;
	tstring m_strId;
	CControlUI* m_pOwner;
	bool  m_bVisible;
};

///////////////////////////////////////////////////////////////////////////////
class DUI_API ImageStyle:public StyleObj
{
public:
	//************************************
	// @brief:    获取图片对象
	// @note:     
	// @return:   ImageObj* 
	//************************************
	ImageObj* GetImageObj();

	//************************************
	// @brief:    获取图片路径值
	// @note:     
	// @return:   LPCTSTR 
	//************************************
	LPCTSTR GetImageFile();

	//************************************
	// @brief:    设置图片路径
	// @note:     
	// @param: 	  LPCTSTR lpszPath 
	// @return:   void 
	//************************************
	void SetImage(LPCTSTR lpszPath);

	//************************************
	// @brief:    设置图片的二进制值
	// @note:     
	// @param: 	  LPVOID lpData 
	// @param: 	  long nSize 
	// @return:   void 
	//************************************
	void SetImage(LPVOID lpData, long nSize);

	//************************************
	// @brief:    设置图片对象
	// @note:     
	// @param: 	  ImageObj * pImg 
	// @return:   void 
	//************************************
	void SetImage(ImageObj* pImg);

	//************************************
	// @brief:    设置图片拉伸模式
	// @note:     
	// @param: 	  UITYPE_PAINT PaintMode UIPAINT_NORMAL/UIPAINT_STRETCH/UIPAINT_CENTER
	// @return:   void 
	//************************************
	void SetPaintMode(UITYPE_PAINT PaintMode);

public:
	ImageStyle(CControlUI* pOwner);
	virtual ~ImageStyle(void);
	bool LoadImage();
	void SetGray(bool bGray);
	bool NeedMask(){return m_bNeedMask;}
	void NeedMask(bool bNeedMask){m_bNeedMask = bNeedMask;}
	bool InitedMask(){return m_bInitMask;}
	void InitedMask(bool bInitedMask){m_bInitMask = bInitedMask;}
	virtual void ReleaseResource();
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR  lpszValue);//重载基类
	virtual bool Render(IRenderDC* pRenderDC, RECT& rcPaint, UINT uState);
	virtual void OnLanguageChange();
protected:
	tstring m_strImageFile;
	uint8   m_arrImgSeq[5];
	ImageObj* m_pImageObj;
	bool    m_bNeedMask;
	bool	m_bInitMask;
	bool   m_bHole;
	bool   m_bCache;
	bool   m_bGray;
	bool   m_bSmoothing;
	bool   m_bMultiLanguage;
	UITYPE_PAINT   m_PaintMode;
};

////////////////////////////////////////////////////////////////////////////////////////
class DUI_API TextStyle:public StyleObj
{
public:
	//************************************
	// @brief:    获取文字对象
	// @note:     该对象属于全局对象，谨慎修改
	// @return:   FontObj* 
	//************************************
	FontObj* GetFontObj();

	//************************************
	// @brief:    获取文字排版方式
	// @note:     
	// @return:   UINT 
	//************************************
	UINT GetAlign();

	//************************************
	// @brief:    设置文字排版方式
	// @note:     
	// @param: 	  UINT uAlignStyle 
	// @return:   void 
	//************************************
	void SetAlign(UINT uAlignStyle);

	//************************************
	// @brief:    设置文本
	// @note:     当样式没有设置文本时，默认绘制控件Owner的文本
	// @param: 	  LPCTSTR lpszText 
	// @return:   void 
	//************************************
	void SetText(LPCTSTR lpszText);

	//************************************
	// @brief:    获取文本
	// @note:     
	// @return:   LPCTSTR 
	//************************************
	LPCTSTR GetText();

	//************************************
	// @brief:    获取文本绘制方式
	// @note:     
	// @return:   UITYPE_FONT 
	//************************************
	UITYPE_FONT GetFontEffect();
public:
	TextStyle(CControlUI* pOwner);
	virtual ~TextStyle(void);
	void SetTextColor(DWORD dwColor);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR  lpszValue); //重载基类
	virtual bool Render(IRenderDC* pRenderDC, RECT& rcPaint, UINT uState);
	virtual void OnLanguageChange();
protected:
	UINT   m_uAlignStyle;
	uint8  m_nFontIndex;
	DWORD  m_dwTextColor;
	LPTSTR m_lpszText;
	LPTSTR m_lpszTextKey;
	bool   m_bWrapSize;
	bool   m_bCalcRect;
	UITYPE_FONT  m_nFontEffect;              //文字样式特效索引，配置参见xml
};
//////////////////////////////////////////////////////////////////////////////////////////

class DUI_API FillStyle:public StyleObj
{
public:
	//************************************
	// @brief:    设置填充色
	// @note:     
	// @param: 	  DWORD dwColor RGB格式
	// @return:   void 
	//************************************
	void SetFillColor(DWORD dwColor);

	//************************************
	// @brief:    获取填充色
	// @note:     
	// @return:   DWORD RGB格式
	//************************************
	DWORD GetFillColor();

public:
	void SetBorderColor(DWORD dwColor);
	void SetGradientColor(DWORD dwColor1, DWORD dwColor2);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR  lpszValue); //重载基类
	virtual bool Render(IRenderDC* pRenderDC, RECT& rcPaint, UINT uState);

	FillStyle(CControlUI* pOwner);
	virtual ~FillStyle(void);
protected:
	DWORD m_dwColor1; //fillcolor or gradientbegincolor
	DWORD m_dwColor2; //bordercolor or  gradientendcolor
	uint8 m_nGradientMode;
	uint8 m_nBorderMode;
	int m_nRoundWidth, m_nRoundHeight;
	HBRUSH m_hbr;
};
///////////////////////////////////////////////////////////////////////////////////////////

class DUI_API LineStyle:public StyleObj
{
public:
	LineStyle(CControlUI* pOwner);
	virtual ~LineStyle(void);
public:
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR  lpszValue); //重载基类
	virtual bool Render(IRenderDC* pRenderDC, RECT& rcPaint, uint8 nState);
protected:
	DWORD m_dwColor1; //linecolor or gradientbegincolor
	DWORD m_dwColor2; //gradientendcolor
	uint8 m_nLineStyle;
	int  m_nPenWidth;
	uint8 m_nGradientMode;
};

///////////////////////////////////////////////////////////////////////////////////////////