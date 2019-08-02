/*********************************************************************
 *       Copyright (C) 2010,应用软件开发
 *********************************************************************
 *   Date             Name                 Description
 *   2010-09-27       xqb
*********************************************************************/

#ifndef _UIRICHEDIT_H
#define _UIRICHEDIT_H
class CTextHostBase;
#include <Richedit.h>
#include <richole.h>
#include <TextServ.h>

class CRichEditUI;
/////////////////////////////////////////////
void DUI_API DetectURLs(LPCTSTR lpszText, CStdPtrArray& arrLinksOut);
//////////////////////////////////////////////////
/**文本格式
*/
typedef struct tagTextFormat
{
	DWORD dwMask;//!<字体掩码
	DWORD dwEffects;//!<字体效果（粗体、斜体、下划线、删除线等）
	DWORD dwFontSize;//!<尺寸（以POINT为单位）
	COLORREF crTextColor;//!<字体颜色
	TCHAR  szFaceName[32];//!<字体名称
	LCID   lcid;
    // Locale ID
}TEXTFORMAT;


#define  LINK_URL 0
class DUI_API CLinkObj
{
public:
	CLinkObj();
	virtual ~CLinkObj();

public:
	DWORD m_dwType; //类型 LINK_TYPE
	tstring m_strShowTxt;//显示的内容
	tstring m_strLinkTxt;//操作的内容
	tstring m_strId;	/// 标识符
	LONG	m_cpMin;
	LONG	m_cpMax;
};

/////////////////////////////////////////////////////////////////////
class DUI_API COleObject: public IOleObject, public IViewObject2
{
public:
	COleObject();
	virtual ~COleObject();
	void SetOwner(CRichEditUI* pOwner);

public:
	// IOleObject接口
	virtual HRESULT  __stdcall  QueryInterface(REFIID iid, void ** ppvObject);
	virtual ULONG  __stdcall  AddRef();
	virtual ULONG  __stdcall  Release();
	virtual HRESULT  __stdcall SetClientSite( IOleClientSite *pClientSite);
	virtual HRESULT  __stdcall GetClientSite( IOleClientSite **ppClientSite);
	virtual HRESULT  __stdcall SetHostNames( LPCOLESTR szContainerApp,  LPCOLESTR szContainerObj);
	virtual HRESULT  __stdcall Close( DWORD dwSaveOption);
	virtual HRESULT  __stdcall SetMoniker( DWORD dwWhichMoniker, IMoniker *pmk);
	virtual HRESULT  __stdcall GetMoniker( DWORD dwAssign,  DWORD dwWhichMoniker,IMoniker **ppmk);
	virtual HRESULT  __stdcall InitFromData( IDataObject *pDataObject, BOOL fCreation,  DWORD dwReserved);
	virtual HRESULT  __stdcall GetClipboardData(DWORD dwReserved, IDataObject **ppDataObject);
	virtual HRESULT  __stdcall DoVerb(LONG iVerb, LPMSG lpmsg,  IOleClientSite *pActiveSite, LONG lindex, HWND hwndParent, LPCRECT lprcPosRect);
	virtual HRESULT  __stdcall EnumVerbs(IEnumOLEVERB **ppEnumOleVerb);
	virtual HRESULT  __stdcall Update();
	virtual HRESULT  __stdcall IsUpToDate();
	virtual HRESULT  __stdcall GetUserClassID(CLSID *pClsid);
	virtual HRESULT  __stdcall GetUserType(DWORD dwFormOfType,  LPOLESTR *pszUserType);
	virtual HRESULT __stdcall  SetExtent(DWORD dwDrawAspect, SIZEL  *psizel);
	virtual HRESULT  __stdcall GetExtent(DWORD dwDrawAspect, SIZEL *psizel       );
	virtual HRESULT  __stdcall Advise(IAdviseSink *pAdvSink, DWORD *pdwConnection   );
	virtual HRESULT  __stdcall Unadvise(DWORD dwConnection );
	virtual HRESULT  __stdcall EnumAdvise(IEnumSTATDATA **ppenumAdvise);
	virtual HRESULT  __stdcall GetMiscStatus( DWORD dwAspect, DWORD *pdwStatus );
	virtual HRESULT  __stdcall SetColorScheme(LOGPALETTE *pLogpal);

