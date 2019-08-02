#ifndef TURNOBJECTWND_H_
#define TURNOBJECTWND_H_

#include "AnimationWnd.h"

// CTurnObjectWnd Class:翻转动画
class CTurnObjectWnd : public CAnimationWnd
{
public:
	CTurnObjectWnd( HBITMAP hBitmap, RECT rect, CWindowUI *pWindow );
	virtual ~CTurnObjectWnd();
	void Play( );

protected:
	void OnTimer(UINT_PTR nIDEvent);
	void UpdateAnimation( int iAlpha = 255 );

private:
	HDC m_hMemoryDC;
	HBITMAP m_hBmpSideA;
	Bitmap *m_pBmpSideA;
	int m_iAnimationIndex;

	CWindowUI *m_pWindow;
};

// CPosChangeWnd Class:放大缩小动画
class CPosChangeWnd : public CAnimationWnd
{
public:
	CPosChangeWnd( HBITMAP hBitmap, RECT rect );
	virtual ~CPosChangeWnd();
	void Play( );

protected:
	void OnTimer(UINT_PTR nIDEvent);
	void UpdateAnimation( int iAlpha = 255 );

private:
	HDC m_hMemoryDC;
	HBITMAP m_hBmpSideA;
	Bitmap *m_pBmpSideA;
	int m_iAnimationIndex;
};

#endif