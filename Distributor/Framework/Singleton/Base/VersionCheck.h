#pragma once


class VersionCheck 
{
public:
	VersionCheck();
	//��ȡ�汾��
	wstring getProductVersion(wstring strPath);

	wstring GetIniVersion();
	wstring GetNowVersion();
};
