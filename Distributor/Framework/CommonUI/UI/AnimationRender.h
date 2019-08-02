#pragma once

class COMMONUI_API AnimationRender
{
	friend class CToggleLayoutUI;
	friend class CAutoGridLayoutUI;
public:
	typedef enum{
		ANIMATIONLeft2Right = 0,
		ANIMATIONRight2Left,
		ANIMATIONUnset,
	}ANIMATIONMOVETYPE;
	AnimationRender();
	~AnimationRender();
	void SetAnimations(UINT uElapse, UINT uFrames);
	void SetOffset(int nOffsetX, int nOffsetY);
	void DrawFrameToMemDC(CControlUI* pOwner, int nMoveType);
	void Render(IRenderDC* pRenderDC);
	void Render(IRenderDC* pRenderDC, int nCurRenderX, int nCurRenderY);

	bool IsAnimation(){return m_bAnimation;}
	bool StartAnimation();
	bool StopAnimation();
	void UpdateAnimation();

	static DWORD CALLBACK AnimationProc(LPVOID lp);
protected:
	void CalcFrame(int nFrameIndex);
	UINT m_uFrames;
	UINT m_uElapse;
	int m_nWidth;
	int m_nHeight;
	int m_nOffsetX;
	int m_nOffsetY;
	int m_nCurRenderX;
	int m_nCurRenderY;
	int m_nAnimationIndex;
	int m_enMoveType;
	bool m_bAnimation;
	DibObj m_DibObj;
	CControlUI* m_pOwner;
	HWND m_hwndOwner;
	DWORD m_hThreadID;
};

//////////////////////////////////////////////////////////////////////////

class COMMONUI_API IStaticUI
{
public:
	IStaticUI();
	~IStaticUI();

	// 添加不参与动画的控件
	virtual void AddStaticItem(CControlUI* item);
	// 移除不参与动画的控件（移除后如果控件在动画区域显示，将出现在动画中）
	virtual void RemoveStaticItem(CControlUI* item);
	virtual void StaticUIRender(IRenderDC* pRenderDC, RECT& rcPaint);
	virtual void Hide();
	virtual void Show();

private:
	vector<CControlUI*> m_vCtrls;
	vector<CControlUI*> m_vTemp;
	CRITICAL_SECTION m_vcs;
};