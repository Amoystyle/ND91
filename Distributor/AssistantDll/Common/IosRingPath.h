#pragma once

#include <string>

using namespace std;

namespace ND91Assistant
{
    // ���������Ŀ¼
    class IosRingPath
	{ 
	public:
        IosRingPath(const wstring workFolder, bool cracked);

#pragma region ��������ģ�鱾����ʱĿ¼

		wstring GetLocalRingListPath() const;     // ���ػ����Զ��������б�·��
		wstring GetLocalRingSetListPath() const;  // ���ػ�������Զ������������ļ�
		wstring GetLocalRingWorkPath() const;

#pragma endregion

#pragma region ��������ģ�鱾�ֻ�Ŀ¼

		wstring GetIosRingListPath() const;		// �ֻ��Զ��������б��ļ�·��
		wstring GetIosRingSetListPath() const;    // �ֻ�����Զ������������ļ�·��
		wstring GetIosUserRingPath() const;		// �ֻ����ϵͳ�����ļ�Ŀ¼
		wstring GetIosSyetemRingPath() const;     // �ֻ����ϵͳ�����ļ�Ŀ¼
		wstring GetIosRingLibraryFolder() const;

#pragma endregion

    private:
        const wstring _workFolder;        // ��ʱ����Ŀ¼
        bool    _cracked;         // �Ƿ�Խ��
    };
}
