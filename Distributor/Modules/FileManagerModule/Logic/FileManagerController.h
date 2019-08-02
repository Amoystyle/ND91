#pragma once

class MainView;
class FileManagerController
	: public BaseHandler<InitRootMsg>
	, public BaseHandler<RefreshNodeMsg>
	, public BaseHandler<RefreshAppMsg>
	, public BaseHandler<DragdropFileMsg>
{
public:
	FileManagerController(wstring deviceId);
	~FileManagerController();

	bool Init(CContainerUI* pContainer);

	void Show();

	void Hide();


	virtual bool Handle(const InitRootMsg* pMsg);

	virtual bool Handle(const RefreshNodeMsg* pMsg);

	virtual bool Handle(const RefreshAppMsg* pMsg);

	virtual bool Handle(const DragdropFileMsg* pMsg);


private:
	MainView* _pMainView;
};