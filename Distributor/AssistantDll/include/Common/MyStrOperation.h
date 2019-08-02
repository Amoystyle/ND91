#pragma once

#include <string>
#include <vector>
#include "stdint.h"
using namespace std;

/// �ַ�������
class CLASSINDLL_CLASS_DECL MyStrOperation
{
public:

    // ��ȡʱ������, nDouble(����)��λ��1��ʶ���룬1000��ʶ��
    static wstring FormatTime(int nSecond, int nDouble=1);
    static wstring FormatTime(time_t nTime);

    // ���������
    static int Random( int start, int end );

    // �����ַ��ж�
    static bool IsChineseLetter(wchar_t ch);
    static bool IsChineseString(wstring str);

    // ����תƴ��
    static string ChineseToPinyin(wstring chinese);
};
