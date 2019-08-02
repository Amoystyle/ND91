#pragma once

#include "Module/Driver/BaseDriverController.h"

namespace ND91Assistant
{   

    // ��ѯ��ȷ�����豸��Ϣ
    class CLASSINDLL_CLASS_DECL SearchDriverInfo : public BaseDriverController
    {
    public:
        // hardID Ҫ����Ӳ��ID
        SearchDriverInfo(string hardID);
        // ��������
        ~SearchDriverInfo();

        // �ύ�ӿ�
        bool Action();

    private:
        string _hardID; // Ҫ����Ӳ��ID
    };
}