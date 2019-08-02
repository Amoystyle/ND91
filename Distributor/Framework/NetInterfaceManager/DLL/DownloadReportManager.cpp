#include "StdAfx.h"
#include "DownloadReportManager.h"

DownloadReportManager* DownloadReportManager::_pReportManager = NULL;
DownloadReportManager::DownloadReportManager(void)
{
}

DownloadReportManager::~DownloadReportManager(void)
{
}

DownloadReportManager* DownloadReportManager::GetInstance()
{
	if (NULL == _pReportManager)
	{
		_pReportManager = new DownloadReportManager;
	}

	return _pReportManager;
}

void DownloadReportManager::Attach(IDownloadReport* pDownloadReport)
{
	AutoLock;
	if (NULL == pDownloadReport)
	{
		return;
	}

	for (vector<IDownloadReport*>::iterator iter = _vec_DownloadReport.begin(); iter != _vec_DownloadReport.end(); ++iter)
	{
		if ((*iter) == pDownloadReport)
		{
			return;
		}
	}

	_vec_DownloadReport.push_back(pDownloadReport);
}

void DownloadReportManager::DisAttach(IDownloadReport* pDownloadReport)
{
	AutoLock;
	if (NULL == pDownloadReport)
	{
		return;
	}

	for (vector<IDownloadReport*>::iterator iter = _vec_DownloadReport.begin(); iter != _vec_DownloadReport.end(); ++iter)
	{
		if ((*iter) == pDownloadReport)
		{
			_vec_DownloadReport.erase(iter);
			break;
		}
	}

}

void DownloadReportManager::ReportProgress(UINT unTaskID, bool bDownloadSuccess, UINT unAllSize, UINT unCurDownSize)
{
	AutoLock;
	for (vector<IDownloadReport*>::iterator iter = _vec_DownloadReport.begin(); iter != _vec_DownloadReport.end(); ++iter)
	{
		if (*iter && (*iter)->GetTaskID() == unTaskID)
		{
			(*iter)->ReportProgress(bDownloadSuccess, unAllSize, unCurDownSize);
		}
	}
}