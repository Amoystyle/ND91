#pragma once

#include <string>
using namespace std;
//�汾��Ϣ������
//�����ĸ��汾ֵ v1.v2.v3.v4
class CVersion
{ 
public:
	CVersion();
    CVersion(string strVer);
	CVersion(int maTemp, int mjTemp,int rTemp, int bTemp);
    ~CVersion(void);

    static CVersion GetFileVersion(const wchar_t* szFileFullName);

    //��pVer�Ƚ�
    //return <0 С��pVer  =0��� >0 ����
    int CompareTo(CVersion* pVer);
	wstring ToStringW();

    /// <summary>
    /// �Ƿ���Ҫ������׿�ػ� by QiuChang 2012.10.19
    /// </summary>
    bool NeedUpdateAndroidDeamon();

public:
    int _nVer1;
    int _nVer2;
    int _nVer3;
    int _nVer4;
};
