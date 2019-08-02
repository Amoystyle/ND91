#pragma once

#include <string>
using namespace std;
//版本信息处理类
//保持四个版本值 v1.v2.v3.v4
class CVersion
{ 
public:
	CVersion();
    CVersion(string strVer);
	CVersion(int maTemp, int mjTemp,int rTemp, int bTemp);
    ~CVersion(void);

    static CVersion GetFileVersion(const wchar_t* szFileFullName);

    //跟pVer比较
    //return <0 小于pVer  =0相等 >0 大于
    int CompareTo(CVersion* pVer);
	wstring ToStringW();

    /// <summary>
    /// 是否需要升级安卓守护 by QiuChang 2012.10.19
    /// </summary>
    bool NeedUpdateAndroidDeamon();

public:
    int _nVer1;
    int _nVer2;
    int _nVer3;
    int _nVer4;
};
