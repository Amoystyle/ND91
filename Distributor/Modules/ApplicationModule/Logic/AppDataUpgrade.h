#pragma once
#include "Module/Application/AppData.h"
#include "Common/AppNameSpace.h"
using namespace ND91Assistant;

App_CC_BEGIN
class AppDataUpgrade : public AppData
{
public:
	AppDataUpgrade(const AppData*);
	~AppDataUpgrade();

	std::wstring PiracyUrl() const { return m_PiracyUrl; }
	void PiracyUrl(std::wstring val) { m_PiracyUrl = val; }

	std::wstring LegalUrl() const { return m_LegalUrl; }
	void LegalUrl(std::wstring val) { m_LegalUrl = val; }

	std::wstring PiracyNewVersion() const { return m_PiracyNewVersion; }
	void PiracyNewVersion(std::wstring val) { m_PiracyNewVersion = val; }

	std::wstring LegalNewVersion() const { return m_LegalNewVersion; }
	void LegalNewVersion(std::wstring val) { m_LegalNewVersion = val; }

	unsigned long FileSize() const { return m_FileSize; }
	void FileSize(unsigned long val) { m_FileSize = val; }

	int FID() const { return m_fid; }
	void FID(int val) { m_fid = val; }

	int ItunesID() const { return m_itunesid; }
	void ItunesID(int val) { m_itunesid = val; }

	bool IsUpgrading() const { return m_bIsUpgrading; }
	void IsUpgrading(bool val) { m_bIsUpgrading = val; }

private:
	wstring m_PiracyUrl;
	wstring m_LegalUrl;
	wstring m_PiracyNewVersion;
	wstring m_LegalNewVersion;
	unsigned long m_FileSize;
	int m_fid;
	int m_itunesid;
	bool m_bIsUpgrading;
};
App_CC_END