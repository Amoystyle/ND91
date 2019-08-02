#pragma once

//��¼�ɹ������Ϣ
struct LoginStruct
{
	tstring name;//�û���
	bool isCPACompetence;//�Ƿ���CPAȨ��

	LoginStruct()
	{
		name = L"";
		isCPACompetence =  false;
	}
};

//��¼�ɹ���Ϣ
struct LoginMsg : BaseMessage<LoginMsg>
{
	LoginStruct pLoginInfo;

};

//��¼�˳���Ϣ
struct QuitMsg : BaseMessage<QuitMsg>
{

};

struct GetLoginUserInfoMsg : BaseMessage<GetLoginUserInfoMsg>
{
	LoginStruct pLoginInfo;
};


struct CpaWebButtonStatusChangedMsg : BaseMessage<CpaWebButtonStatusChangedMsg>
{
	bool bIsExist;
	//wstring strPath;
	wstring strHashValue;

	/*enum ENUM_PLATFORM
	{
		PLATFORM_Android,
		PLATFORM_Iphone,
		PLATFORM_Ipad,
	};  platformֵ�����ö������Ĭ��Ϊ0��PLATFORM_Android��,ipad ��iphoneƽ̨����PLATFORM_Iphone���ֵ*/
	int platform;
	CpaWebButtonStatusChangedMsg()
	{
		bIsExist = false;
		//strPath = L"";
		strHashValue = L"";
		platform = 0;
	}
};

struct CpaWebButtonStatusChangedMsgEx : BaseMessage<CpaWebButtonStatusChangedMsgEx>
{
	VARIANT param[2];
		int cArgs;
	CpaWebButtonStatusChangedMsgEx()
	{
		cArgs = 0;
	}
};

struct CpaWebIsCancelMsg : BaseMessage<CpaWebIsCancelMsg>
{
	bool bIsCancel;
	bool bIsAndroidPlatform;//Ĭ��Ϊtrue��ΪfalseΪios�豸��ΪtrueΪandroid�豸
	CpaWebIsCancelMsg()
	{
		bIsCancel = false;
		bIsAndroidPlatform = true;
	}
};

struct LocalCpaInfoInitFinishedMsg : BaseMessage<LocalCpaInfoInitFinishedMsg>
{
	bool bIsAndroidPlatform;//Ĭ��Ϊtrue��ΪfalseΪios�豸��ΪtrueΪandroid�豸
	bool bIsAllFinished;     //�Ƿ�ȫ������
	LocalCpaInfoInitFinishedMsg()
	{
		bIsAndroidPlatform = true;
		bIsAllFinished = false;
	}
};



//������Դ��װ��������ݵ���Ϣ
struct WebSoftInstallMsg : public BaseMessage<WebSoftInstallMsg>
{
	vector<wstring> _st_Hash; //����Ĺ�ϣֵ�б�
	vector<wstring> _st_Sn;   //��װ���豸sn�б�
};

//�����һ��һ����ӵĻ�nParam����Ϊ0
struct LocalCpaAddOrRemoveMsg : public BaseMessage<LocalCpaAddOrRemoveMsg>
{
	map<string,wstring> mapRemoveCpa;
	vector<wstring> vecAddoveCpa;
	bool bIsAndroidPlatform;//Ĭ��Ϊtrue��ΪfalseΪios�豸��ΪtrueΪandroid�豸
	LocalCpaAddOrRemoveMsg()
	{
		bIsAndroidPlatform = true;
	}
};

//�����һ��һ����ӵĻ�nParam����Ϊ0
struct LocalCpaPathMsg : public BaseMessage<LocalCpaPathMsg>
{
	LocalCpaPathMsg()
	{
	}
	vector<wstring> _vec_Path;
	vector<wstring> _vec_Hash;
};
struct AddCpaButtonStatusChangedMsg :  public BaseMessage<AddCpaButtonStatusChangedMsg>
{
	int platform;

};

struct CreateCpaDispatherMsg : public BaseMessage<CreateCpaDispatherMsg>
{
};


struct HomeWebCpaButtonStatusChangedMsg : BaseMessage<HomeWebCpaButtonStatusChangedMsg>
{
	wstring _st_sn;
	bool    _bLinkOut;
	int phoneType;//0��ʾEnumPhoneType_Android��1��ʾEnumPhoneType_IPhone
};

//���cpa����Ƿ��Ѿ���װ��
struct CheckCpaInstalledMsg : BaseMessage<CheckCpaInstalledMsg>
{
	string _strHash;
	string _strImei;
	string _strReturn;
};

//���ز���װ��Ϣ
struct CpaReDownLoadAndInstall
{
	CpaReDownLoadAndInstall()
	{
		st_bIsDownLoadAndInstal = false;
	}
	bool   st_bIsDownLoadAndInstal;
	vector<wstring> st_vec_Hash;
	vector<wstring> st_vec_sn;
};