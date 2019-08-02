#pragma once

#include <string>

using namespace std;

namespace ND91Assistant
{
    // ���������Ŀ¼
    class ThemePath
	{ 
	public:
        ThemePath(const wstring workFolder);

        wstring GetPath() const;          // ����ģ��Ĺ���Ŀ¼
        wstring GetThemeTempPath() const;      // ����ģ������ʱ�ļ�
        wstring GetPreviewPath() const;   // ����Ԥ����
        wstring GetAdpPath() const;       // ���apt����ʱ��װ�����ݵ����ص�·��
        wstring GetUploadPath() const;     // 

    private:
        const wstring _workFolder;        // ��ʱ����Ŀ¼
    };
}
