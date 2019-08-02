#pragma once

class WebSoftModel;
class WebSoftData;
class WebAppListUI:public BaseView
{
public:
	WebAppListUI();
	~WebAppListUI(void);

	void ShowListData(const vector<WebSoftData*>* pSofts);
	void ShowAppIcon(int fid);
	void RemoveAll();

	wstring GetStyleID();

	void Init(CContainerUI* pContainer);

private:

	void OnCreate();

	void AddItem(const WebSoftData* pData=NULL);

};
