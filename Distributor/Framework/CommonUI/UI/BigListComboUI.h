#ifndef BigListComboUI_h__
#define BigListComboUI_h__

const int PER_ITEM_HEIGHT = 29;//列表每项的高度
class COMMONUI_API CBigListComBoxUI : public CComboBoxUI
{
public:
	CBigListComBoxUI();
	virtual ~CBigListComBoxUI();
	UI_DECLARE_DYNCREATE();
	virtual void ShowDrop();
};

#endif // BigListComboUI_h__