#pragma once

class COMMONUI_API BaseView
	: public CCompositeUI
{
public:
	BaseView();
	~BaseView();

	virtual void Init(CContainerUI* pContainer);

	virtual void Show();
	
	void Hide();

protected:
	virtual wstring GetStyleID() = 0;

private:
	bool _bAdded;
};