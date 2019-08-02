#pragma once

RENCENT_INSTALLCASE_BEGIN

class LocalAppinfoList;
//����Ӧ���б���������
class LocalAppInfoItem
{
public:
	LocalAppInfoItem(){};
	~LocalAppInfoItem(){};
	//����Ӧ����Ϣ�����б���
	static CSimpleListItemUI* CreateAppInfoItem( InstallsAppInfo * appinfo,LocalAppinfoList* pAppInfoList,CCompositeUI *compui,bool bupdate = false,bool busercase=false,bool bsystemcase = true, int select=-1, bool insertToFront = false, vector<wstring>* upidlist=NULL);

	//���������
	static void UpdateAppInfoItem(InstallsAppInfo * appinfo,LocalAppinfoList* pAppInfoLis, int select);

	//�����������״̬
	static void UpdateAppInfoItemUpgradeState(InstallsAppInfo * appinfo,LocalAppinfoList* pAppInfoLis, CCompositeUI *compui);

	static void SetToolTipText(InstallsAppInfo * appinfo, CListContainerItemUI* item);

	//���ݷ�����Ϣ���ɷ�����
	static CSimpleListItemUI* CreateCaseItem( InstallsCaseData * casedata,LocalAppinfoList* pAppInfoList,CCompositeUI *compui);
	//���·�����
	static void UpdateCaseItem(InstallsCaseData * casedata,LocalAppinfoList* pAppInfoList);
	//����Ŀ¼��Ϣ����Ŀ¼��
	static CListContainerItemUI* CreateFolderSumItem( AppCountByFolder * casedata, int index,LocalAppinfoList* pAppInfoList,CCompositeUI *compui);

	static void UpdateFolderSumItem(AppCountByFolder * casedata, LocalAppinfoList* pAppInfoList, CCompositeUI *compui);
};

RENCENT_INSTALLCASE_END
