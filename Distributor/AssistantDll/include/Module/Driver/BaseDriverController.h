#pragma once

#include "Module/Driver/Driver.h"

// ���������ӿڻ��࣬�ṩϵ�нӿ�����Ҫ��ͨ�÷����ͱ���
namespace ND91Assistant
{   
    class CLASSINDLL_CLASS_DECL BaseDriverController
    {
    public:
        BaseDriverController() {};
        ~BaseDriverController() {};

        // ��ȡ���
        bool GetResult() {return _result;};

    protected:
        Driver* _pDriver;   // ��ǰ�����Driver
        bool _result;       // �������

        int GetOSData();    // ��õ�ǰOS����Ϣ
        string GetUrlParam(vector<string>* pNames, vector<string>* pValues);    // ���Ҫ���͵�����
    };
}