#pragma once


class CVersion;
// 共用类，用来获取系统的信息，如是否64位，操作系统版本等
class CLASSINDLL_CLASS_DECL CSystemInfo
{
public:
	CSystemInfo();
	~CSystemInfo();

public:
	// 判断操作系统是否是64位
	static bool is64();
	//是否是Vista系统
	static bool IsVista();
	//是否是Windows7系统
	static bool IsWindows7();
	//是否是XP系统
	static bool IsXP();
    //获取系统信息
    static wstring GetSystemInfo();
	//获取电脑的MAC地址
	static std::string GetMac();
	//获取win系统版本
	static std::string GetOs();
	static bool IsNeedInstalliTunes(wstring minVer = _T("7,5,0,20"));
private:
	//获取操作系统版本
	static bool GetOSVer(CVersion* pVer);
};
