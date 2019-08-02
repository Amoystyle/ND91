#pragma once

#include "../cul.h"

#include <string>
using namespace std;

//版本信息处理类
//保持四个版本值 v1.v2.v3.v4
class CUL_CLASS_DECL CVersion
{ 
public:
	CVersion();
    CVersion(string strVer);
	CVersion(int maTemp, int mjTemp,int rTemp, int bTemp);
    ~CVersion(void);

    // 取文件版本号
    static CVersion GetFileVersion(const wchar_t* szFileFullName);

    //跟pVer比较
    //return <0 小于pVer  =0相等 >0 大于
    int CompareTo(CVersion* pVer);

    // 输出为字符串
	wstring ToStringW();

public:
    int _nVer1;     // 第1段版本号
    int _nVer2;     // 第2段版本号
    int _nVer3;     // 第3段版本号
    int _nVer4;     // 第4段版本号
};