	// IViewObject接口
	virtual HRESULT WINAPI Draw(DWORD dwDrawAspect, LONG lindex, void *pvAspect, DVTARGETDEVICE *ptd,
		HDC hdcTargetDev, HDC hdcDraw, LPCRECTL lprcBounds, LPCRECTL lprcWBounds,
		BOOL ( WINAPI *pfnContinue )(ULONG_PTR dwContinue), ULONG_PTR dwContinue);
	virtual HRESULT WINAPI GetColorSet(DWORD dwDrawAspect, LONG lindex, void *pvAspect, 
		DVTARGETDEVICE *ptd, HDC hicTargetDev, LOGPALETTE **ppColorSet);
	virtual HRESULT WINAPI Freeze(DWORD dwDrawAspect, LONG lindex, void *pvAspect, DWORD *pdwFreeze);
	virtual HRESULT WINAPI Unfreeze(DWORD dwFreeze);
	virtual HRESULT WINAPI SetAdvise(DWORD aspects, DWORD advf, IAdviseSink *pAdvSink);
	virtual HRESULT WINAPI GetAdvise(DWORD *pAspects, DWORD *pAdvf, IAdviseSink **ppAdvSink);

	// IViewObject2接口
	virtual HRESULT WINAPI GetExtent(DWORD dwDrawAspect, LONG lindex, DVTARGETDEVICE *ptd, LPSIZEL lpsizel);


	// IImageShow接口
	virtual HRESULT WINAPI GetRect(LPRECT lpRect);

	long GetCharPos();
	void SetCharPos(long nPos);

	
	virtual void OnMouseEnter(TEventUI& event);
	virtual void OnMouseLeave(TEventUI& event);
	virtual void OnMouseMove(TEventUI& event);
	virtual void OnClick(TEventUI& event);
	virtual void OnRClick(TEventUI& event);
	virtual void OnDbClick(TEventUI& event);
	virtual void LoadResource();
	virtual void ReleaseResource();
public:
	IAdviseSink* GetIAdviseSink(){return m_spAdviseSink;};
	LONG m_dwRef;
	IOleClientSite *m_pOleClientSite;
	IAdviseSink* m_spAdviseSink;
	RECT m_rcBounds;
	CRichEditUI* m_pOwner;
	SIZE  m_sizeExtent;
	long  m_nCharPos;
	DWORD m_dwId;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
class DUI_API COleImageObj:public COleObject
{
	friend class CRichEditUI;
public:
	COleImageObj();
	virtual ~COleImageObj();
	static COleImageObj* LoadImage(LPCTSTR lpszPath, bool bCache = true);
	static COleImageObj* LoadImage(HICON hIcon );
	static COleImageObj* LoadImage(HBITMAP hBitmap );

public:
	ImageObj* GetImage();
	void SetImage(ImageObj* pImageObj);

	tstring GetFilePath();
	void SetFilePath(LPCTSTR lpszPath);

	int  GetImageType();
	void SetImageType(int nType);

