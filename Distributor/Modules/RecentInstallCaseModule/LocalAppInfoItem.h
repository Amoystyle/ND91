#pragma once

RENCENT_INSTALLCASE_BEGIN

class LocalAppinfoList;
//本地应用列表项生成类
class LocalAppInfoItem
{
public:
	LocalAppInfoItem(){};
	~LocalAppInfoItem(){};
	//根据应用信息生成列表项
	static CSimpleListItemUI* CreateAppInfoItem( InstallsAppInfo * appinfo,LocalAppinfoList* pAppInfoList,CCompositeUI *compui,bool bupdate = false,bool busercase=false,bool bsystemcase = true, int select=-1, bool insertToFront = false, vector<wstring>* upidlist=NULL);

	//更新软件项
	static void UpdateAppInfoItem(InstallsAppInfo * appinfo,LocalAppinfoList* pAppInfoLis, int select);

	//更新软件升级状态
	static void UpdateAppInfoItemUpgradeState(InstallsAppInfo * appinfo,LocalAppinfoList* pAppInfoLis, CCompositeUI *compui);

	static void SetToolTipText(InstallsAppInfo * appinfo, CListContainerItemUI* item);

	//根据方案信息生成方案项
	static CSimpleListItemUI* CreateCaseItem( InstallsCaseData * casedata,LocalAppinfoList* pAppInfoList,CCompositeUI *compui);
	//更新方案项
	static void UpdateCaseItem(InstallsCaseData * casedata,LocalAppinfoList* pAppInfoList);
	//根据目录信息生成目录项
	static CListContainerItemUI* CreateFolderSumItem( AppCountByFolder * casedata, int index,LocalAppinfoList* pAppInfoList,CCompositeUI *compui);

	static void UpdateFolderSumItem(AppCountByFolder * casedata, LocalAppinfoList* pAppInfoList, CCompositeUI *compui);
};

RENCENT_INSTALLCASE_END
