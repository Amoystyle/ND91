#pragma once

class DUI_API StyleObj
{
public:
	//************************************
	// @brief:    ������ʽ�ɼ����
	// @note:     
	// @param: 	  bool bVisible 
	// @return:   void 
	//************************************
	void SetVisible(bool bVisible);

	//************************************
	// @brief:    ��ȡ��ʽ�ɼ����
	// @note:     
	// @return:   bool 
	//************************************
	bool IsVisible();

	//************************************
	// @brief:    �������ƴ�����ʽ
	// @note:     
	// @param: 	  LPCTSTR lpszName ��ʽ����:Image/Text/Fill
	// @param: 	  CControlUI * pOwner ��ʽ��ӵ���߿ؼ�
	// @return:   StyleObj* Ŀ����ʽָ��
	//************************************
	static StyleObj* CreateStyle(LPCTSTR lpszName, CControlUI* pOwner);

	//************************************
	// @brief:    ��ȡ��ʽID
	// @note:     
	// @return:   LPCTSTR 
	//************************************
	LPCTSTR GetId();

	//************************************
	// @brief:    ��ȡ��ʽ����
	// @note:     
	// @return:   UITYPE_STYLE ��ʽ����UISTYLE_IMAGE/UISTYLE_TEXT/UISTYLE_FILL
	//************************************
	UITYPE_STYLE GetType();

	//************************************
	// @brief:    ������Ը����������λ��
	// @note:     
	// @param: 	  int l left
	// @param: 	  int t top
	// @param: 	  int r right
	// @param: 	  int b bottom
	// @param: 	  int a ��㷽ʽ
	// @return:   void 
	//************************************
	void SetInitPos(int l,int t,int r,int b, int a);

	//************************************
	// @brief:    ��ȡ��Ը����������λ��
	// @note:     
	// @param: 	  int & l ���left
	// @param: 	  int & t ���top
	// @param: 	  int & r ���right
	// @param: 	  int & b ���bottom
	// @return:   void 
	//************************************
	void GetInitPos(int& l,int& t,int& r,int& b);

	//************************************
	// @brief:    ��ȡ��Ը���������㷽ʽ
	// @note:     
	// @return:   int ��㷽ʽ
	//************************************
	int GetAnchor();

	//************************************
	// @brief:    ������ʽ�����������е�λ��
	// @note:     
	// @param: 	  RECT & rectRegion 
	// @return:   void 
	//************************************
	void SetRect(RECT& rectRegion);

	//************************************
	// @brief:    ��ȡ��ʽ�����������е�λ��
	// @note:     
	// @return:   RECT 
	//************************************
	RECT GetRect();

	//************************************
	// @brief:    ����ʽ�Ƿ񺭸Ǹõ�λ��
	// @note:     
	// @param: 	  POINT point 
	// @return:   bool 
	//************************************
	bool HitTest(POINT point);

	//************************************
	// @brief:    ������ʽ����
	// @note:     
	// @param: 	  LPCTSTR lpszName 
	// @param: 	  LPCTSTR lpszValue 
	// @return:   void 
	//************************************
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR  lpszValue); //���ػ���

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
	// @brief:    ��ȡͼƬ����
	// @note:     
	// @return:   ImageObj* 
	//************************************
	ImageObj* GetImageObj();

	//************************************
	// @brief:    ��ȡͼƬ·��ֵ
	// @note:     
	// @return:   LPCTSTR 
	//************************************
	LPCTSTR GetImageFile();

	//************************************
	// @brief:    ����ͼƬ·��
	// @note:     
	// @param: 	  LPCTSTR lpszPath 
	// @return:   void 
	//************************************
	void SetImage(LPCTSTR lpszPath);

	//************************************
	// @brief:    ����ͼƬ�Ķ�����ֵ
	// @note:     
	// @param: 	  LPVOID lpData 
	// @param: 	  long nSize 
	// @return:   void 
	//************************************
	void SetImage(LPVOID lpData, long nSize);

	//************************************
	// @brief:    ����ͼƬ����
	// @note:     
	// @param: 	  ImageObj * pImg 
	// @return:   void 
	//************************************
	void SetImage(ImageObj* pImg);

	//************************************
	// @brief:    ����ͼƬ����ģʽ
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
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR  lpszValue);//���ػ���
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
	// @brief:    ��ȡ���ֶ���
	// @note:     �ö�������ȫ�ֶ��󣬽����޸�
	// @return:   FontObj* 
	//************************************
	FontObj* GetFontObj();

	//************************************
	// @brief:    ��ȡ�����Ű淽ʽ
	// @note:     
	// @return:   UINT 
	//************************************
	UINT GetAlign();

	//************************************
	// @brief:    ���������Ű淽ʽ
	// @note:     
	// @param: 	  UINT uAlignStyle 
	// @return:   void 
	//************************************
	void SetAlign(UINT uAlignStyle);

	//************************************
	// @brief:    �����ı�
	// @note:     ����ʽû�������ı�ʱ��Ĭ�ϻ��ƿؼ�Owner���ı�
	// @param: 	  LPCTSTR lpszText 
	// @return:   void 
	//************************************
	void SetText(LPCTSTR lpszText);

	//************************************
	// @brief:    ��ȡ�ı�
	// @note:     
	// @return:   LPCTSTR 
	//************************************
	LPCTSTR GetText();

	//************************************
	// @brief:    ��ȡ�ı����Ʒ�ʽ
	// @note:     
	// @return:   UITYPE_FONT 
	//************************************
	UITYPE_FONT GetFontEffect();
public:
	TextStyle(CControlUI* pOwner);
	virtual ~TextStyle(void);
	void SetTextColor(DWORD dwColor);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR  lpszValue); //���ػ���
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
	UITYPE_FONT  m_nFontEffect;              //������ʽ��Ч���������òμ�xml
};
//////////////////////////////////////////////////////////////////////////////////////////

class DUI_API FillStyle:public StyleObj
{
public:
	//************************************
	// @brief:    �������ɫ
	// @note:     
	// @param: 	  DWORD dwColor RGB��ʽ
	// @return:   void 
	//************************************
	void SetFillColor(DWORD dwColor);

	//************************************
	// @brief:    ��ȡ���ɫ
	// @note:     
	// @return:   DWORD RGB��ʽ
	//************************************
	DWORD GetFillColor();

public:
	void SetBorderColor(DWORD dwColor);
	void SetGradientColor(DWORD dwColor1, DWORD dwColor2);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR  lpszValue); //���ػ���
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
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR  lpszValue); //���ػ���
	virtual bool Render(IRenderDC* pRenderDC, RECT& rcPaint, uint8 nState);
protected:
	DWORD m_dwColor1; //linecolor or gradientbegincolor
	DWORD m_dwColor2; //gradientendcolor
	uint8 m_nLineStyle;
	int  m_nPenWidth;
	uint8 m_nGradientMode;
};

///////////////////////////////////////////////////////////////////////////////////////////