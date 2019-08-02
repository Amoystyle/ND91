#pragma once

#include "Core/DeviceInfo.h"
#include <map>
#include <vector>
#include <string>

using namespace std;

namespace ND91Assistant
{
    // �ֻ��豸��Ϣ������
    // ���ڽ����ֻ����ص����ݵĽ�����
    class AndroidPropertyData
    {
    public:
        AndroidPropertyData(const char* pData, size_t nSize);
        ~AndroidPropertyData();

        //��ȡ����ֵ
        wstring GetPropertyValue(string strProp);

    private:
        //��ʽ��ֵ
        string FormatValue(string str);

    private:
        map<string, string> _PropList;
    };
}