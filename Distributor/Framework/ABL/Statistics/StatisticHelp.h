#pragma once
//#include "InstallsCase/InstallsCaseData.h"

class CStatistics;
class InstallsCaseData;
class InstallsAppInfo;
class StatisticHelp : 
	public BaseHandler<PostInstallAppMsg>
	,public BaseHandler<PostInstallPlanListMsg>
	,public BaseHandler<PostZJAppNumOfAnPlanMsg>
	,public BaseHandler<PostZJInstallDurationMsg>
	,public BaseHandler<PostZJInstallPlanNumMsg>
	,public BaseHandler<PostInstallPlanMsg>
{
public:
    StatisticHelp(CStatistics* pStatistics);
    ~StatisticHelp();

	// Summary  : 上报装机助手同时最大连接设备数
	// Returns  : bool
	bool PostZJDeviceConnectNum();

	// Summary  : 上报装机方案的总数
	// Returns  : bool
	bool PostZJInstallPlanNum(vector<InstallsCaseData *> DataList);

	// Summary  : 上报单个装机方案安装时长
	// Returns  : bool
	bool PostZJInstallDuration(int num, int allSize);

	// Summary  : 上报单个装机方案的应用总数
	// Returns  : bool
	bool PostZJAppNumOfAnPlan(int num);

	// Summary  : 上报装机助手启动时间
	// Returns  : bool
	bool PostZJStartTime();

	// Summary  : 上报安装单个应用的大小和时长统计
	// Returns  : bool
	bool PostInstallApp( int size, int duration );

	// Summary  : 上报装机方案的大小，个数和时长的统计
	// Returns  : bool
	bool PostInstallPlan( int size, int num );

	virtual bool Handle( const PostDeviceConnectNumMsg* pMsg );

	virtual bool Handle( const PostStartTimeMsg* pMsg );

	virtual bool Handle( const PostInstallAppMsg* pMsg );

	virtual bool Handle( const PostInstallPlanListMsg* pMsg );

	virtual bool Handle( const PostZJAppNumOfAnPlanMsg* pMsg );

	virtual bool Handle( const PostZJInstallDurationMsg* pMsg );

	virtual bool Handle( const PostZJInstallPlanNumMsg* pMsg );

	virtual bool Handle( const PostInstallPlanMsg* pMsg );

	// Summary  : 上报单个装机方案的应用列表数据
	// Returns  : bool
	bool PostInstallPlanList( InstallsAppInfo* appInfo );

private:
	CStatistics* _pStatistics;

};