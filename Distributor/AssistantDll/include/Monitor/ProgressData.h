#pragma once

#include "AssistantDll.h"
#include <string>

#pragma warning (disable:4251)

namespace ND91Assistant
{
    // ��������
    struct CLASSINDLL_CLASS_DECL ProgressData
    {
        __int64 _currentTaskProgress;       // ��ǰ����������
        __int64 _currentTaskSum;            // ��ǰ���������
        std::wstring _currentTaskName;  // ��ǰ��������ֵ�������ļ���

		__int64 _allTaskProgresss;          // ��ǰ����������
		__int64 _allTaskSum;		        // �������������
        
        __int64 _taskNumberCompleted;       // ��ǰ��ɵ�������
        __int64 _taskNumberSum;             // �������������

        // ���캯����ȫ������
		ProgressData();

        // תΪ�ı�
        std::wstring ToString() const;
    };
}
