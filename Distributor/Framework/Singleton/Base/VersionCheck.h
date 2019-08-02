#pragma once


class VersionCheck 
{
public:
	VersionCheck();
	//ªÒ»°∞Ê±æ∫≈
	wstring getProductVersion(wstring strPath);

	wstring GetIniVersion();
	wstring GetNowVersion();
};
