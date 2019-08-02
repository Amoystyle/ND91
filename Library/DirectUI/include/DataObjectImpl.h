/*********************************************************************
 *       Copyright (C) 2010,应用软件开发
 *********************************************************************
 *   Date             Name                 Description
 *   2010-10-22       xqb
*********************************************************************/
#ifndef _DATAOBJECTIMPL_H
#define _DATAOBJECTIMPL_H
//////////////////////////////////////////////////////////
class DUI_API CDataObject : public IDataObject
{
	friend class CDropSource;
public:
	// IUnknown members
	HRESULT __stdcall QueryInterface (REFIID iid, void ** ppvObject);
	ULONG   __stdcall AddRef (void);
	ULONG   __stdcall Release (void);

	// IDataObject members
	HRESULT __stdcall GetData (FORMATETC *pFormatEtc, STGMEDIUM *pmedium);
	HRESULT __stdcall GetDataHere (FORMATETC *pFormatEtc, STGMEDIUM *pmedium);
	HRESULT __stdcall QueryGetData (FORMATETC *pFormatEtc);
	HRESULT __stdcall GetCanonicalFormatEtc (FORMATETC *pFormatEct, FORMATETC *pFormatEtcOut);
	HRESULT __stdcall SetData (FORMATETC *pFormatEtc, STGMEDIUM *pMedium,  BOOL fRelease);
	HRESULT __stdcall EnumFormatEtc (DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc);
	HRESULT __stdcall DAdvise (FORMATETC *pFormatEtc, DWORD advf, IAdviseSink *, DWORD *);
	HRESULT __stdcall DUnadvise (DWORD      dwConnection);
	HRESULT __stdcall EnumDAdvise (IEnumSTATDATA **ppEnumAdvise);
		// Constructor / Destructor
	CDataObject ();
	~CDataObject ();

	// 设置数据
	bool  SetTextData(LPCTSTR lpszText);
	bool  SetFileData(vector<tstring>& vtFileList);
	bool  SetHBitmapData(HBITMAP hBitmap);
	bool  SetHTMLData(LPCTSTR lpszHTML);
	bool  SetUserData(int nFormat, LPVOID lpData, int  nSize);
	void  RemoveAll();
private:
	std::vector<FORMATETC*> m_pFormatEtc;
	std::vector<STGMEDIUM*> m_pStgMedium;
	LONG m_lRefCount;
	CControlUI* m_pDragControl;
	int LookupFormatEtc(FORMATETC *pFormatEtc);
	void CopyMedium(STGMEDIUM* pMedDest, STGMEDIUM* pMedSrc, FORMATETC* pFmtSrc);
};
#endif