#pragma once

#include "Module/Driver/BaseDriverController.h"

// ȡ���������ص�ַ������ֱ����������
namespace ND91Assistant
{   
    class CLASSINDLL_CLASS_DECL GetDriverUrl : public BaseDriverController
    {
    public:
        // deviceID �豸ID
        // driverID ����ID
        // �����Ҫֱ�����أ��򱣴浽�õ�ַ
        GetDriverUrl(string deviceID, string driverID, wstring strFilePath=L"");
        ~GetDriverUrl();

        // ���ʽӿ�,����������浽_strFilePath
        bool Action();
        // ��ʽ������Ҫ�ĵ�ַ
        string GetUrl();

    private:
        string _driverUrl;      // ������ַ
        wstring _strFilePath;   // �����Ҫֱ�����أ��򱣴浽�õ�ַ

        string _deviceID;       // ������豸ID
        string _driverID;       // ���������ID
    };
}