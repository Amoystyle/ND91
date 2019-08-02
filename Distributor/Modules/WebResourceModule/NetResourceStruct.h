#pragma once

/*******************************************************************************
文件定义：存放网络资源的数据结构定义
*******************************************************************************/

enum ENUM_URL_OPREATE //网络资源上部对网页的操作
{

	Etype_UrlOperate_None,
	Etype_UrlOperate_Forword, //向前
	Etype_UrlOperate_Back,    //向后
	Etype_UrlOperate_Reflash, //刷新
	Etype_UrlOperate_Home     //主页
};

//资源站点结构

enum ENUM_PLATFORM
{
	PLATFORM_Android,
	PLATFORM_Iphone,
	PLATFORM_Ipad,
};

// 设备操作系统类型 - 以后上层逐渐用这个替换掉ABI和DDM定义的类型
enum NetEnumPhoneOS
{
	ePhoneOS_None_Net = -1,     //不区分系统
	ePhoneOS_Android_Net = 0,
	ePhoneOS_Ios_Net = 1,       
	ePhoneOS_IosJailBreak = 0x0101,     // 已越狱
	ePhoneOS_IosUnJailBreak = 0x0102,   // 未越狱
};

enum NetEnumAppSort  //网络--应用分类
{
	Etype_Application,  //应用
	Etype_Game,			//游戏
	Etype_Theme,		//主题
	Etype_Music,		//音乐
	Etype_Wallpaper,	//壁纸
	Etype_Video,		//视频
	Etype_Ebook,		//电子书
	Etype_Phonelife	    //手机生活
};


struct NetNode
{
	NetNode()
	{
		nodeId = L"";
		nodeName = L"";
		nodeIcon = L"";
		initUrl = L"";
		nodeUrl = L"";
		searchUrl = L"";
		isselect = false;
		ieid = L"";
		tagicon = L"";
		isshow = false;
		statid = L"";
		isPartner = false;
		strValue = L"";
	}

	NetNode(const NetNode& node)
	{
		nodeId = node.nodeId;
		nodeName = node.nodeName;
		nodeIcon = node.nodeIcon;
		initUrl = node.initUrl;
		nodeUrl = node.nodeUrl;
		searchUrl = node.searchUrl;
		isselect = node.isselect;
		ieid = node.ieid;
		tagicon = node.tagicon;
		isshow = node.isshow;
		statid = node.statid;
		isPartner = node.isPartner;
		strValue = node.strValue;
	}

	NetNode& operator=(const NetNode& node)
	{
		nodeId = node.nodeId;
		nodeName = node.nodeName;
		nodeIcon = node.nodeIcon;
		initUrl = node.initUrl;
		nodeUrl = node.nodeUrl;
		searchUrl = node.searchUrl;
		isselect = node.isselect;
		ieid = node.ieid;
		tagicon = node.tagicon;
		isshow = node.isshow;
		statid = node.statid;
		isPartner = node.isPartner;
		strValue = node.strValue;
		return *this;
	}
	tstring nodeId;		//子节点Id
	tstring nodeName;	//子节点名称
	tstring nodeIcon;	//子节点图标
	wstring initUrl;	//初始地址
	tstring nodeUrl;	//子节点导航地址
	tstring searchUrl;	//子节点搜索地址
	bool isselect;		//是否选中
	tstring ieid;		//所用的IE控件的id
	tstring tagicon;	//扩展的图标
	bool isshow;		//是否要显示
	tstring statid;		//功能统计Id
	bool isPartner;		//是否合作站点（默认是）
	tstring strValue;   //对应服务端的value值
};

struct NetSite
{
	NetSite()
	{
		siteId = L"";
		siteName = L"";
		layoutIndex = 0;
		isJailbreak = false;
		platform = ePhoneOS_None_Net;
	}
	NetSite(const NetSite& site)
	{
		siteId = site.siteId;
		siteName = site.siteName;
		layoutIndex = site.layoutIndex;
		isJailbreak = site.isJailbreak;
		platform    = site.platform;
		for (vector<NetNode>::const_iterator iter = site.nodes.begin(); iter != site.nodes.end(); iter++)
		{
			NetNode nodeTmp = (*iter);
			NetNode node(nodeTmp);
			nodes.push_back(node);
		}
	}

	NetSite& operator=(const NetSite& site)
	{
		siteId = site.siteId;
		siteName = site.siteName;
		layoutIndex = site.layoutIndex;
		isJailbreak = site.isJailbreak;
		for (vector<NetNode>::const_iterator iter = site.nodes.begin(); iter != site.nodes.end(); iter++)
		{
			NetNode nodeTmp = (*iter);
			NetNode node(nodeTmp);
			nodes.push_back(node);
		}
		return *this;
	}
	tstring siteId;
	tstring siteName;
	int layoutIndex;
	NetEnumPhoneOS platform;
	bool isJailbreak;
	vector<NetNode> nodes;
};

struct NetModuleData
{
	NetModuleData()
	{
		moduleId = L"";
	}
	tstring moduleId;
	NetEnumPhoneOS platform;
	vector<NetSite> sites;
};

struct NetResultData
{
    NetResultData()
    {
        //isSuccess = false;
        version = 0;
    };
	//bool isSuccess;
	int version;
	vector<NetModuleData> moduledatas;
};

//资源模块结构

struct NetModuleStruct
{
	tstring moduleid;
	tstring modulename;
	tstring moduleicon;
	int layoutindex;
};

struct NetResultModule
{
	bool isSuccess;
	tstring version;
	vector<NetModuleStruct> modules;
};

enum NetNodeType
{
	NetNodeTypeBegin,
	ItunesApp2 = NetNodeTypeBegin,//不越狱免费装
	Index,						  //装机必备
	Search,						  //搜索
	News,					      //新闻资讯
	Cpa,                          //Cpa
	NetNodeTypeEnd
};

enum ButonStatus
{
	CpaButonStatus_NONE,			//空
	CpaButonStatus_DOWNLOAD,		//下载
	CpaButonStatus_DOWNLOADFINISH,  //已下载
	CpaButonStatus_INSTALL			//安装
};

enum CPaDispatcherID
{
	HomeWebCpa
};
