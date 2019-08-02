/*********************************************************************
 *       Copyright (C) 2010,应用软件开发
 *********************************************************************
 *   Date             Name                 Description
 *   2010-09-27       xqb
*********************************************************************/
#ifndef __UIANIMATION_H_
#define __UIANIMATION_H_

#define _Style_Animation_Bk_ 0
#define enAnimationDefaultTimerID		0
class DUI_API CAnimationUI:public CControlUI
{
public:
	CAnimationUI();
	virtual ~CAnimationUI();
	UI_DECLARE_DYNCREATE();

public:
	ImageObj* GetImage();
	virtual void SetImage(LPCTSTR lpszImage);
	virtual void SetImage(ImageObj* pImageRender);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	void StopAnimation();
	void StartAnimation();
	void SetLoop(bool bLoop);
	LPCTSTR GetImageFileName();
protected:
	virtual bool Event(TEventUI& event);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	UINT m_nFrameCount;
	UINT m_nActiveFrame;
	UINT m_nTimerElapse;
	int m_nFrameDelay;
	bool m_bInitAnimation;
	bool m_bLoop;
	bool m_bGif;
};
#endif