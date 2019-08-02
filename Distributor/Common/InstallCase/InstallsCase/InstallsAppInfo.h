#pragma once
#include "CBase_ACK.h"

enum  _INSTALLS_CASE_APP_UPDATEABLE_FALG
{
	INSTALLS_CASE_UPDATEABLE_UNKNOW,
	INSTALLS_CASE_UPDATEABLE_YES,
	INSTALLS_CASE_UPDATEABLE_NO
};

enum  _INSTALLS_CASE_APP_LEGALUPDATEABLE_FALG
{
	INSTALLS_CASE_LEGALUPDATEABLE_UNKNOW,
	INSTALLS_CASE_LEGALUPDATEABLE_YES,
	INSTALLS_CASE_LEGALUPDATEABLE_NO
};

enum _INSTALLS_CASE_APP_STATUS
{
	INSTALLS_CASE_STATUS_UNKNOW,
	INSTALLS_CASE_STATUS_NO_DOWNLOAD,
	INSTALLS_CASE_STATUS_DOWNLOADING,
	_INSTALLS_CASE_STATUS_DOWNLOAD_FAIL,
	INSTALLS_CASE_STATUS_HAS_DOWNLAOD,
	_INSTALLS_CASE_STATUS_INSTALLING,
	INSTALLS_CASE_STATUS_INSTALL_FAIL,
	INSTALLS_CASE_STATUS_INSTALL_SUCESS
};
//����װӦ�ó�����Ϣ��
class InstallsAppInfo
{
public:
	InstallsAppInfo(UINT id);
	InstallsAppInfo(InstallsAppInfo* appinfo, bool autodelete);
	InstallsAppInfo(InstallsAppInfo* appinfo,int newID);
	~InstallsAppInfo(){};
public:
	UINT m_id;												//id
	wstring m_strName;										//Ӧ�ó�������
	wstring m_strVersion;									//�汾��
	unsigned long m_FileSize;								//�ļ���С
	wstring m_strFileType;									//�ļ����� :ipa,pxl,apk...
	time_t m_tDownloadTime;									//����ʱ��
	_INSTALLS_CASE_APP_UPDATEABLE_FALG m_bUpdateable;		//�Ƿ���Ը��±�־ 0
	_INSTALLS_CASE_APP_LEGALUPDATEABLE_FALG m_bLegalUpdateable;	//�Ƿ����������±�־ 0
	wstring m_Iconpath;										//ͼ��
	bool m_bSelected;										//�Ƿ�ѡ��
	_INSTALLS_CASE_APP_STATUS m_status;						//״̬
	wstring m_strSavePath;									//�ļ�����·��			
	wstring m_strPackageID;									//��װ����ʶ��
	wstring m_strDownLoadURL;								//���ص�ַ
	wstring m_strLegalDownLoadURL;							//�������ص�ַ����������õ���
	wstring m_VerCode;
	wstring m_Purchaser;									//������
	int m_isLegalSoft;										//�Ƿ����� 1Ϊ����
	wstring m_SupportDevice;								//֧�ֵ��豸
	wstring m_SDKVersion;
	int m_fid;												//���id,��������
	int m_itemID;
	wstring m_strNewVersion;								//���������İ汾��
	wstring m_strNewLegalVersion;							//��������������汾��
	bool m_bRemoving;
	bool m_AutoDelete;
};
//�ļ��б�
class InstallsAppList : public CBase_ACK<InstallsAppInfo>
{
public:
	InstallsAppList(){};
	~InstallsAppList(){};
};