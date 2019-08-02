#pragma once

INSTALLCASE_BEGIN

class LocalAppinfoList
{
public:
	LocalAppinfoList(CListUI* listui,CCompositeUI* compui);
	~LocalAppinfoList(){};

	void ClearAppList();
	void AddAppItem(InstallsAppInfo* appinfo,bool isupdate,bool isuserdata=false,bool issystemdata = false, int select=-1, bool insertToFront=false, vector<wstring>* upidlist=NULL);
	void RemoveAppItem(CControlUI* item);
	void RemoveAppItemByID(int index);
	void UpdateAppItemByPath(wstring path);

	void AddCaseItem(InstallsCaseData *CaseData);
	void UpdateCaseItem(InstallsCaseData* caseData);
	void RemoveCaseItem(InstallsCaseData *CaseData);
	void ClearCaseList();

	void  AddFolderItem(AppCountByFolder* FolderItem, int index);
	void ClearFolderList();

	void SetShowList(CListUI* listui){_pAppInfoList=listui;}

	bool ListContainerItemEvent(TEventUI& event);
	CListUI* GetAppList(){return _pAppInfoList;}
private:
	CListUI* _pAppInfoList;
	CCompositeUI* _compUI;
};

INSTALLCASE_END