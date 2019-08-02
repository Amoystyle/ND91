#pragma once

// 接口编号
#define Act1 "201"
#define Act2 "202"
#define Act3 "203"
#define Act4 "204"
#define Act5 "205"
#define Act6 "10"

#define ProjCode "2900"
#define MD5Key "E8DC62E4-4031-47B3-86D2-5146F9D8443A"

#define ITUNESAPPURL "http://itunesapp2.sj.91.com/controller.ashx"
#define SEARCHAPPURL "http://ressearch2.sj.91.com/service.ashx"	 

// 排序方式
#define RatingScoreDESC "f_ratingscoreDESC"
#define RatingScoreASC	"f_ratingscoreASC"
#define DownnumDESC 	"f_downnumDESC"
#define DownnumASC		"f_downnumASC"
#define UpttimeDESC 	"f_infoupt_timeDESC"
#define UpttimeASC		"f_infoupt_timeASC"
#define PriceDESC		"PriceDESC"
#define PriceASC		"PriceASC"


// 平台类型
enum PlatFrom
{
	IPHONE,
	IPAD,
};

// 显示图标分类
enum ShowAppType
{
	HOT24APP,
	SOFTOFAPP,
	SOFTOFGAME,
	SEARCHAPP,	
};

// 
enum FilterType
{
	DownLoadLevel,
	PriceLevel,
	VersionLevel,
};

// 应用分类
class WebSoftCategory 
{
public:
	WebSoftCategory()
	{
		_id		= 0;
		_sortNum= 0;
	}
	~WebSoftCategory(){}

	int		_id;					//	分类id
	int		_sortNum;				//	分类号
	wstring _name;					//	分类名称
	wstring _imgSrc;				//	分类图标
};

// 应用信息
class WebSoftData 
{
public:
	WebSoftData()
	{
		_id			= 0;		
		_Price	= 0;	
		_size		= 0;		
		_cateid		= 0;	
		_downNum	= 0;
		_ratingScore= 0;	
		
		_forShow	= false;
		_tq			= false;
		_isIpa		= false;
		_bIpad		= false;
		_appType	= SOFTOFAPP;
	}
	~WebSoftData(){}
	

	int		_id;					//	应用id
	int		_Price;					//	优惠价格
	int		_size;					//	大小
	int		_cateid;				//	所属分类id
	int		_downNum;				//	下载总数
	int		_ratingScore;			//	星级描述
	int		_ratingNum;

	wstring _name;					//	应用名称
	wstring _namesLug;
	wstring _imgSrc;				//	应用图标
	wstring _summary;				//	应用说明
	wstring _desc;					//  详细描述
	wstring _identifier;			//	应用标识
	wstring _version;				//	应用版本
	wstring	_categoryName;			//	所属分类名称
	wstring _demand;				//	系统要求
	wstring	_alias;
	wstring	_author;				//  生产公司
	wstring _addTime;				//	上架时间
	wstring _localpath;				//  应用图标本地路径
	std::string	_downUrl;			//	下载地址

	bool    _bIpad;					// 是否是ipad平台应用
	bool    _isIpa;					// 是否是ipa
	bool	_forShow;
	bool	_tq;

	ShowAppType  _appType;

	vector<wstring> _VerPicDatas;	// 游戏详细图片列表

};