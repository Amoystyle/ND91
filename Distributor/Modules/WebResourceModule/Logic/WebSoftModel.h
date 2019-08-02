#pragma once
#include "WebSoftInfo.h"



class WebSoftModel
{
public:
	WebSoftModel(PlatFrom platType );
    ~WebSoftModel();
	
	// Summary  : 获取24小时热门应用列表
	// Returns  : bool
	// Parameter: int total				搜索的总个数
	bool Get24HoursHotAppList();


	// Summary  : 获取游戏或者软件分类列表
	// Returns  : bool
	// Parameter: bool isGame			是否是游戏
	bool GetSoftCategoryList(bool isGame);


	// Summary  : 获取软件数据列表,带分页
	// Returns  : bool
	// Parameter: int categoryId	分类id
	// Parameter: int size			每页个数
	// Parameter: int page			页码
	// Parameter: int sortType		排序方式
	bool GetAppDataList( int categoryId, int count,int page, std::string sortType );


	// Summary  : 获取游戏数据列表
	// Returns  : bool
	// Parameter: int categoryId	分类id
	// Parameter: int count			每页个数
	// Parameter: int page			页码
	// Parameter: int sortType		排序方式
	bool GetGameDataList( int categoryId, int count,int page, std::string sortType );


	// Summary  : 获取单个软件详细信息		
	// Returns  : void*
	// Parameter: std::string appId			应用id
	bool GetOneAppInfo( int appId );


	// Summary  : 搜索软件接口
	// Returns  : bool
	// Parameter: wstring categoryId	搜索关键字
	// Parameter: int count				每页个数
	// Parameter: int page				页码
	bool SearchAppList(	wstring strKeyWord, int count,int page );


	// Summary  : 获取应用界面显示的图标
	// Returns  : bool
	bool GetAppsIconFromWeb( vector<int> fids, vector<wstring> urls );



	// Summary  : UI获取应用数据集合的通用接口 (容器中的数据由调用者释放)
	// Returns  : bool
	// Parameter: vector<WebSoftData * > & verDatas			单页面数据
	// Parameter: int& total								该分类总数据	
	bool GetAppDatas(vector<WebSoftData*>& verDatas, int& total);
	// Summary  : UI获取应用分类集合的通用接口 (容器中的数据由调用者释放)
	// Returns  : bool
	// Parameter: vector<WebSoftCategory * > & verDatas
	bool GetCategoryDatas(vector<WebSoftCategory*>& verCats);



private:

// 	virtual bool Action(int nAction, BaseParam* pParam);  
// 	bool GetHotApps(BaseParam* pParam);
// 	bool GetSoftCategorys(BaseParam* pParam);
// 	bool GetAppDatas(BaseParam* pParam);
// 	bool GetGameDatas(BaseParam* pParam);
// 	bool GetSoftInfo(BaseParam* pParam);
// 	bool SearchApps(BaseParam* pParam);
// 	bool DownLoadApp(BaseParam* pParam);
// 	bool GetAppIcon(BaseParam* pParam);


private:

	std::string			_postData;			// 发送数据
	std::string			_CatsUrl;			// 发送数据
	std::string			_platForm;
	PlatFrom			_platType;

	int							_total;		// 该分类包含总应用数
 	vector<WebSoftData*>		_VerDatas;	// 返回数据列表
 	vector<WebSoftCategory*>	_VerCats;	// 返回数据列表

	
	CRITICAL_SECTION   _slock;

};