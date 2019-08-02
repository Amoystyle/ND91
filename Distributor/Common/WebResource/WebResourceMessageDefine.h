#pragma once

//登录成功后的信息
struct LoginStruct
{
	tstring name;//用户名
	bool isCPACompetence;//是否有CPA权限

	LoginStruct()
	{
		name = L"";
		isCPACompetence =  false;
	}
};

//登录成功消息
struct LoginMsg : BaseMessage<LoginMsg>
{
	LoginStruct pLoginInfo;

};

//登录退出消息
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
	};  platform值按这个枚举来，默认为0（PLATFORM_Android）,ipad 和iphone平台都用PLATFORM_Iphone这个值*/
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
	bool bIsAndroidPlatform;//默认为true，为false为ios设备，为true为android设备
	CpaWebIsCancelMsg()
	{
		bIsCancel = false;
		bIsAndroidPlatform = true;
	}
};

struct LocalCpaInfoInitFinishedMsg : BaseMessage<LocalCpaInfoInitFinishedMsg>
{
	bool bIsAndroidPlatform;//默认为true，为false为ios设备，为true为android设备
	bool bIsAllFinished;     //是否全部结束
	LocalCpaInfoInitFinishedMsg()
	{
		bIsAndroidPlatform = true;
		bIsAllFinished = false;
	}
};



//网络资源安装软件，传递的信息
struct WebSoftInstallMsg : public BaseMessage<WebSoftInstallMsg>
{
	vector<wstring> _st_Hash; //软件的哈希值列表
	vector<wstring> _st_Sn;   //安装的设备sn列表
};

//如果是一个一个添加的话nParam设置为0
struct LocalCpaAddOrRemoveMsg : public BaseMessage<LocalCpaAddOrRemoveMsg>
{
	map<string,wstring> mapRemoveCpa;
	vector<wstring> vecAddoveCpa;
	bool bIsAndroidPlatform;//默认为true，为false为ios设备，为true为android设备
	LocalCpaAddOrRemoveMsg()
	{
		bIsAndroidPlatform = true;
	}
};

//如果是一个一个添加的话nParam设置为0
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
	int phoneType;//0表示EnumPhoneType_Android，1表示EnumPhoneType_IPhone
};

//检查cpa软件是否已经安装过
struct CheckCpaInstalledMsg : BaseMessage<CheckCpaInstalledMsg>
{
	string _strHash;
	string _strImei;
	string _strReturn;
};

//下载并安装消息
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