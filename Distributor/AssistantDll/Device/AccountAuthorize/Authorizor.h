#pragma once

struct FileVersionInfo;

namespace ND91Assistant
{
    class DeviceData;
    class Authorization;
    class iTunesAuthorize;

    enum AuthorizorError
    {
        ArgsError           = -9124,
        ConnectModeError    = -9125,
        AuthorizeException  = -9126,
        InternalError       = -9128,
    };

    // 正版IPA授权管理器
	class Authorizor
	{
	public:
		Authorizor(const DeviceData* pDevice);
		~Authorizor();

        // 初始化缓存数据
        bool Init();

        // 当前设备是否适合授权
        // 返回 0：适合授权
        // -1 设备固件版本低于4.0
        // -2 连接方式不是USB
        // -3 iTunes版本过低
        // -4 未安装iTunes
        // -5 iTunes版本为简易版
        static int IsValideToAuthorize(const DeviceData* pDevice);

        // 对设备授权
        bool Authorize(wstring data, wstring ipaName);

        // 修复闪退修复
        int FixFlashExit();

        // ipa安装成功后.如果无法知道是否是正版免费体验的,
        // 尝试对设备上已有账号进行一次修复.[临时解决方案]
        void AttemptFix();

        // 授权缓存文件目录
        static wstring _CacheFolder;

	private:

		void InitInternal( );
        
        // 解析授权信息（调用者负责释放Authorization*)
        Authorization* DecryptAuthorization(wstring data);

        // 获取iTunes版本号
        wstring GetiTunesVersion();

        // 授权
        bool Authorize(Authorization* au);

        // 判断当前要授权的账号对应的授权文件是否在缓存里面,有的话使用缓存.
        bool IsAuthorizationInCache(Authorization* au, Authorization* localCache);

        // 启动进程授权
        int AuthorizeFromExternal(Authorization* au);
        int AuthorizeFromExternalITunesAuthorize(Authorization* au);

        // 缓存授权账号信息
        void CacheAccount(Authorization* au);

        // 缓存一份授权账户到设备
        void SerilizeCacheAccountToDevice();

        // 从服务器获取授权文件信息,并解压到指定目录
        bool GetAuthorizationFromServer(Authorization* authorization);

        // 缓存授权信息
        void CacheAuthorization(Authorization* au);

        // MD5加密
        wstring GetMD5(wstring Data);

        // 获取最近的服务器地址,以免硬编码的 defaultPath 地址失效了
        wstring GetNewestUrl();

        // 按创建授权文件的时间降序排序
        static bool SortByCreateDate(Authorization* v1, Authorization* v2);

        // 映射账号id 到pathMap 表里面,用于生成存放授权信息文件夹名称,明文也没啥问题就是.
        wstring MapPath(wstring url);

        // 杀死/启动Helper_16进程
        void KillProcessHelper_16();
        int CreateProcessHelper_16(Authorization* au);

        // 获取当前设备安装的正版IPA所对应账户DSID
        void GetAppDSIDS(vector<wstring>& dsids);

        // iTunes版本信息
        static void GetItunesVersionInfo();

	private:

        const DeviceData*       _pDevice;               // 当前设备
        vector<wstring>         _authorizedAccounts;    // 当前设备已经授权账户
        vector<Authorization*>  _authorizations;        // 所有授权文件信息

        wstring                 _softID;                // 当前授权软件id,如果有
        wstring                 _ipaName;               // 当前授权ipa名字,如果有.

        wstring                 _ConfigFolderOnDevice;  // 配置文件在设备上的目录
        wstring                 _infoFileOnDevice;      // 配置文件在设备上的路径
        //static wstring          _RNG;                   // Des解密KEY;

        static bool                                _isSimpleiTunes;    // 用户环境是否是简版iTunes,默认非简版
        static FileVersionInfo*    _pITunesVersionInfo; // iTunes版本信息

        //wstring                 _md5RNG;                // md5KEY;
        wstring                 _defaultPath;           // 默认授权文件服务器地址
        wstring                 _pathMap;               // 生成本地目录随机值
        wstring                 _helperName;            // helper进程名称

        bool                    _isInitialized;         // 是否已经初始化

        iTunesAuthorize*        _externalAuthorize;     // 外部授权接口

        vector<wstring>         _attemptFixErrorIDs;    // 修复失败的应用ID
	};
}