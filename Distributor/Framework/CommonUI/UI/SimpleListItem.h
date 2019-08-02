#pragma once

class COMMONUI_API CSimpleListItemUI : public CListContainerItemUI
{
public:
	CSimpleListItemUI(void);
	~CSimpleListItemUI(void);


	virtual bool Event(TEventUI& event);

	virtual bool    IsSelect();
	virtual void	Select(bool bSelect = true, bool bRedraw = true);
	bool  _lastshift;
	bool  _lastshiftup;
};
