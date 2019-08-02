#pragma once

#ifdef NETDISPATCHCONTROLLER_EXPORTS
#define NETDISPATCHCONTROLLER_API __declspec(dllexport)
#else
#define NETDISPATCHCONTROLLER_API __declspec(dllimport)
#endif
//���ع���������ʱ���ã�������Ϣ����
class IDownloadReport;
class DownloadReportManager : public BaseCSLock
{

private:
	DownloadReportManager(void);
	~DownloadReportManager(void);

public:

	void Attach(IDownloadReport* pDownloadReport);

	void DisAttach(IDownloadReport* pDownloadReport);

	static DownloadReportManager* GetInstance();

	void ReportProgress(UINT unTaskID, bool bDownloadSuccess, UINT unAllSize, UINT unCurDownSize);
private:
	static DownloadReportManager* _pReportManager;

	vector<IDownloadReport*> _vec_DownloadReport;
};

class NETDISPATCHCONTROLLER_API IDownloadReport
{
public:
	IDownloadReport(){DownloadReportManager::GetInstance()->Attach(this);}
	~IDownloadReport(){DownloadReportManager::GetInstance()->DisAttach(this);}
	virtual UINT GetTaskID() = 0;
	virtual void ReportProgress(bool bDownloadSuccess, UINT unAllSize, UINT unCurDownSize) = 0;
};