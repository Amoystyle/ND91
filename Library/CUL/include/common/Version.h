#pragma once

#include "../cul.h"

#include <string>
using namespace std;

//�汾��Ϣ������
//�����ĸ��汾ֵ v1.v2.v3.v4
class CUL_CLASS_DECL CVersion
{ 
public:
	CVersion();
    CVersion(string strVer);
	CVersion(int maTemp, int mjTemp,int rTemp, int bTemp);
    ~CVersion(void);

    // ȡ�ļ��汾��
    static CVersion GetFileVersion(const wchar_t* szFileFullName);

    //��pVer�Ƚ�
    //return <0 С��pVer  =0��� >0 ����
    int CompareTo(CVersion* pVer);

    // ���Ϊ�ַ���
	wstring ToStringW();

public:
    int _nVer1;     // ��1�ΰ汾��
    int _nVer2;     // ��2�ΰ汾��
    int _nVer3;     // ��3�ΰ汾��
    int _nVer4;     // ��4�ΰ汾��
};
