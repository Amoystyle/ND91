/*********************************************************************
 *       Copyright (C) 2010,应用软件开发
 *********************************************************************
 *   Date             Name                 Description
 *   2010-10-22       xqb
*********************************************************************/
#ifndef _DRAGDROPIMPL_H
#define _DRAGDROPIMPL_H

class CDropSource : public IDropSource 
{ 
public: 
	HRESULT __stdcall QueryInterface (REFIID iid, void ** ppvObject); 
	ULONG __stdcall AddRef (void); 
	ULONG __stdcall Release (void); 
	// 
	// IDropSource members 
	// 
	HRESULT __stdcall QueryContinueDrag (BOOL fEscapePressed, DWORD grfKeyState); 
	HRESULT __stdcall GiveFeedback (DWORD dwEffect); 
	// 
	// Constructor / Destructor 
	// 
	CDropSource(IDataObject* pDataObject, CControlUI* pControl = NULL); 
	~CDropSource(); 
	void CancelDrag();
	virtual void OnDragSourceCancel();
	virtual void OnDragSourceDrop();
	virtual void OnDragSourceMove();
private: 
	 LONG  m_lRefCount; 
	 bool cancel_drag_;
	 CControlUI* m_pControl;
	 IDataObject* m_pDataObject;
}; 

//////////////////////////////////////////////////////////////
class CDropTarget : public IDropTarget
{
public:
	CDropTarget(CWindowUI* pWindow, DWORD dwAcceptKeyState = MK_LBUTTON);
	~CDropTarget();

public:
	// Interface IDropTarget
	HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
	HRESULT STDMETHODCALLTYPE DragEnter(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect);
	HRESULT STDMETHODCALLTYPE DragLeave(void);
	HRESULT STDMETHODCALLTYPE Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD __RPC_FAR *pdwEffect);

	// Interface IUnknown	
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject);
	ULONG STDMETHODCALLTYPE AddRef(void);
	ULONG STDMETHODCALLTYPE Release(void);

protected:
	ULONG cRefs;
	CWindowUI* m_pWindow;
	DWORD m_AcceptKeyState;
	CControlUI* m_pControl;
	IDataObject* m_pDataObject;
	bool m_bFiles;
};
#endif