#pragma once


namespace ND91Assistant
{
    class DeviceData;

	// 从服务端获取应用数据结构体
	class CLASSINDLL_CLASS_DECL AppUpdateInfo
	{
		public:
			AppUpdateInfo()
			{		
				_nfid		= 0;		// 值为0时说明没有从服务器取到数据
				_itunesid	= 0;		// 值为0时说明没有从服务器取到数据
				_nsize		= 0;
				_nDownNum	= 0;
				_bLegal		= false;
				_bUpdate	= false;	

				_localPath  = L"";
			}

			~AppUpdateInfo(void){;}
		
		public:

			int			_nfid;			// 软件id
			int			_itunesid;		// itunesid 正版ios应用专有
			int			_nsize;			// 软件大小	
			int			_nDownNum;		// 被下载次数
			bool		_bLegal;		// 是否是正版应用	false 非正版 true 正版
			bool		_bUpdate;		// 是否需要更新	
			time_t		_addTime;		// 上架时间
			wstring		_name;			// 软件名
			wstring		_identifier;	// 软件标识
			wstring     _buildVersion;	// 构建版本
			wstring		_oldVersion;	// 旧版本号
			wstring		_newVersion;	// 新版本号
			wstring		_verCode;		// 版本ID	
			wstring		_url;			// 新版本URL	(虚拟的URL, 真正的app地址为其他的url, 可以通过GetAppsRealUpdateUrls获取)

			wstring     _localPath;    //升级的软件对应的本地软件的路径

	};

	class CLASSINDLL_CLASS_DECL CGetAppUpdateInfo
	{
		public:
			CGetAppUpdateInfo( int deviceType );		// 获取本地PC上的应用的升级信息
			CGetAppUpdateInfo( DeviceData* pDevice );	// 获取手机应用的升级信息
			~CGetAppUpdateInfo(void);

			// vector<AppUpdateInfo*>* pinfo 即是输入参数也是返回结果，
			// 输入参数AppUpdateInfo需要 _identifier, _oldVersion, _verCode即可
			// 结果返回AppUpdateInfo所有成员数据
			// Summary  : 获取设备所有应用升级信息				(旧接口，返回xml格式数据)
			// Returns  : bool		是否获取成功
			// Parameter: vector<AppUpdateInfo*>* pinfo		程序信息集合 
			bool GetAppUpdateInfo( vector<AppUpdateInfo*>* pinfo );

			// vector<AppUpdateInfo*>* pinfo 即是输入参数也是返回结果，
			// 输入参数AppUpdateInfo需要 _identifier, _buildVersion, _oldVersion, _verCode即可
			// 结果返回AppUpdateInfo所有成员数据
			// Summary  : 获取设备所有应用升级信息				(act=405, 新接口，返回json格式数据,但是json里面包含xml)
			// Returns  : bool		是否获取成功
			// Parameter: vector<AppUpdateInfo*>* pinfo		程序信息集合
			bool GetAppUpdateData( vector<AppUpdateInfo*>* pinfo );


			// vector<AppUpdateInfo*>* pinfo 即是输入参数也是返回结果，
			// 非正版升级	：	act=404  输入参数AppUpdateInfo需要传 _identifier, _oldVersion, _verCode	例如: com.clickgamer.AngryBirds,	5.1.2
			// 正版体验升级	：	act=605  输入参数AppUpdateInfo需要传 _nfid, _oldVersion, _verCode		例如: 578036166,	5.1.2
			// 结果返回AppUpdateInfo所有成员数据
			// Summary  : 获取设备所有应用升级信息, 包括正版与非正版				(最新接口，返回纯json格式数据)
			// Returns  : bool		是否获取成功
			// Parameter: vector<AppUpdateInfo*>* pinfo		程序信息集合
			bool GetAppUpdateDataNew( vector<AppUpdateInfo*>* pinfo, bool bLegal );

			// Summary  : 通过AppUpdateInfo的_nfid(软件id)获取真正的升级所需的URL;
			// Returns  : bool
			// Parameter: vector<AppUpdateInfo * > * pinfo			
			bool GetAppsRealUpdateUrls( vector<AppUpdateInfo*>* pinfo );

			// Summary  : 通过AppUpdateInfo的_identifier(软件标识)获取正版应用的itunesid;
			// Returns  : bool
			// Parameter: vector<AppUpdateInfo * > * pinfo
			bool GetItunesidByIdentify( vector<AppUpdateInfo*>* pinfo );

		private:
			
			DeviceData*		_pDevice;

			int				_deviceType;

			// 旧接口, 解析返回的XML数据
			bool ParseInfo(string imginfo, vector<AppUpdateInfo*>* pinfo);

			// 新接口, 解析返回的Json含xml的数据
			bool ParseData(string appInfos, vector<AppUpdateInfo*>* pinfo);

			// 最新新接口, 解析返回的纯Json的数据
			bool ParseJsonData(string appInfos, vector<AppUpdateInfo*>* pinfo, bool bLegal );
	};
}