	void SetKey(LPCTSTR lpszKey);
	LPCTSTR GetKey();
	bool IsCache(){return m_bCache;}
	bool IsThumbnail();
	int  GetFrameCount();
	void SetId(DWORD dwId);
	double GetScale();
protected:
	virtual void LoadResource();
	virtual void ReleaseResource();
	virtual void OnMouseLeave(TEventUI& event);
	virtual void OnMouseMove(TEventUI& event);
	virtual void OnClick(TEventUI& event);
	virtual void OnDbClick(TEventUI& event);
	virtual HRESULT WINAPI Draw(DWORD dwDrawAspect, LONG lindex, void *pvAspect, DVTARGETDEVICE *ptd,
		HDC hdcTargetDev, HDC hdcDraw, LPCRECTL lprcBounds, LPCRECTL lprcWBounds,
		BOOL ( WINAPI *pfnContinue )(ULONG_PTR dwContinue), ULONG_PTR dwContinue);
	void NextFrame();
	int GetFrameDelay();
private:
	int     m_nImageType;  
	int		m_nActiveFrame;  //当前帧
	int     m_nFrameDelay;
	ImageObj* m_ImageObj;
	ImageObj* m_pZoomImage;
	tstring m_strKey;
	tstring m_strPath;
	bool    m_bCache;
	bool m_bInThumbnail;
	int     m_nCurWidth;
};



///////////////////////////////////////////////////////////////////
class DUI_API CRichEditUI:public CLabelUI, public INotifyUI
{
	friend class CTextHost;
	friend class IExRichEditOleCallback;
	friend class COleObject;
public:
	 CRichEditUI();
	 virtual ~CRichEditUI();
	 UI_DECLARE_DYNCREATE();

	 //消息映射
	 UIBEGIN_MSG_MAP
		 EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, IDR_MENU_UIEDIT,  OnMenuClick)
	 UIEND_MSG_MAP
public:
	IRichEditOle*  GetIRichEditOle();
	//字体相关接口
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	virtual void SetTextColor(DWORD dwTextColor);//设置字体颜色
	DWORD GetTextColor();
	void SetFontName(LPCTSTR lpszFontName);//设置字体格式
	LPCTSTR GetFontName();
	void SetFontSize(LONG size);//设置字体大小
	LONG GetFontSize();
	void SetBold(BOOL flag);//设置字体为粗体
	void SetItalic(BOOL flag);//设置字体为斜体
	void SetUnderline(BOOL flag);//设置字体为下划线
	BOOL IsBold();//是否为粗体？
	BOOL IsItalic();//斜体?
	BOOL IsUnderline();//下划线?
	void SetReadOnly(bool bReadOnly);
	bool IsReadOnly();
	void SetMaxChar(int nMax);
	void SetThumbnail(bool bThumbnail);  //是否显示缩略图


	//格式格式
	DWORD GetDefaultCharFormat(CHARFORMAT2 &cf);
	void  GetDefaultCharFormat(TEXTFORMAT &tf);
	bool  SetDefaultCharFormat(CHARFORMAT2 &cf);
	DWORD GetSelectionCharFormat(CHARFORMAT2 &cf);
	bool SetSelectionCharFormat(CHARFORMAT2 &cf);
	DWORD GetParaFormat(PARAFORMAT2 &pf);
	bool SetParaFormat(PARAFORMAT2 &pf);

	//位置相关接口
	int GetLineCount();
	int GetFirstVisibleLine();
	int LineIndex(int nLine = -1);
	void GetCharPos(long lChar, POINT& pt);
	long LineFromChar(long nIndex);
	int CharFromPos(POINT& pt);

	//选择相关接口
	void GetSel(CHARRANGE &cr);
	void GetSel(long& nStartChar, long& nEndChar);
	void SetSel(CHARRANGE &cr);
	void SetSel(long nStartChar, long nEndChar);
	void ReplaceSel(CONST TCHAR * lpszNewText, bool bCanUndo = false);
	void HideSelection(bool bHide);
	bool IsHideSelection();

	//文本相关接口
	long GetTextLength(DWORD dwFlags);
	void GetText(tstring& sText);
	void GetText(CHARRANGE &cr, tstring& sText);
	void GetSelText(tstring& sText);
	void SetText(LPCTSTR lpcszText);
	void InsertText(LPCTSTR lpcszText, int nOffsetPos = 0, bool bReplaceSel = false, TEXTFORMAT* pcf = NULL);
	void InsertLink(LPCTSTR lpcszText, CLinkObj* pLinkObj, int nOffsetPos = 0,  bool bReplaceSel = false, TEXTFORMAT* pcf = NULL);
	void CancelLink(CLinkObj* pLinkObj, TEXTFORMAT* pcf = NULL);
	void SetLink(CHARRANGE &cr, TEXTFORMAT* pcf = NULL);
	// 图像处理
	void InsertImage(COleImageObj* pImgObj, long nCharPos = REO_CP_SELECTION);

