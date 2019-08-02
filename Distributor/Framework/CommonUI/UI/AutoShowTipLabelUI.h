#ifndef AutoShowTipLabelUI_h__
#define AutoShowTipLabelUI_h__

class COMMONUI_API CAutoShowTipLabelUI : public CLabelUI
{
public:
	CAutoShowTipLabelUI();
	~CAutoShowTipLabelUI();
	UI_DECLARE_DYNCREATE();
	//virtual void  SetText(LPCTSTR lpszText);

	virtual void  SetRect(RECT& rect);
};
#endif // AutoShowTipLabelUI_h__