#pragma once

// �ӿڱ��
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

// ����ʽ
#define RatingScoreDESC "f_ratingscoreDESC"
#define RatingScoreASC	"f_ratingscoreASC"
#define DownnumDESC 	"f_downnumDESC"
#define DownnumASC		"f_downnumASC"
#define UpttimeDESC 	"f_infoupt_timeDESC"
#define UpttimeASC		"f_infoupt_timeASC"
#define PriceDESC		"PriceDESC"
#define PriceASC		"PriceASC"


// ƽ̨����
enum PlatFrom
{
	IPHONE,
	IPAD,
};

// ��ʾͼ�����
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

// Ӧ�÷���
class WebSoftCategory 
{
public:
	WebSoftCategory()
	{
		_id		= 0;
		_sortNum= 0;
	}
	~WebSoftCategory(){}

	int		_id;					//	����id
	int		_sortNum;				//	�����
	wstring _name;					//	��������
	wstring _imgSrc;				//	����ͼ��
};

// Ӧ����Ϣ
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
	

	int		_id;					//	Ӧ��id
	int		_Price;					//	�Żݼ۸�
	int		_size;					//	��С
	int		_cateid;				//	��������id
	int		_downNum;				//	��������
	int		_ratingScore;			//	�Ǽ�����
	int		_ratingNum;

	wstring _name;					//	Ӧ������
	wstring _namesLug;
	wstring _imgSrc;				//	Ӧ��ͼ��
	wstring _summary;				//	Ӧ��˵��
	wstring _desc;					//  ��ϸ����
	wstring _identifier;			//	Ӧ�ñ�ʶ
	wstring _version;				//	Ӧ�ð汾
	wstring	_categoryName;			//	������������
	wstring _demand;				//	ϵͳҪ��
	wstring	_alias;
	wstring	_author;				//  ������˾
	wstring _addTime;				//	�ϼ�ʱ��
	wstring _localpath;				//  Ӧ��ͼ�걾��·��
	std::string	_downUrl;			//	���ص�ַ

	bool    _bIpad;					// �Ƿ���ipadƽ̨Ӧ��
	bool    _isIpa;					// �Ƿ���ipa
	bool	_forShow;
	bool	_tq;

	ShowAppType  _appType;

	vector<wstring> _VerPicDatas;	// ��Ϸ��ϸͼƬ�б�

};