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

	// Summary  : �ϱ�װ������ͬʱ��������豸��
	// Returns  : bool
	bool PostZJDeviceConnectNum();

	// Summary  : �ϱ�װ������������
	// Returns  : bool
	bool PostZJInstallPlanNum(vector<InstallsCaseData *> DataList);

	// Summary  : �ϱ�����װ��������װʱ��
	// Returns  : bool
	bool PostZJInstallDuration(int num, int allSize);

	// Summary  : �ϱ�����װ��������Ӧ������
	// Returns  : bool
	bool PostZJAppNumOfAnPlan(int num);

	// Summary  : �ϱ�װ����������ʱ��
	// Returns  : bool
	bool PostZJStartTime();

	// Summary  : �ϱ���װ����Ӧ�õĴ�С��ʱ��ͳ��
	// Returns  : bool
	bool PostInstallApp( int size, int duration );

	// Summary  : �ϱ�װ�������Ĵ�С��������ʱ����ͳ��
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

	// Summary  : �ϱ�����װ��������Ӧ���б�����
	// Returns  : bool
	bool PostInstallPlanList( InstallsAppInfo* appInfo );

private:
	CStatistics* _pStatistics;

};