	// 以下这几个函数很不规范，需要整理
	void FindImage(CStdPtrArray& items, LPCTSTR lpszKey);
	void FindOleImage(LPVOID lpPointer, long &nStartPos, long &nEndPos);
	int GetImageCount();
	void GetImageObj(CStdPtrArray& items, long nStartPos = 0, long nEndPos = -1);

	void SetMinThumbnailWidth(int nMinThumbnailWidth);

	
	//滚动相关接口
	void EndDown();
	void SetScrollPos(int nPos);

	//编辑接口
    void Copy();
	void Cut();
	bool Redo();
	bool Undo();
	void Clear();
	void Paste();

	bool SetAutoURLDetect(bool bEnable = true);
	long StreamIn(int nFormat, EDITSTREAM &es);
	long StreamOut(int nFormat, EDITSTREAM &es);
	void SetBkChange();
	void SetStartIndent(int nStartIndent);

	HRESULT TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *plresult);

	// 消息前面加上系列号
	void BeginNewDotSeq();
	void InsertDot();
	
	void SetRich(BOOL bNew);
	bool GetZoom(int& nNum, int& nDen);
	bool SetZoom(int nNum, int nDen);

	UINT SetUndoLimit(UINT nLimit);
	void EmptyUndoBuffer();

	IOleObject* FindObject(POINT& pt);
protected:
	virtual bool CanPaste();
	virtual void Paste(IDataObject* pDataObject);
	virtual bool Copy(IDataObject** ppDataObject);
	virtual void OnOleEvent(TEventUI&event, IOleObject* pOleObj);

protected:
	bool OnMenuClick(TNotifyUI* pNotifyUI);
	bool IsMousePressOnScrollBar();
	bool ScrollBarEventDelegate(TEventUI& event);
	void GetDropTarget(IDropTarget** ppdt);
	void  DrawDot(IRenderDC* pRenderDC);
	void  GetClientRect(LPRECT lpRect);
	
	void ResizeImage(RECT rect);
	bool ReadHTMLFromClipboard(HGLOBAL hMem);
	void RemoveInvalidImageObj();
	void GetVisibleCharRang( long& nVFirstChar, long& nVEndChar );
	int  GetLastLineHeight();
	CLinkObj* FindLink(POINT& pt);
	void NeedUpdate();

	void SetRect(RECT& rectRegion);
 	void Init();
	void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	bool Event(TEventUI& event);
	BOOL LoadImage();
	void ReleaseImage();  
	CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);
	IRichEditOleCallback* m_pRichEditOleCallback;
	CTextHostBase* m_pTextHost;
	ImageObj* m_pImageRender;
	CScrollbarUI* m_pVerticalScrollbar;
	COLORREF m_txtColor;
	tstring m_szFontName;
	LONG m_nFontSize;
	BOOL m_bBold;
	BOOL m_bItalic;
	BOOL m_bUnderline;
	bool m_bReadOnly;

	DibObj* m_pDibMgr;
	CStdPtrArray m_listLinkObj;     //链接列表
	DWORD m_dwImgObjID;
	void DrawGif();
	bool m_bBkChanged;
	static UINT m_clip_board_format;
	tstring m_strTempPath;
	tstring m_strImageFile;
	bool m_bResizing;
	bool m_bThumbnail;
	int  m_nStartIndent;
	bool m_bUpdateNeeded;
    bool m_bVScrollBarFixing;
	ImageObj* m_pDotImage;
	IOleObject* m_pOleObjHover;
	vector<int>m_vecDotSeq;
	int  m_nPreInsertDot;
	int  m_nUrlPos;
	bool m_bHideSelection;
	bool m_bEnabeImage;

	bool m_bTransparent;//用于是否绘制背景
};
#endif